
// This also includes all opengl stuff
#include "StandardIncludes.h"

// Project Includes
#include "Debug.h"
#include "GUI.h"
#include "ObjectManager.h"
#include "Scene.h"
#include "VertexFormatManager.h"

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
void mouseButtonCallback( GLFWwindow* window, int button, int action, int mods )
{
	// Handle AntTweakBar
	if( GUI::MouseButtonHandler(button, action) )
		return;
}


// Define the cursor movement callback
static void cursorPositionCallback( GLFWwindow* window, double xpos, double ypos )
{
	// Handle AntTweakBar
	if( GUI::MouseMoveHandler( xpos, ypos ) )
		return;
}

// Define the callback on scrolling
void scrollCallback( GLFWwindow* window, double xoffset, double yoffset )
{
	// Handle AntTweakBar
	if( GUI::MouseScrollHandler( yoffset ) )
		return;
}

// Define the key input callback  
static void keyCallback( GLFWwindow* window, int key, int scancode, int action, int mods )
{
	// Handle AntTweakBar
	if( GUI::KeyHandler( key, action ) )
		return;

	// Handle all our key presses
	if( key == GLFW_KEY_ESCAPE && action == GLFW_PRESS )
		glfwSetWindowShouldClose( window, GL_TRUE );

}

// Define the unicode input callback
void characterCallback( GLFWwindow* window, unsigned int codepoint )
{
	// Handle AntTweakBar
	if( GUI::CharHandler( codepoint ) )
		return;
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
	uint32_t windowWidth = 800;
	uint32_t windowHeight = 600;
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
	glfwSetCharCallback( window, characterCallback );

	// Initialize GLEW 
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();

	// If GLEW hasn't initialized  
	if( err != GLEW_OK )
	{
		Debug::Log( GLUBYTETOSTR( glewGetErrorString( err ) ), LogType::Error );
		return -1;
	}

	// Detect OpenGL version
	const GLubyte* y = glGetString( GL_VERSION );
	std::string glVer = GLUBYTETOSTR( glGetString( GL_VERSION ) );
	Debug::Log( "Starting Program with OpenGL version: " + glVer, LogType::Info );

	// Init GUI
	GUI::Initialize( windowWidth, windowHeight );

	// Set a background color  
	glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );

	// Load Scene
	Scene::Load( window, "" );

	// Main Loop  
	do
	{
		// Update all objects
		ObjectManager::ExecUpdate();

		// Clear color buffer  
		glClear( GL_COLOR_BUFFER_BIT );

		// Render all objects
		ObjectManager::ExecRender();

		// Draw GUI
		GUI::Draw();

		// Swap buffers  
		glfwSwapBuffers( window );
		// Get and organize events, like keyboard and mouse input, window resizing, etc...  
		glfwPollEvents();

	} // Check if the ESC key had been pressed or if the window had been closed  
	while( !glfwWindowShouldClose( window ) );

	// Unload scene
	Scene::Unload();
	// Cleanup GUI
	GUI::Cleanup();

	// Close OpenGL window and terminate GLFW  
	glfwDestroyWindow( window );
	// Finalize and clean up GLFW  
	glfwTerminate();

	exit( EXIT_SUCCESS );
}