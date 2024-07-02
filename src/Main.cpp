#include <iostream>

#include "App.h"
#include "Vulkan/VulkanDevice.h"

int main(int ArgCount, char** Args) {
  std::cout << "Starting up...\n";

  App App(1280, 720, "Raytracing Experiments");
  VulkanDevice Device(App);
  App.Run();
}
