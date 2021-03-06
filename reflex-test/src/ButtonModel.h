#ifndef BUTTONMODEL_H_
#define BUTTONMODEL_H_

#include <stdint.h>
#include "ReflexTestData.h"

#define BUTTONS_BTN0_MASK       0x1
#define BUTTONS_BTN1_MASK       0x2
#define BUTTONS_BTN2_MASK       0x4
#define BUTTONS_BTN3_MASK       0x8

/**
 * Initialize ButtonModel variables in ReflexTestData if necessary.
 */
void ButtonModel_Init(void);

/**
 * This will take the value, determine the highest precedence
 * button pressed, and store it in ReflexTestData.
 * @param value number representing which button(s) was/were pressed
 */
void ButtonModel_SetPressedButton(int32_t value);

/**
 * Whenever a timer is stopped in ButtonHardware, this function
 * will pass on the elapsed time to ReflexTestData.
 * @param time a value representing the elapsed time in seconds
 */
void ButtonModel_SetResponseTime(double time);

/**
 * Called by the Conductor to get the current state from ReflexTestData.
 * @return  returns the current state
 */
ReflexTest_st ButtonModel_GetCurrentState(void);

#endif //BUTTONMODEL_H_
