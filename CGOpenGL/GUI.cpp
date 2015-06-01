#include "GUI.h"

#include "AntTweakBar.h"


GUI::GUI()
{
}


GUI::~GUI()
{
}

/// <summary>
/// Initializes the GUI, adapted to the window dimensions
/// </summary>
/// <param name="windowWidth">Width of the window.</param>
/// <param name="windowHeight">Height of the window.</param>
void GUI::Initialize( uint32_t windowWidth, uint32_t windowHeight )
{
	// Start Tweak Bar
	TwInit( TW_OPENGL, NULL );
	TwWindowSize( windowWidth, windowHeight );
}

/// <summary>
/// Deletes all the GUI elements and de-inits AntTweakBar
/// </summary>
void GUI::Cleanup()
{
	TwTerminate();
}

/// <summary>
/// Draws the GUI
/// </summary>
void GUI::Draw()
{
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
	return TwEventCharGLFW( codepoint, 0 );
}
