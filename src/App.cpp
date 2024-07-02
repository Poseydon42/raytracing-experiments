#include "App.h"

#include <iostream>

#include "Assert.h"

App::App(unsigned WindowWidth, unsigned WindowHeight, std::string_view Name) {
  ASSERT(glfwInit(), "failed to initialize GLFW");

  ASSERT(glfwVulkanSupported(),
         "Vulkan must be supported to run this application");

  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  glfwWindowHint(GLFW_RESIZABLE, false);
  m_Window = glfwCreateWindow(WindowWidth, WindowHeight, Name.data(), nullptr,
                              nullptr);
  ASSERT(m_Window, "failed to create application window");
}

App::~App() {
  glfwDestroyWindow(m_Window);
  glfwTerminate();
}

void App::Run() {
  while (!glfwWindowShouldClose(m_Window)) {
    glfwPollEvents();
  }
}
