#ifndef STRINGS_H_
#define STRINGS_H_

#define LEN(x) (sizeof(x) - 1)                 // Helper macro to get the length of a string literal at compile time (without null terminator)
#define STRING(x) STR_##x, STR_##x##_LEN       // Helper macro to expand to "string, string_length" for use in parameter lists

// ===== Mode select option labels =====

#define STR_SELECT_CURRENT_OPTION     ">"     // Text label for the current selected option
#define STR_SELECT_CURRENT_OPTION_LEN LEN(STR_SELECT_CURRENT_OPTION) // Length of the current selected option label

#define STR_MODE_SELECT_EASY       "Easy"      // Text label for the "Easy" game mode option
#define STR_MODE_SELECT_EASY_LEN   LEN(STR_MODE_SELECT_EASY)   // Length of the "Easy" game mode option label

#define STR_MODE_SELECT_MEDIUM     "Medium"    // Text label for the "Medium" game mode option
#define STR_MODE_SELECT_MEDIUM_LEN LEN(STR_MODE_SELECT_MEDIUM) // Length of the "Medium" game mode option label

#define STR_MODE_SELECT_HARD       "Hard"      // Text label for the "Hard" game mode option
#define STR_MODE_SELECT_HARD_LEN   LEN(STR_MODE_SELECT_HARD)   // Length of the "Hard" game mode option label

#define STR_MODE_SELECT_ENDLESS       "Endless"    // Text label for the "Endless" game mode option
#define STR_MODE_SELECT_ENDLESS_LEN   LEN(STR_MODE_SELECT_ENDLESS) // Length of the "Endless" game mode option label

#define STR_MODE_SELECT_RANDOM       "Random"    // Text label for the "Random" game mode option
#define STR_MODE_SELECT_RANDOM_LEN   LEN(STR_MODE_SELECT_RANDOM) // Length of the "Random" game mode option label

#define STR_MODE_SELECT_RETURN       "Return"    // Text label for the "Return" option that goes back to the main menu
#define STR_MODE_SELECT_RETURN_LEN   LEN(STR_MODE_SELECT_RETURN) // Length of the "Return" option label


// ===== Home menu strings =====

#define STR_MENU_TITLE_TOP        "DUNGEON"    // Top line of the game title on the home menu ("DUNGEON")
#define STR_MENU_TITLE_TOP_LEN    LEN(STR_MENU_TITLE_TOP) // Length of the top title line

#define STR_MENU_TITLE_BOTTOM     "CRAWLER"    // Bottom line of the game title on the home menu ("CRAWLER")
#define STR_MENU_TITLE_BOTTOM_LEN LEN(STR_MENU_TITLE_BOTTOM) // Length of the bottom title line

#define STR_MENU_PLAY             ">Play<"     // Label for the play button on the home menu (shown as ">Play<")
#define STR_MENU_PLAY_LEN         LEN(STR_MENU_PLAY) // Length of the play button label


// ===== Mode select header =====

#define STR_MODE_SELECT_TITLE     "Game Mode"  // Header text shown at the top of the mode select screen
#define STR_MODE_SELECT_TITLE_LEN LEN(STR_MODE_SELECT_TITLE) // Length of the mode select header text


// ===== Shared description fragments (used in mode descriptions) =====

#define STR_DESC_LOTS_OF_COINS      "Lots of coins,"   // Description fragment: generous amount of coins
#define STR_DESC_LOTS_OF_COINS_LEN  LEN(STR_DESC_LOTS_OF_COINS) // Length of the "Lots of coins," description fragment

#define STR_DESC_REDUCED_COINS      "Reduced coins,"   // Description fragment: reduced amount of coins
#define STR_DESC_REDUCED_COINS_LEN  LEN(STR_DESC_REDUCED_COINS) // Length of the "Reduced coins," description fragment

#define STR_DESC_LOTS_OF_TIME       "lots of time!"    // Description fragment: plenty of time available
#define STR_DESC_LOTS_OF_TIME_LEN   LEN(STR_DESC_LOTS_OF_TIME) // Length of the "lots of time!" description fragment

#define STR_DESC_REDUCED_TIME       "reduced time!"    // Description fragment: time limit is reduced
#define STR_DESC_REDUCED_TIME_LEN   LEN(STR_DESC_REDUCED_TIME) // Length of the "reduced time!" description fragment

#define STR_DESC_NO_TIME_LIMIT      "no time limit!"   // Description fragment: no time limit in this mode
#define STR_DESC_NO_TIME_LIMIT_LEN  LEN(STR_DESC_NO_TIME_LIMIT) // Length of the "no time limit!" description fragment

#define STR_DESC_RANDOM_COINS       "Random coins,"    // Description fragment: random coin distribution
#define STR_DESC_RANDOM_COINS_LEN   LEN(STR_DESC_RANDOM_COINS) // Length of the "Random coins," description fragment

#define STR_DESC_RANDOM_TIME_LIMIT      "random time limit!"   // Description fragment: random time limit
#define STR_DESC_RANDOM_TIME_LIMIT_LEN  LEN(STR_DESC_RANDOM_TIME_LIMIT) // Length of the "random time limit!" description fragment

#define STR_DESC_RETURN_TO_MENU      "Return to menu"  // Description line for the "Return" menu option
#define STR_DESC_RETURN_TO_MENU_LEN  LEN(STR_DESC_RETURN_TO_MENU) // Length of the "Return to menu" description line

// ===== Infobox / HUD strings =====

#define STR_INFOBOX_COINS_SEPARATOR  "/"          // Separator between current coins and target coins (e.g., "1/5")
#define STR_INFOBOX_COINS_SEPARATOR_LEN LEN(STR_INFOBOX_COINS_SEPARATOR) // Length of the coins separator string

#define STR_INFOBOX_TITLE_COINS   "Coins"      // Title text for the coins section of the infobox
#define STR_INFOBOX_TITLE_COINS_LEN LEN(STR_INFOBOX_TITLE_COINS) // Length of the coins title text

#define STR_INFOBOX_TITLE_TIME    "Time"       // Title text for the time section of the infobox
#define STR_INFOBOX_TITLE_TIME_LEN LEN(STR_INFOBOX_TITLE_TIME)  // Length of the time title text

#define STR_INFOBOX_TIME_SUFFIX_S "s"          // Suffix character for seconds in the time display
#define STR_INFOBOX_TIME_SUFFIX_S_LEN LEN(STR_INFOBOX_TIME_SUFFIX_S) // Length of the seconds suffix

#define STR_INFOBOX_TIME_INF      "Inf"        // Text shown when time is infinite
#define STR_INFOBOX_TIME_INF_LEN  LEN(STR_INFOBOX_TIME_INF)    // Length of the infinite time text

// ===== Pause menu =====

#define STR_PAUSE_TITLE            "Paused"    // Title text for the pause menu
#define STR_PAUSE_TITLE_LEN        LEN(STR_PAUSE_TITLE) // Length of the pause menu title text

#define STR_PAUSE_OPTION_RESUME    "Resume"    // Pause menu option: resume the game
#define STR_PAUSE_OPTION_RESUME_LEN LEN(STR_PAUSE_OPTION_RESUME) // Length of the "Resume" option label

#define STR_PAUSE_OPTION_RESTART   "Restart"  // Pause menu option: restart the current run
#define STR_PAUSE_OPTION_RESTART_LEN LEN(STR_PAUSE_OPTION_RESTART) // Length of the "Restart" option label

#define STR_PAUSE_OPTION_HOME_MENU "Home Menu" // Pause menu option: return to the home menu
#define STR_PAUSE_OPTION_HOME_MENU_LEN LEN(STR_PAUSE_OPTION_HOME_MENU) // Length of the "Home Menu" option label

// ===== Status menu (game_menu.c) =====

#define STR_STATUS_YOU                "You"             // Status screen label: "You"
#define STR_STATUS_YOU_LEN            LEN(STR_STATUS_YOU) // Length of "You"

#define STR_STATUS_WIN                "Win"             // Status screen label for a win
#define STR_STATUS_WIN_LEN            LEN(STR_STATUS_WIN) // Length of "Win"

#define STR_STATUS_LOSE               "Lose"            // Status screen label for a loss
#define STR_STATUS_LOSE_LEN           LEN(STR_STATUS_LOSE) // Length of "Lose"

#define STR_STATUS_PRESS_ENTER_LINE1  "Press enter to" // First line of prompt to return to home
#define STR_STATUS_PRESS_ENTER_LINE1_LEN LEN(STR_STATUS_PRESS_ENTER_LINE1) // Length of "Press enter to"

#define STR_STATUS_PRESS_ENTER_LINE2  "return to home." // Second line of prompt to return to home
#define STR_STATUS_PRESS_ENTER_LINE2_LEN LEN(STR_STATUS_PRESS_ENTER_LINE2) // Length of "return to home."

#endif // !STRINGS_H_
