#pragma once

#include "StandardIncludes.h"

#include "AntTweakBar.h"

class GUI
{
	friend class AppManager;
public:
	void Draw();

	void ToggleGUI();

	// Input events
	int MouseButtonHandler( GLFWwindow* window, int glfwButton, int glfwAction, int glfwMods );
	int MouseMoveHandler( GLFWwindow* window, double xpos, double ypos );
	int MouseScrollHandler( GLFWwindow* window, double xoffset, double yoffset );
	int KeyHandler( GLFWwindow* window, int glfwKey, int glfwScancode, int glfwAction, int glfwMods );
	int CharHandler( GLFWwindow* window, int glfwChar, int glfwMods );

	// Expose these vars as public for easy sync, bit hackish, but easy
	glm::ivec3 cubes = { 1, 1, 1 };
	glm::vec3 cubeSpacing = { 2, 2, 2 };
	glm::vec3 cubeOffsets = { 0, 0, 0 };
	glm::vec3 cubeScale = { 0, 0, 0 };
	bool cubeRandRot = false;

	glm::ivec3 lights = { 1, 1, 1 };
	glm::vec3 lightSpacing = { 5, 5, 5 };
	glm::vec3 lightOffsets = { 0, 0, 0 };
	glm::vec3 lightColor = { 1, 1, 1 };
	float lightRange = 10.0f;
	float lightRangeVariance = 0.0f;
	bool lightRandomColors = false;

	bool autoRegenerate = false;
	float autoRegenTime = 1.0f;
	float curAutoRegen = 0.0f;

private:
	GUI( uint32_t windowWidth, uint32_t windowHeight );
	~GUI();

	bool active = false;
	TwBar* mainBar;
	std::string mainBarName = "mainBar";

	// Vars
	double fps;
	double fpsLast;
	double fpsRunning;
	float movespeed;
	bool clustering;
};

