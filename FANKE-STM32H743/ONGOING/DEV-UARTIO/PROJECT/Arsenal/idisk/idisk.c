/**********************************************************************
*file:磁盘文件
*author:残梦
*versions:V1.0
*date:2023.10.10
*note:
**********************************************************************/
#include "idisk.h"
#include "sdmmc.h"

static int32_t Disk_Mount_SD(void);
static int Disk_File_Read_SystemParameter(void);

static status_EnumDef disk_state[eDisk_Num] = {eStatus_Invalid};//磁盘状态

/****************************************
@function:磁盘挂载
@param:void
@return:-1--失败,0--正常
@note:
****************************************/
int32_t Disk_Mount(void)
{
  if(Disk_Mount_SD() < 0)
  {
    disk_state[eDisk_SD] = eStatus_Invalid;
    return -1;
  }else{disk_state[eDisk_SD] = eStatus_Valid;}

  //Disk_File_Read_SystemParameter();
  return 0;
}

/****************************************
@function:挂载磁盘-SD卡
@param:void
@return:-1--失败,0--正常
@note:
****************************************/
static int32_t Disk_Mount_SD(void)
{
  BYTE work[_MAX_SS];
  HAL_SD_CardInfoTypeDef SdCard;

  retSD = f_mount(&SDFatFS,SDPath,1);//挂载盘符A
  if(retSD == FR_NO_FILESYSTEM)//没有文件系统就格式化创建创建文件系统
  {
      retSD = f_mkfs(SDPath,0,0,work,sizeof(work));
      if(retSD == FR_OK)
      {
          retSD = f_mount(NULL,SDPath,1);//格式化后，先取消挂载
          retSD = f_mount(&SDFatFS,SDPath,1);//挂载
      }
      else//格式化失败
      {
        printf("Description Failed to format the SD card...%d\n",retSD);
        goto SD_FAIL;
      }
  }
  else if(retSD != FR_OK)//挂载失败
  {
    printf("Mount failure=%d\n",retSD);
    goto SD_FAIL;
  }

  retSD = f_mount(&SDFatFS,SDPath,1);
  if(retSD != FR_OK){printf("f_mount():retSD=%d\n",retSD);goto SD_FAIL;}

  if(HAL_SD_GetCardInfo(&hsd1,&SdCard) != HAL_OK){printf("HAL_SD_GetCardInfo()\n");goto SD_FAIL;}
  printf("SD card volume:%.2fGB\n",(float)((uint64_t)SdCard.BlockNbr * (uint64_t)SdCard.BlockSize / 1024.0f / 1024.0f / 1024.0f));

  return 0;
  SD_FAIL:
  {
    printf("Error[Disk_Mount_SD()]:The disk fails to be mounted...\n");
    return -1;
  }
}

/****************************************
@function:获取磁盘状态
@param:void
@return:见status_EnumDef
@note:
****************************************/
status_EnumDef Disk_Status_Get(Disk_List_EnumDef disk)
{
    return disk_state[disk];
}

/****************************************
@function:读取板参数文件
@param:
@return:-1--读取失败，0--成功
@note:
****************************************/
static int Disk_File_Read_SystemParameter(void)
{
    FRESULT res_sd;
    UINT fnum;
    char string[200];
    int32_t ByteNum = 0,value = 0;
    uint32_t line = 0;

    if(!Disk_Status_Get(eDisk_SD))return -1;
    memset(string,0,sizeof(string));
    sprintf(string,"%sSystemParameter.txt",SDPath);
    res_sd = f_open(&SDFile, string, FA_OPEN_EXISTING | FA_READ);
    if(res_sd == FR_NO_FILE)//文件不存在
    {
        printf("file does not exist:%s\n",string);

        //创建默认文件
        res_sd = f_open(&SDFile, string,FA_CREATE_ALWAYS | FA_WRITE );
        if(res_sd != FR_OK)
        {
            printf("[%d]:Failed to create the file!%s\n",res_sd,string);
            return -1;
        }

        memset(string,0,sizeof(string));
        //sprintf(string,"%s\n",bsp_BoardVersion);
        ByteNum = strlen(string);
        res_sd=f_write(&SDFile,string,ByteNum,&fnum);
        if(&SDFile != NULL){res_sd = f_close(&SDFile);}
    }
    else if(res_sd != FR_OK)
    {
        printf("[%d]:File opening failure!%s\n",res_sd,string);
        return -1;
    }

    line = 0;
    while(!(f_eof(&SDFile)))
    {
        memset(string,0,sizeof(string));f_gets(string,sizeof(string),&SDFile);if(strlen(string) == 0){break;}
        switch(line++)
        {
            case 0:
            {
                //sscanf(string,"RemoteControlID[%d]:{set range:0xFA-0xFD}\n",&value);
                printf("%s\n",string);
            }break;
            default:break;
        }
    }
    if(&SDFile != NULL){res_sd = f_close(&SDFile);}
    return 0;
}




