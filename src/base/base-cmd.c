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

#include "angbase.h"
#include "base-cmd.h"
#include "direction.h"

errr (*cmd_get_hook)(cmd_context c, bool wait);

#define CMD_QUEUE_SIZE 20
#define prev_cmd_idx(idx) ((idx + CMD_QUEUE_SIZE - 1) % CMD_QUEUE_SIZE)

static int cmd_head = 0;
static int cmd_tail = 0;
static game_command cmd_queue[CMD_QUEUE_SIZE];

static bool repeat_prev_allowed = FALSE;
static bool repeating = FALSE;


game_command *cmd_get_top(void)
{
	return &cmd_queue[prev_cmd_idx(cmd_head)];
}


/*
 * Insert the given command into the command queue.
 */
errr cmd_insert_s(game_command *cmd)
{
	/* If queue full, return error */
	if (cmd_head + 1 == cmd_tail) return 1;
	if (cmd_head + 1 == CMD_QUEUE_SIZE && cmd_tail == 0) return 1;

	/* Insert command into queue. */
	if (cmd->command != CMD_REPEAT)
	{
		cmd_queue[cmd_head] = *cmd;
	}
	else
	{
		int cmd_prev = cmd_head - 1;

		if (!repeat_prev_allowed) return 1;

		/* If we're repeating a command, we duplicate the previous command 
		   in the next command "slot". */
		if (cmd_prev < 0) cmd_prev = CMD_QUEUE_SIZE - 1;
		
		if (cmd_queue[cmd_prev].command != CMD_NULL)
			cmd_queue[cmd_head] = cmd_queue[cmd_prev];
	}

	/* Advance point in queue, wrapping around at the end */
	cmd_head++;
	if (cmd_head == CMD_QUEUE_SIZE) cmd_head = 0;

	return 0;	
}

/*
 * Get the next game command, with 'wait' indicating whether we
 * are prepared to wait for a command or require a quick return with
 * no command.
 */
errr cmd_get(cmd_context c, game_command **cmd, bool wait)
{
	/* If we're repeating, just pull the last command again. */
	if (repeating)
	{
		*cmd = &cmd_queue[prev_cmd_idx(cmd_tail)];
		return 0;
	}

	/* If there are no commands queued, ask the UI for one. */
	if (cmd_head == cmd_tail) 
		cmd_get_hook(c, wait);

	/* If we have a command ready, set it and return success. */
	if (cmd_head != cmd_tail)
	{
		*cmd = &cmd_queue[cmd_tail++];
		if (cmd_tail == CMD_QUEUE_SIZE) cmd_tail = 0;

		return 0;
	}

	/* Failure to get a command. */
	return 1;
}

/* Return the index of the given command in the command array. */
static int cmd_idx(cmd_code code)
{
	size_t i;

	for (i = 0; i < N_ELEMENTS(game_cmds); i++)
	{
		if (game_cmds[i].cmd == code)
			return i;
	}

	return -1;
}

void cmd_set_arg_choice(game_command *cmd, int n, int choice)
{
	int idx = cmd_idx(cmd->command);

	assert(n <= CMD_MAX_ARGS);
	assert(game_cmds[idx].arg_type[n] & arg_CHOICE);

	cmd->arg[n].choice = choice;
	cmd->arg_type[n] = arg_CHOICE;
	cmd->arg_present[n] = TRUE;
}

void cmd_set_arg_string(game_command *cmd, int n, const char *str)
{
	int idx = cmd_idx(cmd->command);

	assert(n <= CMD_MAX_ARGS);
	assert(game_cmds[idx].arg_type[n] & arg_STRING);

	cmd->arg[n].string = string_make(str);
	cmd->arg_type[n] = arg_STRING;
	cmd->arg_present[n] = TRUE;
}

void cmd_set_arg_direction(game_command *cmd, int n, int dir)
{
	int idx = cmd_idx(cmd->command);

	assert(n <= CMD_MAX_ARGS);
	assert(game_cmds[idx].arg_type[n] & arg_DIRECTION);

	cmd->arg[n].direction = dir;
	cmd->arg_type[n] = arg_DIRECTION;
	cmd->arg_present[n] = TRUE;
}

void cmd_set_arg_target(game_command *cmd, int n, int target)
{
	int idx = cmd_idx(cmd->command);

	assert(n <= CMD_MAX_ARGS);
	assert(game_cmds[idx].arg_type[n] & arg_TARGET);

	cmd->arg[n].direction = target;
	cmd->arg_type[n] = arg_TARGET;
	cmd->arg_present[n] = TRUE;
}

void cmd_set_arg_point(game_command *cmd, int n, int x, int y)
{
	int idx = cmd_idx(cmd->command);

	assert(n <= CMD_MAX_ARGS);
	assert(game_cmds[idx].arg_type[n] & arg_POINT);

	cmd->arg[n].point.x = x;
	cmd->arg[n].point.y = y;
	cmd->arg_type[n] = arg_POINT;
	cmd->arg_present[n] = TRUE;
}

void cmd_set_arg_item(game_command *cmd, int n, int item)
{
	int idx = cmd_idx(cmd->command);

	assert(n <= CMD_MAX_ARGS);
	assert(game_cmds[idx].arg_type[n] & arg_ITEM);

	cmd->arg[n].item = item;
	cmd->arg_type[n] = arg_ITEM;
	cmd->arg_present[n] = TRUE;
}

void cmd_set_arg_number(game_command *cmd, int n, int num)
{
	int idx = cmd_idx(cmd->command);

	assert(n <= CMD_MAX_ARGS);
	assert(game_cmds[idx].arg_type[n] & arg_NUMBER);

	cmd->arg[n].number = num;
	cmd->arg_type[n] = arg_NUMBER;
	cmd->arg_present[n] = TRUE;
}

/*
 * Inserts a command in the queue to be carried out, with the given
 * number of repeats.
 */
errr cmd_insert_repeated(cmd_code c, int nrepeats)
{
	game_command cmd = { 0 };

	if (cmd_idx(c) == -1)
		return 1;

	cmd.command = c;
	cmd.nrepeats = nrepeats;

	return cmd_insert_s(&cmd);
}

/* 
 * Inserts a command in the queue to be carried out. 
 */
errr cmd_insert(cmd_code c)
{
	return cmd_insert_repeated(c, 0);
}


/* 
 * Request a game command from the uI and carry out whatever actions
 * go along with it.
 */
void process_command(cmd_context ctx, bool no_request)
{
	game_command *cmd;

	/* If we've got a command to process, do it. */
	if (cmd_get(ctx, &cmd, !no_request) == 0)
	{
		int oldrepeats = cmd->nrepeats;
		int idx = game_command_index(cmd->command);
		size_t i;

		if (idx == -1) return;

		if ((game_cmds[idx].arg_type[0] == arg_ITEM) && !cmd->arg_present[0])) {
			if (!game_command_item_selector(cmd->command)) {
				return;
			}
		}


		/* XXX avoid dead objects from being re-used on repeat.
		 * this needs to be expanded into a general safety-check
		 * on args */
		if (!game_command_validate(cmd)) {
			return;
		}

		/* Do some sanity checking on those arguments that might have 
		   been declared as "unknown", such as directions and targets. */
		if (!game_command_check(cmd)) {
			return;
		}

		/* Command repetition */
		if (game_cmds[idx].repeat_allowed)
		{
			/* Auto-repeat only if there isn't already a repeat length. */
			if (game_cmds[idx].auto_repeat_n > 0 && cmd->nrepeats == 0)
				cmd_set_repeat(game_cmds[idx].auto_repeat_n);
		}
		else
		{
			cmd->nrepeats = 0;
			repeating = FALSE;
		}

		/* 
		 * The command gets to unset this if it isn't appropriate for
		 * the user to repeat it.
		 */
		repeat_prev_allowed = TRUE;

		if (game_cmds[idx].fn)
			game_cmds[idx].fn(cmd->command, cmd->arg);

		/* If the command hasn't changed nrepeats, count this execution. */
		if (cmd->nrepeats > 0 && oldrepeats == cmd_get_nrepeats())
			cmd_set_repeat(oldrepeats - 1);
	}
}

/* 
 * Remove any pending repeats from the current command. 
 */
void cmd_cancel_repeat(void)
{
	game_command *cmd = &cmd_queue[prev_cmd_idx(cmd_tail)];

	if (cmd->nrepeats || repeating)
	{
		/* Cancel */
		cmd->nrepeats = 0;
		repeating = FALSE;
		
		/* Redraw the state (later) */
		p_ptr->redraw |= (PR_STATE);
	}
}

/* 
 * Update the number of repeats pending for the current command. 
 */
void cmd_set_repeat(int nrepeats)
{
	game_command *cmd = &cmd_queue[prev_cmd_idx(cmd_tail)];

	cmd->nrepeats = nrepeats;
	if (nrepeats) repeating = TRUE;
	else repeating = FALSE;

	/* Redraw the state (later) */
	p_ptr->redraw |= (PR_STATE);
}

/* 
 * Return the number of repeats pending for the current command. 
 */
int cmd_get_nrepeats(void)
{
	game_command *cmd = &cmd_queue[prev_cmd_idx(cmd_tail)];
	return cmd->nrepeats;
}

/*
 * Do not allow the current command to be repeated by the user using the
 * "repeat last command" command.
 */
void cmd_disable_repeat(void)
{
	repeat_prev_allowed = FALSE;
}
