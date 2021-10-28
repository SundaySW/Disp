//
// Created by outlaw on 13.04.2021.
//

#ifndef DISP_PASSWORDSCREEN_HPP
#define DISP_PASSWORDSCREEN_HPP


namespace PassScreenVarsPositions{
    uint8_t     firstEnter =    8;
    uint8_t     secondEnter =   10;
    uint8_t     thirdEnter =    12;
    uint8_t     fourthEnter =   14;
    uint8_t     positionArr[4] = {firstEnter,secondEnter,thirdEnter,fourthEnter};
    BTN_TYPE actualPassword[4] = {BTN_MINUS,BTN_MINUS,BTN_PLUS,BTN_PLUS};
}

template<const uint8_t *bufferAddr>
class PasswordScreen: public IScreen{
    friend class Singleton<PasswordScreen>;
public:
    void update_partOf_buffer() override {
        for(int i = 0; i < count; i++){
            printTextWithCoords(" *", PassScreenVarsPositions::positionArr[i]);
        }
    };

    void update_whole_buffer() override {
        //printTextWithCoords("Enter password", 0);
        fillScreenFromBuffer(IScreen::getBufferAddr());
    }

    const PasswordScreen & operator=(const PasswordScreen &) = delete;

protected:

private:
    explicit constexpr PasswordScreen():IScreen(PASSWORD_SCREEN, bufferAddr) {};
    PasswordScreen(const PasswordScreen&) = default;

    BTN_TYPE enteredPassword[4] = {};
    uint8_t count = 0;

    SCREEN_TYPE finishEnter(){
        count = 0;
        for(uint8_t i = 0; i < 4;i++)
            if(enteredPassword[i] != PassScreenVarsPositions::actualPassword[i]) return MAIN_SCREEN;
        return ADMIN_SETTINGS_SCREEN;
    }

    SCREEN_TYPE bnt1_func() override {
        enteredPassword[count++] = BTN1;
        if(count > 3) return finishEnter();
        return IScreen::getType();
    };

    SCREEN_TYPE bnt2_func() override {
        enteredPassword[count++] = BTN2;
        if(count > 3) return finishEnter();
        return IScreen::getType();
    };

    SCREEN_TYPE bntPLUS_func() override {
        enteredPassword[count++] = BTN_PLUS;
        if(count > 3) return finishEnter();
        return IScreen::getType();
    };

    SCREEN_TYPE bntMINUS_func() override {
        enteredPassword[count++] = BTN_MINUS;
        if(count > 3) return finishEnter();
        return IScreen::getType();
    };

    SCREEN_TYPE bntMODE_func() override {
        return IScreen::getType();
    };
};


#endif //DISP_PASSWORDSCREEN_HPP
