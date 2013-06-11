/*
 * File: pickfile.c
 * Purpose: pick a file from a directory, using the virtual terminal.
 *
 * Copyright (c) 2013 Brett Reid
 *
 * This work is free software; you can redistribute it and/or modify it
 * under the terms of either:
 *
 * a) the GNU General Public License as published by the Free Software
 *    Foundation, version 2, or
 *
 * b) the "Angband license":
 *    This software may be copied and distributed for educational, research,
 *    and not for profit purposes provided that this copyright and statement
 *    are included in all such copies.  Other copyrights may also apply.
 */

#include "angband.h"
#include "ui-menu.h"
#include "pickfile.h"

static int comp_pstring(const void *a, const void *b)
{
	return strcmp(*(const char**)a,*(const char**)b);
}

/* Pick a file from the given directory, with one of the given extensions.
 * If ext1 is NULL, allow any file in the directory.
 * If ext1 is NULL, and ext4 is not, only list files that have ext4 as a
 * prefix, and possibly ext2 or ext3 as an extension.
 *
 * Return value is 0 in success, -1 if the selection was canceled, otherwise
 * < 0 if there was an error, > 0 if the buffer was to small.
 *
 * The filenames, including path, can have up to 1024 characters.
 */
errr file_pick(char*out, int outlen, const char *title, const char *dir,
               const char *ext1, const char *ext2,
               const char *ext3, const char *ext4)
{
	ang_dir *pdir;
	char buf[1024];
	menu_type *menu;
	region r;
	ui_event selected;
	char *ext;
	char **filelist;
	int count = 0;
	int i = 0;
	int prefixlen = 0;

	/* get the number of matching files in the directory */
	pdir = dir_open(dir);
	if (!pdir) {
		return -4;
	}

	if (ext1) {
		while (dir_read(pdir, buf, 1024)) {
			ext = strrchr(buf, '.');
			if (ext) {
				if (streq(ext+1,ext1)) {
					count++;
				} else
				if (ext2 && streq(ext+1,ext2)) {
					count++;
				} else
				if (ext3 && streq(ext+1,ext3)) {
					count++;
				} else
				if (ext4 && streq(ext+1,ext4)) {
					count++;
				}
			}
		}
	} else
	if (ext4) {
		/* only display items with ext4 as prefix */
		while (dir_read(pdir, buf, 1024)) {
			if (prefix(buf, ext4)) {
				if (ext2 || ext3) {
					ext = strrchr(buf, '.');
					if (ext) {
						if (ext2 && streq(ext+1,ext2)) {
							count++;
						} else
						if (ext3 && streq(ext+1,ext3)) {
							count++;
						}
					}
				} else {
					count++;
				}
			}
		}
	} else
	{
		while (dir_read(pdir, buf, 1024)) {
			count++;
		}
	}

	dir_close(pdir);

	if (count) {
		/* allocate the list of files */
		filelist = C_RNEW(count, char*);
		if (!filelist) {
			return -3;
		}

		/* build the list of files */
		pdir = dir_open(dir);
		if (!pdir) {
			return -4;
		}

		i = 0;
		if (ext1) {
			while (dir_read(pdir, buf, 1024)) {
				ext = strrchr(buf, '.');
				if (ext) {
					if (streq(ext+1,ext1)) {
						filelist[i++] = (char*) string_make(buf);
					} else
					if (ext2 && streq(ext+1,ext2)) {
						filelist[i++] = (char*) string_make(buf);
					} else
					if (ext3 && streq(ext+1,ext3)) {
						filelist[i++] = (char*) string_make(buf);
					} else
					if (ext4 && streq(ext+1,ext4)) {
						filelist[i++] = (char*) string_make(buf);
					}
				}
			}
		} else
		if (ext4) {
			/* only display items with ext4 as prefix */
			prefixlen = strlen(ext4);
			while (dir_read(pdir, buf, 1024)) {
				if (prefix(buf, ext4)) {
					if (ext2 || ext3) {
						ext = strrchr(buf, '.');
						if (ext) {
							if (ext2 && streq(ext+1,ext2)) {
								filelist[i++] = (char*) string_make(buf+prefixlen);
							} else
							if (ext3 && streq(ext+1,ext3)) {
								filelist[i++] = (char*) string_make(buf+prefixlen);
							}
						}
					} else {
						filelist[i++] = (char*) string_make(buf+prefixlen);
					}
				}
			}
		} else
		{
			while (dir_read(pdir, buf, 1024)) {
				filelist[i++] = (char*) string_make(buf);
			}
		}

		dir_close(pdir);
	} else {
		/* no files were found, display an empty list */
		filelist = NULL;
	}

	/* Sort the strings */
	sort((void *)filelist, count, sizeof(*filelist), comp_pstring);

	menu = menu_new(MN_SKIN_COLUMNS, menu_find_iter(MN_ITER_STRINGS));
	if (!menu) {
		if (filelist) {
			for (i = 0; i < count; i++) {
				string_free(filelist[i]);
			}
			FREE(filelist);
		}
		return -3;
	}
	menu->flags = MN_NO_ACTION | MN_DBL_TAP | MN_NO_TAGS;
	menu->title = (char*)title;
	menu->prompt = (char*)"Pick a file or press ESC to cancel";
	menu_setpriv(menu, count, filelist);
	screen_save();

	r.col = 1;
	r.row = 1;
	r.width = -1;
	r.page_rows = -1;
	menu_layout(menu, &r);
	region_erase_bordered(&r);
	selected = menu_select(menu, 0, TRUE);

	screen_load();

	if (selected.type == EVT_SELECT) {
		int cursor = menu->cursor;
		if (prefixlen) {
			strnfmt(out, outlen, "%s%s", ext4, filelist[cursor]);
		} else {
  			strncpy(out, filelist[cursor], outlen);
		}
		out[outlen-1] = 0;
	}

	if (filelist) {
		for (i = 0; i < count; i++) {
			string_free(filelist[i]);
		}
		FREE(filelist);
	}
	FREE(menu);

	if (selected.type != EVT_SELECT) {
		return -1;
	}
	return 0;
}

