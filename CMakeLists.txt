cmake_minimum_required(VERSION 3.12)
project(tinyfoo)
find_package(SDL3 REQUIRED CONFIG REQUIRED COMPONENTS SDL3-shared)
set(CMAKE_CXX_STANDARD 20)
include_directories(${SDL3_INCLUDE_DIRS})
include_directories(tinyfoo "include/" "build/lbs" "build/toml" "build/nlohmann")
file(GLOB MAIN_SRC CONFIGURE_DEPENDS "src/*.cpp")
if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic")
endif()
if (1)
	include_directories("build/imgui/")
	file(GLOB IMGUI_SRC
		"build/imgui/imgui_draw.cpp"
		"build/imgui/imgui_tables.cpp"
		"build/imgui/imgui_widgets.cpp"
		"build/imgui/imgui.cpp"
		"build/imgui/imgui_impl_sdl3.cpp"
		"build/imgui/imgui_impl_sdlrenderer3.cpp"
		"build/imgui/imgui_impl_opengl2.cpp"
		"build/imgui/imgui_impl_opengl3.cpp"
	)
endif()
add_compile_definitions(CMAKE_BUILD=1)
add_compile_definitions(_DEBUG=1)
add_executable(tinyfoo ${MAIN_SRC} ${IMGUI_SRC})
target_link_libraries(tinyfoo PRIVATE SDL3::SDL3)
