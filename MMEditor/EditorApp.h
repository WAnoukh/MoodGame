#pragma once
#include <glad\glad.h>
#include <GLFW\glfw3.h>

#include "../WindowMaker/GameWindow.h"
#include "../src/World/World.h"
#include "../src/World/WorldReader.h"
#include "../WindowMaker/GLTexture.h"
#include "../src/World/MapRenderer.h"
#include "EditableWorld/EditableWorld.h"
#include "EditableWorld/WorldEditor.h"
#include "EditableWorld/WorldEditorRenderer.h"

class EditorApp
{
public:
    EditorApp() = default;
    
    void Init();
    int Run();
    
    void Tick(float deltaTime);

    void Render();

    void RenderMenu();
    
    float GetFps();
    
    static EditorApp& GetInstance();

    void AskForLayoutRefresh();

    void framebufferSizeEvent(int width, int height);

    void ScrollCallBackEvent(GLFWwindow* window, bool guiWantToCapture, double xOffset, double yOffset);

    void MouseButtonCallBackEvent(GLFWwindow* window, bool guiWantToCapture, int button, int action, int mods);

    void MousePositionCallBackEvent(GLFWwindow* window, bool guiWantToCapture, double xPos, double yPos);

    void KeyboardKeyCallBackEvent(GLFWwindow* window, bool guiWantToCapture, int key, int scancode, int action,
                                  int mods);

    void BindInput();
private:
    inline static EditorApp* instance = nullptr;
    GameWindow window = GameWindow();

    float lastDeltaTimes[200] = {0.0f};
    int lastDeltaTimesIndex = 0;

    World world;
    //WorldReader reader = WorldReader(world, "../res/DefaultMap.map");
    WorldReader reader = WorldReader(world, "../res/2cube.map");
    EditableWorld editableWorld;
    MapRenderer mapRenderer{world};
    WorldEditor worldEditor{&editableWorld};
    WorldEditorRenderer worldEditorRenderer{editableWorld};
    GLTexture shownTexture;

    double lastMouseX = 0.0;
    double lastMouseY = 0.0;
    float mouseDeltaX = 0.0f;
    float mouseDeltaY = 0.0f;

    float zoom = 10.0f;
    float xOffset = 0;
    float yOffset = 0;

    bool panning = false;
    int selectedCorner = -1;

    char openPath[256] = "../res/test1.map";

    void WindowToWorld(float x, float y, float& outX, float& outY);
    void DeltaWindowToWorld(float dx, float dy, float& outDx, float& outDy);
};
