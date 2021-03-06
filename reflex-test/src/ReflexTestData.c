/*
 * ReflexTestData.c
 *
 *  Created on: Jun 3, 2015
 *      Author: travis
 */

#include "ReflexTestData.h"

#include <stdio.h>
#include <stdlib.h>

static uint32_t sequence[REFLEXTESTDATA_SEQUENCE_LENGTH];
static int32_t index = 0;
static int32_t pressedButton = 0;
static double responseTime;
static ReflexTest_st currentState = init_st;
static double min = 5.0;
static double max = 0.0;
static double average = 0.0;
static double highScores[REFLEXTESTDATA_NUMBER_OF_HIGH_SCORES];

void ReflexTestData_Init() {
  // Initialize all of the global variables.
  index = 0;
  pressedButton = 0;
  responseTime = 0.0;
  min = 5.0;
  max = 0.0;
  average = 0.0;
  currentState = init_st;

  int i;
  for (i = 0; i < REFLEXTESTDATA_SEQUENCE_LENGTH; i++) {
    sequence[i] = 0;
    highScores[i] = 5.0;  // initialize high scores to slow values
  }
}

bool ReflexTestData_IsSequenceDone() {
  return (index == (REFLEXTESTDATA_SEQUENCE_LENGTH));
}

int32_t ReflexTestData_GetLed() {
  // Return the value of the current LED
  if (index < REFLEXTESTDATA_SEQUENCE_LENGTH) {
   return sequence[index];
  }
  else {
    return REFLEXTESTDATA_ERROR;
  }
}

void ReflexTestData_IncrementIndex() {
  if (index < REFLEXTESTDATA_SEQUENCE_LENGTH) {
    index++;
  }
}

int32_t ReflexTestData_GetCurrentIndex() {
  return index;
}

/**
 * Helper function that prints our the contents of the generated sequence.
 */
void ReflexTestData_PrintSequence() {
  int i;
  printf("{");
  for (i = 0; i < REFLEXTESTDATA_SEQUENCE_LENGTH-1; i++) {
    printf("%lu, ", sequence[i]);
  }
  printf("%lu};\n", sequence[REFLEXTESTDATA_SEQUENCE_LENGTH-1]);
}

uint32_t* ReflexTestData_GenerateSequence(int32_t seed) {

  index = 0; //reset the global index whenever a new sequence is generated

  srand(seed); //initialize random number generator
  int i;
  for (i = 0; i < REFLEXTESTDATA_SEQUENCE_LENGTH; i++) {
    // Generate values between 1-4
    sequence[i] = (rand() % REFLEXTESTDATA_NUMBER_OF_LEDS) + 1;
  }

#ifdef REFLEXTESTDATA_DEBUG
  ReflexTestData_PrintSequence();
#endif

  return sequence;
}

int32_t ReflexTestData_GetPressedButton() {
  return pressedButton;
}

void ReflexTestData_SetPressedButton(int32_t value) {
  pressedButton = value;
}

void ReflexTestData_SetCurrentState(ReflexTest_st newState) {
  currentState = newState;
}

ReflexTest_st ReflexTestData_GetCurrentState() {
  return currentState;
}

bool ReflexTestData_IsCorrectButtonPressed() {
  return (pressedButton == sequence[index]);
}

void ReflexTestData_SetResponseTime(double inputTime) {
  responseTime = inputTime;
}

double ReflexTestData_GetResponseTime() {
  return responseTime;
}

void ReflexTestData_SetMinResponseTime(double responseTime) {
  min = responseTime;
}

double ReflexTestData_GetMinResponseTime() {
  return min;
}

void ReflexTestData_SetMaxResponseTime(double responseTime) {
  max = responseTime;
}

double ReflexTestData_GetMaxResponseTime() {
  return max;
}

void ReflexTestData_SetAverageResponseTime(double responseTime) {
  average = responseTime;
}

double ReflexTestData_GetAverageResponseTime() {
  return average;
}

double* ReflexTestData_GetHighScores() {
  return highScores;
}

/**
 * Helper function that performs a comparison of two double values.
 * @param  a The 1st double to compare.
 * @param  b The 2nd double to compare
 * @return   -1 if A is less than b, 1 if A is greater than b, or 0 otherwise.
 */
int compare_function(const void *a,const void *b) {
  double *x = (double *) a;
  double *y = (double *) b;

  if (*x < *y) {
    return -1;
  }
  else if (*x > *y) {
    return 1;
  }

  return 0;
}

void ReflexTestData_UpdateScores() {
  // If the slowest score in the high scores list is slower than the most recent
  // score, replace the slowest score with the new average, and then sort.
  if (highScores[REFLEXTESTDATA_NUMBER_OF_HIGH_SCORES-1] > average) {
    highScores[REFLEXTESTDATA_NUMBER_OF_HIGH_SCORES-1] = average;
    qsort(highScores, REFLEXTESTDATA_NUMBER_OF_HIGH_SCORES, sizeof(double), compare_function);
  }
}

/**
 * Helper function used in Unit testing to give access to the scores array.
 */
ReflexTestData_TestOnly_SetScores(double testScores[REFLEXTESTDATA_NUMBER_OF_HIGH_SCORES]) {
  int i;
  for (i = 0; i < REFLEXTESTDATA_NUMBER_OF_HIGH_SCORES; i++) {
    highScores[i] = testScores[i];
  }
}
