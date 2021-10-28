//
// Created by outlaw on 24.03.2021.
//

#ifndef DISPAPP_USERMODIFYSCREEN_HPP
#define DISPAPP_USERMODIFYSCREEN_HPP

struct AdminSettingsScr{

    static const uint8_t nofSheets = 2;

    std::string varNamesArray[nofSheets][BaseSettingsScr::nofLines]{
            {"","Time1","Time2","Weight"},
            {"","Save & Exit"}
    };
    uint32_t* varValuesArray[nofSheets][BaseSettingsScr::nofLines]{
            {nullptr, &Grind1Time,&Grind2Time,&WeightPerCycle}
    };
    uint32_t varBoundariesArray[nofSheets][BaseSettingsScr::nofLines]{
            {0,300,300,99},
            {0,0,0,0}
    };
};



#endif //DISPAPP_USERMODIFYSCREEN_HPP