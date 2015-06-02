#pragma once

#include "StandardIncludes.h"

namespace std
{
	template <>
	struct tr1::hash < pair<int, int> >
	{
	public:
		size_t operator()( std::pair<int, int> x ) const throw()
		{
			size_t h = hash<int>()(x.first) ^ hash<int>()(x.second);
			return h;
		}
	};
}

class Input
{
	friend class AppManager;
public:

	void PostMouseButtonEvent( int button, int action, int mods );
	void PostMousePosEvent( double xpos, double ypos );
	void PostKeyEvent( int key, int scancode, int action, int mods );

	void RegisterMouseButtonHook( int button, int action, std::function<void()> callback );
	void RegisterKeyEventHook( int key, int action, std::function<void()> callback );

	bool IsLMBPressed() const;
	bool IsRMBPressed() const;
	float GetInputAxisX() const;
	float GetInputAxisY() const;
	float GetMouseDeltaX() const;
	float GetMouseDeltaY() const;

private:
	Input();
	~Input();
	
	// State
	bool leftMousePressed = false;
	bool rightMousePressed = false;
	float inputAxisX = 0.0f;
	float inputAxisY = 0.0f;

	double lastMousePosX = 0.0;
	double lastMousePosY = 0.0;
	double mousePosX = 0.0;
	double mousePosY = 0.0;
	float mouseDeltaXNorm = 0.0;
	float mouseDeltaYNorm = 0.0;

	// Hooks
	std::unordered_multimap<std::pair<int, int>, std::function<void()>> mouseButtonHooks;
	std::unordered_multimap<std::pair<int, int>, std::function<void()>> keyHooks;
};