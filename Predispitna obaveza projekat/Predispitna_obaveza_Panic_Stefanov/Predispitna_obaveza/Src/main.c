/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
Zadatak 3:

Na ekranu napraviti dirke sa notama sintisajzera... Do, re, mi... kada korisnik klikne na dirke,
potrebno je da buzzer proizvodi odgovarajuće frekvencije tih muzičkih nota. Svaka od nota svira
samo dok je dirka pritisnuta.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "usb_device.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "usbd_cdc_if.h"
#include "SSD1963.h"
#include "STMPE610.h"
#include "GUI.h"
#include "LCDConf.h"

#include <stdint.h>
#include <stdio.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

I2C_HandleTypeDef hi2c1;

TIM_HandleTypeDef htim4;

/* USER CODE BEGIN PV */
extern volatile GUI_TIMER_TIME OS_TimeMS;
uint16_t X_koordinata = 0;
uint16_t Y_koordinata = 0;
uint16_t keypressed = 0;
uint8_t prethodno_prisutan_dodir = 0;

GUI_MEMDEV_Handle hMem;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
static void MX_TIM4_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

void set_buzzer(uint16_t frekvencija, uint8_t faktor_ispune){
	__HAL_TIM_SetAutoreload(&htim4, 3840000/frekvencija); //set frequency
	__HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_3, faktor_ispune); //set volume
}

void HAL_SYSTICK_Callback(void)
{
	OS_TimeMS++;
}

void DrawNoteText(int x, int y, int width, int height, const char *note) {
    int textX = x + width / 2 - 10 + 3; // Centriranje teksta na sredinu bele tipke
    int textY = y + height - 30; // Pomeranje teksta prema dnu bele tipke
    GUI_SetBkColor(GUI_WHITE);
    GUI_SetColor(GUI_BLACK);
    GUI_SetFont(&GUI_Font16B_ASCII); // Podešavanje veličine fonta po potrebi
    GUI_DispStringAt(note, textX, textY);
}

void DrawWhiteKey(int x, int y, int width, int height) {
    // Crtanje belod dela tipke
    //GUI_SetColor(GUI_WHITE);
    //GUI_FillRect(x, y, x + width, y + height);

    // Crtanje okvira belih tipki
    GUI_SetColor(GUI_BLACK);
    GUI_DrawRect(x, y, x + width, y + height);
}

void DrawBlackKey(float x, int y, int width, int height) {
    GUI_SetColor(GUI_BLACK);
    GUI_FillRect(x, y, x + width, y + height);
}

void DrawPianoKeys() {

    GUI_Init();
    GUI_SetBkColor(GUI_WHITE);
    GUI_Clear();


    GUI_SetFont(&GUI_Font32_1);
    GUI_SetColor(GUI_BLACK);
    GUI_SetBkColor(GUI_WHITE);

    // Definirajte širinu i visinu tipki klavira
    int keyWidth = 33;
    int keyHeight = 150;

    // Pomoćne promenljive za crtanje tipki
    float x = 2;
    int y = 55;

    //naziv tipki
    const char* noteNames[] = {"C4", "D4", "E4", "F4", "G4", "A4", "B4",
                               "C5", "D5", "E5", "F5", "G5", "A5", "B5"};


    // Crtanje tipki redom C,D,E,F...
    for (int i = 0; i < 14; i ++) {
    	 // Bela tipka i tekst
    	    DrawWhiteKey(x, y, keyWidth, keyHeight);
    	    DrawNoteText(x, y, keyWidth, keyHeight, noteNames[i]);

            // Dodavanje crnih tipki između određenih belih tipki
            if (i == 0 || i == 1 || i == 3 || i == 4 || i == 5 || i == 7 || i == 8 || i == 10 || i == 11 || i == 12) {
                DrawBlackKey(x + keyWidth - 9, y, 19, keyHeight / 2); //-9 kako bi presla crna tipka preko prve bele tipke
            }

    	    // Povecanje x za širinu bele tipke
    	    x += keyWidth + 1; // +1 za crnu liniju
    }

    // Prikaz rezultata
    GUI_Exec();
}

// Funkcija koja se poziva kada se detektuje dodir
void process_touch() {
    STMPE610_read_xyz();
    GUI_TOUCH_Exec();
    keypressed = GUI_GetKey();

    X_koordinata = STMPE610_GetX_TOUCH();
    Y_koordinata = STMPE610_GetY_TOUCH();

    // Provera da li je dodir unutar određenih koordinata
    if (X_koordinata > 2 && X_koordinata < 35 && Y_koordinata > 55 && Y_koordinata < 205) {
        set_buzzer(261.63, 80); // Postavljanje frekvencije i jačine zvuka za prvu koordinatu
        prethodno_prisutan_dodir = 1; //promenljiva koja nam omogucava da se cuje zvuk samo kada je pritisnuta tipka
    }
    else if (X_koordinata > 26 && X_koordinata < 45 && Y_koordinata > 55 && Y_koordinata < 130) {
        set_buzzer(277.18, 80); // Postavljanje frekvencije i jačine zvuka za drugu koordinatu
        prethodno_prisutan_dodir = 1; //promenljiva koja nam omogucava da se cuje zvuk samo kada je pritisnuta tipka
    }
    else if (X_koordinata > 35 && X_koordinata < 68 && Y_koordinata > 55 && Y_koordinata < 205) {
        set_buzzer(293.66, 80); // Postavljanje frekvencije i jačine zvuka za drugu koordinatu
        prethodno_prisutan_dodir = 1; //promenljiva koja nam omogucava da se cuje zvuk samo kada je pritisnuta tipka
    }
    else if (X_koordinata > 59 && X_koordinata < 78 && Y_koordinata > 55 && Y_koordinata < 130) {
        set_buzzer(311.13, 80); // Postavljanje frekvencije i jačine zvuka za drugu koordinatu
        prethodno_prisutan_dodir = 1; //promenljiva koja nam omogucava da se cuje zvuk samo kada je pritisnuta tipka
    }
    else if(X_koordinata > 68 && X_koordinata < 101 && Y_koordinata > 55 && Y_koordinata < 205){
    	set_buzzer(329.63, 80); // Postavljanje frekvencije i jačine zvuka za prvu koordinatu
    	        prethodno_prisutan_dodir = 1; //promenljiva koja nam omogucava da se cuje zvuk samo kada je pritisnuta tipka
    }
    else if(X_koordinata > 101 && X_koordinata < 134 && Y_koordinata > 55 && Y_koordinata < 205){
    	set_buzzer(349.23, 80); // Postavljanje frekvencije i jačine zvuka za prvu koordinatu
    	        prethodno_prisutan_dodir = 1; //promenljiva koja nam omogucava da se cuje zvuk samo kada je pritisnuta tipka
     }
    else if (X_koordinata > 125 && X_koordinata < 144 && Y_koordinata > 55 && Y_koordinata < 130) {
        set_buzzer(369.99, 80); // Postavljanje frekvencije i jačine zvuka za drugu koordinatu
        prethodno_prisutan_dodir = 1; //promenljiva koja nam omogucava da se cuje zvuk samo kada je pritisnuta tipka
    }
    else if (X_koordinata > 158 && X_koordinata < 177 && Y_koordinata > 55 && Y_koordinata < 130) {
        set_buzzer(415.30, 80); // Postavljanje frekvencije i jačine zvuka za drugu koordinatu
        prethodno_prisutan_dodir = 1; //promenljiva koja nam omogucava da se cuje zvuk samo kada je pritisnuta tipka
    }
    else if(X_koordinata > 134 && X_koordinata < 167 && Y_koordinata > 55 && Y_koordinata < 205){
        set_buzzer(392, 80); // Postavljanje frekvencije i jačine zvuka za prvu koordinatu
        prethodno_prisutan_dodir = 1; //promenljiva koja nam omogucava da se cuje zvuk samo kada je pritisnuta tipka
        }
    else if (X_koordinata > 191 && X_koordinata < 210 && Y_koordinata > 55 && Y_koordinata < 130) {
        set_buzzer(466.16, 80); // Postavljanje frekvencije i jačine zvuka za drugu koordinatu
        prethodno_prisutan_dodir = 1; //promenljiva koja nam omogucava da se cuje zvuk samo kada je pritisnuta tipka
    }
    else if(X_koordinata > 167 && X_koordinata < 200 && Y_koordinata > 55 && Y_koordinata < 205){
        set_buzzer(440, 80); // Postavljanje frekvencije i jačine zvuka za prvu koordinatu
        prethodno_prisutan_dodir = 1; //promenljiva koja nam omogucava da se cuje zvuk samo kada je pritisnuta tipka
        }
    else if(X_koordinata > 200 && X_koordinata < 233 && Y_koordinata > 55 && Y_koordinata < 205){
        set_buzzer(493.88, 80); // Postavljanje frekvencije i jačine zvuka za prvu koordinatu
        prethodno_prisutan_dodir = 1; //promenljiva koja nam omogucava da se cuje zvuk samo kada je pritisnuta tipka
        }
    else if (X_koordinata > 257 && X_koordinata < 276 && Y_koordinata > 55 && Y_koordinata < 130) {
        set_buzzer(554.37, 80); // Postavljanje frekvencije i jačine zvuka za drugu koordinatu
        prethodno_prisutan_dodir = 1; //promenljiva koja nam omogucava da se cuje zvuk samo kada je pritisnuta tipka
    }
    else if(X_koordinata > 233 && X_koordinata < 266 && Y_koordinata > 55 && Y_koordinata < 205){
        set_buzzer(523.25, 80); // Postavljanje frekvencije i jačine zvuka za prvu koordinatu
        prethodno_prisutan_dodir = 1; //promenljiva koja nam omogucava da se cuje zvuk samo kada je pritisnuta tipka
        }
    else if (X_koordinata > 290 && X_koordinata < 309 && Y_koordinata > 55 && Y_koordinata < 130) {
        set_buzzer(622.25, 80); // Postavljanje frekvencije i jačine zvuka za drugu koordinatu
        prethodno_prisutan_dodir = 1;
    }
    else if(X_koordinata > 266 && X_koordinata < 299 && Y_koordinata > 55 && Y_koordinata < 205){
        set_buzzer(587.33, 80); // Postavljanje frekvencije i jačine zvuka za prvu koordinatu
        prethodno_prisutan_dodir = 1; //promenljiva koja nam omogucava da se cuje zvuk samo kada je pritisnuta tipka
        }
    else if(X_koordinata > 299 && X_koordinata < 332 && Y_koordinata > 55 && Y_koordinata < 205){
        set_buzzer(659.26, 80); // Postavljanje frekvencije i jačine zvuka za prvu koordinatu
        prethodno_prisutan_dodir = 1; //promenljiva koja nam omogucava da se cuje zvuk samo kada je pritisnuta tipka
        }
    else if(X_koordinata > 332 && X_koordinata < 365 && Y_koordinata > 55 && Y_koordinata < 205){
        set_buzzer(698.49, 80); // Postavljanje frekvencije i jačine zvuka za prvu koordinatu
        prethodno_prisutan_dodir = 1; //promenljiva koja nam omogucava da se cuje zvuk samo kada je pritisnuta tipka
        }
    else if (X_koordinata > 389 && X_koordinata < 408 && Y_koordinata > 55 && Y_koordinata < 130) {
        set_buzzer(739.99, 80); // Postavljanje frekvencije i jačine zvuka za drugu koordinatu
        prethodno_prisutan_dodir = 1;
    }
    else if(X_koordinata > 365 && X_koordinata < 398 && Y_koordinata > 55 && Y_koordinata < 205){
        set_buzzer(783.99, 80); // Postavljanje frekvencije i jačine zvuka za prvu koordinatu
        prethodno_prisutan_dodir = 1; //promenljiva koja nam omogucava da se cuje zvuk samo kada je pritisnuta tipka
        }
    else if (X_koordinata > 422 && X_koordinata < 441 && Y_koordinata > 55 && Y_koordinata < 130) {
        set_buzzer(830.61, 80); // Postavljanje frekvencije i jačine zvuka za drugu koordinatu
        prethodno_prisutan_dodir = 1;
    }
    else if(X_koordinata > 398 && X_koordinata < 431 && Y_koordinata > 55 && Y_koordinata < 205){
        set_buzzer(880, 80); // Postavljanje frekvencije i jačine zvuka za prvu koordinatu
        prethodno_prisutan_dodir = 1; //promenljiva koja nam omogucava da se cuje zvuk samo kada je pritisnuta tipka
        }
    else if (X_koordinata > 455 && X_koordinata < 474 && Y_koordinata > 55 && Y_koordinata < 130) {
        set_buzzer(932.33, 80); // Postavljanje frekvencije i jačine zvuka za drugu koordinatu
        prethodno_prisutan_dodir = 1;
    }
    else if(X_koordinata > 431 && X_koordinata < 464 && Y_koordinata > 55 && Y_koordinata < 205){
        set_buzzer(987.77, 80); // Postavljanje frekvencije i jačine zvuka za prvu koordinatu
        prethodno_prisutan_dodir = 1; //promenljiva koja nam omogucava da se cuje zvuk samo kada je pritisnuta tipka
        }
    else {
        // Ako dodir nije detektovan unutar određenih koordinata, iskljucuje se zvuk
        if (prethodno_prisutan_dodir == 1) {
            set_buzzer(0, 0); // Isključivanje zvučnika
            prethodno_prisutan_dodir = 0;
        }
    }

    // Malo kašnjenje kako bi se izbeglo često izvršavanje ove funkcije
    GUI_Delay(10);
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
  MX_I2C1_Init();
  MX_TIM4_Init();
  MX_USB_DEVICE_Init();
  Init_LCD_GPIO();
  Init_TOUCH_GPIO(hi2c1);

  STMPE610_Init();
  //WM_SetCreateFlags(WM_CF_MEMDEV); // eliminise flickering

  GUI_Init();
  /* USER CODE BEGIN 2 */
  HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_3);
  //set_buzzer(400,80);//provera da li buzzer radi

  //provera da li ekran radi
  /*
  GUI_SetBkColor(GUI_YELLOW);
  GUI_Clear();
  GUI_SetColor(GUI_BLACK);
  GUI_SetFont(&GUI_Font32_1);
  GUI_DispString("Hello");*/

  //iscrtavanje klavira
  DrawPianoKeys();

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  //provera da li je neka tipka pritisnuta
	  process_touch();
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
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

  /** Configure the main internal regulator output voltage 
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 25;
  RCC_OscInitStruct.PLL.PLLN = 384;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 8;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Activate the Over-Drive mode 
  */
  if (HAL_PWREx_EnableOverDrive() != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_6) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_I2C1|RCC_PERIPHCLK_CLK48;
  PeriphClkInitStruct.I2c1ClockSelection = RCC_I2C1CLKSOURCE_PCLK1;
  PeriphClkInitStruct.Clk48ClockSelection = RCC_CLK48SOURCE_PLL;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.Timing = 0x20303E5D;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Analogue filter 
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Digital filter 
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

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

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM4_Init 1 */

  /* USER CODE END TIM4_Init 1 */
  htim4.Instance = TIM4;
  htim4.Init.Prescaler = 25;
  htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim4.Init.Period = 9600;
  htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim4.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim4) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim4, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim4) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim4, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim4, &sConfigOC, TIM_CHANNEL_3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM4_Init 2 */

  /* USER CODE END TIM4_Init 2 */
  HAL_TIM_MspPostInit(&htim4);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOG, GPIO_PIN_15, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3|GPIO_PIN_4, GPIO_PIN_RESET);

  /*Configure GPIO pin : PG15 */
  GPIO_InitStruct.Pin = GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

  /*Configure GPIO pins : PB3 PB4 */
  GPIO_InitStruct.Pin = GPIO_PIN_3|GPIO_PIN_4;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

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
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
