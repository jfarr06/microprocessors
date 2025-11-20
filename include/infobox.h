/**
 * @file: infobox.h
 * @desc: Information box (HUD) interface - displays coins and timer
 * @auth: James Farrelly (C24402114)
 */

#ifndef INFOBOX_H_
#define INFOBOX_H_

#include <stdint.h>
#include <stdbool.h>

#define RUNNING_STATUS_RUN  (0x0) /* The game is running. */
#define RUNNING_STATUS_WIN  (0x1) /* The game has been won. */
#define RUNNING_STATUS_LOSS (0x2) /* The game has been lost. */

/**
 * Initialize the information box (HUD) display
 */
void init_infobox(void);

/**
 * Update infobox display and timer countdown
 */
void step_infobox(void);

/**
 * Renders the infobox now
 */
void render_infobox(void);

/**
 * Set the target number of coins to collect
 * @param coins Target coin count
 */
void set_infobox_coins(uint8_t coins);

/**
 * Increment the current coin count by one
 */
void inc_infobox_coins(void);

/**
 * Set the starting time for the countdown timer
 * @param time Starting time in seconds
 */
void set_infobox_start_time(uint16_t time);

/**
 * Gets the running status of the game.
 */
uint8_t get_running_status(void);

/**
 * Get the current remaining time in seconds
 * @return Remaining time in seconds, 0 if no timer
 */
uint16_t get_infobox_time_remaining(void);

#endif // !INFOBOX_H_
