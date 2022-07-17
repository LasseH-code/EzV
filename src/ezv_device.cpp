#include "ezv_base.h"

namespace ezv
{
    bool EzV::initVulkanInstance(uint32_t instanceExtensionCount, const char** instanceExtensions)
    {
        uint32_t layerPropertyCount = 0; 
        VKA(vkEnumerateInstanceLayerProperties(&layerPropertyCount,0));
        VkLayerProperties* layerProperties = new VkLayerProperties[layerPropertyCount];
        VKA(vkEnumerateInstanceLayerProperties(&layerPropertyCount, layerProperties));
#ifdef EZV_INFO_OUTPUT
        LOG_INFO("-----AVAILABLE LAYERS-----");
        for (uint32_t i = 0; i < layerPropertyCount; i++)
        {
            LOG_INFO(layerProperties[i].layerName, ": ", layerProperties[i].description);
        }
        //LOG_INFO("--------------------------");
#endif // EZV_INFO_OUTPUT
        delete[] layerProperties;
        
        const char* enabledLayers[] = {
            "VK_LAYER_KHRONOS_validation"
        };
        
        /*uint32_t availableInstanceExtensionCount;
        VKA(vkEnumerateInstanceExtensionProperties(0, &availableInstanceExtensionCount, 0));
        VkExtensionProperties* instanceExtensionProperties = new VkExtensionProperties[availableInstanceExtensionCount];
        VKA(vkEnumerateInstanceExtensionProperties(0, &availableInstanceExtensionCount, instanceExtensionProperties));
        LOG_INFO("Vulkan Extensions: ");
        for (uint32_t i = 0; i < availableInstanceExtensionCount; i++)
        {
                LOG_INFO(instanceExtensionProperties[i].extensionName);
        }
        LOG_EMPTY();
        delete[] instanceExtensionProperties;*/
        
        VkApplicationInfo applicationInfo = {VK_STRUCTURE_TYPE_APPLICATION_INFO};
        applicationInfo.pApplicationName = "Vulkan Tutorial";
        applicationInfo.applicationVersion = VK_MAKE_VERSION(0, 0, 1);
        applicationInfo.apiVersion = VK_API_VERSION_1_2;
        
        VkInstanceCreateInfo createInfo = {VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO};
        createInfo.pApplicationInfo = &applicationInfo;
        createInfo.enabledLayerCount = ARRAY_COUNT(enabledLayers);
        createInfo.ppEnabledLayerNames = enabledLayers;
        createInfo.enabledExtensionCount = instanceExtensionCount;
        createInfo.ppEnabledExtensionNames = instanceExtensions;
        
#ifdef EZV_INFO_OUTPUT
        LOG_INFO("----INSTANCE EXTENSIONS---");
        for (uint32_t i = 0; i < instanceExtensionCount; i++)
        {
            LOG_INFO(instanceExtensions[i]);
        }
        //LOG_INFO("--------------------------");
#endif // EZV_INFO_OUTPUT
        
#ifdef EZV_INFO_OUTPUT
        LOG_INFO("-------ACTIVE LAYERS------");
        for (uint32_t i = 0; i < createInfo.enabledLayerCount; i++)
        {
            LOG_INFO(createInfo.ppEnabledLayerNames[i]);
        }
        LOG_INFO("--------------------------");
#endif // EZV_INFO_OUTPUT
        
        if (VK(vkCreateInstance(&createInfo, 0, &context->instance) != VK_SUCCESS))
        {
            LOG_ERROR("Error creating vulkan instance");
            return false;
        }
        //LOG_DEBUG("instance: ", context->instance);
        
        return true;
    }
    
    bool EzV::selectPhysicalDevice()
    {
        uint32_t numDevices = 0;
        VKA(vkEnumeratePhysicalDevices(context->instance, &numDevices, 0));
        if(numDevices == 0)
        {
            LOG_CRIT("Failed to find GPU's with Vulkan Support!");
            context->physicalDevice = 0;
        }
        VkPhysicalDevice* physicalDevices = new VkPhysicalDevice[numDevices];
        
        VKA(vkEnumeratePhysicalDevices(context->instance, &numDevices, physicalDevices));
        LOG_INFO("Found ", numDevices, " GPU(s): ");
        for (uint32_t i = 0; i < numDevices; i++)
        {
            VkPhysicalDeviceProperties properties = {};
            VK(vkGetPhysicalDeviceProperties(physicalDevices[i], &properties));
            LOG_INFO("GPU ", i, ": ", properties.deviceName);
        }
        context->physicalDevice = physicalDevices[0];
        VK(vkGetPhysicalDeviceProperties(context->physicalDevice, &context->physicalDeviceProperties));
        LOG_INFO_IV("Selected GPU: ", context->physicalDeviceProperties.deviceName);
        
        delete[] physicalDevices;
        return true;
    }
    
    bool EzV::createLogicalDevice(uint32_t deviceExtensionCount, const char** deviceExtensions)
    {
        // Queues
        uint32_t numQueueFamilies = 0; 
        VK(vkGetPhysicalDeviceQueueFamilyProperties(context->physicalDevice, &numQueueFamilies, 0));
        VkQueueFamilyProperties* queueFamilies = new VkQueueFamilyProperties[numQueueFamilies];
        VK(vkGetPhysicalDeviceQueueFamilyProperties(context->physicalDevice, &numQueueFamilies, queueFamilies));
        
        uint32_t graphicsQueueIndex = 0;
        for (uint32_t i = 0; i < numQueueFamilies; i++)
        {
            VkQueueFamilyProperties queueFamily = queueFamilies[i];
            if (queueFamily.queueCount > 0)
            {
                if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
                {
                    graphicsQueueIndex = i;
                    break;
                }
            }
        }
        
        float priorities[] = { 1.0f };
        VkDeviceQueueCreateInfo queueCreateInfo = { VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO };
        queueCreateInfo.queueFamilyIndex = graphicsQueueIndex;
        queueCreateInfo.queueCount = 1;
        queueCreateInfo.pQueuePriorities = priorities;
        
        VkPhysicalDeviceFeatures enabledFeatures = {};
        
        VkDeviceCreateInfo createInfo = { VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO };
        createInfo.queueCreateInfoCount = 1;
        createInfo.pQueueCreateInfos = &queueCreateInfo;
        createInfo.enabledExtensionCount = deviceExtensionCount;
        createInfo.ppEnabledExtensionNames = deviceExtensions;
        createInfo.pEnabledFeatures = &enabledFeatures;
        
        
        delete[] queueFamilies;
        if (vkCreateDevice(context->physicalDevice, &createInfo, 0, &context->logicalDevice))
        {
            LOG_ERROR("Failed to create logical device");
            return false;
        }
        
        // Aquire queues
        context->graphicsQueue.familyIndex = graphicsQueueIndex;
        VK(vkGetDeviceQueue(context->logicalDevice, graphicsQueueIndex, 0, &context->graphicsQueue.queue));
        
        return true;
    }
    
    
    EzV::EzV(EzVCreateInfo* creationStruct)
    {
        *creationStruct->p_returnVal = createVulkanInstance(creationStruct);
    }
    
    EzV::~EzV()
    {
        destroySwapchain();
        destroyVulkanInstance();
        destroyRenderPass();
    }
    
    int EzV::createVulkanInstance(EzVCreateInfo* creationStruct)
    {
        context = new VulkanContext;
        swapchain = new VulkanSwapchain;
        
        if (IS_FLAG_SET(creationStruct->m_creationFlags, CREATE_VULKAN_INSTANCE) && !initVulkanInstance(creationStruct->m_instanceExtensionCount, creationStruct->p_instanceExtensions))
        {
            LOG_ERROR("Error whilst creating vulkan instance. aborting");
            return 1;
        }
        
        if (IS_FLAG_SET(creationStruct->m_creationFlags, FIND_PHYSICAL_DEVICE) && !selectPhysicalDevice())
        {
            LOG_ERROR("Error whilst querying for physical device. aborting");
            return 2;
        }
        
        if (IS_FLAG_SET(creationStruct->m_creationFlags, GENERATE_LOGICAL_DEVICE) && !createLogicalDevice(creationStruct->m_deviceExtensionCount, creationStruct->p_deviceExtensions))
        {
            LOG_ERROR("Error whilst creating logical device. aborting");
            return 3;
        }
        
#ifdef EZV_INFO_OUTPUT
        LOG_INFO("Vulkan context created successfully!");
#endif // EZV_INFO_OUTPUT
        
        //createSurface();
        return 0;
    }
    
    void EzV::destroyVulkanInstance()
    {
        if (context)
        {
        
            if (context->logicalDevice)
            {
                VKA(vkDeviceWaitIdle(context->logicalDevice));
                VK(vkDestroyDevice(context->logicalDevice, 0));
            }
            if (context->instance) VK(vkDestroyInstance(context->instance, 0));
            delete context;
            context = 0;

#ifdef EZV_INFO_OUTPUT
            LOG_WARN("Vulkan instance destroyed successfully!");
#endif // EZV_INFO_OUTPUT
        }
    }
}
