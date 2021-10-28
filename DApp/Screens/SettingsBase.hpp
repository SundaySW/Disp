//
// Created by outlaw on 12.04.2021.
//

#ifndef DISP_SETTINGSBASE_HPP
#define DISP_SETTINGSBASE_HPP

namespace BaseSettingsScr{
    uint8_t     FirstLine =     1;
    uint8_t     SecondLine =    17;
    uint8_t     ThirdLine =     9;
    uint8_t     FourthLine =    25;

    uint8_t     FirstLineCursor =     0;
    uint8_t     SecondLineCursor =    16;
    uint8_t     ThirdLineCursor =     8;
    uint8_t     FourthLineCursor =    24;
    uint8_t     cursorPositionsArray[4] = {FirstLineCursor, SecondLineCursor, ThirdLineCursor, FourthLineCursor};

    static const uint8_t nofLines = 4;

    uint32_t varLinePositionsArray[nofLines]{FirstLine, SecondLine, ThirdLine , FourthLine};
};

template<const uint8_t *bufferAddr, SCREEN_TYPE type, class SettingsStruct>
class SettingsBase: public IScreen{
    friend class Singleton<SettingsBase>;
public:
    inline void update_partOf_buffer() override {
        printTextWithCoords(">", BaseSettingsScr::cursorPositionsArray[cursorPosition]);
    }

    inline void update_whole_buffer() override {
        if(sheetNum==0) fillScreenFromBuffer(IScreen::getBufferAddr());

        for(int i = 0; i < BaseSettingsScr::nofLines; i++){
            clearLine(i);

            std::string lineName = settings.varNamesArray[sheetNum][i];

            if(lineName.empty()) continue;

            uint32_t lineValue = *settings.varValuesArray[sheetNum][i];

            if(lineName == "Time1" || lineName == "Time2"){
                printTextWithCoords(lineName, BaseSettingsScr::varLinePositionsArray[i]);
                printTextWithCoords(std::to_string(lineValue/10) + "." + std::to_string(lineValue%10), BaseSettingsScr::varLinePositionsArray[i] + 5);
                continue;
            }
            if(lineName == "Mode"){
                printTextWithCoords(lineName += (lineValue == 0 ? ":   manual" : ":     auto"), BaseSettingsScr::varLinePositionsArray[i]);
                continue;
            }
            if(lineName == "Save & Exit"){
                printTextWithCoords("Save & Exit", BaseSettingsScr::varLinePositionsArray[i]);
                continue;
            }

            printTextWithCoords(lineName, BaseSettingsScr::varLinePositionsArray[i]);
            printTextWithCoords(std::to_string(lineValue), BaseSettingsScr::varLinePositionsArray[i] + 5);
        }
    }

   inline void clearPreviousCursor(){
        printTextWithCoords(" ", BaseSettingsScr::cursorPositionsArray[cursorPosition]);
    };
   inline void clearLine(uint8_t line){
        printTextWithCoords("              ", BaseSettingsScr::varLinePositionsArray[line]);
    }
    explicit constexpr SettingsBase(): IScreen(type, bufferAddr){};
private:

    SettingsBase(const SettingsBase&) = default;

    SettingsStruct settings;
    uint8_t cursor_start_position = 1;
    int8_t cursorPosition = cursor_start_position;
    int8_t sheetNum = 0;

    SCREEN_TYPE bnt1_func() override {
        clearPreviousCursor();
        while (true){
            cursorPosition++;
            if(cursorPosition > (BaseSettingsScr::nofLines - 1)) {
                cursorPosition = 0;
                changeSheet();
            }
            if(settings.varNamesArray[sheetNum][cursorPosition].empty()) continue;
            else break;
        }
        return IScreen::getType();
    };

    SCREEN_TYPE bnt2_func() override {
        clearPreviousCursor();
        while (true){
            cursorPosition--;
            if(cursorPosition < 0) {
                cursorPosition = BaseSettingsScr::nofLines - 1;
                changeSheet();
            }
            if(settings.varNamesArray[sheetNum][cursorPosition].empty()) continue;
            else break;
        }
        return IScreen::getType();
    };

    SCREEN_TYPE bntPLUS_func() override {
        if(checkIfExit()) return MAIN_SCREEN;
        uint32_t changingValue = *settings.varValuesArray[sheetNum][cursorPosition];
        if(++changingValue <= settings.varBoundariesArray[sheetNum][cursorPosition])
            *settings.varValuesArray[sheetNum][cursorPosition] = changingValue;
        update_whole_buffer();
        return IScreen::getType();
    };

    SCREEN_TYPE bntMINUS_func() override {
        if(checkIfExit()) return MAIN_SCREEN;
        uint32_t changingValue = *settings.varValuesArray[sheetNum][cursorPosition];
        if(changingValue > 0)
            *settings.varValuesArray[sheetNum][cursorPosition] = --changingValue;
        update_whole_buffer();
        return IScreen::getType();
    };

    SCREEN_TYPE bntMODE_func() override{
        return MAIN_SCREEN;
    };

    inline bool checkIfExit(){
        if(settings.varNamesArray[sheetNum][cursorPosition] == "Save & Exit"){
            cursorPosition = cursor_start_position;
            sheetNum = 0;
            return true;
        }
        return false;
    }

    inline void changeSheet(){
        if(cursorPosition == 0){
            if(++sheetNum > (settings.nofSheets - 1)) sheetNum = 0;
        }
        else if(--sheetNum < 0) sheetNum = settings.nofSheets - 1;
        update_whole_buffer();
    }
};

#endif //DISP_SETTINGSBASE_HPP