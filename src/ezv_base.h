/* date = June 23rd 2022 7:23 pm */

#ifndef EZV_H
#define EZV_H

#include "logger.h"
//i#include "../memory/memory.h"
#include <vulkan/vulkan.h>
#include <cassert>
#include <vector>

#ifdef EZV_SUPPORT_SDL
#include <SDL2/SDL.h>
#include <SDL2/SDL_vulkan.h>
#endif // EZV_SUPPORT_SDL

#define ASSERT_VULKAN(val) if(val != VK_SUCCESS) {lhg::LOG_CRIT("An error occured"); assert(false);}
#ifndef VK
#define VK(f) f
#endif // VK
#ifndef VKA
#define VKA(f) ASSERT_VULKAN(VK(f))
#endif // VKA

#define ARRAY_COUNT(array) (sizeof(array) / sizeof((array)[0]))
#define IS_FLAG_SET(flag, val) ((val & flag) == val)

//#define VULKAN_INFO_OUTPUT

namespace ezv
{
    static const DWORD CREATE_VULKAN_INSTANCE =        0b1<<0;
    static const DWORD FIND_PHYSICAL_DEVICE =          0b1<<1;
    static const DWORD GENERATE_LOGICAL_DEVICE =       0b1<<2;
    static const DWORD SAVE_GRAPHICS_QUEUE =           0b1<<3; 
    static const DWORD GENERATE_VULKAN_CONTEXT = CREATE_VULKAN_INSTANCE | FIND_PHYSICAL_DEVICE | GENERATE_LOGICAL_DEVICE | SAVE_GRAPHICS_QUEUE;
    
    static const DWORD CREATE_WINDOW_SDL =             0b1<<4;
    static const DWORD CREATE_SWAPCHAIN_KHR =          0b1<<5;
    static const DWORD CREATE_SURFACE_KHR_SDL =        0b1<<6;
    static const DWORD QUERY_INSTANCE_EXTENSIONS_SDL = 0b1<<7;
    static const DWORD INIT_VIDEO_SDL =                0b1<<8;
    static const DWORD SETUP_VIA_SDL = CREATE_WINDOW_SDL | CREATE_SWAPCHAIN_KHR | CREATE_SURFACE_KHR_SDL | QUERY_INSTANCE_EXTENSIONS_SDL | INIT_VIDEO_SDL;
    
    struct EzVSDLWindowCreateInfo
    {
        const char* title;
        int x;
        int y;
        int w;
        int h;
        uint32_t windowFlags;
    };
    
    struct EzVCreateInfo
    {
        DWORD creationFlags;
        VkImageUsageFlags usageFlags;
        int* returnVal;
        
        uint32_t instanceExtensionCount;
        const char** instanceExtensions; 
        
        uint32_t deviceExtensionCount; 
        const char** deviceExtensions;
        
        //void* createSurface; 
        EzVSDLWindowCreateInfo* sdlWindowCS;
    };
    
    struct VulkanQueue
    {
        VkQueue queue;
        uint32_t familyIndex;
    };
    
    struct VulkanSwapchain {
        VkSwapchainKHR swapchain;
        VkSurfaceKHR surface;
        uint32_t width;
        uint32_t height;
        VkFormat format;
        std::vector<VkImage> images;
    };
    
    struct VulkanContext 
    {
        VkInstance instance;
        VkPhysicalDevice physicalDevice;
        VkPhysicalDeviceProperties physicalDeviceProperties;
        VkDevice logicalDevice;
        VulkanQueue graphicsQueue;
    };
    
    class EzV
    {
        public:
        VulkanContext* context = 0;
        VulkanSwapchain* swapchain = 0;
        
        
        public:
        EzV(EzVCreateInfo* creationStruct);
        ~EzV();
        
        int initEzV(EzVCreateInfo* cs);
        bool initVulkanInstance(uint32_t instanceExtensionCount, const char** instanceExtensions);
        bool selectPhysicalDevice();
        bool createLogicalDevice(uint32_t deviceExtensionCount, const char** deviceExtensions);
        bool createSwapchain(VkImageUsageFlags usage);
        void deleteSwapchain();
        
#ifdef EZV_SUPPORT_SDL
        EzV(EzVCreateInfo* cs, SDL_Window* window);
        bool createSDLWindow(EzVSDLWindowCreateInfo* creationStruct, SDL_Window** window);
        bool createSurfaceSDL(SDL_Window** window);
        bool querySDLInstanceExtensions(EzVCreateInfo* cs, SDL_Window** window);
        bool querySDLInstanceExtensions(uint32_t* instanceExtensionCount, const char*** instanceExtensions, SDL_Window** window);
#endif // EZV_SUPPORT_SDL
        
        int createVulkanInstance(EzVCreateInfo* creationStruct);
        void destroyVulkanInstance();
    };
}

#endif //EZV_H
