#pragma once

#include "StandardIncludes.h"

class GUI
{
public:
	GUI();
	~GUI();

	static void Initialize( uint32_t windowWidth, uint32_t windowHeight );
	static void Cleanup();
	static void Draw();

	// Input events
	static int MouseButtonHandler( int button, int action );
	static int MouseMoveHandler( double xpos, double ypos );
	static int MouseScrollHandler( double yoffset );
	static int KeyHandler( int key, int action );
	static int CharHandler( unsigned int codepoint );
};

