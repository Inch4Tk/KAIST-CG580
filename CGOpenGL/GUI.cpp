#include "GUI.h"

#include "AppManager.h"
#include "Debug.h"

GUI::GUI( uint32_t windowWidth, uint32_t windowHeight )
{
	// Start Tweak Bar
	TwInit( TW_OPENGL_CORE, NULL );
	TwWindowSize( windowWidth, windowHeight );

	// For now init the tweak bar here, better to do it separately
	mainBar = TwNewBar( mainBarName.c_str() );
	auto windim = AppManager::GetWindowDimensions();
	TwDefine( (mainBarName + " label='Stats and Config' alpha=180 position='15 15' size='300 " + std::to_string(windim.second - 30) + "'").c_str() );
	if( active )
	{
		TwDefine( (mainBarName + " visible = true").c_str() );
	}
	else
	{
		TwDefine( (mainBarName + " visible = false").c_str() );
	}
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
