#ifndef __BSP_USER_KEY_H__
#define __BSP_USER_KEY_H__

#include "gpio.h"

#define BUTTON_NOT_PRESSED      0
#define BUTTON_PRESSED          1


int8_t BSP_UserKey_Detect(void);
#endif
