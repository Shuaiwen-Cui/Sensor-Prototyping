/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2024 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "dma.h"
#include "fatfs.h"
#include "mdma.h"
#include "sdmmc.h"
#include "usart.h"
#include "gpio.h"
#include "fmc.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include "iled.h"
#include "iusart.h"
#include "isdram.h"
#include "imemory.h"
#include "idisk.h"
#include "arm_math.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
// SDMMC/SD Card
#define NumOf_Blocks 64
#define Test_BlockSize ((BLOCKSIZE * NumOf_Blocks) >> 2) // Define the data size, the SD block size is 512 bytes, because it is a 32-bit array, so divide by 4
#define Test_Addr 0x00

// DSP
#define FFT_LENGTH 1024 // FFT Length, by default 1024 points
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

// DSP
float fft_inputbuf[FFT_LENGTH * 2]; // FFT Input array
float fft_outputbuf[FFT_LENGTH];    // FFT Output array
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
int i;
arm_cfft_radix4_instance_f32 scfft;
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
  void *p = NULL;

  /* USER CODE END 1 */
/* Enable the CPU Cache */

  /* Enable I-Cache---------------------------------------------------------*/
  SCB_EnableICache();

  /* Enable D-Cache---------------------------------------------------------*/
  SCB_EnableDCache();

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_MDMA_Init();
  MX_USART1_UART_Init();
  MX_SDMMC1_SD_Init();
  MX_FMC_Init();
  MX_FATFS_Init();
  /* USER CODE BEGIN 2 */
  SDRAM_InitSequence();
  MCU_serial_init();
  SDRAM_Test();
  // <SDRAM TEST>
  printf("\r\n[PART I: SDRAM AND MEMORY MANAGEMENT TEST]\r\n");
  memory_init();

  void *ptr1 = memory_alloc(1 * 1024 * 1024); // allocate 10 MB
  if (ptr1)
  {
    printf("Allocated 1 MB at %p\r\n", ptr1);
  }
  else
  {
    printf("Failed to allocate 1 MB\r\n");
  }

  // modify the values in the allocated memory
  for (int i = 0; i < 1 * 1024 * 1024 / 4; i++)
  {
    ((uint32_t *)ptr1)[i] = i;
    // print
    if (i % 4096 == 0)
    {
      printf("Value at %p: %d\r\n", &((uint32_t *)ptr1)[i], ((uint32_t *)ptr1)[i]);
    }
  }

  memory_free(ptr1);
  printf("Freed memory at %p\r\n", ptr1);

  // <SD CARD TEST>
  printf("\r\n[PART II: SD CARD TEST]\r\n");

  if (Disk_Mount() < 0)
  {
    Error_Handler();
    printf("SD CARD TEST failed!\r\n");
  }
  else
  {
    printf("SD CARD TEST succeeded!\r\n");
  }

  HAL_SD_CardInfoTypeDef SDCardInfo;
  HAL_SD_GetCardInfo(&hsd1, &SDCardInfo); // HAL_SD_CardInfoTypeDef *pCardInfo);
  printf("Card Type:%d\r\n", SDCardInfo.CardType);
  printf("Card CardVersion:%d\r\n", SDCardInfo.CardVersion);
  printf("Card RCA:%d\r\n", SDCardInfo.RelCardAdd);
  printf("Card Capacity:%d MB\r\n", (uint32_t)(SDCardInfo.LogBlockNbr) >> 11);
  printf("Card BlockSize:%d\r\n\r\n", SDCardInfo.LogBlockSize);

  // <CMSIS DSP TEST>
  printf("\r\n[PART III: CMSIS DSP TEST - FFT]\r\n");
  arm_cfft_radix4_init_f32(&scfft, FFT_LENGTH, 0, 1); // init the scfft structure, set FFT related parameters

  for (i = 0; i < FFT_LENGTH; i++) // generate signal sequence
  {
    fft_inputbuf[2 * i] = 100 +
                          10 * arm_sin_f32(2 * PI * i / FFT_LENGTH) +
                          30 * arm_sin_f32(2 * PI * i * 4 / FFT_LENGTH) +
                          50 * arm_cos_f32(2 * PI * i * 8 / FFT_LENGTH); // generate input signal real part
    fft_inputbuf[2 * i + 1] = 0;                                         // imaginary part is all 0
  }

  arm_cfft_radix4_f32(&scfft, fft_inputbuf); // FFT calculation (base 4)

  arm_cmplx_mag_f32(fft_inputbuf, fft_outputbuf, FFT_LENGTH); // get the amplitude value of the complex modulus of the operation result

  printf("FFT Result:\r\n");
  for (i = 0; i < FFT_LENGTH; i++)
  {
    printf("fft_outputbuf[%d]:%f\r\n", i, fft_outputbuf[i]);
  }
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    LED(1);
    HAL_Delay(500);
    LED(0);
    HAL_Delay(500);

    MCU_printf("There are %d days in a week.\n\r", 7);
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Supply configuration update enable
  */
  HAL_PWREx_ConfigSupply(PWR_LDO_SUPPLY);

  /** Configure the main internal regulator output voltage
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}

  __HAL_RCC_SYSCFG_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE0);

  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 5;
  RCC_OscInitStruct.PLL.PLLN = 192;
  RCC_OscInitStruct.PLL.PLLP = 2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_2;
  RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
  RCC_OscInitStruct.PLL.PLLFRACN = 0;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2
                              |RCC_CLOCKTYPE_D3PCLK1|RCC_CLOCKTYPE_D1PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;
  RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

// MicroLib needed for retargetting
// retarget fputc for printf
int fputc(int ch, FILE *f)
{
  uint8_t temp = (uint8_t)ch;
  HAL_UART_Transmit(&huart1, &temp, 1, HAL_MAX_DELAY);
  return ch;
}

// retarget fgetc for scanf
int fgetc(FILE *f)
{
  while (__HAL_UART_GET_FLAG(&huart1, UART_FLAG_RXNE) == RESET)
    ;                                        // Wait until the data is received
  return (int)(huart1.Instance->RDR & 0xFF); // Read the received data
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
