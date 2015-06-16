#include "AppManager.h"

#include "Debug.h"
#include "Camera.h"
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
	// Updating window
	AppManager::window = window;
	int winWidth, winHeight;
	glfwGetWindowSize( window, &winWidth, &winHeight );
	windowDimensions = std::pair<int, int>( winWidth, winHeight );

	// Update the clustered shading variables determining the cluster sizes
	// The +1 for every amount is just for making life easier when calculating and is not written to
	Config::FOV_Y = glm::radians( 60.0f );
	Config::ASPECT = static_cast<float>(winWidth) / static_cast<float>(winHeight);
	Config::NEAR_PLANE = 0.1f;
	Config::FAR_PLANE = 10000.0f;
	Config::DIM_TILES_X = 64;
	Config::DIM_TILES_Y = 64;
	Config::AMT_TILES_X = static_cast<uint32_t>(winWidth + Config::DIM_TILES_X - 1) / Config::DIM_TILES_X + 1;
	Config::AMT_TILES_Y = static_cast<uint32_t>(winHeight + Config::DIM_TILES_Y - 1) / Config::DIM_TILES_Y + 1;
	float sD = 2.0f * tanf( Config::FOV_Y * 0.5f ) / static_cast<float>(Config::AMT_TILES_Y);
	float zGridLocFar = logf( Config::FAR_PLANE / Config::NEAR_PLANE ) / logf( 1.0f + sD );
	Config::AMT_TILES_Z = static_cast<uint32_t>(ceilf( zGridLocFar ) + 0.5f) + 1;

	// Seed rand
	srand( static_cast<uint32_t>(std::time( 0 )) );

	// Init GUI
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

	// Register hotkeys
	// Camera movement speed related hotkeys
	input->RegisterKeyEventHook( GLFW_KEY_KP_ADD, GLFW_PRESS, []()
	{
		scene->GetActiveCamera()->AddMovespeed( 2.5f );
	} );
	input->RegisterKeyEventHook( GLFW_KEY_KP_SUBTRACT, GLFW_PRESS, []()
	{
		scene->GetActiveCamera()->AddMovespeed( -2.5f );
	} );
	input->RegisterKeyEventHook( GLFW_KEY_KP_MULTIPLY, GLFW_PRESS, []()
	{
		scene->GetActiveCamera()->MultMovespeed( 2.0f );
	} );
	input->RegisterKeyEventHook( GLFW_KEY_KP_DIVIDE, GLFW_PRESS, []()
	{
		scene->GetActiveCamera()->MultMovespeed( 0.5f );
	} );

	// Set a background color  
	glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
}

/// <summary>
/// Terminates this instance.
/// </summary>
void AppManager::Terminate()
{
	SDELETE( input );
	SDELETE( scene );
	SDELETE( time );
	SDELETE( gui );
	SDELETE( objectManager ); // Last/Late because of dependencies
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
		// Enable depth buffer (Maybe not a very good spot to set this)
		glEnable( GL_DEPTH_TEST );
		// Clear color/depth buffer  
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

		// Render all objects
		objectManager->ExecRender();
		// Draw GUI
		gui->Draw();

		// Make a general error check here
		CHECK_GL_ERROR();

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
