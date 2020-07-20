project(LunarEngine)

file(GLOB_RECURSE SOURCE_FILES
        ${PROJECT_SOURCE_DIR}/source/*.c
        ${PROJECT_SOURCE_DIR}/source/*.cpp
        ${CMAKE_SOURCE_DIR}/external/imgui/*.cpp)
file(GLOB_RECURSE HEADER_FILES
        ${PROJECT_SOURCE_DIR}/public/*.h
        ${PROJECT_SOURCE_DIR}/public/*.hpp)

include_directories(${CMAKE_SOURCE_DIR}/Lib/include 
                    ${CMAKE_SOURCE_DIR}/glfw/include
                    ${CMAKE_SOURCE_DIR}/external/imgui)

link_directories(${CMAKE_SOURCE_DIR}/libs)

add_library(LunarEngine STATIC ${SOURCE_FILES} "source/LunarUltralight/UltralightView.cpp" "source/LunarUltralight/UltralightMaterial.cpp" "public/LunarImage.h" "source/Utils/LunarImage.cpp" "public/LunarUltralight/UltralightData.h" "source/LunarUltralight/UltralightImage.cpp" "source/LunarUltralight/UltralightRenderer.cpp" "public/LunarUser/LunarInputManager.h" "public/LunarUser/LunarInputData.h" "source/Utils/LunarInputManager.cpp" "source/LunarUltralight/UltralightInput.cpp" "public/LunarUltralight/HtmlLoader.h" "source/LunarUltralight/HtmlLoader.cpp")
if(WIN32)
target_link_libraries(${PROJECT_NAME} AppCore Ultralight UltralightCore WebCore ${Vulkan_LIBRARY} ${ASSIMP_LIBRARIES} ${WINLIBS} glfw)
else(UNIX)
target_link_libraries(${PROJECT_NAME} glfw )
endif()