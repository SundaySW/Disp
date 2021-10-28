//
// Created by outlaw on 18.03.2021.
//

#ifndef UNTITLED_ISCREEN_HPP
#define UNTITLED_ISCREEN_HPP

#include <string>
#include "Singleton.hpp"

enum SCREEN_TYPE{
    MAIN_SCREEN,
    USER_SETTINGS_SCREEN,
    ADMIN_SETTINGS_SCREEN,
    PASSWORD_SCREEN
};

class IScreen{
public:
    virtual void update_whole_buffer() = 0;
    virtual void update_partOf_buffer() = 0;

    constexpr inline SCREEN_TYPE invoke_btn_func(BTN_TYPE btnType){
        switch (btnType){
            case BTN_TYPE::BTN1: return bnt1_func();
            case BTN_TYPE::BTN2: return bnt2_func();
            case BTN_TYPE::BTN_PLUS: return bntPLUS_func();
            case BTN_TYPE::BTN_MINUS: return bntMINUS_func();
            case BTN_TYPE::BTN_MODE: return bntMODE_func();
            default: return MAIN_SCREEN;
        }
    };

    SCREEN_TYPE getType() const   {return Type;}

    const uint8_t* getBufferAddr() const {return buffer_addr;}

    IScreen() = delete;
    const IScreen & operator=(const IScreen &) = delete ;
    IScreen& operator=(IScreen &iScreen) = delete;

protected:
    constexpr IScreen(SCREEN_TYPE incomeType, const uint8_t *incomeBufferPtr)
        :Type(incomeType),buffer_addr(incomeBufferPtr){};
private:
    const SCREEN_TYPE Type;
    const uint8_t *buffer_addr;
    virtual SCREEN_TYPE bnt1_func() = 0;
    virtual SCREEN_TYPE bnt2_func() = 0;
    virtual SCREEN_TYPE bntPLUS_func() = 0;
    virtual SCREEN_TYPE bntMINUS_func() = 0;
    virtual SCREEN_TYPE bntMODE_func() = 0;
};

//    virtual void Delete() = 0;
//    struct Deleter
//    {
//        void operator()(IScreen* p) const { p->Delete(); }
//    };
//    friend struct IScreen::Deleter;

#endif //UNTITLED_ISCREEN_HPP
