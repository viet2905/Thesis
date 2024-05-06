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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

uint16_t ic_val1 = 0;
uint16_t ic_val2 = 0;
uint16_t diff = 0;
uint8_t distance =0;
uint8_t first_cap = 0; //check the first capture

float pre_pos1=0,pos1=0; //motor1
float pre_pos2=0,pos2=0; //motor2
float w1=0,w2=0; //van toc goc hien tai cua motor 1 và 2
float pre_pwm1 = 0;
float pre_pwm2 = 0;   	
char str[2] ;
char strn[10] = "da nhan";
float vR= 300; //van toc dai mong muon (mm/s)
float wR;//van toc goc mong muon (rad/s)

//calculate value
float pre_e2=0,e2=0; //sai so e2
float inte2=0; //khau tich phan
float vleft=0,vright=0,wleft=0,wright=0; //van toc banh trai va banh phai
float v=0,w=0; //van toc dai va van toc goc
float b=186.5; //khoang cach tam 2 banh (mm)
float D=67; //duong kinh banh xe (mm)
float L=48.65; //khoang cach tu truc dong co den mat cam bien
float pwm1 = 0, pwm2 = 0; //
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim3;
TIM_HandleTypeDef htim4;
TIM_HandleTypeDef htim8;

UART_HandleTypeDef huart2;
DMA_HandleTypeDef hdma_usart2_rx;
DMA_HandleTypeDef hdma_usart2_tx;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_TIM1_Init(void);
static void MX_TIM2_Init(void);
static void MX_TIM3_Init(void);
static void MX_TIM4_Init(void);
static void MX_TIM8_Init(void);
static void MX_USART2_UART_Init(void);
/* USER CODE BEGIN PFP */

void delay_us(uint16_t time)
{
  __HAL_TIM_SET_COUNTER(&htim3, 0);
  while(__HAL_TIM_GET_COUNTER (&htim3) < time);
}

void HCS04_trig (GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, uint8_t timer ,uint8_t cc)
{
  HAL_GPIO_WritePin(GPIOx,GPIO_Pin,GPIO_PIN_SET);
  delay_us(10);
  HAL_GPIO_WritePin(GPIOx,GPIO_Pin,GPIO_PIN_RESET);
	if(timer == 3)
	{
	if(cc == 1)
		{
     __HAL_TIM_ENABLE_IT(&htim3, TIM_IT_CC1 );
		}
	else if(cc == 2)
		{
     __HAL_TIM_ENABLE_IT(&htim3, TIM_IT_CC2 );
		}
	else if(cc == 4)
		{
     __HAL_TIM_ENABLE_IT(&htim3, TIM_IT_CC4 );
		}
	else if(cc == 3)
		{
     __HAL_TIM_ENABLE_IT(&htim3, TIM_IT_CC3 );
		}
	else if(cc == 5 )
		{
			__HAL_TIM_ENABLE_IT(&htim3, TIM_IT_CC3 );
		}
	}
	
	else if(timer == 4)
	{
	if(cc == 1)
		{
     __HAL_TIM_ENABLE_IT(&htim4, TIM_IT_CC1 );
		}
	else if(cc == 2)
		{
     __HAL_TIM_ENABLE_IT(&htim4, TIM_IT_CC2 );
		}
	else if(cc == 3)
		{
     __HAL_TIM_ENABLE_IT(&htim4, TIM_IT_CC3 );
		}
	else if(cc == 4)
		{
     __HAL_TIM_ENABLE_IT(&htim4, TIM_IT_CC4 );
		}
	}
	
	else if(timer == 8)
	{
		if(cc == 1)
		{
			__HAL_TIM_ENABLE_IT(&htim8, TIM_IT_CC1 );
		}
		else if(cc == 2)
		{
			__HAL_TIM_ENABLE_IT(&htim8, TIM_IT_CC2 );
		}
		else if(cc == 3)
		{
			__HAL_TIM_ENABLE_IT(&htim8, TIM_IT_CC3 );
		}
		else if(cc == 4)
		{
			__HAL_TIM_ENABLE_IT(&htim8, TIM_IT_CC4 );
		}
	}
}

//void HCS04_trig()
//{
//  HAL_GPIO_WritePin(Trig_T3_CH2_GPIO_Port,Trig_T3_CH2_Pin,GPIO_PIN_SET);
//  delay_us(10);
//  HAL_GPIO_WritePin(Trig_T3_CH2_GPIO_Port,Trig_T3_CH2_Pin,GPIO_PIN_RESET);
//  __HAL_TIM_ENABLE_IT(&htim3,TIM_IT_CC2);
//}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
	if(htim -> Instance == htim3.Instance) //timer 3
	{
		if(htim -> Channel == HAL_TIM_ACTIVE_CHANNEL_1 ) //channel 1
		{
			if(first_cap == 0) 
			{
				ic_val1 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1 );
				first_cap = 1;
				__HAL_TIM_SET_CAPTUREPOLARITY(htim,TIM_CHANNEL_1, TIM_INPUTCHANNELPOLARITY_FALLING);
			}
			else if(first_cap == 1)
			{
				ic_val2 = HAL_TIM_ReadCapturedValue(htim,TIM_CHANNEL_1);
				__HAL_TIM_SET_COUNTER(htim,0);
				if(ic_val2 > ic_val1)
				{
					diff = ic_val2 - ic_val1;
				}
				else if(ic_val2 < ic_val1)
				{ 
					diff = 0xffff + ic_val2 - ic_val1;
				} 
				distance = 0.017*diff;
				first_cap = 0;
				__HAL_TIM_SET_CAPTUREPOLARITY(htim,TIM_CHANNEL_1, TIM_INPUTCHANNELPOLARITY_RISING);
				__HAL_TIM_DISABLE_IT(htim, TIM_IT_CC1);
			}
		}
	
		if(htim -> Channel == HAL_TIM_ACTIVE_CHANNEL_2 ) //channel 2
		{
				if(first_cap == 0) 
				{
					ic_val1 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_2 );
					first_cap = 1;
					__HAL_TIM_SET_CAPTUREPOLARITY(htim,TIM_CHANNEL_2, TIM_INPUTCHANNELPOLARITY_FALLING);
				}
				else if(first_cap == 1)
				{
					ic_val2 = HAL_TIM_ReadCapturedValue(htim,TIM_CHANNEL_2);
					__HAL_TIM_SET_COUNTER(htim,0);
					if(ic_val2 > ic_val1)
					{
						diff = ic_val2 - ic_val1;
					}
					else if(ic_val2 < ic_val1)
					{ 
						diff = 0xffff + ic_val2 - ic_val1;
					} 
					distance = 0.017*diff;
					first_cap = 0;
					__HAL_TIM_SET_CAPTUREPOLARITY(htim,TIM_CHANNEL_2, TIM_INPUTCHANNELPOLARITY_RISING);
					__HAL_TIM_DISABLE_IT(htim, TIM_IT_CC2);
				}
		}
		if(htim -> Channel == HAL_TIM_ACTIVE_CHANNEL_3 ) //channel 3
		{
			if(first_cap == 0) 
			{
				ic_val1 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_3 );
				first_cap = 1;
				__HAL_TIM_SET_CAPTUREPOLARITY(htim,TIM_CHANNEL_3, TIM_INPUTCHANNELPOLARITY_FALLING);
			}
			else if(first_cap == 1)
			{
				ic_val2 = HAL_TIM_ReadCapturedValue(htim,TIM_CHANNEL_3);
				__HAL_TIM_SET_COUNTER(htim,0);
				if(ic_val2 > ic_val1)
				{
					diff = ic_val2 - ic_val1;
				}
				else if(ic_val2 < ic_val1)
				{ 
					diff = 0xffff + ic_val2 - ic_val1;
				} 
				distance = 0.017*diff;
				first_cap = 0;
				__HAL_TIM_SET_CAPTUREPOLARITY(htim,TIM_CHANNEL_3, TIM_INPUTCHANNELPOLARITY_RISING);
				__HAL_TIM_DISABLE_IT(htim, TIM_IT_CC3);
			}
		}
		if(htim -> Channel == HAL_TIM_ACTIVE_CHANNEL_4 ) //channel 4
		{
			if(first_cap == 0) 
			{
				ic_val1 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_4 );
				first_cap = 1;
				__HAL_TIM_SET_CAPTUREPOLARITY(htim,TIM_CHANNEL_4, TIM_INPUTCHANNELPOLARITY_FALLING);
			}
			else if(first_cap == 1)
			{
				ic_val2 = HAL_TIM_ReadCapturedValue(htim,TIM_CHANNEL_4);
				__HAL_TIM_SET_COUNTER(htim,0);
				if(ic_val2 > ic_val1)
				{
					diff = ic_val2 - ic_val1;
				}
				else if(ic_val2 < ic_val1)
				{ 
					diff = 0xffff + ic_val2 - ic_val1;
				} 
				distance = 0.017*diff;
				first_cap = 0;
				__HAL_TIM_SET_CAPTUREPOLARITY(htim,TIM_CHANNEL_4, TIM_INPUTCHANNELPOLARITY_RISING);
				__HAL_TIM_DISABLE_IT(htim, TIM_IT_CC4);
			}
		}
	}
	
	if(htim -> Instance == htim4.Instance) //timer4
	{
		if(htim -> Channel == HAL_TIM_ACTIVE_CHANNEL_2 ) //channel 2
		{
			if(first_cap == 0) 
			{
				ic_val1 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_2 );
				first_cap = 1;
				__HAL_TIM_SET_CAPTUREPOLARITY(htim,TIM_CHANNEL_2, TIM_INPUTCHANNELPOLARITY_FALLING);
			}
			else if(first_cap == 1)
			{
				ic_val2 = HAL_TIM_ReadCapturedValue(htim,TIM_CHANNEL_2);
				__HAL_TIM_SET_COUNTER(htim,0);
				if(ic_val2 > ic_val1)
				{
					diff = ic_val2 - ic_val1;
				}
				else if(ic_val2 < ic_val1)
				{ 
					diff = 0xffff + ic_val2 - ic_val1;
				} 
				distance = 0.017*diff;
				first_cap = 0;
				__HAL_TIM_SET_CAPTUREPOLARITY(htim,TIM_CHANNEL_2, TIM_INPUTCHANNELPOLARITY_RISING);
				__HAL_TIM_DISABLE_IT(htim, TIM_IT_CC2);
			}
		}
		
		if(htim -> Channel == HAL_TIM_ACTIVE_CHANNEL_3 ) //channel 3
		{
			if(first_cap == 0) 
			{
				ic_val1 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_3 );
				first_cap = 1;
				__HAL_TIM_SET_CAPTUREPOLARITY(htim,TIM_CHANNEL_3, TIM_INPUTCHANNELPOLARITY_FALLING);
			}
			else if(first_cap == 1)
			{
				ic_val2 = HAL_TIM_ReadCapturedValue(htim,TIM_CHANNEL_3);
				__HAL_TIM_SET_COUNTER(htim,0);
				if(ic_val2 > ic_val1)
				{
					diff = ic_val2 - ic_val1;
				}
				else if(ic_val2 < ic_val1)
				{ 
					diff = 0xffff + ic_val2 - ic_val1;
				} 
				distance = 0.017*diff;
				first_cap = 0;
				__HAL_TIM_SET_CAPTUREPOLARITY(htim,TIM_CHANNEL_3, TIM_INPUTCHANNELPOLARITY_RISING);
				__HAL_TIM_DISABLE_IT(htim, TIM_IT_CC3);
			}
		}
	}
	
	if(htim -> Instance == htim8.Instance) //timer8
	{
	if(htim -> Channel == HAL_TIM_ACTIVE_CHANNEL_3 ) //channel 3
		{
			if(first_cap == 0) 
			{
				ic_val1 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_3 );
				first_cap = 1;
				__HAL_TIM_SET_CAPTUREPOLARITY(htim,TIM_CHANNEL_3, TIM_INPUTCHANNELPOLARITY_FALLING);
			}
			else if(first_cap == 1)
			{
				ic_val2 = HAL_TIM_ReadCapturedValue(htim,TIM_CHANNEL_3);
				__HAL_TIM_SET_COUNTER(htim,0);
				if(ic_val2 > ic_val1)
				{
					diff = ic_val2 - ic_val1;
				}
				else if(ic_val2 < ic_val1)
				{ 
					diff = 0xffff + ic_val2 - ic_val1;
				} 
				distance = 0.017*diff;
				first_cap = 0;
				__HAL_TIM_SET_CAPTUREPOLARITY(htim,TIM_CHANNEL_3, TIM_INPUTCHANNELPOLARITY_RISING);
				__HAL_TIM_DISABLE_IT(htim, TIM_IT_CC3);
			}
		}
	}
}

float tocdoquay(float pre_pos,float pos)
{	
	float w;		
	if (pre_pos > pos) 
	{
		w = (pre_pos - pos)*600/44/45;
	}
	else if (pre_pos == pos) w = 0;
	else if (pos > pre_pos) 
	{
		w = (65535 - pos + pre_pos )*600/44/45; //1.15*3.03
	}
	// if(w>130) w=130;
  return w;
}
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void dithang(void)
{
	HAL_GPIO_WritePin(L1_GPIO_Port,L1_Pin,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(L2_GPIO_Port,L2_Pin,GPIO_PIN_SET);
	HAL_GPIO_WritePin(R1_GPIO_Port,R1_Pin,GPIO_PIN_SET);
	HAL_GPIO_WritePin(R2_GPIO_Port,R2_Pin,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(STBY_GPIO_Port,STBY_Pin,GPIO_PIN_SET);
	__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_3,500); //left
	__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_4,500);
}
void dilui(void)
{
	HAL_GPIO_WritePin(L1_GPIO_Port,L1_Pin,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(L2_GPIO_Port,L2_Pin,GPIO_PIN_SET);
	HAL_GPIO_WritePin(R1_GPIO_Port,R1_Pin,GPIO_PIN_SET);
	HAL_GPIO_WritePin(R2_GPIO_Port,R2_Pin,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(STBY_GPIO_Port,STBY_Pin,GPIO_PIN_SET);
	__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_3,500); //left
	__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_4,500);
}

void retrai(void)
{
	HAL_GPIO_WritePin(L1_GPIO_Port,L1_Pin,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(L2_GPIO_Port,L2_Pin,GPIO_PIN_SET);
	HAL_GPIO_WritePin(R1_GPIO_Port,R1_Pin,GPIO_PIN_SET);
	HAL_GPIO_WritePin(R2_GPIO_Port,R2_Pin,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(STBY_GPIO_Port,STBY_Pin,GPIO_PIN_SET);
	__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_3,600); //left
	__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_4,300);
}

void rephai(void)
{
	HAL_GPIO_WritePin(L1_GPIO_Port,L1_Pin,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(L2_GPIO_Port,L2_Pin,GPIO_PIN_SET);
	HAL_GPIO_WritePin(R1_GPIO_Port,R1_Pin,GPIO_PIN_SET);
	HAL_GPIO_WritePin(R2_GPIO_Port,R2_Pin,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(STBY_GPIO_Port,STBY_Pin,GPIO_PIN_SET);
	__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_3,300); //left
	__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_4,600);
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
  {
  /* USER CODE BEGIN 1 */

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
  MX_DMA_Init();
  MX_TIM1_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_TIM4_Init();
  MX_TIM8_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */
	HAL_TIM_IC_Start_IT(&htim3,TIM_CHANNEL_1);
	HAL_TIM_IC_Start_IT(&htim3,TIM_CHANNEL_2);
	HAL_TIM_IC_Start_IT(&htim3,TIM_CHANNEL_3);
	HAL_TIM_IC_Start_IT(&htim3,TIM_CHANNEL_4);
	HAL_TIM_IC_Start_IT(&htim4,TIM_CHANNEL_2);
	HAL_TIM_IC_Start_IT(&htim4,TIM_CHANNEL_3);
//	HAL_TIM_IC_Start_IT(&htim8,TIM_CHANNEL_3);
	HAL_TIM_Encoder_Start(&htim1,TIM_CHANNEL_1);
	HAL_TIM_Encoder_Start(&htim1,TIM_CHANNEL_2);
	HAL_TIM_Encoder_Start(&htim8,TIM_CHANNEL_1);
	HAL_TIM_Encoder_Start(&htim8,TIM_CHANNEL_2);			
	HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_4);
	
	HAL_GPIO_WritePin(L1_GPIO_Port,L1_Pin,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(L2_GPIO_Port,L2_Pin,GPIO_PIN_SET);
	HAL_GPIO_WritePin(R1_GPIO_Port,R1_Pin,GPIO_PIN_SET);
	HAL_GPIO_WritePin(R2_GPIO_Port,R2_Pin,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(STBY_GPIO_Port,STBY_Pin,GPIO_PIN_SET);
//			__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_3,1000); //left
//		__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_4,1000);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
//		HCS04_trig(Trig_T3_CH1_GPIO_Port,Trig_T3_CH1_Pin,3,1);
////		HAL_Delay(100);  
//		HCS04_trig(Trig_T3_CH2_GPIO_Port,Trig_T3_CH2_Pin,3,2);
////		HAL_Delay(100);
//		HCS04_trig(Trig_T3_CH3_GPIO_Port,Trig_T3_CH3_Pin,3,3);
////		HAL_Delay(100);
//		HCS04_trig(Trig_T3_CH4_GPIO_Port,Trig_T3_CH4_Pin,3,4);
////		HAL_Delay(100);
//		HCS04_trig(Trig_T4_CH2_GPIO_Port,Trig_T4_CH2_Pin,4,2);
////		HAL_Delay(100);
//		HCS04_trig(Trig_T4_CH3_GPIO_Port,Trig_T4_CH3_Pin,4,3);
//		HAL_Delay(100);
//		pre_pos1=pos1;
//		pre_pos2=pos2;	
//		HAL_Delay(100);
//		pos1=__HAL_TIM_GetCounter(&htim1);
//		pos2=__HAL_TIM_GetCounter(&htim8);	
//	//		__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_3,600); //left
//	//	__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_4,300);	
//		w1= tocdoquay(pre_pos1,pos1);
//		w2= tocdoquay(pre_pos2,pos2); 
		HAL_UART_Receive_DMA(&huart2,(uint8_t *)str, sizeof(str));
//		if(str[0] == 'F')
//		{
//			dithang();
//		}
//		else if(str[0] == 'R')
//		{
//			rephai();
//		}
//		else if(str[0] == 'L')
//		{
//			retrai();
//		}
//		else if(str[0] == 'G')
//		{
//			dilui();
//		}
//		else 
//		{
//			__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_3,0); //left
//			__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_4,0);
//		}
			w = str[0];
			vR = str[1];
			vleft = 0.5*(2*vR - w*b);
			vright = 0.5*(2*vR + w*b);
			wleft=(vleft*60*2/D)/(2*3.14);
			wright=(vright*60*2/D)/(2*3.14);
			pwm1 = wleft * 1000/130; 
			pwm2 = wright*1000/130;
			__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_3,pwm1); //left
			__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_4,pwm2);
//		HAL_Delay(100);
//		str[0] = 'S';
//		HAL_UART_Transmit_IT(&huart2,(uint8_t *)strn,sizeof(strn));
//		HAL_Delay(100);
		//HAL_Delay(500);
  }
  /* USER CODE END 3 */
}

//void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
//{
//	if(str[0] == 'F')
//		{
//			dithang();
//		}
//		else if(str[0] == 'R')
//		{
//			rephai();
//		}
//		else if(str[0] == 'L')
//		{
//			retrai();
//		}
//		else if(str[0] == 'G')
//		{
//			dilui();
//		}
//		else 
//		{
//			__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_3,0); //left
//			__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_4,0);
//		}

//}
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
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief TIM1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM1_Init(void)
{

  /* USER CODE BEGIN TIM1_Init 0 */

  /* USER CODE END TIM1_Init 0 */

  TIM_Encoder_InitTypeDef sConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM1_Init 1 */

  /* USER CODE END TIM1_Init 1 */
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 0;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 65535;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  sConfig.EncoderMode = TIM_ENCODERMODE_TI12;
  sConfig.IC1Polarity = TIM_ICPOLARITY_RISING;
  sConfig.IC1Selection = TIM_ICSELECTION_DIRECTTI;
  sConfig.IC1Prescaler = TIM_ICPSC_DIV1;
  sConfig.IC1Filter = 0;
  sConfig.IC2Polarity = TIM_ICPOLARITY_RISING;
  sConfig.IC2Selection = TIM_ICSELECTION_DIRECTTI;
  sConfig.IC2Prescaler = TIM_ICPSC_DIV1;
  sConfig.IC2Filter = 0;
  if (HAL_TIM_Encoder_Init(&htim1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM1_Init 2 */

  /* USER CODE END TIM1_Init 2 */

}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 167;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 999;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_PWM_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_3) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_4) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */
  HAL_TIM_MspPostInit(&htim2);

}

/**
  * @brief TIM3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM3_Init(void)
{

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_IC_InitTypeDef sConfigIC = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 83;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 65535;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_IC_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_RISING;
  sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
  sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
  sConfigIC.ICFilter = 0;
  if (HAL_TIM_IC_ConfigChannel(&htim3, &sConfigIC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_IC_ConfigChannel(&htim3, &sConfigIC, TIM_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_IC_ConfigChannel(&htim3, &sConfigIC, TIM_CHANNEL_3) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_IC_ConfigChannel(&htim3, &sConfigIC, TIM_CHANNEL_4) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */

}

/**
  * @brief TIM4 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM4_Init(void)
{

  /* USER CODE BEGIN TIM4_Init 0 */

  /* USER CODE END TIM4_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_IC_InitTypeDef sConfigIC = {0};

  /* USER CODE BEGIN TIM4_Init 1 */

  /* USER CODE END TIM4_Init 1 */
  htim4.Instance = TIM4;
  htim4.Init.Prescaler = 83;
  htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim4.Init.Period = 65535;
  htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim4.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_IC_Init(&htim4) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim4, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_RISING;
  sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
  sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
  sConfigIC.ICFilter = 0;
  if (HAL_TIM_IC_ConfigChannel(&htim4, &sConfigIC, TIM_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_IC_ConfigChannel(&htim4, &sConfigIC, TIM_CHANNEL_3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM4_Init 2 */

  /* USER CODE END TIM4_Init 2 */

}

/**
  * @brief TIM8 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM8_Init(void)
{

  /* USER CODE BEGIN TIM8_Init 0 */

  /* USER CODE END TIM8_Init 0 */

  TIM_Encoder_InitTypeDef sConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM8_Init 1 */

  /* USER CODE END TIM8_Init 1 */
  htim8.Instance = TIM8;
  htim8.Init.Prescaler = 0;
  htim8.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim8.Init.Period = 65535;
  htim8.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim8.Init.RepetitionCounter = 0;
  htim8.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  sConfig.EncoderMode = TIM_ENCODERMODE_TI12;
  sConfig.IC1Polarity = TIM_ICPOLARITY_RISING;
  sConfig.IC1Selection = TIM_ICSELECTION_DIRECTTI;
  sConfig.IC1Prescaler = TIM_ICPSC_DIV1;
  sConfig.IC1Filter = 0;
  sConfig.IC2Polarity = TIM_ICPOLARITY_RISING;
  sConfig.IC2Selection = TIM_ICSELECTION_DIRECTTI;
  sConfig.IC2Prescaler = TIM_ICPSC_DIV1;
  sConfig.IC2Filter = 0;
  if (HAL_TIM_Encoder_Init(&htim8, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim8, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM8_Init 2 */

  /* USER CODE END TIM8_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Stream5_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream5_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream5_IRQn);
  /* DMA1_Stream6_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream6_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream6_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOE, Trig_T3_CH3_Pin|Trig_T3_CH4_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, Trig_T3_CH1_Pin|Trig_T3_CH2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, R2_Pin|R1_Pin|STBY_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, Trig_T4_CH2_Pin|L1_Pin|L2_Pin|Trig_T4_CH3_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : Trig_T3_CH3_Pin Trig_T3_CH4_Pin */
  GPIO_InitStruct.Pin = Trig_T3_CH3_Pin|Trig_T3_CH4_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pins : Trig_T3_CH1_Pin Trig_T3_CH2_Pin */
  GPIO_InitStruct.Pin = Trig_T3_CH1_Pin|Trig_T3_CH2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : R2_Pin R1_Pin STBY_Pin */
  GPIO_InitStruct.Pin = R2_Pin|R1_Pin|STBY_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pins : Trig_T4_CH2_Pin L1_Pin L2_Pin Trig_T4_CH3_Pin */
  GPIO_InitStruct.Pin = Trig_T4_CH2_Pin|L1_Pin|L2_Pin|Trig_T4_CH3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

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
