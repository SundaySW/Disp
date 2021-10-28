//
// Created by Sunday! on 15.03.2021.
//
#ifndef DISP_MAINSCREEN_HPP
#define DISP_MAINSCREEN_HPP

#include "IScreen.hpp"
#include "HardScreenFuncs.hpp"
#include "Button.hpp"

namespace MainScreenVarsPositions{
    uint8_t     OnWeight =      0;
    uint8_t     OnTime =        5;
    uint8_t     Btn1Count =     8;
    uint8_t     Btn2Count =     14;
}

template<const uint8_t *bufferAddr>
class MainScreen: public IScreen{
    friend class Singleton<MainScreen>;
public:
    void update_partOf_buffer() override {
        if(OnTime > 59){
            min += OnTime / 60;
            OnTime = OnTime % 60;
            if(min > 59){
                hours += min / 60;
                min = min % 60;
            }
        }
        printTextWithCoords(std::to_string(hours) + "h:" + (OnTime>9?std::to_string(OnTime) : "0"+std::to_string(OnTime)), MainScreenVarsPositions::OnTime);
        if(!now_grinding && needToReCalc) {
            reCalc();
            update_whole_buffer();
        }
    };

    void update_whole_buffer() override {
        fillScreenFromBuffer(IScreen::getBufferAddr());
        printTextWithCoords(" " + std::to_string(OnWeight) +"g", MainScreenVarsPositions::OnWeight);
        printTextWithCoords(Btn1Count < 100 ? "  " + std::to_string(Btn1Count): " " + std::to_string(Btn1Count), MainScreenVarsPositions::Btn1Count);
        printTextWithCoords(std::to_string(Btn2Count), MainScreenVarsPositions::Btn2Count);
    }

    const MainScreen & operator=(const MainScreen &) = delete;

protected:

private:
    explicit constexpr MainScreen():IScreen(MAIN_SCREEN, bufferAddr) {};
    MainScreen(const MainScreen&) = default;

    uint8_t min = 0;
    uint8_t hours = 0;
    bool needToReCalc = false;

    void startGrinding(BTN_TYPE button){
        selectedBTN = button;
        motorTime = selectedBTN == BTN1 ? Grind1Time : Grind2Time;
        now_grinding = true;
        while (needToReCalc){}
        needToReCalc = true;
        startMotor();
    }

    void finishGrinding(){
        now_grinding = false;
        stopMotor();
    }

    void reCalc(){
        needToReCalc = false;
        if(motorTime>0){
            uint8_t desiredTime = (selectedBTN == BTN1) ? Grind1Time : Grind2Time;
            TimeReminder += (desiredTime - motorTime);
        }
        else (selectedBTN == BTN1) ? Btn1Count++, TotalCounter1++ : Btn2Count++, TotalCounter2++;

        if(TimeReminder / Grind1Time > 0){
            Btn1Count += TimeReminder / Grind1Time;
            TotalCounter1 += TimeReminder / Grind1Time;
            TimeReminder = TimeReminder % Grind1Time;
        }

//        OnWeight = (float)(Grind2Time)/(float)(Grind1Time) * WeightPerCycle;
//        OnWeight += Btn1Count * WeightPerCycle;
        OnWeight = (Btn1Count + 2*Btn2Count) * WeightPerCycle;
    }

    SCREEN_TYPE bnt1_func() override {
        if(Modality){
            selectedBTN = BTN1;
            return IScreen::getType();
        }
        if(now_grinding) finishGrinding();
        else startGrinding(BTN1);
        return IScreen::getType();
    };

    SCREEN_TYPE bnt2_func() override {
        if(Modality){
            selectedBTN = BTN2;
            return IScreen::getType();
        }
        if(now_grinding) finishGrinding();
        else startGrinding(BTN2);
        return IScreen::getType();
    };

    SCREEN_TYPE bntPLUS_func() override {
        if(now_grinding) return IScreen::getType();
        return USER_SETTINGS_SCREEN;
    };

    SCREEN_TYPE bntMINUS_func() override {
        if(now_grinding) return IScreen::getType();
        return PASSWORD_SCREEN;
    };

    SCREEN_TYPE bntMODE_func() override {
        if(Modality && !now_grinding) finishGrinding();
        return IScreen::getType();
    };

};

#endif //DISP_MAINSCREEN_HPP