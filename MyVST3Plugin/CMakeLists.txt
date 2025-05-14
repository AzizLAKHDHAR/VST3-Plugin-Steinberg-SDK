cmake_minimum_required(VERSION 3.14.0)
project(MyVST3Plugin VERSION 1.0.0)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include VST3 SDK
set(VST3_SDK_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../vst3sdk")
list(APPEND CMAKE_MODULE_PATH "${VST3_SDK_ROOT}/cmake/modules")

# Add VST3 SDK modules
set(SMTG_ADD_VST3_HOSTING_SAMPLES OFF CACHE BOOL "Don't build VST3 hosting samples")
set(SMTG_ADD_VST3_PLUGINS_SAMPLES OFF CACHE BOOL "Don't build VST3 plugin samples")
set(SMTG_ADD_VSTGUI ON CACHE BOOL "Add VSTGUI support")
set(VSTGUI_SOURCE_DIR "${VST3_SDK_ROOT}/vstgui4")

# Include the VST3 SDK cmake modules
if(EXISTS "${VST3_SDK_ROOT}/CMakeLists.txt")
    add_subdirectory(${VST3_SDK_ROOT} ${PROJECT_BINARY_DIR}/vst3sdk)
    smtg_enable_vst3_sdk()
else()
    message(FATAL_ERROR "VST3 SDK not found at specified path: ${VST3_SDK_ROOT}")
endif()

# Enable VSTGUI
if(SMTG_ADD_VSTGUI)
    smtg_enable_vstgui_support()
endif()

# Define source files
set(SOURCE_FILES
    source/pluginprocessor.cpp
    source/pluginprocessor.h
    source/plugincontroller.cpp
    source/plugincontroller.h
    source/pluginfactory.cpp
    source/version.h
    source/pluginids.h
)

# Define resource files
set(RESOURCE_FILES
    resource/plugin.uidesc
)

# Create the VST3 plugin
smtg_add_vst3plugin(${PROJECT_NAME}
    ${SOURCE_FILES}
    ${RESOURCE_FILES}
)


# Link with VST3 SDK libraries
target_link_libraries(${PROJECT_NAME}
    PRIVATE
    sdk
    vstgui_support
)

# Set include directories
target_include_directories(${PROJECT_NAME}
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/source
)