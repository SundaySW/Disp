//
// Created by outlaw on 15.03.2021.
//

#ifndef UNTITLED_MAINCONTROLLER_HPP
#define UNTITLED_MAINCONTROLLER_HPP

#include "HardScreenFuncs.hpp"
#include <list>
#include "Singleton.hpp"

//Runtime vars
uint32_t OnTime = 0;
uint32_t OnWeight = 0;
uint32_t Btn1Count = 0;
uint32_t Btn2Count = 0;
BTN_TYPE selectedBTN = BTN_none;
bool now_grinding = false;
uint32_t motorTime = 0;
uint32_t TotalCounterTotal;
uint32_t Modality = 0;

//EEPROM Vars
uint32_t Screen_BackLight_value;
uint32_t Out_BackLight_value;
uint32_t TotalCounter1;
uint32_t TotalCounter2;
uint32_t Grind1Time;
uint32_t Grind2Time;
uint32_t TimeReminder;
uint32_t WeightPerCycle;

uint32_t* EEPROM_VarsPtrArray[] = {
        &Screen_BackLight_value,
        &Out_BackLight_value,
        &TotalCounter1,
        &TotalCounter2,
        &Grind1Time,
        &Grind2Time,
        &TimeReminder,
        &WeightPerCycle,
//        &Modality
};


template<typename ScreenPtrType, typename ListType, typename SCREEN_TYPE>
class MainController {
    friend class Singleton<MainController>;

public:
    MainController() = default;

    void setController(ListType incomeList, ScreenPtrType setCurrentScreen){
        screenList = incomeList;
        currentScreenPtr = setCurrentScreen;
        loadVarsFromEEPROM();
        setBackLightValue(Screen_BackLight_value);
        setOutBackLightValue(Out_BackLight_value);
        currentScreenPtr->update_whole_buffer();
    }

    void btn_pressed(Button btn) {
        SCREEN_TYPE retVal = currentScreenPtr->invoke_btn_func(btn.Type);
        if (retVal != currentScreenPtr->getType()) {
           //todo сделать обработчик на €ркость экрана
            setBackLightValue(Screen_BackLight_value);
            setOutBackLightValue(Out_BackLight_value);
            changeScreen(retVal);
        }
//        currentScreenPtr->update_partOf_buffer();
    }

    auto getBackLightValue() const {
        return Screen_BackLight_value;
    }

    void setBackLightValue(uint8_t value) {
        if (value > 0 && value < 16) changingPWM_Width_Screen(value);
    }
    void setOutBackLightValue(uint8_t value) {
        if (value > 0 && value < 16) changingPWM_Width_Out(value);
    }

    void update() {
        currentScreenPtr->update_partOf_buffer();
        controlLEDS();
    }

    ScreenPtrType getScreenPtrFromList(SCREEN_TYPE findType) {
        for (auto scr: screenList) if (scr->getType() == findType) return scr;
        return currentScreenPtr;
    }

    void motorStopped(){
        now_grinding = false;
        stopMotor();
    }

private:
    ListType screenList;
    ScreenPtrType currentScreenPtr;

    void changeScreen(SCREEN_TYPE switchScreenType) {
        for (auto scr: screenList) if (scr->getType() == switchScreenType) currentScreenPtr = scr;
        saveVarsToEEPROM();
        clearScreen();
        currentScreenPtr->update_whole_buffer();
    }

    void loadVarsFromEEPROM() {
        uint8_t counter = 0;
        for(uint32_t* ptr : EEPROM_VarsPtrArray) I2C_EEPROM_ReadData((uint8_t*)(ptr), 4, (counter++ * 4));
        TotalCounterTotal = TotalCounter1 + (TotalCounter2 * 2);
    }

    void saveVarsToEEPROM(){
        uint16_t varsCollectionSize = sizeof(EEPROM_VarsPtrArray) / sizeof(*EEPROM_VarsPtrArray);
        uint32_t varsCollection[varsCollectionSize];
        uint8_t counter = 0;

        for(uint32_t* var: EEPROM_VarsPtrArray) varsCollection[counter++] = *var;

        I2C_EEPROM_WriteData((uint8_t*)(varsCollection), varsCollectionSize * 4);
    }

    static inline void controlLEDS(){
        if(Modality || now_grinding) buttonsLEDSwitcher(selectedBTN);
        else buttonsLEDSwitcher(BTN_none);
    }
};

//template<typename T>
//void MainController::makeButtons(T func, ...) {
//    va_list args_ptr;
//    va_start(args_ptr,func);
//    for(int i = 0; args_ptr[i] != '\0' && i < 4; ++i ) {
//        ptrArray[i]->setFunc(va_arg(args_ptr, T));
//    }
//    va_end(args_ptr);
//}

#endif //UNTITLED_MAINCONTROLLER_HPP