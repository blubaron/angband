#ifndef INCLUDED_CMD_CODE_H
#define INCLUDED_CMD_CODE_H

/*
 * All valid game commands.  Not all implemented yet.
 */
typedef enum cmd_code
{
	CMD_NULL = 0,	/* A "do nothing" command so that there's something
					   UIs can use as a "no command yet" sentinel. */
	/* 
	 * Splash screen commands 
	 */
	CMD_LOADFILE,
	CMD_NEWGAME,

	/* 
	 * Birth commands 
	 */
	CMD_BIRTH_RESET,
	CMD_CHOOSE_SEX,
	CMD_CHOOSE_RACE,
	CMD_CHOOSE_CLASS,
	CMD_FINALIZE_OPTIONS,
	CMD_BUY_STAT,
	CMD_SELL_STAT,
	CMD_RESET_STATS,
	CMD_ROLL_STATS,
	CMD_PREV_STATS,
	CMD_NAME_CHOICE,
	CMD_ACCEPT_CHARACTER,

	/* 
	 * The main game commands
	 */
	CMD_GO_UP,
	CMD_GO_DOWN,
	CMD_SEARCH,
	CMD_TOGGLE_SEARCH,
	CMD_WALK,
	CMD_JUMP,
	CMD_PATHFIND,

	CMD_INSCRIBE,
	CMD_UNINSCRIBE,
	CMD_TAKEOFF,
	CMD_WIELD,
	CMD_DROP,
	CMD_BROWSE_SPELL,
	CMD_STUDY_SPELL,
	CMD_STUDY_BOOK,
	CMD_CAST, /* Casting a spell /or/ praying. */
	CMD_USE_STAFF,
	CMD_USE_WAND,
	CMD_USE_ROD,
	CMD_ACTIVATE,
	CMD_EAT,
	CMD_QUAFF,
	CMD_READ_SCROLL,
	CMD_REFILL,
	CMD_FIRE,
	CMD_THROW,
	CMD_PICKUP,
	CMD_AUTOPICKUP,
	CMD_DESTROY,
/*	CMD_SQUELCH_TYPE, -- might be a command, might have another interface */
	CMD_DISARM,
	CMD_REST,
/*	CMD_TARGET, -- possibly should be a UI-level thing */
	CMD_TUNNEL,
	CMD_OPEN,
	CMD_CLOSE,
	CMD_JAM,
	CMD_BASH,
	CMD_RUN,
	CMD_HOLD,
	CMD_ENTER_STORE,
	CMD_ALTER,

    /* Store commands */	
	CMD_SELL,
	CMD_BUY,
	CMD_STASH,
	CMD_RETRIEVE,

  /* commands added by Brett */	
  /* use a rod, wand, or fire ammo */
  CMD_USE_AIMED,
  /* use a staff, scroll, potion, or food */
  CMD_USE_UNAIMED,
  /* use a any useable item */
  CMD_USE_ANY,

	/* Hors categorie Commands */
	CMD_SUICIDE,
	CMD_SAVE,

/*	CMD_OPTIONS, -- probably won't be a command in this sense*/
	CMD_QUIT,
	CMD_HELP,
	CMD_REPEAT
}
cmd_code;

typedef enum cmd_context
{
	CMD_INIT,
	CMD_BIRTH,
	CMD_GAME,
	CMD_STORE,
	CMD_DEATH
} cmd_context;

#endif
