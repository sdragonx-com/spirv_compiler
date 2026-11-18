#pragma once

#ifndef _USE_MATH_DEFINES
    #define _USE_MATH_DEFINES
#endif

// std
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

// gl
#include <GL/glew.h>

// glfw
#define GLFW_INCLUDE_GLU
#include <GLFW/glfw3.h>

// glm
#if !defined(GLM_ENABLE_EXPERIMENTAL)
    #define GLM_ENABLE_EXPERIMENTAL
#endif
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/io.hpp>
#include <glm/gtx/normal.hpp>
#include <glm/gtx/rotate_vector.hpp>

// imgui
#if defined(CGL_USE_IMGUI_DOCKING)
    #include <imgui-docking/imgui.h>
    #include <imgui-docking/backends/imgui_impl_glfw.h>
    #include <imgui-docking/backends/imgui_impl_opengl3.h>
#else
    #include <imgui/imgui.h>
    #include <imgui/backends/imgui_impl_glfw.h>
    #include <imgui/backends/imgui_impl_opengl3.h>
#endif

typedef uint8_t byte_t;

typedef glm::u8vec3 vec3ub;
typedef glm::u8vec4 vec4ub;
typedef glm::ivec2 vec2i;
typedef glm::ivec3 vec3i;
typedef glm::ivec4 vec4i;
typedef glm::vec2 vec2f;
typedef glm::vec3 vec3f;
typedef glm::vec4 vec4f;
typedef glm::dvec2 vec2d;
typedef glm::dvec3 vec3d;
typedef glm::dvec4 vec4d;
typedef glm::mat3 mat3f;
typedef glm::mat4 mat4f;

extern vec2i screen;
extern vec2f mouse;
extern vec2f lastpos;
extern int mouse_button;

#define __cgl_unused(a) (void)a

// init GLFW window
GLFWwindow* init_glfw(const std::string& title, int width, int height, bool resizable);

// events
void reshape_event(GLFWwindow* window, int width, int height);
void keyboard_event(GLFWwindow* window, int key, int scancode, int action, int modifier);
void mouse_event(GLFWwindow* window, int button, int action, int mods);
void motion_event(GLFWwindow* window, double x, double y);
void scroll_event(GLFWwindow* window, double x, double y);
void update(GLFWwindow* window, double delay);
void display(GLFWwindow* window);
void display_ui(GLFWwindow* window);
