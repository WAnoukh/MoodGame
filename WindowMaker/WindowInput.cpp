﻿#include "WindowInput.h"

#include <glad\glad.h>
#include <GLFW\glfw3.h>

WindowInput* WindowInput::instance = nullptr;

WindowInput& WindowInput::GetInstance()
{
    if(instance == nullptr)
    {
        instance = new WindowInput();
    }
    return *instance;
}

void WindowInput::KeyboardKeyCallBackEvent(int key, int scancode, int action)
{
    if(action == GLFW_PRESS)
    {
        downKeys.push_front(key);
    }
    else if(action == GLFW_RELEASE)
    {
        downKeys.remove(key);
    }
}

bool WindowInput::IsKeyPressed(int key) const
{
    for(auto& pressedKey : downKeys)
    {
        if(pressedKey == key)
        {
            return true;
        }
    }
    return false;
}
