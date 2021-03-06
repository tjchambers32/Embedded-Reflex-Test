#include "Executor.h"
#include "ButtonConductor.h"
#include "LedConductor.h"
#include "TimerConductor.h"
#include "LcdConductor.h"
#include "ReflexTestData.h"
#include <stdlib.h>
#include <stdio.h>

#define TICK_PERIOD       1                       // 1ms tick period
#define FIVE_SECOND_WAIT  (5000 / (TICK_PERIOD))  // 5000ms = 5s
#define TWO_SECOND_WAIT   (2000 / (TICK_PERIOD))
#define ONE_SECOND_WAIT   (1000 / (TICK_PERIOD))

/**
 * This is a helper function that can print out the current state of the SM.
 * This only prints out the state when the state changes, and can be useful
 * in visually seeing how the state transitions are happening.
 */
void verboseStatePrint(ReflexTest_st currentState) {
  static ReflexTest_st previousState;
  static bool firstPass = true;
  // Only print the message if:
  // 1. This the first pass and the value for previousState is unknown.
  // 2. previousState != currentState - this prevents reprinting the same stat.
  if (previousState != currentState || firstPass) {
    firstPass = false;     // previousState will be defined, firstPass is false.
    previousState = currentState; // keep track of the last state you were in.
    switch(currentState) {  // prints info based on the state that you're in.
      case init_st:
        printf("init_st\n\r");
        break;
      case show_info_st:
        printf("show_info_st\n\r");
        break;
      case wait_info_st:
        printf("wait_info_st\n\r");
        break;
      case wait_five_seconds_st:
        printf("wait_five_seconds_st\n\r");
        break;
      case blank_screen_st:
        printf("blank_screen_st\n\r");
        break;
      case blink_led_st:
        printf("blink_led_st\n\r");
        break;
      case wait_for_button_st:
        printf("wait_for_button_st\n\r");
        break;
      case button_pressed_st:
        printf("button_pressed_st\n\r");
        break;
      case wait_between_flash_st:
        printf("wait_between_flash_st\n\r");
        break;
      case show_stats_st:
        printf("show_stats_st\n\r");
        break;
      case wait_stats_st:
        printf("wait_stats_st\n\r");
        break;
      case update_scores_st:
        printf("update_scores_st\n\r");
        break;
      default:
        printf("Shouldn't have hit this default case\n");
        break;
     }
  }
}

/**
 * This is the primary tick function that drives the game's state machine.
 * It takes the current state of the game, performs the necessary actions,
 * and then returns the next state of the game.
 * @param  currentState The current state of the game.
 * @return              The next state of the game.
 */
ReflexTest_st ReflexTest_TickFunction(ReflexTest_st currentState) {
  // Uncomment the line below to visually see state transitions.
  //verboseStatePrint(currentState);

  static uint32_t fiveSecondTimer = 0;
  static uint32_t flashTimer = 0;
  static uint32_t flashWait = 0;  // value for randomized flash wait
  static uint32_t buttonTimeoutTimer = 0;
  static uint32_t waitStatsTimer = 0;

  // state actions
  switch (currentState) {
    case init_st:
      break;
    case show_info_st:
      break;
    case wait_info_st:
      fiveSecondTimer = 0;  // reset 5 second timer
      break;
    case wait_five_seconds_st:
      fiveSecondTimer++;
      break;
    case blank_screen_st:
      flashTimer = 0; // reset flash timer
      break;
    case wait_between_flash_st:
      flashTimer++;
      break;
    case blink_led_st:
      buttonTimeoutTimer = 0; // reset button timeout timer
      break;
    case wait_for_button_st:
      buttonTimeoutTimer++;
      break;
    case button_pressed_st:
      flashTimer = 0; // reset flash timer
      break;
    case show_stats_st:
      waitStatsTimer = 0;
      break;
    case wait_stats_st:
      waitStatsTimer++;
      break;
    case update_scores_st:
      ReflexTestData_UpdateScores();
      break;
  }

  // state update
  switch (currentState) {
    case init_st:
      currentState = show_info_st;
      break;
    case show_info_st:
      currentState = wait_info_st;
      break;
    case wait_info_st:
      // Wait here until the user pressed a button
      if (ReflexTestData_GetPressedButton() != 0x0) {
        currentState = wait_five_seconds_st;
      }
      else {
    	  currentState = wait_info_st;
      }
      break;
    case wait_five_seconds_st:
      // Wait here until a user pushes a button and holds it for 5 sec.
      if (  fiveSecondTimer >= FIVE_SECOND_WAIT &&
            (ReflexTestData_GetPressedButton() != 0x0)) {
        currentState = blank_screen_st;
      }
      // If they let go of the button before 5 sec, go back to waiting.
      else if (ReflexTestData_GetPressedButton() == 0x0) {
        currentState = wait_info_st;
      }
      else {
        currentState = wait_five_seconds_st;
      }
      break;
    case blank_screen_st:
      currentState = wait_between_flash_st;
      srand((unsigned) ReflexTestData_GetResponseTime());
      ReflexTestData_GenerateSequence(rand());      // Seed a random number

      // Get a flash wait time between 2 and 4 seconds.
      // e.g. v3 = rand() % 30 + 1985;  <-- v3 in the range 1985-2014
      flashWait = (rand() % TWO_SECOND_WAIT) + TWO_SECOND_WAIT;
      break;
    case wait_between_flash_st:
      // Wait here for flashWait and then blink LED if we're not finished
      if (flashTimer >= flashWait && !ReflexTestData_IsSequenceDone()) {
        currentState = blink_led_st;
      }
      // If we are all done, show stats
      else if (ReflexTestData_IsSequenceDone()) {
        currentState = show_stats_st;
      }
      else {
        currentState = wait_between_flash_st;
      }
      break;
    case blink_led_st:
      currentState = wait_for_button_st;
      break;
    case wait_for_button_st:
      // If the user doesn't push a button within two seconds, start the game over
      if ((buttonTimeoutTimer >= FIVE_SECOND_WAIT) && !ReflexTestData_IsCorrectButtonPressed()) {
        currentState = show_info_st;
      }
      // the moment they do push a button, move states.
      else if (ReflexTestData_IsCorrectButtonPressed()) {
        currentState = button_pressed_st;
      }
      else {
        currentState = wait_for_button_st;
      }
      break;
    case button_pressed_st:
      // Get a flash wait time between 2 and 4 seconds.
      // e.g. v3 = rand() % 30 + 1985;  <-- v3 in the range 1985-2014
      flashWait = (rand() % TWO_SECOND_WAIT) + ONE_SECOND_WAIT;
      currentState = wait_between_flash_st;
      break;
    case show_stats_st:
      currentState = wait_stats_st;
      break;
    case wait_stats_st:
      if (waitStatsTimer >= FIVE_SECOND_WAIT) {
        currentState = update_scores_st;
      }
      break;
    case update_scores_st:
      currentState = show_info_st;
      break;
   }
  // return the new state.
  return currentState;
}

void Executor_Init(void) {
  //call init on each of the conductors
  ButtonConductor_Init();
  LedConductor_Init();
  TimerConductor_Init();
  LcdConductor_Init();
  ReflexTestData_Init();
}

bool Executor_Run(void) {
  // Each sub-triad conductor performs ONLY Moore actions
  // based on the current state. The order of these actions matters.
  ButtonConductor_Run();  // First, read the buttons
  TimerConductor_Run();   // Next, perform any timing operations
  LedConductor_Run();     // Next do LED information, which updates the index
  LcdConductor_Run();     // Finally, show state after all computation is done

  //state update next
  ReflexTest_st nextState = ReflexTest_TickFunction(ReflexTestData_GetCurrentState());
  ReflexTestData_SetCurrentState(nextState);
  return true;  // always return true so it can be the condition of a while(1)
}
