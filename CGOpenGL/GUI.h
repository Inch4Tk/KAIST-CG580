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
	int MouseButtonHandler( int button, int action );
	int MouseMoveHandler( double xpos, double ypos );
	int MouseScrollHandler( double yoffset );
	int KeyHandler( int key, int action );
	int CharHandler( unsigned int codepoint );

private:
	GUI( uint32_t windowWidth, uint32_t windowHeight );
	~GUI();

	bool active = false;
	TwBar* mainBar;
	std::string mainBarName = "mainBar";
};

