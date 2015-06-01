#include "ButtonHardware.h"
#include "buttons.h"

#include <stdint.h>


void ButtonHardware_Init(void) {

  buttons_init(); //sets the buttons as inputs

}

void ButtonHardware_Read() {

  int32_t readButton;
  readButton = buttons_read();
  printf("%d \n", readButton);
}
