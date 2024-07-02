#include "VulkanDevice.h"

#include <GLFW/glfw3.h>
#include <array>
#include <cstdlib>
#include <iostream>
#include <optional>
#include <vector>

#include "Assert.h"

VkAllocationCallbacks* GVulkanAllocator = nullptr;

static VKAPI_ATTR VkBool32 VKAPI_CALL
VulkanDebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                    VkDebugUtilsMessageTypeFlagsEXT messageType,
                    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
                    void* pUserData) {

  std::cerr << "Validation layer message: " << pCallbackData->pMessage
            << std::endl;

  return VK_FALSE;
}

static VkInstance CreateInstance() {
  uint32_t GLFWExtensionCount;
  const auto** GLFWExtensions =
      glfwGetRequiredInstanceExtensions(&GLFWExtensionCount);
  std::vector<const char*> Extensions(GLFWExtensions,
                                      GLFWExtensions + GLFWExtensionCount);
  Extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

  std::array<const char*, 1> Layers = {
      "VK_LAYER_KHRONOS_validation",
  };

  VkApplicationInfo ApplicationInfo = {
      .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
      .pApplicationName = "Raytracing Experiments",
      .applicationVersion = VK_MAKE_VERSION(0, 1, 0),
      .pEngineName = "No Engine",
      .engineVersion = 0,
      .apiVersion = VK_API_VERSION_1_3,
  };
  // FIXME: properly handle the case where the validation layers aren't present
  VkInstanceCreateInfo InstanceCreateInfo = {
      .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
      .pApplicationInfo = &ApplicationInfo,
      .enabledLayerCount = static_cast<uint32_t>(Layers.size()),
      .ppEnabledLayerNames = Layers.data(),
      .enabledExtensionCount = static_cast<uint32_t>(Extensions.size()),
      .ppEnabledExtensionNames = Extensions.data(),
  };

  VkInstance Result;
  ASSERT(vkCreateInstance(&InstanceCreateInfo, GVulkanAllocator, &Result) ==
             VK_SUCCESS,
         "failed to create Vulkan instance");

  return Result;
}

static VkDebugUtilsMessengerEXT CreateDebugMessenger(VkInstance Instance) {
  auto vkCreateDebugUtilsMessengerEXT =
      reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(
          vkGetInstanceProcAddr(Instance, "vkCreateDebugUtilsMessengerEXT"));
  ASSERT(vkCreateDebugUtilsMessengerEXT,
         "failed to get address of vkCreateDebugUtilsMessengerEXT");

  VkDebugUtilsMessengerCreateInfoEXT DebugMessengerCreateInfo = {
      .sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
      .messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
                         VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
                         VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT,
      .messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                     VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT,
      .pfnUserCallback = VulkanDebugCallback,
  };

  VkDebugUtilsMessengerEXT Result;
  ASSERT(vkCreateDebugUtilsMessengerEXT(Instance, &DebugMessengerCreateInfo,
                                        GVulkanAllocator,
                                        &Result) == VK_SUCCESS,
         "failed to create Vulkan debug messenger");

  return Result;
}

static VkPhysicalDevice EnumerateAndChoosePhysicalDevice(VkInstance Instance) {
  uint32_t PhysicalDeviceCount;
  vkEnumeratePhysicalDevices(Instance, &PhysicalDeviceCount, nullptr);
  std::cout << "Discovered " << PhysicalDeviceCount << " physical devices:\n";
  std::vector<VkPhysicalDevice> PhysicalDevices(PhysicalDeviceCount);
  vkEnumeratePhysicalDevices(Instance, &PhysicalDeviceCount,
                             PhysicalDevices.data());
  std::optional<size_t> PreferredDeviceIndex = {};
  for (size_t Index = 0; Index < PhysicalDevices.size(); Index++) {
    VkPhysicalDeviceProperties Properties = {};
    vkGetPhysicalDeviceProperties(PhysicalDevices[Index], &Properties);

    uint32_t VersionMajor = VK_VERSION_MAJOR(Properties.apiVersion);
    uint32_t VersionMinor = VK_VERSION_MINOR(Properties.apiVersion);
    uint32_t VersionPatch = VK_VERSION_PATCH(Properties.apiVersion);

    std::cout << "  Device #" << Index << ": " << Properties.deviceName
              << " supporting Vulkan " << VersionMajor << "." << VersionMinor
              << "." << VersionPatch << "\n";

    if (Properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
      PreferredDeviceIndex = Index;
  }

  std::cout << "Using device #" << PreferredDeviceIndex.value_or(0) << "\n";
  return PhysicalDevices[PreferredDeviceIndex.value_or(0)];
}

VulkanDevice::VulkanDevice(const App& App)
    : m_App(App)
    , m_Instance(CreateInstance())
    , m_VkDebugMessenger(CreateDebugMessenger(m_Instance)) {

    VkPhysicalDevice SelectedDevice = EnumerateAndChoosePhysicalDevice(m_Instance);
}

VulkanDevice::~VulkanDevice() {
  auto vkDestroyDebugUtilsMessengerEXT =
      reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(
          vkGetInstanceProcAddr(m_Instance, "vkDestroyDebugUtilsMessengerEXT"));
  if (vkDestroyDebugUtilsMessengerEXT)
    vkDestroyDebugUtilsMessengerEXT(m_Instance, m_VkDebugMessenger,
                                    GVulkanAllocator);

  vkDestroyInstance(m_Instance, GVulkanAllocator);
}
