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
	0.5f,-0.5f,-0.5f,
	0.5f,0.5f,-0.5f,
	0.5f,-0.5f,0.5f,

	0.5f,0.5f,0.5f,
	0.5f,-0.5f,0.5f,
	0.5f,-0.5f,-0.5f,

	0.5f,0.5f,-0.5f,
	0.5f,0.5f,0.5f,
	-0.5f,0.5f,-0.5f,

	0.5f,0.5f,0.5f,
	-0.5f,0.5f,-0.5f,
	-0.5f,0.5f,0.5f,

	-0.5f,-0.5f,-0.5f,
	-0.5f,0.5f,-0.5f,
	-0.5f,-0.5f,0.5f,

	-0.5f,-0.5f,0.5f,
	-0.5f,0.5f,0.5f,
	-0.5f,0.5f,-0.5f,

	-0.5f,-0.5f,0.5f,
	0.5f,-0.5f,-0.5f,
	-0.5f,-0.5f,-0.5f,

	-0.5f,-0.5f,0.5f,
	0.5f,-0.5f,-0.5f,
	0.5f,-0.5f,0.5f,

	-0.5f,0.5f,0.5f,
	-0.5f,-0.5f,0.5f,
	0.5f,0.5f,0.5f,

	-0.5f,-0.5f,0.5f,
	0.5f,0.5f,0.5f,
	0.5f,-0.5f,0.5f,

	-0.5f,0.5f,-0.5f,
	-0.5f,-0.5f,-0.5f,
	0.5f,0.5f,-0.5f,

	-0.5f,-0.5f,-0.5f,
	0.5f,0.5f,-0.5f,
	0.5f,-0.5f,-0.5f,
};

GLfloat color[] =
{
	0.1f,0.0f,0.0f,
	0.0f,0.1f,0.0f,
	0.0f,0.0f,0.1f,

	0.2f,0.0f,0.0f,
	0.0f,0.2f,0.0f,
	0.0f,0.0f,0.2f,

	0.3f,0.0f,0.0f,
	0.0f,0.3f,0.0f,
	0.0f,0.0f,0.3f,

	0.4f,0.0f,0.0f,
	0.0f,0.4f,0.0f,
	0.0f,0.0f,0.4f,

	0.5f,0.0f,0.0f,
	0.0f,0.5f,0.0f,
	0.0f,0.0f,0.5f,

	0.6f,0.0f,0.0f,
	0.0f,0.6f,0.0f,
	0.0f,0.0f,0.6f,

	0.7f,0.0f,0.0f,
	0.0f,0.7f,0.0f,
	0.0f,0.0f,0.7f,

	0.8f,0.0f,0.0f,
	0.0f,0.8f,0.0f,
	0.0f,0.0f,0.8f,

	0.9f,0.0f,0.0f,
	0.0f,0.9f,0.0f,
	0.0f,0.0f,0.9f,

	1.0f,0.0f,0.0f,
	0.0f,1.0f,0.0f,
	0.0f,0.0f,1.0f,

	1.0f,0.5f,0.0f,
	0.0f,1.0f,0.5f,
	0.5f,0.0f,1.0f,

	1.0f,0.0f,0.5f,
	0.5f,1.0f,0.0f,
	0.0f,0.5f,1.0f
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

		GLfloat rotateZ[] = {
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f };

			GLfloat rotateY[] = {
				1.0f, 0.0f, 0.0f, 0.0f,
				0.0f, 1.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 1.0f, 0.0f,
				0.0f, 0.0f, 0.0f, 1.0f };

				GLfloat rotateX[] = {
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
				// color buffer
				unsigned int colorBufferObjID;

				dumpInfo();

				// GL inits
				glClearColor(0.8,0.2,0.5,0);
				glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
				glEnable(GL_DEPTH_TEST);
				printError("GL inits");

				// Load and compile shader
				program = loadShaders("lab1-5.vert", "lab1-5.frag");
				printError("init shader");

				// Upload geometry to the GPU:

				// Allocate and activate Vertex Array Object
				glGenVertexArrays(1, &vertexArrayObjID);
				glBindVertexArray(vertexArrayObjID);
				// Allocate Vertex Buffer Objects
				glGenBuffers(1, &vertexBufferObjID);
				glGenBuffers(1, &colorBufferObjID);


				// VBO for vertex data


				glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObjID);
				printError("Vertice bind");
				glBufferData(GL_ARRAY_BUFFER, 12*9*sizeof(GLfloat), vertices, GL_STATIC_DRAW);
				printError("Vertice data");
				glVertexAttribPointer(glGetAttribLocation(program, "in_Position"), 3, GL_FLOAT, GL_FALSE, 0, 0);
				printError("Vertice pointer");
				glEnableVertexAttribArray(glGetAttribLocation(program, "in_Position"));

				printError("Vertice enable");

				glBindBuffer(GL_ARRAY_BUFFER, colorBufferObjID);
				printError("Color bind");
				glBufferData(GL_ARRAY_BUFFER, 12*9*sizeof(GLfloat), color, GL_STATIC_DRAW);
				printError("Color data");
				glVertexAttribPointer(glGetAttribLocation(program, "in_Color"), 3, GL_FLOAT, GL_FALSE, 0, 0);
				printError("Color pointer");
				glEnableVertexAttribArray(glGetAttribLocation(program, "in_Color"));
				// End of upload of geometry

				printError("Color enable");
			}


			void display(void)
			{
				printError("pre display");

				// clear the screen
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

				glUniformMatrix4fv(glGetUniformLocation(program, "translate"), 1, GL_TRUE, translate);
				glUniformMatrix4fv(glGetUniformLocation(program, "scale"), 1, GL_TRUE, scale);
				glUniformMatrix4fv(glGetUniformLocation(program, "rotateZ"), 1, GL_TRUE, rotateZ);
				glUniformMatrix4fv(glGetUniformLocation(program, "rotateY"), 1, GL_TRUE, rotateY);
				glUniformMatrix4fv(glGetUniformLocation(program, "rotateX"), 1, GL_TRUE, rotateX);



				glBindVertexArray(vertexArrayObjID);	// Select VAO
				glDrawArrays(GL_TRIANGLES, 0, 36);	// draw object


				printError("display");

				glutSwapBuffers();

			}

			void OnTimer(int value)

			{
				GLfloat t = (GLfloat)glutGet(GLUT_ELAPSED_TIME);
				rotateZ[0] = cos(0.005f*t);
				rotateZ[1] = -sin(0.005f*t);
				rotateZ[4] = sin(0.005f*t);
				rotateZ[5] = cos(0.005f*t);


				rotateY[0] = cos(0.005f*t);
				rotateY[2] = -sin(0.005f*t);
				rotateY[8] = sin(0.005f*t);
				rotateY[10] = cos(0.005f*t);

				rotateX[5] = cos(0.005f*t);
				rotateX[6] = -sin(0.005f*t);
				rotateX[9] = sin(0.005f*t);
				rotateX[10] = cos(0.005f*t);

				translate[3] = (0.05f*t);
				translate[7] = (0.05f*t);
				translate[11] = (0.05f*t);

				glutPostRedisplay();

				glutTimerFunc(20, &OnTimer, (value));
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
