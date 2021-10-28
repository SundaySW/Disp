//
// Created by outlaw on 22.03.2021.
//
#ifndef DISP_USERSETTINGSSCREEN_HPP
#define DISP_USERSETTINGSSCREEN_HPP

struct UserSettingsScr{

    constexpr static const uint8_t nofSheets = 2;
    std::string varNamesArray[nofSheets][BaseSettingsScr::nofLines]{
            {"","ScrLight","OutLight","Mode"},
            {"","Save & Exit"}
    };
    uint32_t* varValuesArray[nofSheets][BaseSettingsScr::nofLines]{
            {nullptr, &Screen_BackLight_value, &Out_BackLight_value, &Modality}
    };
    uint32_t varBoundariesArray[nofSheets][BaseSettingsScr::nofLines]{
            {0,15,15,1},
            {0,0,0,0}
    };
};

//    constexpr UserSettingsScreen() = default;
//    ~UserSettingsScreen() override = default;
//   friend IScreen* IScreen::CreateInstance(unsigned char* incomeBuffer, SCREEN_TYPE incomeScreenTYpe);
//
//    void Delete() override{
//       delete this;
//    }

//    IScreen* IScreen::CreateInstance(unsigned char* incomeBuffer, SCREEN_TYPE incomeScreenTYpe) {
//        if (incomeScreenTYpe == UserSettingsScreen::Type)
//            return static_cast<IScreen *>(new UserSettingsScreen(incomeBuffer));
//    }

#endif //DISP_USERSETTINGSSCREEN_HPP