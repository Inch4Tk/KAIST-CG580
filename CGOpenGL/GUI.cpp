#include "GUI.h"

#include "AppManager.h"
#include "Camera.h"
#include "Debug.h"
#include "ObjectManager.h"
#include "Scene.h"
#include "Time.h"

GUI::GUI( uint32_t windowWidth, uint32_t windowHeight )
{
	// Start Tweak Bar
	TwInit( TW_OPENGL_CORE, NULL );
	TwWindowSize( windowWidth, windowHeight );

	// For now init the tweak bar here, better to do it separately
	mainBar = TwNewBar( mainBarName.c_str() );
	auto windim = AppManager::GetWindowDimensions();
	TwDefine( (mainBarName + " label='Stats and Config' valueswidth='150' refresh=0.1 alpha=180 position='15 15' size='300 " + std::to_string(windim.second - 30) + "'").c_str() );
	if( active )
	{
		TwDefine( (mainBarName + " visible = true").c_str() );
	}
	else
	{
		TwDefine( (mainBarName + " visible = false").c_str() );
	}

	// Build all the GUI
	TwAddButton( mainBar, "Stats", nullptr, nullptr, "" );
	TwAddVarRO( mainBar, "Fps", TW_TYPE_DOUBLE, &fpsLast, "" );
	TwAddVarRO( mainBar, "Fps weighted", TW_TYPE_DOUBLE, &fpsRunning, "" );
	TwAddVarRO( mainBar, "Fps avg", TW_TYPE_DOUBLE, &fps, "" );
	TwAddVarRO( mainBar, "Movement speed", TW_TYPE_FLOAT, &movespeed, "" );
	TwAddVarRO( mainBar, "Cluster active", TW_TYPE_BOOLCPP, &clustering, "" );
	TwAddSeparator( mainBar, "Sep0", "" );

	TwAddButton( mainBar, "Cube Configuration", nullptr, nullptr, "" );
	TwAddVarRW( mainBar, "C Gridsize X", TW_TYPE_INT32, &cubes[0], "group='C Gridsize' min=0" );
	TwAddVarRW( mainBar, "C Gridsize Y", TW_TYPE_INT32, &cubes[1], "group='C Gridsize' min=0" );
	TwAddVarRW( mainBar, "C Gridsize Z", TW_TYPE_INT32, &cubes[2], "group='C Gridsize' min=0" );
	TwAddVarRW( mainBar, "C Spacing X", TW_TYPE_FLOAT, &cubeSpacing[0], "group='C Spacing' min=0.1 step=0.5" );
	TwAddVarRW( mainBar, "C Spacing Y", TW_TYPE_FLOAT, &cubeSpacing[1], "group='C Spacing' min=0.1 step=0.5" );
	TwAddVarRW( mainBar, "C Spacing Z", TW_TYPE_FLOAT, &cubeSpacing[2], "group='C Spacing' min=0.1 step=0.5" );
	TwAddVarRW( mainBar, "C Rand Offset X", TW_TYPE_FLOAT, &cubeOffsets[0], "group='C Rand Offset' min=0.0 step=0.5" );
	TwAddVarRW( mainBar, "C Rand Offset Y", TW_TYPE_FLOAT, &cubeOffsets[1], "group='C Rand Offset' min=0.0 step=0.5" );
	TwAddVarRW( mainBar, "C Rand Offset Z", TW_TYPE_FLOAT, &cubeOffsets[2], "group='C Rand Offset' min=0.0 step=0.5" );
	TwAddVarRW( mainBar, "C Rand Scale X", TW_TYPE_FLOAT, &cubeScale[0], "group='C Rand Scale' min=0.0 max=1.0 step=0.02" );
	TwAddVarRW( mainBar, "C Rand Scale Y", TW_TYPE_FLOAT, &cubeScale[1], "group='C Rand Scale' min=0.0 max=1.0 step=0.02" );
	TwAddVarRW( mainBar, "C Rand Scale Z", TW_TYPE_FLOAT, &cubeScale[2], "group='C Rand Scale' min=0.0 max=1.0 step=0.02" );
	TwAddVarRW( mainBar, "C Rand Rotation", TW_TYPE_BOOLCPP, &cubeRandRot, "" );
	TwAddSeparator( mainBar, "Sep1", "" );

	TwAddButton( mainBar, "Light Configuration", nullptr, nullptr, "" );
	TwAddVarRW( mainBar, "L Gridsize X", TW_TYPE_INT32, &lights[0], "group='L Gridsize' min=0" );
	TwAddVarRW( mainBar, "L Gridsize Y", TW_TYPE_INT32, &lights[1], "group='L Gridsize' min=0" );
	TwAddVarRW( mainBar, "L Gridsize Z", TW_TYPE_INT32, &lights[2], "group='L Gridsize' min=0" );
	TwAddVarRW( mainBar, "L Spacing X", TW_TYPE_FLOAT, &lightSpacing[0], "group='L Spacing' min=0.1 step=0.5" );
	TwAddVarRW( mainBar, "L Spacing Y", TW_TYPE_FLOAT, &lightSpacing[1], "group='L Spacing' min=0.1 step=0.5" );
	TwAddVarRW( mainBar, "L Spacing Z", TW_TYPE_FLOAT, &lightSpacing[2], "group='L Spacing' min=0.1 step=0.5" );
	TwAddVarRW( mainBar, "L Rand Offset X", TW_TYPE_FLOAT, &lightOffsets[0], "group='L Rand Offset' min=0.0 step=0.5" );
	TwAddVarRW( mainBar, "L Rand Offset Y", TW_TYPE_FLOAT, &lightOffsets[1], "group='L Rand Offset' min=0.0 step=0.5" );
	TwAddVarRW( mainBar, "L Rand Offset Z", TW_TYPE_FLOAT, &lightOffsets[2], "group='L Rand Offset' min=0.0 step=0.5" );
	TwAddVarRW( mainBar, "L Color", TW_TYPE_COLOR3F, &lightColor[0], "" );
	TwAddVarRW( mainBar, "L Range", TW_TYPE_FLOAT, &lightRange, "min=0.0" );
	TwAddVarRW( mainBar, "L Range variance", TW_TYPE_FLOAT, &lightRangeVariance, "min=0.0" );
	TwAddVarRW( mainBar, "L Rand Colors", TW_TYPE_BOOLCPP, &lightRandomColors, "" );
	TwAddSeparator(mainBar, "Sep2", "");

	TwAddButton( mainBar, "General Configuration", nullptr, nullptr, "" );
	TwAddVarRW( mainBar, "Auto Regenerate", TW_TYPE_BOOLCPP, &autoRegenerate, "" );
	TwAddVarRW( mainBar, "Auto Regen time", TW_TYPE_FLOAT, &autoRegenTime, "min=0.1" );
	TwAddButton( mainBar, "Regenerate", [](void* clientData)
	{
		GUI* g = reinterpret_cast<GUI*>(clientData);
		if( g->lights.x * g->lights.y * g->lights.z > CONFIG_MAX_LIGHTS )
		{
			g->lights.x = 10;
			g->lights.y = 20;
			g->lights.z = 10;
			Debug::Log( "Too many lights, set to some default values" );
		}
		// Regen scene
		AppManager::GetScene()->RegenerateScene();
	}, this, "" );
}


GUI::~GUI()
{
	TwTerminate();
}

/// <summary>
/// Draws the GUI
/// </summary>
void GUI::Draw()
{
	// Sync all the GUI variables and trigger any functions
	fpsLast = AppManager::GetTime()->GetFpsLast();
	fps = AppManager::GetTime()->GetFps();
	fpsRunning = AppManager::GetTime()->GetFpsRunning();
	movespeed = AppManager::GetScene()->GetActiveCamera()->GetMovespeed();
	clustering = AppManager::GetObjectManager()->IsClusteringActive();

	if( lights.x * lights.y * lights.z > CONFIG_MAX_LIGHTS )
	{
		lights.x = 10;
		lights.y = 20;
		lights.z = 10;
		Debug::Log( "Too many lights, set to some default values" );
	}

	// Check for scene autoregeneration
	if( autoRegenerate )
	{
		curAutoRegen += static_cast<float>(AppManager::GetTime()->GetDelta());
		if( curAutoRegen > autoRegenTime )
		{
			AppManager::GetScene()->RegenerateScene();
			curAutoRegen = 0.0f;
		}
	}

	TwDraw();
}

int GUI::MouseButtonHandler( GLFWwindow* window, int glfwButton, int glfwAction, int glfwMods )
{
	
	return TwEventMouseButtonGLFW3( window, glfwButton, glfwAction, glfwMods );
}

int GUI::MouseMoveHandler( GLFWwindow* window, double xpos, double ypos )
{
	return TwEventCursorPosGLFW3( window, xpos, ypos );
}

int GUI::MouseScrollHandler( GLFWwindow* window, double xoffset, double yoffset )
{
	return TwEventScrollGLFW3( window, xoffset, yoffset );
}

int GUI::KeyHandler( GLFWwindow* window, int glfwKey, int glfwScancode, int glfwAction, int glfwMods )
{
	return TwEventKeyGLFW3( window, glfwKey, glfwScancode, glfwAction, glfwMods );
}

int GUI::CharHandler( GLFWwindow* window, int glfwChar, int glfwMods )
{
	return TwEventCharModsGLFW3( window, glfwChar, glfwMods );
}

/// <summary>
/// Toggles the GUI.
/// </summary>
void GUI::ToggleGUI()
{
	active = !active;
	if( active ) // Display bar
	{
		TwDefine( (mainBarName + " visible = true").c_str() );
	}
	else // Hide bar
	{
		TwDefine( (mainBarName + " visible = false").c_str() );
	}
}
