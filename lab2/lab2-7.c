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
GLfloat theta = 0;
GLfloat xi = 0;


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

			Model *bunnyModel;
      Model *hareModel;

			GLuint dirtRef;

			void init(void)
			{


				dumpInfo();
				//Load model
				bunnyModel = LoadModelPlus("bunnyplus.obj");
        hareModel = LoadModelPlus("bunnyplus.obj");
        //treeModel = LoadModelPlus("models/LittleNell/Tree/tree.obj");

        // GL inits
        glClearColor(1.0,1.0,1.0,0);
        glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
        glEnable(GL_DEPTH_TEST);
        printError("GL inits");

        // Load and compile shader
        program = loadShaders("lab2-7.vert", "lab2-7.frag");
        printError("init shader");


trans = T(0, 0, -2);


total = Mult(rot, trans);

						}


			void display(void)
			{
				printError("pre display");

				// clear the screen
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



/*
				glUniformMatrix4fv(glGetUniformLocation(program, "translate"), 1, GL_TRUE, translate);
				glUniformMatrix4fv(glGetUniformLocation(program, "scale"), 1, GL_TRUE, scale);
				glUniformMatrix4fv(glGetUniformLocation(program, "rotateZ"), 1, GL_TRUE, rotateZ);
				glUniformMatrix4fv(glGetUniformLocation(program, "rotateY"), 1, GL_TRUE, rotateY);
				glUniformMatrix4fv(glGetUniformLocation(program, "rotateX"), 1, GL_TRUE, rotateX);
				glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total.m);
*/

				GLfloat t = glutGet(GLUT_ELAPSED_TIME) / 100.0;
				glUniform1f(glGetUniformLocation(program, "t"), t);

				glUniform1i(glGetUniformLocation(program, "texUnit"), 0); // Texture unit 0

        phi = (phi < 2*PI) ? phi+PI/100 : phi-2*PI+PI/100; // What is this I don't even?
        theta = (theta < 2*PI) ? theta+PI/168 : theta-2*PI+PI/168;
        xi = (xi < 2*PI) ? xi+PI/79 : xi-2*PI+PI/79;
        //phi = 1.5*PI;

        vec3 camTrans = {0,0,-3};
        //vec3 camPos = {3.0f*cos(phi), 0.0f, -3+3.0f*sin(phi)};
        vec3 camPos = {3.0f*cos(1.5*PI), 0.0f, -3+3.0f*sin(1.5*PI)};
        vec3 upVec = {0.0,1.0,0.0};

        /* mat4 rotX = Rx(phi);
        mat4 rotY = Ry(phi);
        mat4 rotZ = Rz(phi); */

        mat4 rotX = Rx(0);
        mat4 rotY = Ry(phi);
        mat4 rotZ = Rz(0);

        mat4 rotation = Mult(rotY,Mult(rotX,rotZ));
        glUniformMatrix4fv(glGetUniformLocation(program, "rotationMatrix"), 1, GL_TRUE, rotation.m);

        mat4 lookAtMat = lookAtv(camPos,camTrans,upVec);
      	glUniformMatrix4fv(glGetUniformLocation(program, "lookAtMat"), 1, GL_TRUE, lookAtMat.m);


        vec3 translateBunny = {cos(phi),0,sin(phi)};
        mat4 transMatBunny = T(translateBunny.x,translateBunny.y,translateBunny.z);


        vec3 translateHare = {-cos(phi),0,-sin(phi)};
        mat4 transMatHare = T(translateHare.x,translateHare.y,translateHare.z);


        glUniformMatrix4fv(glGetUniformLocation(program, "translationMatrix"), 1, GL_TRUE, transMatBunny.m);
        glUniformMatrix4fv(glGetUniformLocation(program, "projectionMatrix"), 1, GL_TRUE, projectionMatrix);
        DrawModel(bunnyModel, program, "in_Position", "in_Normal", NULL);

        //update in_Position and in_Normal for tree here
        glUniformMatrix4fv(glGetUniformLocation(program, "translationMatrix"), 1, GL_TRUE, transMatHare.m);
        glUniformMatrix4fv(glGetUniformLocation(program, "projectionMatrix"), 1, GL_TRUE, projectionMatrix);
        DrawModel(hareModel, program, "in_Position", "in_Normal", NULL);

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
