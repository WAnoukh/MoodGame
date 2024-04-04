#include "GameWindow.h"


#include <iostream>

const float plane_vertices[] = {
    // positions        // uv coord
    1.0f,  -1.0f, 0.0f, 1.0f, 1.0f, // top right
    1.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom right
   -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, // bottom left
   -1.0f, -1.0f, 0.0f, 0.0f, 1.0f  // top left 
};

const unsigned int plane_indexes[] = {
    0, 1, 3, // first triangle
    1, 2, 3  // second triangle
};

int GameWindow::init() {
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    #ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif

    // glfw window creation
    // --------------------
    window = glfwCreateWindow(screen_width, screen_height, "DLGame", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
        
    // Setup render plane
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(plane_vertices), plane_vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(plane_indexes), plane_indexes, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));   

    planeShader.compile();
    
    return 0;
}

void GameWindow::PreRender() const
{
    //glClearColor(0.2f, 0.3f, 0.3f, 1.00f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void GameWindow::RenderScreen() const
{
    glBindVertexArray(VAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, displayedTextureId);

    planeShader.use();

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void GameWindow::PostRender() const {
    glfwSwapBuffers(window);
    glfwPollEvents();   
}

void GameWindow::terminate() const
{
    glfwTerminate();
}

void GameWindow::BindframebufferSizeEvent(GLFWframebuffersizefun framebuffer_size_callback)
{
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
}

void GameWindow::BindScrollCallBackEvent(GLFWscrollfun scroll_callback)
{
    glfwSetScrollCallback(window, scroll_callback);
}

void GameWindow::BindMouseButtonCallBackEvent(GLFWmousebuttonfun mouse_button_callback)
{
    glfwSetMouseButtonCallback(window, mouse_button_callback);
}

void GameWindow::BindMousePositionCallBackEvent(GLFWcursorposfun mouse_position_callback)
{
    glfwSetCursorPosCallback(window, mouse_position_callback);
}

void GameWindow::BindKeyboardKeyCallBackEvent(GLFWkeyfun key_callback)
{
    glfwSetKeyCallback(window, key_callback);
}

void GameWindow::SetDisplayedTexture(unsigned int texture)
{
    displayedTextureId = texture;
}