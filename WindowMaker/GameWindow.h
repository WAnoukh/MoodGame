#pragma once
#include <glad\glad.h>
#include <GLFW\glfw3.h>

#include "shader_s.h"


struct GLFWwindow;
// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

class Application;

class GameWindow
{
public:
	GameWindow(): window(nullptr) {}
	GameWindow(const GameWindow&) = delete;
	GameWindow& operator=(GameWindow const&) = delete;

	GLFWwindow * getGLFWwindow() { return window; };

    int init();

	void RenderScreen() const;
	void PreRender() const;
	void PostRender() const;

	void terminate() const;
	
    bool shouldClose() const { return glfwWindowShouldClose(window); }

	void BindframebufferSizeEvent(GLFWframebuffersizefun framebuffer_size_callback);
	void BindScrollCallBackEvent(GLFWscrollfun scroll_callback);
	void BindMouseButtonCallBackEvent(GLFWmousebuttonfun mouse_button_callback);
	void BindMousePositionCallBackEvent(GLFWcursorposfun mouse_position_callback);
	void BindKeyboardKeyCallBackEvent(GLFWkeyfun key_callback);

	void SetDisplayedTexture(unsigned int texture);

	void SetScreenSize(size_t width, size_t height);

	void GetScreenSize(size_t& width, size_t& height) const;

private:
    GLFWwindow* window = nullptr;
	size_t screen_width = 800;
	size_t screen_height = 600;
	unsigned int VBO, VAO, EBO;

	Shader planeShader = Shader("../WindowMaker/Plane.vert", "../WindowMaker/Plane.frag");
	unsigned int displayedTextureId = 0;
};

