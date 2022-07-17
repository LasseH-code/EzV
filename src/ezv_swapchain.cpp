#include "ezv_base.h"

namespace ezv
{
    bool EzV::createSwapchain(VkImageUsageFlags usage)
    {
        VkBool32 supportsPresent;
        VKA(vkGetPhysicalDeviceSurfaceSupportKHR(context->physicalDevice, context->graphicsQueue.familyIndex, swapchain->surface, &supportsPresent));
        if (!supportsPresent)
        {
            LOG_ERROR("Graphics Queue does not support present");
            *swapchain = {}; 
            return false;
        }
        
        uint32_t numFormats = 0;
        VKA(vkGetPhysicalDeviceSurfaceFormatsKHR(context->physicalDevice, swapchain->surface, &numFormats, 0));
        VkSurfaceFormatKHR* availableFormats = new VkSurfaceFormatKHR[numFormats];
        VKA(vkGetPhysicalDeviceSurfaceFormatsKHR(context->physicalDevice, swapchain->surface, &numFormats, availableFormats));
        if (numFormats <= 0)
        {
            LOG_ERROR("No surface formats available. aborting swapchain creation");
            delete[] availableFormats;
            return false;
        }
        
#ifdef EZV_INFO_OUTPUT
        LOG_INFO("-----AVAILABLE FORMATS----");
        for (uint32_t i = 0; i < numFormats; i++)
        {
            LOG_INFO(availableFormats[i].format);
        }
        LOG_INFO("--------------------------");
#endif // EZV_INFO_OUTPUT
        
        // First available format should be sensible (normally not HDR)
        swapchain->format = availableFormats[0].format;
        VkColorSpaceKHR colorSpace = availableFormats[0].colorSpace;
        
        VkSurfaceCapabilitiesKHR surfaceCapabilities;
        VKA(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(context->physicalDevice, swapchain->surface, &surfaceCapabilities));
        if (surfaceCapabilities.currentExtent.width == 0xFFFFFFFF)
        {
            surfaceCapabilities.currentExtent.width = surfaceCapabilities.minImageExtent.width;
        }
        if (surfaceCapabilities.currentExtent.height == 0xFFFFFFFF)
        {
            surfaceCapabilities.currentExtent.height = surfaceCapabilities.minImageExtent.height;
        }
        if (surfaceCapabilities.maxImageCount == 0)
        {
            surfaceCapabilities.maxImageCount = 8; // error can occur, when the minImageCount of the swapchain is above 8
        }
        swapchain->width = surfaceCapabilities.currentExtent.width;
        swapchain->height = surfaceCapabilities.currentExtent.height;
        
        VkSwapchainCreateInfoKHR createInfo = {VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR};
        createInfo.surface = swapchain->surface;
        createInfo.minImageCount = (surfaceCapabilities.minImageCount<3 ? 3 : surfaceCapabilities.minImageCount);
        createInfo.imageFormat = swapchain->format;
        createInfo.imageColorSpace = colorSpace;
        createInfo.imageExtent = surfaceCapabilities.currentExtent;
        createInfo.imageArrayLayers = 1;
        createInfo.imageUsage = usage;
        createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        createInfo.preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
        createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
        createInfo.presentMode = VK_PRESENT_MODE_FIFO_KHR;
        createInfo.clipped = VK_FALSE;
        VKA(vkCreateSwapchainKHR(context->logicalDevice, &createInfo, 0, &swapchain->swapchain));
        
        uint32_t numImages;
        VKA(vkGetSwapchainImagesKHR(context->logicalDevice, swapchain->swapchain, &numImages, 0));
        
        // Aquire swapchainImages
        swapchain->images.resize(numImages);
        VKA(vkGetSwapchainImagesKHR(context->logicalDevice, swapchain->swapchain, &numImages, swapchain->images.data()));
        
        delete[] availableFormats;
        return true;
    }
    
    void EzV::destroySwapchain()
    {
        if(swapchain)
        {
            if(swapchain->swapchain)vkDestroySwapchainKHR(context->logicalDevice, swapchain->swapchain, 0);
            if(swapchain->surface)vkDestroySurfaceKHR(context->instance, swapchain->surface, 0);
            delete swapchain;
            swapchain = 0;
            LOG_WARN("Swapchain destroyed successfully!");
        }
    }
}
