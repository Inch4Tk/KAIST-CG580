#include "Input.h"


Input::Input()
{
}


Input::~Input()
{
}

void Input::PostMouseButtonEvent( int button, int action, int mods )
{

}

void Input::PostMousePosEvent( double xpos, double ypos )
{

}

void Input::PostKeyEvent( int key, int scancode, int action, int mods )
{

}

void Input::ListenMouseButtonEvent( std::function<void()> callback )
{

}

void Input::ListenKeyEvent( std::function<void()> callback )
{

}
