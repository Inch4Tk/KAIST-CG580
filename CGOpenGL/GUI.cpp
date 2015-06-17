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
	TwAddVarRW( mainBar, "C Spacing", TW_TYPE_DIR3F, &cubeSpacing[0], "" );
	TwAddVarRW( mainBar, "C Rand Offset", TW_TYPE_DIR3F, &cubeOffsets[0], "" );
	TwAddVarRW( mainBar, "C Rand Scale", TW_TYPE_DIR3F, &cubeScale[0], "" );
	TwAddVarRW( mainBar, "C Rand Rotation", TW_TYPE_BOOLCPP, &cubeRandRot, "" );
	TwAddSeparator( mainBar, "Sep1", "" );

	TwAddButton( mainBar, "Light Configuration", nullptr, nullptr, "" );
	TwAddVarRW( mainBar, "L Gridsize X", TW_TYPE_INT32, &lights[0], "group='L Gridsize' min=0" );
	TwAddVarRW( mainBar, "L Gridsize Y", TW_TYPE_INT32, &lights[1], "group='L Gridsize' min=0" );
	TwAddVarRW( mainBar, "L Gridsize Z", TW_TYPE_INT32, &lights[2], "group='L Gridsize' min=0" );
	TwAddVarRW( mainBar, "L Spacing", TW_TYPE_DIR3F, &lightSpacing[0], "" );
	TwAddVarRW( mainBar, "L Rand Offset", TW_TYPE_DIR3F, &lightOffsets[0], "" );
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
		// Set some barrier values for directions
		g->cubeSpacing = glm::clamp( g->cubeSpacing, 0.1f, FLT_MAX );
		g->cubeOffsets = glm::clamp( g->cubeOffsets, 0.0f, FLT_MAX );
		g->cubeScale = glm::clamp( g->cubeScale, 0.0f, 1.0f );
		g->lightSpacing = glm::clamp( g->lightSpacing, 0.1f, FLT_MAX );
		g->lightOffsets = glm::clamp( g->lightOffsets, 0.0f, FLT_MAX );
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

int GUI::MouseButtonHandler( int button, int action )
{
	return TwEventMouseButtonGLFW( button, action );
}

int GUI::MouseMoveHandler( double xpos, double ypos )
{
	return TwEventMousePosGLFW( static_cast<int>(xpos), static_cast<int>(ypos) );
}

int GUI::MouseScrollHandler( double yoffset )
{
	return TwEventMouseWheelGLFW( static_cast<int>(yoffset) );
}

int GUI::KeyHandler( int key, int action )
{
	return TwEventKeyGLFW( key, action );
}

int GUI::CharHandler( unsigned int codepoint )
{
	return TwEventCharGLFW( codepoint, GLFW_PRESS );
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
