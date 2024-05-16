#include <stdio.h>
#include <stdint.h>
#include "main.h"
#include "led.h"
#include "usart.h"
#include "sdmmc_sd.h"
#include "ff.h"
#include "ff_gen_drv.h"
#include "sd_diskio.h"

extern int myparameter;

void yuxiao_print(void)
{
	printf("hello world!");
}

int calculation(int a, int b)
{
	int c;
	
	c = a + b;
	
	return c
}
