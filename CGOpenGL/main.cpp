//Include GLEW  
#include <GL/glew.h>

//Include GLFW  
#include <GLFW/glfw3.h>

// Include GLM
#include <glm/glm.hpp>

#include "StandardIncludes.h"

// Project Includes
#include "Debug.h"
#include "ObjectManager.h"
#include "Scene.h"
#include "VertexFormatManager.h"

//Define an error callback
static void error_callback( int error, const char* description )
{
	fputs( description, stderr );
	_fgetchar();
}

//Define the key input callback  
static void key_callback( GLFWwindow* window, int key, int scancode, int action, int mods )
{
	if( key == GLFW_KEY_ESCAPE && action == GLFW_PRESS )
		glfwSetWindowShouldClose( window, GL_TRUE );
}

int main( void )
{
	//Set the error callback  
	glfwSetErrorCallback( error_callback );

	//Initialize GLFW  
	if( !glfwInit() )
	{
		exit( EXIT_FAILURE );
	}

	//Set the GLFW window creation hints - these are optional  
	glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 ); //Request a specific OpenGL version  
	glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 ); //Request a specific OpenGL version
	glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE );
	glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );

	//Declare a window object  
	GLFWwindow* window;

	//Create a window and create its OpenGL context  
	window = glfwCreateWindow( 640, 480, "Test Window", NULL, NULL );

	//If the window couldn't be created  
	if( !window )
	{
		Debug::Log( "Failed to open GLFW window.\n", LogType::Error );
		glfwTerminate();
		exit( EXIT_FAILURE );
	}

	//This function makes the context of the specified window current on the calling thread.   
	glfwMakeContextCurrent( window );

	//Sets the key callback  
	glfwSetKeyCallback( window, key_callback );

	//Initialize GLEW 
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();

	//If GLEW hasn't initialized  
	if( err != GLEW_OK )
	{
		Debug::Log( GLUBYTETOSTR( glewGetErrorString( err ) ), LogType::Error );
		return -1;
	}

	// Detect OpenGL version
	const GLubyte* y = glGetString( GL_VERSION );
	std::string glVer = GLUBYTETOSTR( glGetString( GL_VERSION ) );
	Debug::Log( "Starting Program with OpenGL version: " + glVer, LogType::Info );

	//Set a background color  
	glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );

	// Load Scene
	Scene::Load("");

	//Main Loop  
	do
	{
		// Update all objects
		ObjectManager::ExecUpdate();

		//Clear color buffer  
		glClear( GL_COLOR_BUFFER_BIT );

		// Render all objects
		ObjectManager::ExecRender();

		//Swap buffers  
		glfwSwapBuffers( window );
		//Get and organize events, like keyboard and mouse input, window resizing, etc...  
		glfwPollEvents();

	} //Check if the ESC key had been pressed or if the window had been closed  
	while( !glfwWindowShouldClose( window ) );

	// Unload scene
	Scene::Unload();

	//Close OpenGL window and terminate GLFW  
	glfwDestroyWindow( window );
	//Finalize and clean up GLFW  
	glfwTerminate();

	exit( EXIT_SUCCESS );
}