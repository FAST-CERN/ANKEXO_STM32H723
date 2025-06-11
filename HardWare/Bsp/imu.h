#ifndef IMU_H
#define IMU_H

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"
#include <stdbool.h>


void IMU_INIT(void);

void IMU_LOOP(void);


#ifdef __cplusplus
}
#endif


#endif /* IMU_H */
