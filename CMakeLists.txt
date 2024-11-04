cmake_minimum_required(VERSION 3.16.0)

project(vulkan_test1
        VERSION 0.1.0
        DESCRIPTION "This is a Vulkan test to see if I can render."
        LANGUAGES C CXX
    )

add_executable(${PROJECT_NAME} main.cpp)

target_include_directories(${PROJECT_NAME} PUBLIC
    "C:/VulkanSDK/1.3.296.0/Include"
    "C:/Users/jacki/Desktop/Graphics_Bin/SDL-preview-3.1.3/include"
)
target_link_directories(${PROJECT_NAME} PUBLIC
    "C:/Users/jacki/Desktop/Graphics_Bin/SDL-preview-3.1.3/build/Release"
    "C:/VulkanSDK/1.3.296.0/Lib"
)

add_subdirectory(src)