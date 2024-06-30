/**
 * @file imemory.h
 * @author SHUAIWEN CUI (cswoffice@163.com)
 * @brief This is the header file for the memory management module.
 * @version 1.0
 * @date 2024-06-30
 * @ref ALIENTEK memory.h & memory.c for STM32H7xx
 * @copyright Copyright (c) 2024
 * 
 * ! In this module, the memory banks are managed block by block, and there is a table to record the status of each block.
 * 
 */

#ifndef __IMEMORY_H
#define __IMEMORY_H

#include "stm32h7xx.h"
#include <stddef.h>
#include <stdint.h>
#include "iusart.h"
#include "isdram.h"

void memory_init();
void* memory_alloc(size_t size);
void memory_free(void* ptr);


#endif /* __IMEMORY_H */












