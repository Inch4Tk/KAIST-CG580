#include "AppManager.h"

#include "GUI.h"
#include "Input.h"
#include "ObjectManager.h"
#include "Scene.h"
#include "Time.h"

GLFWwindow* AppManager::window = nullptr;
GUI* AppManager::gui = nullptr;
Input* AppManager::input = nullptr;
ObjectManager* AppManager::objectManager = nullptr;
Scene* AppManager::scene = nullptr;
Time* AppManager::time = nullptr;
std::pair<int, int> AppManager::windowDimensions = std::pair<int, int>( 0, 0 );

AppManager::AppManager()
{
}


AppManager::~AppManager()
{
}

/// <summary>
/// Initializes the app state
/// </summary>
/// <param name="window">The window.</param>
void AppManager::Initialize( GLFWwindow* window )
{
	AppManager::window = window;

	// Init GUI
	int winWidth, winHeight;
	glfwGetWindowSize( window, &winWidth, &winHeight );
	windowDimensions = std::pair<int, int>(winWidth, winHeight);
	gui = new GUI( winWidth, winHeight );

	// Init Input
	input = new Input();

	// Init ObjectManager
	objectManager = new ObjectManager();

	// Init Time
	time = new Time();

	// Load Scene
	scene = new Scene();
	scene->LoadScene( "" );

	// Set a background color  
	glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
}

/// <summary>
/// Terminates this instance.
/// </summary>
void AppManager::Terminate()
{
	SDELETE( gui );
	SDELETE( input );
	SDELETE( objectManager );
	SDELETE( scene );
	SDELETE( time );
}

/// <summary>
/// The apps main loop
/// </summary>
void AppManager::MainLoop()
{
	// Main Loop  
	do
	{
		// Updating tasks
		// Update frame time
		time->NextFrame();
		// Update input
		input->Update();
		// Update all objects
		objectManager->ExecUpdate();

		// Rendering tasks
		// Clear color buffer  
		glClear( GL_COLOR_BUFFER_BIT );
		// Render all objects
		objectManager->ExecRender();
		// Draw GUI
		gui->Draw();

		// Swap buffers  
		glfwSwapBuffers( window );
		// Get and organize events, like keyboard and mouse input, window resizing, etc...  
		glfwPollEvents();

	} // Check if the ESC key had been pressed or if the window had been closed  
	while( !glfwWindowShouldClose( window ) );
}

/// <summary>
/// Gets the window.
/// </summary>
/// <returns></returns>
GLFWwindow* AppManager::GetWindow()
{
	return window;
}

/// <summary>
/// Gets the GUI.
/// </summary>
/// <returns></returns>
GUI* AppManager::GetGUI()
{
	return gui;
}

/// <summary>
/// Gets the input.
/// </summary>
/// <returns></returns>
Input* AppManager::GetInput()
{
	return input;
}

/// <summary>
/// Gets the object manager.
/// </summary>
/// <returns></returns>
ObjectManager* AppManager::GetObjectManager()
{
	return objectManager;
}

/// <summary>
/// Gets the scene.
/// </summary>
/// <returns></returns>
Scene* AppManager::GetScene()
{
	return scene;
}

/// <summary>
/// Gets the window dimensions.
/// </summary>
/// <returns></returns>
std::pair<int, int> AppManager::GetWindowDimensions()
{
	return windowDimensions;
}

/// <summary>
/// Gets the time.
/// </summary>
/// <returns></returns>
Time* AppManager::GetTime()
{
	return time;
}
