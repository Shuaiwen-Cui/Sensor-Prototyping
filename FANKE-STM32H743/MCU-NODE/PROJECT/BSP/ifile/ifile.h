/**
 * @file ifile.h
 * @author SHUAIWEN CUI (shuaiwencui AT gmail DOT com)
 * @brief This is the header file for the ifile.c file
 * @version 1.0
 * @date 2024-07-17
 * @ref https://blog.csdn.net/Mculover666/article/details/102688285；https://blog.csdn.net/qq_36561846/article/details/133808890
 * @copyright Copyright (c) 2024
 *
 */
#ifndef __IFILE_H
#define __IFILE_H

/**
 * @name INCLUDES
 */

#include "stm32h7xx.h"
#include "main.h"
#include "ff.h"         // FATFS main header file
#include "diskio.h"     // Disk I/O header file
#include "fatfs.h"      // FATFS configuration header file (if using STM32CubeMX)
#include "ff_gen_drv.h" // Generic driver header file
#include "ffconf.h"
#include "stdio.h"

/*定义自己的存储设备*/
/*用户存储设备扇区字节数*/
#define User_Sector 512
/*用户存储设备FatFs对象*/
#define User_FatFs 	SDFatFS
/*用户存储设备卷路径*/
#define User_SDPath SDPath
 
/*函数声明*/
void Mount_FatFs(void);
void FatFs_GetDiskInfo(void);
void FatFs_ScanDir(const TCHAR* PathName);
void FatFs_ReadTXTFile(TCHAR *filename);
void FatFs_WriteTXTFile(TCHAR *filename,uint16_t year, uint8_t month, uint8_t day);
void FatFs_GetFileInfo(TCHAR *filename);
void FatFs_DeleteFile(TCHAR *filename);
void FatFs_PrintfFileDate(WORD date, WORD time);


#endif /* __IFILE_H */
