#pragma once
#include <list>
#include <map>

class InputManager
{
public:
    static InputManager& GetInstance();
    
    void KeyboardKeyCallBackEvent(int key, int scancode, int action);
    bool IsKeyPressed(int key) const;
private:
    std::list<int> pressedKeys;
    inline static InputManager* instance = nullptr;
};
