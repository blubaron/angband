/* File: angband.h */

/*
 * Copyright (c) 1997 Ben Harrison, James E. Wilson, Robert A. Koeneke
 *
 * This software may be copied and distributed for educational, research,
 * and not for profit purposes provided that this copyright and statement
 * are included in all such copies.  Other copyrights may also apply.
 */

#ifndef INCLUDED_ANGBASE_H
#define INCLUDED_ANGBASE_H

/*
 * Include the low-level includes.
 */
#include "h-basic.h"
#include "direction.h"

/*
 * Include the mid-level includes.
 */
#include "z-bitflag.h"
#include "z-file.h"
#include "z-form.h"
#include "z-util.h"
#include "z-virt.h"
#include "z-rand.h"
#include "z-term.h"
#include "z-quark.h"
#include "z-msg.h"

/*
 * Include the high-level includes.
 */
#include "base-defines.h"
#include "base-cmd.h"

#include "ui.h"
#include "z-type.h"

#include "inkey.h"
#include "ui-util.h"


/* enternals that the game needs to implement */
/* #include "../base_from_game/cmd_code.h" */
#include "../minimal/buildid.h"
extern command_list_entry *game_cmds;
extern int game_command_index(cmd_code command);
extern const char *game_command_verb(cmd_code command);
extern bool game_command_item_selector(cmd_code command);
extern bool game_command_check(game_command *cmd);
extern bool game_command_validate(game_command *cmd);

extern bool player_is_dead(void);
extern void player_add_redraw_flag(int flags);
extern void player_redraw_stuff(void);
extern bool option_get(const char *name);

extern bool character_saved;
extern bool character_generated;

extern bool s16b character_icky;   /* Depth of the game in special mode */
extern bool s16b character_xtra;   /* Depth of the game in startup mode */

extern s16b signal_count;

/***** Some older copyright messages follow below *****/


/*
 * Note that these copyright messages apply to an ancient version
 * of Angband, as in, from pre-2.4.frog-knows days, and thus the
 * references to version numbers may be rather misleading...
 */

/*
 * UNIX ANGBAND Version 5.0
 */

/* Original copyright message follows. */

/*
 * ANGBAND Version 4.8	COPYRIGHT (c) Robert Alan Koeneke
 *
 *	 I lovingly dedicate this game to hackers and adventurers
 *	 everywhere...
 *
 *	 Designer and Programmer:
 *		Robert Alan Koeneke
 *		University of Oklahoma
 *
 *	 Assistant Programmer:
 *		Jimmey Wayne Todd
 *		University of Oklahoma
 *
 *	 Assistant Programmer:
 *		Gary D. McAdoo
 *		University of Oklahoma
 *
 *	 UNIX Port:
 *		James E. Wilson
 *		UC Berkeley
 *		wilson@ernie.Berkeley.EDU
 *		ucbvax!ucbernie!wilson
 */

/*
 *	 ANGBAND may be copied and modified freely as long as the above
 *	 credits are retained.	No one who-so-ever may sell or market
 *	 this software in any form without the expressed written consent
 *	 of the author Robert Alan Koeneke.
 */


#endif

