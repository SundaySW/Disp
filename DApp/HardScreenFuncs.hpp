//
// Created by outlaw on 25.03.2021.
//

#ifndef DISPAPP_HARDSCREENFUNCS_HPP
#define DISPAPP_HARDSCREENFUNCS_HPP

#include <string>
#include "main.h"
#include "Button.hpp"

#define DISP_CLEAR							((uint8_t)0x01U) //0b00000001
#define DISP_CONTROL						((uint8_t)0x0FU) //0b00001111
#define DISP_FUNCTION_SET					((uint8_t)0x30U) //0b00110000
#define DISP_ENTRY_MODE						((uint8_t)0x06U) //0b00000110
#define DISP_LINE0							((uint8_t)0x80U)
#define DISP_COLUMN0						((uint8_t)0x80U)
#define DEV_ADDR                            ((uint16_t)0xA0) // HAL expects address to be shifted one bit to the left(orig=0x50)

/**
  * @brief  Set delay on base of APB freq and tim settingsBack.
  * @param  delay in micro seconds
  * @retval None
  */
static inline void delay(uint16_t delay){
    __disable_irq ();
    TIM14->CNT = 0;
    while(TIM14->CNT < delay){}
    __enable_irq ();
}

static constexpr inline uint32_t calc_bits(uint8_t command){
    uint32_t bits = 0x0000U;
    bits = bits | (command & ((uint8_t)0x07U));
    command = command & ((uint8_t)0xF8U);
    return bits | (command << (uint8_t)0x07U);
}

static inline void Disp_sendCOMMAND(uint8_t command){
    uint32_t bits = calc_bits(command);
    GPIOB->BRR = (uint32_t)0x7C0FU; 			//DISP0-7 - 0 + RW clear bits
    GPIOA->BRR = (uint32_t)0x8800U; 			//PA11(DISP_E)+PA15(DISP_RS)bits clear
    GPIOA->BSRR = (uint32_t)DISP_E_Pin;			//E - 1
    GPIOB->BSRR = (uint32_t)bits;
    GPIOA->BRR = (uint32_t)DISP_E_Pin;			//E - 0
    delay(40);
}

static inline void Disp_sendDATA(uint8_t command){
    uint32_t bits = calc_bits(command);
    GPIOB->BRR = 	(uint32_t)0x7C0FU;			//DISP0-7 - 0 & RW - 0
    GPIOA->BSRR = 	(uint32_t)0x8800;			//RS - 1 & E - 1
    GPIOB->BSRR = 	(uint32_t)bits;
    GPIOA->BRR = 	(uint32_t)DISP_E_Pin;		//E - 0
    delay(40);
}

static inline  void Disp_init(){
    GPIOA->BSRR = (uint32_t)DISP_CS1_Pin;
    GPIOB->BRR = (uint32_t)DISP_RST_Pin;
    delay(10);
    GPIOB->BSRR = (uint32_t)DISP_RST_Pin;
    delay(40000);
    Disp_sendCOMMAND(DISP_FUNCTION_SET);
    delay(100);
    Disp_sendCOMMAND(DISP_FUNCTION_SET);
    delay(42);
    Disp_sendCOMMAND(0xCU);
    delay(100);
    Disp_sendCOMMAND(DISP_CLEAR);
    delay(10000);
    Disp_sendCOMMAND(DISP_ENTRY_MODE);
}

static inline void fillScreen(uint8_t data){
    Disp_sendCOMMAND(0x36U);
    Disp_sendCOMMAND(0x40U);
    uint16_t y, x, indx;
    for(y = 0; y < 32; y++){
        Disp_sendCOMMAND((uint8_t) (DISP_LINE0 + y));
        Disp_sendCOMMAND(DISP_COLUMN0);
        indx = y*16;
        for(x = 0; x < 16; x++) Disp_sendDATA(data);
        indx += 32*16;
        for(x = 0; x < 16; x++) Disp_sendDATA(data);
    }
}

static inline void clearScreen(){
    Disp_sendCOMMAND(DISP_CLEAR);
    delay(1000);
}

static inline void fillScreenFromBuffer(const uint8_t *buf_adr_ptr){
    Disp_sendCOMMAND(0x36U);
    Disp_sendCOMMAND(0x40U);
    uint16_t y, x, indx;
    for(y = 0; y < 32; y++){
        Disp_sendCOMMAND((uint8_t) (DISP_LINE0 + y));
        Disp_sendCOMMAND(DISP_COLUMN0);
        indx = y*16;
        for(x = 0; x < 16; x++) Disp_sendDATA(*(buf_adr_ptr+(x+indx)));
        indx += 32*16;
        for(x = 0; x < 16; x++) Disp_sendDATA(*(buf_adr_ptr+(x+indx)));
    }
}

inline void printTextWithCoords(const std::string& incomeText, uint8_t Position){
    Disp_sendCOMMAND(DISP_FUNCTION_SET);
    Disp_sendCOMMAND(0x80U + Position);
    for(char letter: incomeText) Disp_sendDATA(letter);
}

uint16_t I2C_MEM_START_ADDR = 0x0100;
uint16_t I2C_MEM_2lIST_ADDR = 0x0200;

void I2C_EEPROM_WriteData(uint8_t* dataptr, uint16_t size) {
    uint16_t memAddr = I2C_MEM_START_ADDR;
    HAL_StatusTypeDef status;
    HAL_I2C_Mem_Write(&hi2c2, DEV_ADDR, memAddr, I2C_MEMADD_SIZE_16BIT,
                      dataptr, size, HAL_MAX_DELAY);

    for(;;) {
        status = HAL_I2C_IsDeviceReady(&hi2c2, DEV_ADDR, 1, HAL_MAX_DELAY);
        if(status == HAL_OK)
            break;
    }
}

void I2C_EEPROM_ReadData(uint8_t* dataptr, uint16_t size, uint16_t shiftFromStartAddr) {
    shiftFromStartAddr += I2C_MEM_START_ADDR;
    HAL_I2C_Mem_Read(&hi2c2, DEV_ADDR, shiftFromStartAddr, I2C_MEMADD_SIZE_16BIT, dataptr, size, HAL_MAX_DELAY);
}

void changingPWM_Width_Screen(uint8_t value){
    TIM1->CCR1 = value;
}

void changingPWM_Width_Out(uint8_t value){
    TIM17->CCR1 = value;
}

void buttonsLEDSwitcher(BTN_TYPE button){
    HAL_GPIO_WritePin(BTN1_LED_GPIO_Port, BTN1_LED_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(BTN2_LED_GPIO_Port, BTN2_LED_Pin, GPIO_PIN_RESET);
    switch (button) {
        case BTN1:
            HAL_GPIO_WritePin(BTN2_LED_GPIO_Port, BTN2_LED_Pin, GPIO_PIN_SET);
            break;
        case BTN2:
            HAL_GPIO_WritePin(BTN1_LED_GPIO_Port, BTN1_LED_Pin, GPIO_PIN_SET);
            break;
        case BTN_PLUS:
        case BTN_MINUS:
        case BTN_MODE:
        case BTN_none:
            break;
    }
}

void startMotor(){
    HAL_GPIO_WritePin(OUT2_GPIO_Port, OUT2_Pin, GPIO_PIN_SET);
    HAL_TIM_Base_Start_IT(&htim3);
}

void stopMotor(){
    HAL_GPIO_WritePin(OUT2_GPIO_Port, OUT2_Pin, GPIO_PIN_RESET);
    HAL_TIM_Base_Stop_IT(&htim3);
}

#endif //DISPAPP_HARDSCREENFUNCS_HPP