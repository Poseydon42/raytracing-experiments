#pragma once

#include <vulkan/vulkan.h>

#include "App.h"

extern VkAllocationCallbacks* GVulkanAllocator;

class VulkanDevice {
public:
  VulkanDevice(const App& App);

  ~VulkanDevice();

private:
  const App& m_App;

  VkInstance m_Instance = VK_NULL_HANDLE;
  VkDebugUtilsMessengerEXT m_VkDebugMessenger;

  VkDevice m_Device = VK_NULL_HANDLE;
};
