#pragma once

#include "StandardIncludes.h"

// Forwards
class GUI;
class Input;
class ObjectManager;
class Scene;

class AppManager
{
public:

	static void Initialize( GLFWwindow* window );
	static void Terminate();
	static void MainLoop();

	static GLFWwindow* GetWindow();
	static GUI* GetGUI();
	static Input* GetInput();
	static ObjectManager* GetObjectManager();
	static Scene* GetScene();

private:
	AppManager();
	~AppManager();

	// Components
	static GLFWwindow* window;
	static GUI* gui;
	static Input* input;
	static ObjectManager* objectManager;
	static Scene* scene;

};

