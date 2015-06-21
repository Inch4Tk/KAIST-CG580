
// This also includes all opengl stuff
#include "StandardIncludes.h"

// Project Includes
#include "AppManager.h"
#include "Config.h"
#include "Debug.h"
#include "GUI.h"
#include "Input.h"

// Export Nvidia optimus enablement, to direct app to run from optimus device ón windows if possible
#ifdef _WIN32
#include <wtypes.h>
extern "C" {
	_declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
}
#endif

// Define an error callback
static void errorCallback( int error, const char* description )
{
	fputs( description, stderr );
	_fgetchar();
}

// Defines the callback executed on mouse button events
static void mouseButtonCallback( GLFWwindow* window, int button, int action, int mods )
{
	// Handle AntTweakBar
	if( AppManager::GetGUI()->MouseButtonHandler( window, button, action, mods ) )
		return;

	AppManager::GetInput()->PostMouseButtonEvent( button, action, mods );
}


// Define the cursor movement callback
static void cursorPositionCallback( GLFWwindow* window, double xpos, double ypos )
{
	// Handle AntTweakBar
	if( AppManager::GetGUI()->MouseMoveHandler( window, xpos, ypos ) )
		return;

	AppManager::GetInput()->PostMousePosEvent( xpos, ypos );
}

// Define the callback on scrolling
static void scrollCallback( GLFWwindow* window, double xoffset, double yoffset )
{
	// Handle AntTweakBar
	if( AppManager::GetGUI()->MouseScrollHandler( window, xoffset, yoffset ) )
		return;
}

// Define the key input callback  
static void keyCallback( GLFWwindow* window, int key, int scancode, int action, int mods )
{
	// Handle AntTweakBar
	AppManager::GetGUI()->KeyHandler( window, key, scancode, action, mods );
	AppManager::GetInput()->PostKeyEvent( key, scancode, action, mods );

	// Quit on escape
	if( key == GLFW_KEY_ESCAPE && action == GLFW_PRESS )
		glfwSetWindowShouldClose( window, GL_TRUE );

}

// Define the unicode input callback
static void characterCallback( GLFWwindow* window, unsigned int codepoint, int mod )
{
	// Handle AntTweakBar
	if( AppManager::GetGUI()->CharHandler( window, codepoint, mod ) )
		return;
}

// Define the window focus callback
static void winFocusCallback( GLFWwindow* window, int focused )
{
	AppManager::GetInput()->WindowFocus( focused );
}

int main( void )
{
	// Set the error callback  
	glfwSetErrorCallback( errorCallback );

	// Initialize GLFW  
	if( !glfwInit() )
	{
		exit( EXIT_FAILURE );
	}
	
	// Set the GLFW window creation hints - these are optional  
	glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 4 ); //Request a specific OpenGL version  
	glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 ); //Request a specific OpenGL version
	glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE );
	glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );

	// Declare a window object  
	GLFWwindow* window;

	// Create a window and create its OpenGL context
	uint32_t windowWidth = 1920;
	uint32_t windowHeight = 1080;
	window = glfwCreateWindow( windowWidth, windowHeight, "Clustered Forward Shading", NULL, NULL );

	// If the window couldn't be created  
	if( !window )
	{
		Debug::Log( "Failed to open GLFW window.\n", LogType::Error );
		glfwTerminate();
		exit( EXIT_FAILURE );
	}

	// This function makes the context of the specified window current on the calling thread.   
	glfwMakeContextCurrent( window );

	// Sets glfw callbacks
	glfwSetMouseButtonCallback( window, mouseButtonCallback );
	glfwSetCursorPosCallback( window, cursorPositionCallback );
	glfwSetScrollCallback( window, scrollCallback );
	glfwSetKeyCallback( window, keyCallback );
	glfwSetCharModsCallback( window, characterCallback );
	glfwSetWindowFocusCallback( window, winFocusCallback );

	// Initialize GLEW 
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();

	// If GLEW hasn't initialized  
	if( err != GLEW_OK )
	{
		Debug::Log( GLUBYTETOSTR( glewGetErrorString( err ) ), LogType::Error );
		exit( EXIT_FAILURE );
	}

	// Check for the shader image load store extension (DX: Unordered Access View)
	if( !GLEW_EXT_shader_image_load_store )
	{
		Debug::Log("GLEW_EXT_shader_image_load_store not present. Not possible to run application. (Need OpenGL 4.3 support)" );
		exit( EXIT_FAILURE );
	}

	// Throw away the GLEW invalid enum error, that is produced by a bug in GLEW
	// http://stackoverflow.com/questions/10857335/opengl-glgeterror-returns-invalid-enum-after-call-to-glewinit
	auto glErr = glGetError();
	if( glErr != GL_NO_ERROR && glErr != GL_INVALID_ENUM )
	{
		Debug::LogFailure( __FILE__, __LINE__, "Error initializing OpenGL." );
	}

	// Detect OpenGL version
	const GLubyte* y = glGetString( GL_VERSION );
	std::string glVer = GLUBYTETOSTR( glGetString( GL_VERSION ) );
	Debug::Log( "Starting Program with OpenGL version: " + glVer, LogType::Info );

	// Handle the Applications resources
	AppManager::Initialize( window );
	AppManager::MainLoop();
	AppManager::Terminate();

	// Close OpenGL window and terminate GLFW  
	glfwDestroyWindow( window );
	// Finalize and clean up GLFW  
	glfwTerminate();

	exit( EXIT_SUCCESS );
}