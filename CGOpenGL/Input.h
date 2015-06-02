#pragma once

#include "StandardIncludes.h"

class Input
{
	friend class AppManager;
public:

	void PostMouseButtonEvent( int button, int action, int mods );
	void PostMousePosEvent( double xpos, double ypos );
	void PostKeyEvent( int key, int scancode, int action, int mods );

	void ListenMouseButtonEvent( std::function<void()> callback );
	void ListenKeyEvent( std::function<void()> callback );

private:
	Input();
	~Input();

};

