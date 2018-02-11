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
#include "loadobj.h"
#include "LoadTGA.h"
#include "VectorUtils3.h"


// Globals
// Data would normally be read from files

#define near 1.0

#define far 30.0

#define right 0.5

#define left -0.5

#define top 0.5

#define bottom -0.5

#define PI 3.14159265


GLfloat projectionMatrix[] = {    2.0f*near/(right-left), 0.0f, (right+left)/(right-left), 0.0f,

                                            0.0f, 2.0f*near/(top-bottom), (top+bottom)/(top-bottom), 0.0f,

                                            0.0f, 0.0f, -(far + near)/(far - near), -2*far*near/(far - near),

                                            0.0f, 0.0f, -1.0f, 0.0f };

mat4 rot, trans, total;


GLfloat phi = 0;

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
	1.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 1.0f, 2.0f,
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

			unsigned int bunnyVertexArrayObjID;
			unsigned int bunnyVertexBufferObjID;
			unsigned int bunnyIndexBufferObjID;
			unsigned int bunnyNormalBufferObjID;
			unsigned int bunnyTexCoordBufferObjID;
			// vertex buffer object, used for uploading the geometry
			unsigned int vertexBufferObjID;
			// color buffer
			unsigned int colorBufferObjID;

			// Reference to shader program
			GLuint program;

			Model *m;

			GLuint dirtRef;

			void init(void)
			{


				dumpInfo();
				//Load model
				m = LoadModel("bunnyplus.obj");
				//Load first, dirty texture:
				LoadTGATextureSimple("dirt.tga", &dirtRef);

				// GL inits
				glClearColor(1,1,1,0);
				glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
				glEnable(GL_DEPTH_TEST);
				printError("GL inits");

				// Load and compile shader
				program = loadShaders("lab2-4.vert", "lab2-4.frag");
				printError("init shader");


				// BUNNY TIME
				glGenVertexArrays(1, &bunnyVertexArrayObjID);
				printError("vertexArray");
				glGenBuffers(1, &bunnyVertexBufferObjID);
				printError("buffers vertexBufferObjID");
				glGenBuffers(1, &bunnyIndexBufferObjID);
				printError("buffers indexbuffer");
				glGenBuffers(1, &bunnyTexCoordBufferObjID);
				printError("buffers bunnyTexCoordBufferObjID");
				glGenBuffers(1, &bunnyNormalBufferObjID);
				printError("buffers normalbuffer");

				glBindVertexArray(bunnyVertexArrayObjID);
				printError("bindbertex");


				//Bunnyplus
				//glEnable(GL_TEXTURE_2D);
				//printError("Gl enable");

// Texture binding
glBindTexture(GL_TEXTURE_2D,dirtRef);
printError("Dirttexture bind");

    	if (m->texCoordArray != NULL)

    	{

	        glBindBuffer(GL_ARRAY_BUFFER, bunnyTexCoordBufferObjID);
					printError("bindbuffer bunnyTexCoordBufferObjID");
	        glBufferData(GL_ARRAY_BUFFER, m->numVertices*2*sizeof(GLfloat), m->texCoordArray, GL_STATIC_DRAW);
					printError("bufferdata");
	        glVertexAttribPointer(glGetAttribLocation(program, "inTexCoord"), 2, GL_FLOAT, GL_FALSE, 0, 0);
					printError("vertexattribpointer");
	        glEnableVertexAttribArray(glGetAttribLocation(program, "inTexCoord"));
					printError("enablevertexattribarray");
    	}

				// VBO for vertex data
printError("Bunny EARLY");


glBindBuffer(GL_ARRAY_BUFFER, bunnyVertexBufferObjID);
printError("Bunny vertex buffer");
glBufferData(GL_ARRAY_BUFFER, m->numVertices*3*sizeof(GLfloat), m->vertexArray, GL_STATIC_DRAW);
printError("Bunny vertex data");

glVertexAttribPointer(glGetAttribLocation(program, "in_Position"), 3, GL_FLOAT, GL_FALSE, 0, 0);
printError("Bunny pointer position");
glEnableVertexAttribArray(glGetAttribLocation(program, "in_Position"));
printError("Bunny enable position");

// VBO for normal data

glBindBuffer(GL_ARRAY_BUFFER, bunnyNormalBufferObjID);
printError("Bunny normalBuffer");
glBufferData(GL_ARRAY_BUFFER, m->numVertices*3*sizeof(GLfloat), m->normalArray, GL_STATIC_DRAW);
printError("Bunny data 3*");

glVertexAttribPointer(glGetAttribLocation(program, "in_Normal"), 3, GL_FLOAT, GL_FALSE, 0, 0);
printError("Bunny pointer normal");
glEnableVertexAttribArray(glGetAttribLocation(program, "in_Normal"));
printError("Bunny enable normal");


glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bunnyIndexBufferObjID);
printError("Bunny element buffer");

glBufferData(GL_ELEMENT_ARRAY_BUFFER, m->numIndices*sizeof(GLuint), m->indexArray, GL_STATIC_DRAW);
printError("Bunny element data");


trans = T(0, 0, -2);
rot = Ry(0.0f);

total = Mult(rot, trans);

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
				glUniformMatrix4fv(glGetUniformLocation(program, "projectionMatrix"), 1, GL_TRUE, projectionMatrix);
				glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total.m);


				GLfloat t = glutGet(GLUT_ELAPSED_TIME) / 100.0;
				glUniform1f(glGetUniformLocation(program, "t"), t);

				glUniform1i(glGetUniformLocation(program, "textUnit"), 0); // Texture unit 0

        phi = (phi < 2*PI) ? phi+PI/50 : phi-2*PI+PI/50; // What is this I don't even?
        phi = 1.5*PI;

        vec3 camTrans = {0,0,-3};
        vec3 camPos = {3.0f*cos(phi), 0.0f, -3+3.0f*sin(phi)};
        vec3 upVec = {0,1,0};

        mat4 lookAtMat = lookAtv(camPos,camTrans,upVec);
      	glUniformMatrix4fv(glGetUniformLocation(program, "lookAtMat"), 1, GL_TRUE, lookAtMat.m);

/*
				glBindVertexArray(vertexArrayObjID);	// Select VAO
				glDrawArrays(GL_TRIANGLES, 0, 36);	// draw object
				*/

				glBindVertexArray(bunnyVertexArrayObjID);    // Select VAO
		    glDrawElements(GL_TRIANGLES, m->numIndices, GL_UNSIGNED_INT, 0L);


				printError("display");

				glutSwapBuffers();

			}

			void OnTimer(int value)

			{
				GLfloat t = 0.01*(GLfloat)glutGet(GLUT_ELAPSED_TIME);
				rotateZ[0] = cos(0.05f*t);
				rotateZ[1] = -sin(0.05f*t);
				rotateZ[4] = sin(0.05f*t);
				rotateZ[5] = cos(0.05f*t);


				rotateY[0] = cos(0.05f*t);
				rotateY[2] = -sin(0.05f*t);
				rotateY[8] = sin(0.05f*t);
				rotateY[10] = cos(0.05f*t);

				rotateX[5] = cos(0.05f*t);
				rotateX[6] = -sin(0.05f*t);
				rotateX[9] = sin(0.05f*t);
				rotateX[10] = cos(0.05f*t);

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
				glutCreateWindow ("Bunny example");
				init();
				glutDisplayFunc(display);
				OnTimer(0);
				glutMainLoop();
				return 0;
			}
