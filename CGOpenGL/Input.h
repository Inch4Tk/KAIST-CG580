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

private:
	Input();
	~Input();
	
	std::unordered_multimap<std::pair<int, int>, std::function<void()>> mouseButtonHooks;
	std::unordered_multimap<std::pair<int, int>, std::function<void()>> keyHooks;
};