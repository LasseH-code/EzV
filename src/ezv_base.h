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

#define ASSERT_VULKAN(val) if(val != VK_SUCCESS) {LOG_CRIT("An error occured"); assert(false);}
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
    typedef enum EZV_CREATE_FLAGS : DWORD
    {
        CREATE_VULKAN_INSTANCE = 0b1 << 0,
        FIND_PHYSICAL_DEVICE = 0b1 << 1,
        GENERATE_LOGICAL_DEVICE = 0b1 << 2,
        SAVE_GRAPHICS_QUEUE = 0b1 << 3,
        GENERATE_VULKAN_CONTEXT = CREATE_VULKAN_INSTANCE | FIND_PHYSICAL_DEVICE | GENERATE_LOGICAL_DEVICE | SAVE_GRAPHICS_QUEUE,

        CREATE_WINDOW_SDL = 0b1 << 4,
        CREATE_SWAPCHAIN_KHR = 0b1 << 5,
        CREATE_SURFACE_KHR_SDL = 0b1 << 6,
        QUERY_INSTANCE_EXTENSIONS_SDL = 0b1 << 7,
        INIT_VIDEO_SDL = 0b1 << 8,
        SETUP_VIA_SDL = CREATE_WINDOW_SDL | CREATE_SWAPCHAIN_KHR | CREATE_SURFACE_KHR_SDL | QUERY_INSTANCE_EXTENSIONS_SDL | INIT_VIDEO_SDL,

        ADD_BASIC_RENDER_PASS = 0b1 << 9,
    } EZV_CREATE_FLAGS;
    
    struct EzVSDLWindowCreateInfo
    {
        const char* title;
        int x;
        int y;
        int w;
        int h;
        uint32_t windowFlags;
    };

    typedef enum RenderSubPassSamples
    {
        RENDER_SUB_PASS_SAMPLE_COUNT_1 = VK_SAMPLE_COUNT_1_BIT,
        RENDER_SUB_PASS_SAMPLE_COUNT_2 = VK_SAMPLE_COUNT_2_BIT,
        RENDER_SUB_PASS_SAMPLE_COUNT_4 = VK_SAMPLE_COUNT_4_BIT,
        RENDER_SUB_PASS_SAMPLE_COUNT_8 = VK_SAMPLE_COUNT_8_BIT,
        RENDER_SUB_PASS_SAMPLE_COUNT_16 = VK_SAMPLE_COUNT_16_BIT,
        RENDER_SUB_PASS_SAMPLE_COUNT_32 = VK_SAMPLE_COUNT_32_BIT,
        RENDER_SUB_PASS_SAMPLE_COUNT_64 = VK_SAMPLE_COUNT_64_BIT,
    } RenderPassSamples;

    typedef enum RenderSubPassTypes
    {
        RENDER_SUB_PASS_TYPE_COLOR_CUSTOM, // Loads VkSubpassDescription passed into it
        RENDER_SUB_PASS_TYPE_COLOR_LOAD, // Loads Buffer
        RENDER_SUB_PASS_TYPE_COLOR_NORMAL, // Overwrites Buffer
        RENDER_SUB_PASS_TYPE_COLOR_CLEAR, // Clears Buffer
    } RenderSubPassTypes;

    typedef enum RenderPassTypes
    {
        RENDER_PASS_TYPE_CUSTOM,
        RENDER_PASS_TYPE_COLOR_LOAD,
        RENDER_PASS_TYPE_COLOR_NORMAL,
        RENDER_PASS_TYPE_COLOR_CLEAR,
        RENDER_PASS_TYPE_CUSTOM_SUBPASSES,
    } RenderPassTypes;

    struct EzVSubRenderPassCreateInfo
    {
        DWORD m_subRenderPassCreateInfo;
        VkSubpassDescription* p_customSubRenderPassDescription;

        DWORD m_samples;
    };

    struct EzVRenderPassCreateInfo
    {
        RenderPassTypes m_renderPassType;
        VkRenderPass* p_customRenderPass;
        RenderSubPassSamples m_samples;
        EzVSubRenderPassCreateInfo** p_customSubRenderPasses;
    };

    struct EzVCreateInfo
    {
        DWORD m_creationFlags;
        VkImageUsageFlags m_usageFlags;
        int* p_returnVal;
        
        uint32_t m_instanceExtensionCount;
        const char** p_instanceExtensions; 
        
        uint32_t m_deviceExtensionCount; 
        const char** p_deviceExtensions;
        
        uint32_t m_renderPassCount;
        EzVRenderPassCreateInfo** p_renderPasses;

        //void* createSurface; 
        EzVSDLWindowCreateInfo* p_sdlWindowCS;
    };
    
    struct VulkanQueue
    {
        VkQueue queue;
        uint32_t familyIndex;
    };
    
    struct VulkanRenderpass
    {
        VkRenderPass m_renderpass;
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
        std::vector<VulkanRenderpass> renderpasses;
        
        public:
        EzV(EzVCreateInfo* creationStruct);
        ~EzV();
        
        int initEzV(EzVCreateInfo* cs);
        bool initVulkanInstance(uint32_t instanceExtensionCount, const char** instanceExtensions);
        bool selectPhysicalDevice();
        bool createLogicalDevice(uint32_t deviceExtensionCount, const char** deviceExtensions);
        bool createSwapchain(VkImageUsageFlags usage);
        void destroySwapchain();
        
#ifdef EZV_SUPPORT_SDL
        EzV(EzVCreateInfo* cs, SDL_Window* window);
        bool createSDLWindow(EzVSDLWindowCreateInfo* creationStruct, SDL_Window** window);
        bool createSurfaceSDL(SDL_Window** window);
        bool querySDLInstanceExtensions(EzVCreateInfo* cs, SDL_Window** window);
        bool querySDLInstanceExtensions(uint32_t* instanceExtensionCount, const char*** instanceExtensions, SDL_Window** window);
#endif // EZV_SUPPORT_SDL
        
        bool createRenderPass(EzVRenderPassCreateInfo* createInfo);
        void destroyRenderPasses();

        int createVulkanInstance(EzVCreateInfo* creationStruct);
        void destroyVulkanInstance();
    };
}

#endif //EZV_H
