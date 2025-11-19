/**
 * @file: infobox.h
 * @desc: Information box (HUD) interface - displays coins and timer
 * @auth: James Farrelly (C24402114)
 */

#ifndef INFOBOX_H_
#define INFOBOX_H_

/**
 * Initialize the information box (HUD) display
 */
void init_infobox(void);

/**
 * Update infobox display and timer countdown
 */
void step_infobox(void);

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

#endif // !INFOBOX_H_
