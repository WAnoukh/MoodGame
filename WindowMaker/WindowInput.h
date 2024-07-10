﻿#pragma once
#include <list>

class WindowInput
{
public:
    static WindowInput& GetInstance();
    static bool IsShiftPressed();
    void KeyboardKeyCallBackEvent(int key, int scancode, int action);
    bool IsKeyPressed(int key) const;
private:
    std::list<int> downKeys;
    static WindowInput* instance ;
};