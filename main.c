/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define SIZE 50
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef huart1;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART1_UART_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
char buff[SIZE];
int mode;
int Number1, Number2, Number3;
int end1;
float fNumbers[5];

static float pow(float val, int pow)
{
    float res = 1.0;
    if(pow == 0) return 1;
    if(pow > 0)
    {

        while(pow != 0)
        {
            res*=val;
            pow--;
        }
    }
    if(pow < 0)
    {
        pow*=-1;


        while(pow != 0)
        {
            res/=val;
        }
    }

    return res;
}

static float atof(const char* str)
{
    int sign = 1;
    int point = 0;
    int pt_count = 0;
    float res = 0;
    int i = 0;
    while(str[i] != 0)
    {

    	if(!((str[i] >= '0' && str[i] <= '9') || str[i] == '.' || str[i] == '-')) break;
        if((str[i] >= '0' && str[i] <= '9') || str[i] == '.' || str[i] == '-')
        {

            if(point == 1)
                pt_count++;
            if(str[i] == '-')
            {
                sign = -1;
                i++;
                continue;
            }
            if(str[i] == '.')
            {
                point = 1;
                i++;
                continue;
            }
            res *= 10;
            res += str[i] - '0';
            i++;
        }

    }
    return sign * res / pow(10,pt_count);
}

void  getNumber(const char* str,int start,int end,float* arr)
{

    int size = end-start;
    char num[SIZE];
    int j = 0;
    //float *qwe = num;

        int k = 0;
        for(int i = start; i <= end;i++)
        {
            char symbol = str[i];
            if((symbol >= '0' && symbol <= '9') || symbol == '.' || symbol == '-')
            {
                num[j++] = str[i];
            }
            else if(!((symbol >= '0' && symbol <= '9') || symbol == '.' || symbol == '-'))
            {
                if(j>0)
                    arr[k++] = atof(num);
                j = 0;
                memset(num,'\0',SIZE);
            }
            if(str[i] == 0) break;
        }
    //return qwe;
}

int charNumInStr(char* str, char _char, int len)
{
	int res = -1;
	for (int i = 0; i<len; i++){
		if (str[i] == _char ) res = i;
	}
	return res;
}

int cropNumberFromStr(char* str, int start, int end){
	char resStr[SIZE];
	for (int i = start; i<end; i++){
		resStr[i-start] = str[i];
	}
	resStr[end] = '\0';
	return atoi(resStr);
}

int fcropNumberFromStr(char* str, int start, int end){
	char resStr[SIZE];
	for (int i = start; i<end; i++){
		resStr[i-start] = str[i];
	}
	resStr[end] = '\0';
	return atof(resStr);
}
float sigma(float* arr)
{
	float res = 0.0;
	for(int i = 0;i<=3;i++)
	{
		res += arr[i];
	}
	return res;
}
void taskRead()
{
	  HAL_UART_Receive(&huart1, buff, SIZE, 1000);
	  if (charNumInStr(buff, '@', SIZE) > -1)
	  {
		  int start1 = charNumInStr(buff, '&', SIZE)+1;
		  end1 = charNumInStr(buff, '@', SIZE);
		  //Number1 = cropNumberFromStr(buff,start1,end1);
		  //fNumbers = cropNumberFromStr(buff, start1, end1);
		  getNumber(buff,0,end1,fNumbers);
		  mode = 1;
	  }
}

void taskWrite(){
	char str[SIZE];
	sprintf(str,"&%f;%f;%f@",fNumbers[0],fNumbers[1],fNumbers[2]);
	HAL_UART_Transmit(&huart1, str, SIZE, 1000);
	mode = 0;
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
	mode = 0;


  /* USER CODE END 1 */

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
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
  //f = atof(ch);
  //char ch[50];// = "&12.9;13.12;5;-213.5342;432@";
 // sprintf(ch,"&%f@",432.0);
  //getNumber(ch,0,50,fNumbers);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	 if(mode == 0) taskRead();
	 if(mode == 1) taskWrite();
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
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

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

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
