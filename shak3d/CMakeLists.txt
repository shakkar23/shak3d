# CMakeList.txt : CMake project for shak3d, include source and define
# project specific logic here.
#

# Add source to this project's executable.

set(CMAKE_CXX_STANDARD 20)
project ("shak3d")
# Find SDL2 and OpenGL
find_package(SDL2 CONFIG REQUIRED)
if(NOT WIN32)
    find_package(OpenGL REQUIRED)
endif()
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(glm CONFIG REQUIRED)


include_directories(${SDL2_INCLUDE_DIRS} ${SDL2main_INCLUDE_DIRS} ${OPENGL_INCLUDE_DIR} ${CMAKE_BINARY_DIR})
add_executable (shak3d      "main.cpp" "Game.cpp" "SDL2-Inputs/inputs.cpp"  "Util/math.hpp")


target_link_libraries(shak3d PRIVATE SDL2_ttf::SDL2_ttf SDL2_image::SDL2_image ${SDL2_LIBRARIES}  ${OPENGL_LIBRARIES} glm::glm-header-only)
# TODO: Add tests and install targets if needed.
# Copy SDL2 DLLs to output folder on Windows
if(WIN32)
    foreach(DLL ${SDL2_DLLS})
        add_custom_command(TARGET shak3d POST_BUILD COMMAND
            ${CMAKE_COMMAND} -E copy_if_different ${DLL} $<TARGET_FILE_DIR:shak3d>)
    endforeach()
endif()


# move assets folder into build directory every time we build

add_custom_command(TARGET shak3d PRE_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory
	${CMAKE_SOURCE_DIR}/assets
	$<TARGET_FILE_DIR:shak3d>/assets
)

# move shaders folder as well

add_custom_command(TARGET shak3d PRE_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory
	${CMAKE_SOURCE_DIR}/shaders
	$<TARGET_FILE_DIR:shak3d>/shaders
)