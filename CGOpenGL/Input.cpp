#include "Input.h"


Input::Input()
{
}


Input::~Input()
{
}

void Input::PostMouseButtonEvent( int button, int action, int mods )
{
	// Set state

	// Find the hooks that are triggered and call back
	auto range = mouseButtonHooks.equal_range( std::pair<int, int>( button, action ) );
	for( auto it = range.first; it != range.second; ++it )
	{
		it->second();
	}
}

void Input::PostMousePosEvent( double xpos, double ypos )
{
	// Set state
}

void Input::PostKeyEvent( int key, int scancode, int action, int mods )
{
	// Set state

	// Find the hooks that are triggered and call back
	auto range = keyHooks.equal_range( std::pair<int, int>( key, action ) );
	for( auto it = range.first; it != range.second; ++it )
	{
		it->second();
	}
}

/// <summary>
/// Registers a mouse button hook
/// </summary>
/// <param name="button">The button.</param>
/// <param name="action">The action.</param>
/// <param name="callback">The callback.</param>
void Input::RegisterMouseButtonHook( int button, int action, std::function<void()> callback )
{
	mouseButtonHooks.emplace(std::pair<int, int>(button, action), callback);
}

/// <summary>
/// Registers a key hook
/// </summary>
/// <param name="key">The key.</param>
/// <param name="action">The action.</param>
/// <param name="callback">The callback.</param>
void Input::RegisterKeyEventHook( int key, int action, std::function<void()> callback )
{
	keyHooks.emplace( std::pair<int, int>( key, action ), callback );

}
