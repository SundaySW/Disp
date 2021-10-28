
#include <cstring>
#include "main.h"
#include "tim.h"
#include "gpio.h"
#include "logo.h"
#include "i2c.h"
#include "iwdg.h"
#include "MainScreenBackGround.h"
#include "passBackGround.h"
#include "settingsBackGround.h"
#include "MainController.hpp"
#include "Screens/MainScreen.hpp"
#include "Screens/SettingsBase.hpp"
#include "Screens/UserSettingsScreen.hpp"
#include "Screens/AdminSettingsScreen.hpp"
#include "Screens/PasswordScreen.hpp"
#include "Button.hpp"

void SystemClock_Config();

using ScreenList = std::list<IScreen*>;
ScreenList list1;
Button btn1(BTN2);
Button btn2(BTN1);
Button btn3(BTN_PLUS);
Button btn4(BTN_MINUS);
Button btn5(BTN_MODE);

auto mainScreen = Singleton<MainScreen<MainScreenBackPtr>>::GetInstance();
auto passwordScreen = Singleton<PasswordScreen<PassBackPtr>>::GetInstance();
auto userSettingsScreen = Singleton<SettingsBase<SettingsScreenBackPtr,USER_SETTINGS_SCREEN,UserSettingsScr>>::GetInstance();
auto adminSettingsScreen = Singleton<SettingsBase<SettingsScreenBackPtr,ADMIN_SETTINGS_SCREEN,AdminSettingsScr>>::GetInstance();
auto mainController = Singleton<MainController<IScreen*,ScreenList, SCREEN_TYPE>>::GetInstance();

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if(htim->Instance == TIM3){
        if(--motorTime == 0) mainController->motorStopped();
    }
    if(htim->Instance == TIM15){
        HAL_IWDG_Refresh(&hiwdg);
        mainController->update();
    }
    if(htim->Instance == TIM16){
        OnTime++;
    }
}

bool grinding_button_check = false;
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if(now_grinding){
        if(grinding_button_check){
            grinding_button_check = false;
        }else {
            grinding_button_check = true;
            return;
        }
    }

    switch (GPIO_Pin) {
        case BTN_1_Pin:
            mainController->btn_pressed(btn1);
            break;
        case BTN_2_Pin:
            mainController->btn_pressed(btn2);
            break;
        case BTN_PLUS_Pin:
            mainController->btn_pressed(btn3);
            break;
        case BTN_MINUS_Pin:
            mainController->btn_pressed(btn4);
            break;
        default:
            break;
    }
}

void EXTI_clear_enable(){
    __HAL_GPIO_EXTI_CLEAR_IT(BTN_1_Pin);
    __HAL_GPIO_EXTI_CLEAR_IT(BTN_2_Pin);
    __HAL_GPIO_EXTI_CLEAR_IT(BTN_PLUS_Pin);
    __HAL_GPIO_EXTI_CLEAR_IT(BTN_MINUS_Pin);
    NVIC_ClearPendingIRQ(EXTI4_15_IRQn);
    HAL_NVIC_EnableIRQ(EXTI4_15_IRQn);
}

/**
  * @brief  The application entry point.
  * @retval int
  */
int main()
{
    HAL_Init();
    SystemClock_Config();

    MX_GPIO_Init();
    MX_TIM1_Init();
    MX_TIM3_Init();
    MX_TIM14_Init();
    MX_TIM15_Init();
    MX_TIM16_Init();
    MX_TIM17_Init();

    MX_I2C2_Init();

    HAL_TIM_Base_Start(&htim14);
    HAL_TIM_PWM_Start_IT(&htim1, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start_IT(&htim17, TIM_CHANNEL_1);

    MX_IWDG_Init();

    Disp_init();
    delay(120);

    list1.push_back(mainScreen);
    list1.push_back(userSettingsScreen);
    list1.push_back(adminSettingsScreen);
    list1.push_back(passwordScreen);
    mainController->setController(list1, mainScreen);

    HAL_TIM_Base_Start_IT(&htim15);
    HAL_TIM_Base_Start_IT(&htim16);

    EXTI_clear_enable();

    while (1){}
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_1);

  if(LL_FLASH_GetLatency() != LL_FLASH_LATENCY_1)
  {
  Error_Handler();
  }
  LL_RCC_HSI_Enable();

   /* Wait till HSI is ready */
  while(LL_RCC_HSI_IsReady() != 1)
  {
    
  }
  LL_RCC_HSI_SetCalibTrimming(16);
  LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSI_DIV_2, LL_RCC_PLL_MUL_12);
  LL_RCC_PLL_Enable();

   /* Wait till PLL is ready */
  while(LL_RCC_PLL_IsReady() != 1)
  {
    
  }
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);

   /* Wait till System clock is ready */
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL)
  {
  
  }
  LL_SetSystemCoreClock(48000000);

   /* Update the time base */
  if (HAL_InitTick (TICK_INT_PRIORITY) != HAL_OK)
  {
    Error_Handler();  
  };
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{

  /* User can add his own implementation to report the HAL error return state */
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
