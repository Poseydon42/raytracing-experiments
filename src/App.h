#pragma once

#include <GLFW/glfw3.h>
#include <string_view>

class App {
public:
  App(unsigned WindowWidth, unsigned WindowHeight, std::string_view Name);

  ~App();

  void Run();

private:
  GLFWwindow* m_Window;
};
