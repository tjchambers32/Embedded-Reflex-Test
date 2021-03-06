/*
 * ReflexTestData.h
 *
 *  Created on: Jun 3, 2015
 *      Author: travis
 */

#ifndef REFLEXTESTDATA_H_
#define REFLEXTESTDATA_H_

#include <stdint.h>
#include <stdbool.h>

// Uncomment the line below to enable debug output.
// #define REFLEXTESTDATA_DEBUG

#define REFLEXTESTDATA_NUMBER_OF_HIGH_SCORES 10
#define REFLEXTESTDATA_ERROR -1
#define REFLEXTESTDATA_SEQUENCE_LENGTH 10
#define REFLEXTESTDATA_NUMBER_OF_LEDS 4

//state machine
typedef enum reflexTest_st {
  init_st,                // init state
  show_info_st,           // display instructions on the lcd
  wait_info_st,           // Wait state for showing instructions
  wait_five_seconds_st,   // wait_five_seconds before blinking an led
  blank_screen_st,        // Blanks the screen to indicate game start
  blink_led_st,           // blink an led
  wait_for_button_st,     // wait for the user to press a button
  button_pressed_st,      // user pressed the correct button.
  wait_between_flash_st,  // wait one second
  show_stats_st,          // show user's fastest, slowest, and average time
  wait_stats_st,          // Wait state for displaying stats
  update_scores_st,       // compute average and rank score
} ReflexTest_st;

/**
 * Initialization function. Should initialize the index, high scores, etc.
 */
void ReflexTestData_Init(void);

/**
 * Sets the current state to the parameter passed in.
 * @param newState The Reflex Data state to set the currentState to.
 */
void ReflexTestData_SetCurrentState(ReflexTest_st newState);

/**
 * Returns the current state.
 * @return  The current ReflexTest state.
 */
ReflexTest_st ReflexTestData_GetCurrentState(void);

/**
 * Generates a random sequence of REFLEXTESTDATA_SEQUENCE_LENGTH and Returns
 * a pointer to the preallocated array. This array memory does NOT need to below
 * deallocated.
 * @param  seed The seed to seed srand() with for generating random numbers.
 * @return      A pointer to the array containing the sequences.
 */
uint32_t* ReflexTestData_GenerateSequence(int32_t seed);

/**
 * Returns the LED # to flash in the sequence. Used by LED Model.
 * @return  The value of the LED to flash.
 */
int32_t ReflexTestData_GetLed(void);

/**
 * Increments the internal index.
 */
void ReflexTestData_IncrementIndex();

/**
 * Returns the current index of the sequence.
 * @return The current index of the sequence.
 */
int32_t ReflexTestData_GetCurrentIndex();

/**
 * Checks to see if entire sequence has already been shown. Used by LED Model.
 * @return  TRUE if the sequence is over.
 */
bool ReflexTestData_IsSequenceDone(void);

/**
 * Gets the value of the pressed button. Used by the button model.
 * @return  Return the value of the pressed button.
 */
int32_t ReflexTestData_GetPressedButton(void);

/**
 * Returns true if a button is currently pressed.
 * @return  TRUE if a button is currently being pressed.
 */
bool ReflexTestData_IsCorrectButtonPressed(void);

/**
 * Sets the value of the pressed button. Used by the button model.
 * @param value The value to set the pressed button to.
 */
void ReflexTestData_SetPressedButton(int32_t value);

/**
 * Returns the response time between a flashed LED and a pushed button.
 * Used by the timer model.
 * @return  The time between a flashed LED and a pushed button.
 */
double ReflexTestData_GetResponseTime(void);

/**
 * Saves the passed-in reponse time as the most recent response.
 * @param responseTime The value to store as the most recent response.
 */
void ReflexTestData_SetResponseTime(double inputTime);

/**
 * Updates the high scores list.
 */
void ReflexTestData_UpdateScores();

/**
 * Sets the minimum response time.
 * @param responseTime the minimum response time from the player's round.
 */
void ReflexTestData_SetMinResponseTime(double responseTime);

/**
 * The minimum response time from a player's round.
 * @return The minimum response time from a player's round.
 */
double ReflexTestData_GetMinResponseTime();

/**
 * Sets the maximum response time.
 * @param responseTime the maximum response time from the player's round.
 */
void ReflexTestData_SetMaxResponseTime(double responseTime);

/**
 * The maximum response time from a player's round.
 * @return The maximum response time from a player's round.
 */
double ReflexTestData_GetMaxResponseTime();

/**
 * Sets the average response time.
 * @param responseTime the average response time from the player's round.
 */
void ReflexTestData_SetAverageResponseTime(double responseTime);

/**
 * The average response time from a player's round.
 * @return The average response time from a player's round.
 */
double ReflexTestData_GetAverageResponseTime();

/**
 * The 10 highest scores since the board has been turned on.
 * @return A pointer to the array containing 10 highest scores.
 */
double* ReflexTestData_GetHighScores();

#endif /* REFLEXTESTDATA_H_ */
