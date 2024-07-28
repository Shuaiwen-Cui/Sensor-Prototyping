/**
 * @file app_control.h
 * @brief This script is to control the application related codes.
 * @version 1.0
 * @date 2024-07-22
 * @copyright Copyright (c) 2024
 */

#ifndef APP_CONTROL_H
#define APP_CONTROL_H

/**
 * @name INCLUDES
 * 
 */

#include "main.h"
#include "bsp_init.h"   // Board support package header file & printf retargetting statement
#include "ff.h"
#include "diskio.h"     // Disk I/O header file
#include "fatfs.h"      // FATFS configuration header file (if using STM32CubeMX)
#include "ff_gen_drv.h" // Generic driver header file
#include "ffconf.h"
#include "ifile.h"
#include "usart.h"
#include "iusart.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "arm_math.h"
#include "wt.h"
#include "ica.h"    


/**
 * @name MACROS
 * 
 */
#define LAB_NUM_FLOORS 6
#define SIM_NUM_FLOORS 12
#define LAB_SIGNAL_LEN 2000
#define SIM_SIGNAL_LEN 4000
#define WINDOW_SIZE 200
#define WINDOW_GAP 100
#define NUM_SOURCE (LAB_NUM_FLOORS / 2)
#define WT_DEC_LEVEL 4

/**
 * @name VARIABLES
 * 
 */


/**
 * @name FUNCTIONS
 * 
 */
int Sudden_Damage_Detection(void);
#endif /* APP_CONTROL_H */