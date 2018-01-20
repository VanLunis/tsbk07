// Lab 1-1.
// This is the same as the first simple example in the course book,
// but with a few error checks.
// Remember to copy your file to a new on appropriate places during the lab so you keep old results.
// Note that the files "lab1-1.frag", "lab1-1.vert" are required.

// Should work as is on Linux and Mac. MS Windows needs GLEW or glee.
// See separate Visual Studio version of my demos.
#ifdef __APPLE__
	#include <OpenGL/gl3.h>
	// Linking hint for Lightweight IDE
	// uses framework Cocoa
#endif
#include "MicroGlut.h"
#include "GL_utilities.h"
#include <math.h>

// Globals
// Data would normally be read from files
GLfloat vertices[] =
{
	0.9f,0.9f,0.0f,
	-0.5f,0.5f,0.0f,
	0.5f,-0.5f,0.0f
};

GLfloat translate[] = {
	1.0f, 0.0f, 0.0f, 0.5f,
	0.0f, 1.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 1.0f };

	GLfloat scale[] = {
		0.5f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.5f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f };

		GLfloat rotate[] = {
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f };

// vertex array object
unsigned int vertexArrayObjID;

// Reference to shader program
GLuint program;

void init(void)
{
	// vertex buffer object, used for uploading the geometry
	unsigned int vertexBufferObjID;


	dumpInfo();

	// GL inits
	glClearColor(0.8,0.2,0.5,0);
	glDisable(GL_DEPTH_TEST);
	printError("GL inits");

	// Load and compile shader
	program = loadShaders("lab1-4.vert", "lab1-4.frag");
	printError("init shader");

	// Upload geometry to the GPU:

	// Allocate and activate Vertex Array Object
	glGenVertexArrays(1, &vertexArrayObjID);
	glBindVertexArray(vertexArrayObjID);
	// Allocate Vertex Buffer Objects
	glGenBuffers(1, &vertexBufferObjID);


	// VBO for vertex data


	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObjID);
	glBufferData(GL_ARRAY_BUFFER, 9*sizeof(GLfloat), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(glGetAttribLocation(program, "in_Position"), 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(glGetAttribLocation(program, "in_Position"));

	// End of upload of geometry

	printError("init arrays");
}


void display(void)
{
	printError("pre display");

	// clear the screen
	glClear(GL_COLOR_BUFFER_BIT);

	glUniformMatrix4fv(glGetUniformLocation(program, "translate"), 1, GL_TRUE, translate);
	glUniformMatrix4fv(glGetUniformLocation(program, "scale"), 1, GL_TRUE, scale);
	glUniformMatrix4fv(glGetUniformLocation(program, "rotate"), 1, GL_TRUE, rotate);


	glBindVertexArray(vertexArrayObjID);	// Select VAO
	glDrawArrays(GL_TRIANGLES, 0, 3);	// draw object


	printError("display");

	glutSwapBuffers();

}

void OnTimer(int value)

{
	float tmpValue = value;
	rotate[0] = cos(tmpValue);
	rotate[1] = -sin(tmpValue);
	rotate[4] = sin(tmpValue);
	rotate[5] = cos(tmpValue);


	translate[3] = cos(value%5);
	translate[7] = -sin(value%5);
	translate[11] = sin(value%5);

    glutPostRedisplay();

    glutTimerFunc(20, &OnTimer, (value+1));
/* Dat 50 fps */
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitContextVersion(3, 2);
	glutCreateWindow ("GL3 white triangle example");
	init();
	glutDisplayFunc(display);
	OnTimer(0);
	glutMainLoop();
	return 0;
}
