//
// Created by outlaw on 15.03.2021.
//

#ifndef UNTITLED_BUTTON_HPP
#define UNTITLED_BUTTON_HPP

enum BTN_TYPE{
    BTN1,
    BTN2,
    BTN_PLUS,
    BTN_MINUS,
    BTN_MODE,
    BTN_none
};

struct Button{
    constexpr explicit Button(BTN_TYPE type): Type(type){}
    const BTN_TYPE Type;
};

#endif //UNTITLED_BUTTON_HPP