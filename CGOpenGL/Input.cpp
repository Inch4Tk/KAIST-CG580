#include "Input.h"

#include "Debug.h"
#include "AppManager.h"


Input::Input()
{
}


Input::~Input()
{
}

/// <summary>
/// Posts the mouse button event.
/// </summary>
/// <param name="button">The button.</param>
/// <param name="action">The action.</param>
/// <param name="mods">The mods.</param>
void Input::PostMouseButtonEvent( int button, int action, int mods )
{
	// Set state
	if( button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS )
		leftMousePressed = true;
	if( button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE )
		leftMousePressed = false;
	if( button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS )
		rightMousePressed = true;
	if( button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE )
		rightMousePressed = false;

	// Find the hooks that are triggered and call back
	auto range = mouseButtonHooks.equal_range( std::pair<int, int>( button, action ) );
	for( auto it = range.first; it != range.second; ++it )
	{
		it->second();
	}
}

/// <summary>
/// Posts the mouse position event.
/// </summary>
/// <param name="xpos">The xpos.</param>
/// <param name="ypos">The ypos.</param>
void Input::PostMousePosEvent( double xpos, double ypos )
{
	// Set state
	lastMousePosX = mousePosX;
	lastMousePosY = mousePosY;
	mousePosX = xpos;
	mousePosY = ypos;

	// Debatable, whether we should normalize to window dims, or just to multiple of aspect
	std::pair<int, int> windowDim = AppManager::GetWindowDimensions();
	mouseDeltaXNorm = (mousePosX - lastMousePosX) / static_cast<double>(windowDim.first);
	mouseDeltaYNorm = (mousePosY - lastMousePosY) / static_cast<double>(windowDim.second);
}

/// <summary>
/// Posts the key event.
/// </summary>
/// <param name="key">The key.</param>
/// <param name="scancode">The scancode.</param>
/// <param name="action">The action.</param>
/// <param name="mods">The mods.</param>
void Input::PostKeyEvent( int key, int scancode, int action, int mods )
{
	// Set state
	// Left right axis (R)
	if( (key == GLFW_KEY_D || key == GLFW_KEY_RIGHT) && action == GLFW_PRESS )
		inputAxisX += 1.0f;
	if( (key == GLFW_KEY_D || key == GLFW_KEY_RIGHT) && action == GLFW_RELEASE )
		inputAxisX -= 1.0f;
	if( (key == GLFW_KEY_A || key == GLFW_KEY_LEFT) && action == GLFW_PRESS )
		inputAxisX -= 1.0f;
	if( (key == GLFW_KEY_A || key == GLFW_KEY_LEFT) && action == GLFW_RELEASE )
		inputAxisX += 1.0f;
	// Up down axis (Y)
	if( (key == GLFW_KEY_W || key == GLFW_KEY_UP) && action == GLFW_PRESS )
		inputAxisY += 1.0f;
	if( (key == GLFW_KEY_W || key == GLFW_KEY_UP) && action == GLFW_RELEASE )
		inputAxisY -= 1.0f;
	if( (key == GLFW_KEY_S || key == GLFW_KEY_DOWN) && action == GLFW_PRESS )
		inputAxisY -= 1.0f;
	if( (key == GLFW_KEY_S || key == GLFW_KEY_DOWN) && action == GLFW_RELEASE )
		inputAxisY += 1.0f;

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

/// <summary>
/// Determines whether Left Mouse Button is pressed.
/// </summary>
/// <returns></returns>
bool Input::IsLMBPressed() const
{
	return leftMousePressed;
}

/// <summary>
/// Determines whether Right Mouse Button is pressed.
/// </summary>
/// <returns></returns>
bool Input::IsRMBPressed() const
{
	return rightMousePressed;
}

/// <summary>
/// Gets the input axisX.
/// </summary>
/// <returns></returns>
double Input::GetInputAxisX() const
{
	return inputAxisX;
}

/// <summary>
/// Gets the input axisY.
/// </summary>
/// <returns></returns>
double Input::GetInputAxisY() const
{
	return inputAxisY;
}

/// <summary>
/// Gets the normalized mouse delta x.
/// </summary>
/// <returns></returns>
double Input::GetMouseDeltaX() const
{
	return mouseDeltaXNorm;
}

/// <summary>
/// Gets the normalized mouse delta y.
/// </summary>
/// <returns></returns>
double Input::GetMouseDeltaY() const
{
	return mouseDeltaYNorm;
}
