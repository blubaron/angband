/*
 * File: game-cmd.c
 * Purpose: Handles the queueing of game commands.
 *
 * Copyright (c) 2008-9 Antony Sidwell
 *
 * This work is free software; you can redistribute it and/or modify it
 * under the terms of either:
 *
 * a) the GNU General Public License as published by the Free Software
 *    Foundation, version 2, or
 *
 * b) the "Angband licence":
 *    This software may be copied and distributed for educational, research,
 *    and not for profit purposes provided that this copyright and statement
 *    are included in all such copies.  Other copyrights may also apply.
 */

#include "angband.h"
#include "cmds.h"
#include "base-cmd.h"
#include "game-cmd.h"
#include "object/object.h"
#include "object/tvalsval.h"
#include "spells.h"
#include "target.h"

const struct command_list_entry game_cmds[] =
{
	{ CMD_LOADFILE, "load a savefile", { arg_NONE }, NULL, FALSE, 0 },
	{ CMD_NEWGAME, "start a new game", { arg_NONE }, NULL, FALSE, 0 },

	{ CMD_BIRTH_RESET, "go back to the beginning", { arg_NONE }, NULL, FALSE, 0 },
	{ CMD_CHOOSE_SEX, "select sex", { arg_CHOICE }, NULL, FALSE, 0 },
	{ CMD_CHOOSE_RACE, "select race", { arg_CHOICE }, NULL, FALSE, 0 },
	{ CMD_CHOOSE_CLASS, "select class", { arg_CHOICE }, NULL, FALSE, 0 },
 	{ CMD_FINALIZE_OPTIONS, "finalise options", { arg_CHOICE }, NULL, FALSE },
	{ CMD_BUY_STAT, "buy points in a stat", { arg_CHOICE }, NULL, FALSE, 0 },
	{ CMD_SELL_STAT, "sell points in a stat", { arg_CHOICE }, NULL, FALSE, 0 },
	{ CMD_RESET_STATS, "reset stats", { arg_CHOICE }, NULL, FALSE, 0 },
	{ CMD_ROLL_STATS, "roll new stats", { arg_NONE }, NULL, FALSE, 0 },
	{ CMD_PREV_STATS, "use previously rolled stats", { arg_NONE }, NULL, FALSE, 0 },
	{ CMD_NAME_CHOICE, "choose name", { arg_STRING }, NULL, FALSE, 0 },
	{ CMD_ACCEPT_CHARACTER, "accept character", { arg_NONE }, NULL, FALSE, 0 },

	{ CMD_GO_UP, "go up stairs", { arg_NONE }, do_cmd_go_up, FALSE, 0 },
	{ CMD_GO_DOWN, "go down stairs", { arg_NONE }, do_cmd_go_down, FALSE, 0 },
	{ CMD_SEARCH, "search", { arg_NONE }, do_cmd_search, TRUE, 10 },
	{ CMD_TOGGLE_SEARCH, "toggle search mode", { arg_NONE }, do_cmd_toggle_search, FALSE, 0 },
	{ CMD_WALK, "walk", { arg_DIRECTION }, do_cmd_walk, TRUE, 0 },
	{ CMD_RUN, "run", { arg_DIRECTION }, do_cmd_run, FALSE, 0 },
	{ CMD_JUMP, "jump", { arg_DIRECTION }, do_cmd_jump, FALSE, 0 },
	{ CMD_OPEN, "open", { arg_DIRECTION }, do_cmd_open, TRUE, 99 },
	{ CMD_CLOSE, "close", { arg_DIRECTION }, do_cmd_close, TRUE, 99 },
	{ CMD_TUNNEL, "tunnel", { arg_DIRECTION }, do_cmd_tunnel, TRUE, 99 },
	{ CMD_HOLD, "stay still", { arg_NONE }, do_cmd_hold, TRUE, 0 },
	{ CMD_DISARM, "disarm", { arg_DIRECTION }, do_cmd_disarm, TRUE, 99 },
	{ CMD_BASH, "bash", { arg_DIRECTION }, do_cmd_bash, TRUE, 99 },
	{ CMD_ALTER, "alter", { arg_DIRECTION }, do_cmd_alter, TRUE, 99 },
	{ CMD_JAM, "jam", { arg_DIRECTION }, do_cmd_spike, FALSE, 0 },
	{ CMD_REST, "rest", { arg_CHOICE }, do_cmd_rest, FALSE, 0 },
	{ CMD_PATHFIND, "walk", { arg_POINT }, do_cmd_pathfind, FALSE, 0 },
	{ CMD_PICKUP, "pickup", { arg_ITEM }, do_cmd_pickup, FALSE, 0 },
	{ CMD_AUTOPICKUP, "autopickup", { arg_NONE }, do_cmd_autopickup, FALSE, 0 },
	{ CMD_WIELD, "wear or wield", { arg_ITEM, arg_NUMBER }, do_cmd_wield, FALSE, 0 },
	{ CMD_TAKEOFF, "take off", { arg_ITEM }, do_cmd_takeoff, FALSE, 0 },
	{ CMD_DROP, "drop", { arg_ITEM, arg_NUMBER }, do_cmd_drop, FALSE, 0 },
	{ CMD_UNINSCRIBE, "un-inscribe", { arg_ITEM }, do_cmd_uninscribe, FALSE, 0 },
	{ CMD_EAT, "eat", { arg_ITEM }, do_cmd_use, FALSE, 0 },
	{ CMD_QUAFF, "quaff", { arg_ITEM, arg_TARGET }, do_cmd_use, FALSE, 0 },
	{ CMD_USE_ROD, "zap", { arg_ITEM, arg_TARGET }, do_cmd_use, FALSE, 0 },
	{ CMD_USE_STAFF, "use", { arg_ITEM }, do_cmd_use, FALSE, 0 },
	{ CMD_USE_WAND, "aim", { arg_ITEM, arg_TARGET }, do_cmd_use, FALSE, 0 },
	{ CMD_READ_SCROLL, "read", { arg_ITEM, arg_TARGET }, do_cmd_use, FALSE, 0 },
	{ CMD_ACTIVATE, "activate", { arg_ITEM, arg_TARGET }, do_cmd_use, FALSE, 0 },
	{ CMD_REFILL, "refuel with", { arg_ITEM }, do_cmd_refill, FALSE, 0 },
	{ CMD_FIRE, "fire", { arg_ITEM, arg_TARGET }, do_cmd_fire, FALSE, 0 },
	{ CMD_THROW, "throw", { arg_ITEM, arg_TARGET }, do_cmd_throw, FALSE, 0 },
	{ CMD_DESTROY, "ignore", { arg_ITEM }, do_cmd_destroy, FALSE, 0 },
	{ CMD_ENTER_STORE, "go into", { arg_NONE }, do_cmd_store, FALSE, 0 },
	{ CMD_INSCRIBE, "inscribe", { arg_ITEM, arg_STRING }, do_cmd_inscribe, FALSE, 0 },
	{ CMD_STUDY_SPELL, "study", { arg_CHOICE }, do_cmd_study_spell, FALSE, 0 },
	{ CMD_STUDY_BOOK, "study", { arg_ITEM }, do_cmd_study_book, FALSE, 0 },
	{ CMD_CAST, "cast", { arg_CHOICE, arg_TARGET }, do_cmd_cast, FALSE, 0 },
	{ CMD_SELL, "sell", { arg_ITEM, arg_NUMBER }, do_cmd_sell, FALSE, 0 },
	{ CMD_STASH, "stash", { arg_ITEM, arg_NUMBER }, do_cmd_stash, FALSE, 0 },
	{ CMD_BUY, "buy", { arg_CHOICE, arg_NUMBER }, do_cmd_buy, FALSE, 0 },
	{ CMD_RETRIEVE, "retrieve", { arg_CHOICE, arg_NUMBER }, do_cmd_retrieve, FALSE, 0 },
	{ CMD_USE_AIMED, "use", { arg_ITEM, arg_TARGET }, do_cmd_use, FALSE, 0 },
	{ CMD_USE_UNAIMED, "use", { arg_ITEM, arg_TARGET }, do_cmd_use, FALSE, 0 },
	{ CMD_USE_ANY, "use", { arg_ITEM, arg_TARGET }, do_cmd_use, FALSE, 0 },
	{ CMD_SUICIDE, "commit suicide", { arg_NONE }, do_cmd_suicide, FALSE, 0 },
	{ CMD_SAVE, "save", { arg_NONE }, do_cmd_save_game, FALSE, 0 },
	{ CMD_QUIT, "quit", { arg_NONE }, do_cmd_quit, FALSE, 0 },
	{ CMD_HELP, "help", { arg_NONE }, NULL, FALSE, 0 },
	{ CMD_REPEAT, "repeat", { arg_NONE }, NULL, FALSE, 0 },

};

/* Item selector type (everything required for get_item()) */
struct item_selector
{
	cmd_code command;
	const char *type;
	bool (*filter)(const object_type *o_ptr);
	int mode;
};

/** List of requirements for various commands' objects */
struct item_selector item_selector[] =
{
	{ CMD_INSCRIBE, NULL, NULL, (USE_EQUIP | USE_INVEN | USE_FLOOR | IS_HARMLESS) },
	{ CMD_UNINSCRIBE, NULL, obj_has_inscrip, (USE_EQUIP | USE_INVEN | USE_FLOOR) },
	{ CMD_WIELD, NULL, obj_can_wear, (USE_INVEN | USE_FLOOR) },
	{ CMD_TAKEOFF, NULL, obj_can_takeoff, USE_EQUIP },
	{ CMD_DROP, NULL, NULL, (USE_EQUIP | USE_INVEN) },
	{ CMD_THROW, NULL, NULL, (USE_EQUIP | USE_INVEN | USE_FLOOR) },
	{ CMD_FIRE, NULL, obj_can_fire, (USE_INVEN | USE_EQUIP | USE_FLOOR | QUIVER_TAGS) },
	{ CMD_USE_STAFF, "staff",  obj_is_staff, (USE_INVEN | USE_FLOOR | SHOW_FAIL) },
	{ CMD_USE_WAND, "wand", obj_is_wand, (USE_INVEN | USE_FLOOR | SHOW_FAIL) },
	{ CMD_USE_ROD, "rod", obj_is_rod, (USE_INVEN | USE_FLOOR | SHOW_FAIL) },
	{ CMD_ACTIVATE, NULL, obj_is_activatable, (USE_EQUIP | SHOW_FAIL) },
	{ CMD_EAT, "food", obj_is_food, (USE_INVEN | USE_FLOOR) },
	{ CMD_QUAFF, "potion", obj_is_potion, (USE_INVEN | USE_FLOOR) },
	{ CMD_READ_SCROLL, "scroll", obj_is_scroll, (USE_INVEN | USE_FLOOR) },
	{ CMD_REFILL, "fuel source", obj_can_refill, (USE_INVEN | USE_FLOOR) },
	{ CMD_USE_AIMED, NULL, obj_is_used_aimed, (USE_EQUIP |USE_INVEN | USE_FLOOR | SHOW_FAIL | QUIVER_TAGS) },
	{ CMD_USE_UNAIMED, NULL, obj_is_used_unaimed, (USE_EQUIP |USE_INVEN | USE_FLOOR | SHOW_FAIL) },
	{ CMD_USE_ANY, NULL, obj_is_useable, (USE_EQUIP |USE_INVEN | USE_FLOOR | SHOW_FAIL | QUIVER_TAGS) },

};

const char *game_command_verb(cmd_code cmd)
{
	size_t i;
	for (i = 0; i < N_ELEMENTS(game_cmds); i++) {
		if (game_cmds[i].cmd == cmd)
			return game_cmds[i].verb;
	}
	return NULL;
}

/* Return the index of the given command in the command array. */
int game_command_index(cmd_code code)
{
	size_t i;

	for (i = 0; i < N_ELEMENTS(game_cmds); i++)
	{
		if (game_cmds[i].cmd == code)
			return i;
	}

	return -1;
}

bool game_command_item_selector(cmd_code command)
{
	int i;

	int item;
	const char *type;
	const char *type2;
	const char *verb;

	char prompt[1024], none[1024];

	verb = game_command_verb(command);

	for (i = 0; i < N_ELEMENTS(item_selector); i++)
	{
		struct item_selector *is = &item_selector[i];

		if (is->command != command)
			continue;

		type = is->type;
		type2 = is->type;

		/* Pluralise correctly or things look weird */
		if (!type) {
			type = "item";
			type2 = "items";
		}

		strnfmt(prompt, sizeof(prompt), "%s which %s?", verb, type);
		strnfmt(none, sizeof(none), "You have no %s you can %s.",
				type2, verb);

		item_tester_hook = is->filter;
		if (!get_item(&item, prompt, none, command, is->mode))
			return FALSE;

		cmd_set_arg_item(cmd, 0, item);

		break;
	}
	return TRUE;
}

bool game_command_validate(game_command *cmd)
{
	int idx = game_command_index(cmd->command);

	if ((game_cmds[idx].arg_type[0] == arg_ITEM) && cmd->arg_present[0]) {
		object_type *o_ptr = object_from_item_idx(cmd->arg[0].item);
		if (!o_ptr->kind)
			return FALSE;
	}
	return TRUE;
}

bool game_command_check(game_command *cmd)
{
	switch (cmd->command)
	{
		case CMD_INSCRIBE:
		{
			char o_name[80];
			char tmp[80] = "";

			object_type *o_ptr = object_from_item_idx(cmd->arg[0].item);
			
			object_desc(o_name, sizeof(o_name), o_ptr, ODESC_PREFIX | ODESC_FULL);
			msg("Inscribing %s.", o_name);
			message_flush();
			
			/* Use old inscription */
			if (o_ptr->note)
				strnfmt(tmp, sizeof(tmp), "%s", quark_str(o_ptr->note));
			
			/* Get a new inscription (possibly empty) */
			if (!get_string("Inscription: ", tmp, sizeof(tmp)))
				return;

			cmd_set_arg_string(cmd, 1, tmp);
			break;
		}

		case CMD_OPEN:
		{
			if (OPT(easy_open) && (!cmd->arg_present[0] ||
					cmd->arg[0].direction == DIR_UNKNOWN))
			{
				int y, x;
				int n_closed_doors, n_locked_chests;
			
				n_closed_doors = count_feats(&y, &x, cave_iscloseddoor, FALSE);
				n_locked_chests = count_chests(&y, &x, FALSE);
			
				if (n_closed_doors + n_locked_chests == 1)
					cmd_set_arg_direction(cmd, 0, coords_to_dir(y, x));
			}

			goto get_dir;
		}

		case CMD_CLOSE:
		{
			if (OPT(easy_open) && (!cmd->arg_present[0] ||
					cmd->arg[0].direction == DIR_UNKNOWN))
			{
				int y, x;
			
				/* Count open doors */
				if (count_feats(&y, &x, cave_isopendoor, FALSE) == 1)
					cmd_set_arg_direction(cmd, 0, coords_to_dir(y, x));
			}

			goto get_dir;
		}

		case CMD_DISARM:
		{
			if (OPT(easy_open) && (!cmd->arg_present[0] ||
					cmd->arg[0].direction == DIR_UNKNOWN))
			{
				int y, x;
				int n_visible_traps, n_trapped_chests;
			
				n_visible_traps = count_feats(&y, &x, cave_isknowntrap, TRUE);
				n_trapped_chests = count_chests(&y, &x, TRUE);

				if (n_visible_traps + n_trapped_chests == 1)
					cmd_set_arg_direction(cmd, 0, coords_to_dir(y, x));
			}

			goto get_dir;
		}

		case CMD_TUNNEL:
		case CMD_WALK:
		case CMD_RUN:
		case CMD_JUMP:
		case CMD_BASH:
		case CMD_ALTER:
		case CMD_JAM:
		{
		get_dir:

			/* Direction hasn't been specified, so we ask for one. */
			if (!cmd->arg_present[0] ||
					cmd->arg[0].direction == DIR_UNKNOWN)
			{
				int dir;
				if (!get_rep_dir(&dir))
					return;

				cmd_set_arg_direction(cmd, 0, dir);
			}
				
			break;
		}

		case CMD_DROP:
		{
			if (!cmd->arg_present[1])
			{
				object_type *o_ptr = object_from_item_idx(cmd->arg[0].item);
				int amt = get_quantity(NULL, o_ptr->number);
				if (amt <= 0)
					return;

				cmd_set_arg_number(cmd, 1, amt);
			}

			break;
		}
			
		/* 
		 * These take an item number and a  "target" as arguments, 
		 * though a target isn't always actually needed, so we'll 
		 * only prompt for it via callback if the item being used needs it.
		 */
		case CMD_USE_WAND:
		case CMD_USE_ROD:
		case CMD_QUAFF:
		case CMD_ACTIVATE:
		case CMD_READ_SCROLL:
		case CMD_FIRE:
		case CMD_THROW:
		case CMD_USE_ANY:
		case CMD_USE_AIMED:
		case CMD_USE_UNAIMED:
		{
			bool get_target = FALSE;
			object_type *o_ptr = object_from_item_idx(cmd->arg[0].choice);

			/* If we couldn't resolve the item, then abort this */
			if (!o_ptr->kind) break;

			/* Thrown objects always need an aim, others might, depending
			 * on the object */
			if (obj_needs_aim(o_ptr) || cmd->command == CMD_THROW)
			{
				if (!cmd->arg_present[1])
					get_target = TRUE;

				if (cmd->arg[1].direction == DIR_UNKNOWN)
					get_target = TRUE;

				if (cmd->arg[1].direction == DIR_TARGET && !target_okay())
					get_target = TRUE;
			}

			if (get_target && !get_aim_dir(&cmd->arg[1].direction))
					return;

			player_confuse_dir(p_ptr, &cmd->arg[1].direction, FALSE);
			cmd->arg_present[1] = TRUE;

			break;
		}
			
		/* This takes a choice and a direction. */
		case CMD_CAST:
		{
			bool get_target = FALSE;

			if (spell_needs_aim(p_ptr->class->spell_book, cmd->arg[0].choice))
			{
				if (!cmd->arg_present[1])
					get_target = TRUE;

				if (cmd->arg[1].direction == DIR_UNKNOWN)
					get_target = TRUE;

				if (cmd->arg[1].direction == DIR_TARGET && !target_okay())
					get_target = TRUE;
			}

			if (get_target && !get_aim_dir(&cmd->arg[1].direction))
					return;

			player_confuse_dir(p_ptr, &cmd->arg[1].direction, FALSE);
			cmd->arg_present[1] = TRUE;
				
			break;
		}

		case CMD_WIELD:
		{
			object_type *o_ptr = object_from_item_idx(cmd->arg[0].choice);
			int slot = wield_slot(o_ptr);
			
			/* Usually if the slot is taken we'll just replace the item in the slot,
			 * but in some cases we need to ask the user which slot they actually
			 * want to replace */
			if (p_ptr->inventory[slot].kind)
			{
				if (o_ptr->tval == TV_RING)
				{
					const char *q = "Replace which ring? ";
					const char *s = "Error in obj_wield, please report";
					item_tester_hook = obj_is_ring;
					if (!get_item(&slot, q, s, CMD_WIELD, USE_EQUIP)) return;
				}
			
				if (obj_is_ammo(o_ptr) && !object_similar(&p_ptr->inventory[slot],
					o_ptr, OSTACK_QUIVER))
				{
					const char *q = "Replace which ammunition? ";
					const char *s = "Error in obj_wield, please report";
					item_tester_hook = obj_is_ammo;
					if (!get_item(&slot, q, s, CMD_WIELD, USE_EQUIP)) return;
				}
			}

			/* Set relevant slot */
			cmd_set_arg_number(cmd, 1, slot);

			break;
		}

		default: 
		{
			/* I can see the point of the compiler warning, but still... */
			return FALSE;
		}
	}
	return TRUE;
}
