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
vec3 lookAtPoint = {0,0,-30};


GLfloat phi = 0;
GLfloat theta = 0;
GLfloat xi = 0;



			// vertex array object
			unsigned int vertexArrayObjID;

			unsigned int WallVertexArrayObjID;
			unsigned int WallVertexBufferObjID;
			unsigned int WallIndexBufferObjID;
			unsigned int WallNormalBufferObjID;
			unsigned int WallTexCoordBufferObjID;
			// vertex buffer object, used for uploading the geometry
			unsigned int vertexBufferObjID;
			// color buffer
			unsigned int colorBufferObjID;

			// Reference to shader program
			GLuint program;

			Model *wallModel;
      Model *roofModel;
      Model *balconyModel;
      Model *bladeModel1;
      Model *bladeModel2;
      Model *bladeModel3;
      Model *bladeModel4;

			GLuint dirtRef;

			void init(void)
			{


				dumpInfo();
				//Load model
				wallModel = LoadModelPlus("windmill/windmill-walls.obj");
        balconyModel = LoadModelPlus("windmill/windmill-balcony.obj");
        roofModel = LoadModelPlus("windmill/windmill-roof.obj");
        bladeModel1 = LoadModelPlus("windmill/blade.obj");
        bladeModel2 = LoadModelPlus("windmill/blade.obj");
        bladeModel3 = LoadModelPlus("windmill/blade.obj");
        bladeModel4 = LoadModelPlus("windmill/blade.obj");
        //treeModel = LoadModelPlus("models/LittleNell/Tree/tree.obj");

        // GL inits
        glClearColor(1.0,1.0,1.0,0);
        glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
        glEnable(GL_DEPTH_TEST);
        printError("GL inits");

        // Load and compile shader
        program = loadShaders("lab3-1.vert", "lab3-1.frag");
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

        vec3 camTrans = {0,0,0};
        //vec3 camPos = {3.0f*cos(phi), 0.0f, -3+3.0f*sin(phi)};
        vec3 camPos = {3.0f*cos(1.5*PI), 0.0f, -3+3.0f*sin(1.5*PI)};
        vec3 upVec = {0.0,1.0,0.0};


        /* mat4 rotX = Rx(phi);
        mat4 rotY = Ry(phi);
        mat4 rotZ = Rz(phi); */

        glUniformMatrix4fv(glGetUniformLocation(program, "projectionMatrix"), 1, GL_TRUE, projectionMatrix);

        mat4 rotX = Rx(phi);
        mat4 rotY = Ry(0);
        mat4 rotZ = Rz(0);

        mat4 rotationMill = Ry(phi);
        //mat4 rotationBladeStandard = Mult(rotY,Mult(rotX,rotZ));
        mat4 rotationBladeStandard;

        //glUniformMatrix4fv(glGetUniformLocation(program, "rotationMatrix"), 1, GL_TRUE, rotation.m);

        mat4 lookAtMat = lookAtv(camPos,camTrans,upVec);
      	glUniformMatrix4fv(glGetUniformLocation(program, "lookAtMat"), 1, GL_TRUE, lookAtMat.m);
        mat4 transformationMatrix;

        vec3 translateWall = {0, -7, 15};
        mat4 transMatWall = T(translateWall.x,translateWall.y,translateWall.z);

        //Draw wall
        transformationMatrix = Mult(transMatWall, rotationMill);
        glUniformMatrix4fv(glGetUniformLocation(program, "transformationMatrix"), 1, GL_TRUE, transformationMatrix.m);
        glUniformMatrix4fv(glGetUniformLocation(program, "projectionMatrix"), 1, GL_TRUE, projectionMatrix);
        DrawModel(wallModel, program, "in_Position", "in_Normal", NULL);

        //Draw Balcony
        transformationMatrix = Mult(transMatWall, rotationMill);
        glUniformMatrix4fv(glGetUniformLocation(program, "transformationMatrix"), 1, GL_TRUE, transformationMatrix.m);
        DrawModel(balconyModel, program, "in_Position", "in_Normal", NULL);

        //Draw roof
        transformationMatrix = Mult(transMatWall, rotationMill);
        glUniformMatrix4fv(glGetUniformLocation(program, "transformationMatrix"), 1, GL_TRUE, transformationMatrix.m);
        DrawModel(roofModel, program, "in_Position", "in_Normal", NULL);

        vec3 translateBlade = {sin(phi), 0, cos(phi)};
        mat4 transMatBlade = T(translateBlade.x, translateBlade.y, translateBlade.z);



        vec3 moveOutvec = {4.5, 9, 0};
        mat4 moveOut = T(moveOutvec.x, moveOutvec.y, moveOutvec.z);

        size_t blade;

        for (blade = 0; blade < 4; blade++)
        {
          rotX = Rx(phi + (PI/2)*blade);
          rotationBladeStandard = Mult(rotY,Mult(rotX,rotZ));

          mat4 First = Mult(moveOut, rotationBladeStandard);
          mat4 Second = Mult(rotationMill, First);
          transformationMatrix = Mult(transMatWall, Second);
          glUniformMatrix4fv(glGetUniformLocation(program, "transformationMatrix"), 1, GL_TRUE, transformationMatrix.m);
          DrawModel(bladeModel1, program, "in_Position", "in_Normal", NULL);
        }

        //transformationMatrix = Mult(rotationMill, Mult(Rz(phi + i*(PI/2)), Mult(transMatBlade, rotationBladeStandard)));



        //glUniformMatrix4fv(glGetUniformLocation(program, "translationMatrix"), 1, GL_TRUE, transMatWall.m);

				printError("display");

				glutSwapBuffers();

			}

			void OnTimer(int value)

			{
				//GLfloat t = 0.01*(GLfloat)glutGet(GLUT_ELAPSED_TIME);
				glutPostRedisplay();

				glutTimerFunc(20, &OnTimer, (value));
				/* Dat 50 fps */
			}

			int main(int argc, char *argv[])
			{
				glutInit(&argc, argv);
				glutInitContextVersion(3, 2);
				glutCreateWindow ("Wall example");
				init();
				glutDisplayFunc(display);
				OnTimer(0);
				glutMainLoop();
				return 0;
			}
