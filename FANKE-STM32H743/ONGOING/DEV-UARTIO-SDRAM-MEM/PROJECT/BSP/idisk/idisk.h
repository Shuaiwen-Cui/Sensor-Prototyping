#ifndef IDISK_H_
#define IDISK_H_
#ifdef __cplusplus
extern "C" {
#endif
#include "main.h"
#include "fatfs.h"

typedef enum
{
    eStatus_Invalid = 0,
    eStatus_Valid = 1
}status_EnumDef;

typedef enum
{
  eDisk_SD = 0,
  eDisk_Num
}Disk_List_EnumDef;//磁盘资源列表

int32_t Disk_Mount(void);
status_EnumDef Disk_Status_Get(Disk_List_EnumDef disk);

#ifdef __cplusplus
}
#endif
#endif /* IDISK_H_ */


