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

/*
#define near 1.0

#define far 30.0

#define right 0.5

#define left -0.5

#define top 0.5

#define bottom -0.5
*/
#define near 1.0

#define far 500.0

#define right 0.5

#define left -0.5

#define top 0.5

#define bottom -0.5


#define PI 3.14159265


GLfloat projectionMatrix[] = {    2.0f*near/(right-left), 0.0f, (right+left)/(right-left), 0.0f,

                                            0.0f, 2.0f*near/(top-bottom), (top+bottom)/(top-bottom), 0.0f,

                                            0.0f, 0.0f, -(far + near)/(far - near), -2*far*near/(far - near),

                                            0.0f, 0.0f, -1.0f, 0.0f };


Point3D lightSourcesColorsArr[] = { {1.0f, 0.0f, 0.0f}, // Red light

                                 {0.0f, 1.0f, 0.0f}, // Green light

                                 {0.0f, 0.0f, 1.0f}, // Blue light

                                 {1.0f, 1.0f, 1.0f} }; // White light

GLint isDirectional[] = {0,0,1,1};
GLfloat specularExponent[] = {100.0, 200.0, 60.0, 50.0, 300.0, 150.0};

Point3D lightSourcesDirectionsPositions[] = { {10.0f, 5.0f, 0.0f}, // Red light, positional

                                       {0.0f, 5.0f, 10.0f}, // Green light, positional

                                       {-1.0f, 0.0f, 0.0f}, // Blue light along X

                                       {0.0f, 0.0f, -1.0f} }; // White light along Z


mat4 rot, trans, total;
vec3 lookAtPoint = {0,0,-30};
static const float speed = 1;


GLfloat phi = 0;
GLfloat phiInner = 0;
GLfloat theta = 0;
GLfloat xi = 0;

vec3 camPos = {3.0f, 0.0f, -3+3.0f};
vec3 upVec = {0.0,1.0,0.0};



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
      GLuint skyShader;
      GLuint groundShader;

	     Model *wallModel;
      Model *roofModel;
      Model *balconyModel;
      Model *bladeModel;
      Model *skyBox;
      Model *ground;
      Model *bunnyModel;

			GLuint skyTexture;
      GLuint groundTexture;
      GLuint dirtTexture;
      GLuint rutorTexture;

      mat4 groundScaleMat;
      mat4 tmpGroundScaler;

			void init(void)
			{


				dumpInfo();



				//Load models
				wallModel = LoadModelPlus("windmill/windmill-walls.obj");
        balconyModel = LoadModelPlus("windmill/windmill-balcony.obj");
        roofModel = LoadModelPlus("windmill/windmill-roof.obj");
        bladeModel = LoadModelPlus("windmill/blade.obj");
        skyBox = LoadModelPlus("skybox.obj");
        ground = LoadModelPlus("ground.obj");
        bunnyModel = LoadModelPlus("bunny.obj");


        // GL inits
        glClearColor(1.0,1.0,1.0,0);
        glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
        glEnable(GL_DEPTH_TEST);
        printError("GL inits");

        // Load and compile shader
        program = loadShaders("lab3-5.vert", "lab3-5.frag");
        skyShader = loadShaders("sky3-5.vert", "sky3-5.frag");
        groundShader = loadShaders("ground3-5.vert", "ground3-5.frag");
        printError("init shader");

        // Skybox texture init
        glUseProgram(skyShader);
        glActiveTexture(GL_TEXTURE0);
        LoadTGATextureSimple("SkyBox512.tga", &skyTexture);

        // Ground texture inits
        glUseProgram(groundShader);
        glActiveTexture(GL_TEXTURE1);
        LoadTGATextureSimple("grass.tga", &groundTexture);
        int groundScaler = 10;
        tmpGroundScaler = S(groundScaler, 0.0001,groundScaler);
        groundScaleMat = Mult(T(0, -7, 0), tmpGroundScaler);

        // Multitex magic init
        glUseProgram(program);
        glActiveTexture(GL_TEXTURE2);
        LoadTGATextureSimple("SkyBox512.tga", &dirtTexture);
        glActiveTexture(GL_TEXTURE3);
        LoadTGATextureSimple("maskros512.tga", &rutorTexture);


trans = T(0, 0, -2);


total = Mult(rot, trans);

						}


void handleKeyEvents(vec3* cameraLocation, vec3* lookAtPoint, vec3* upVector, const float* movement_speed)
{
  // This is the direction the camera is looking
  vec3 direction;
  direction = VectorSub(*cameraLocation, *lookAtPoint);
  direction = Normalize(direction);
  direction = ScalarMult(direction,*movement_speed);

  if ( glutKeyIsDown('s') ) {
      *cameraLocation = VectorAdd(*cameraLocation, direction);
      *lookAtPoint = VectorAdd(*lookAtPoint, direction);

  }
  if ( glutKeyIsDown('w') ) {
      *cameraLocation = VectorSub(*cameraLocation, direction);
      *lookAtPoint = VectorSub(*lookAtPoint, direction);
  }
  if (glutKeyIsDown('d')) {
      *lookAtPoint = VectorSub(*lookAtPoint,*cameraLocation);
      *lookAtPoint = MultVec3(Ry(-PI/50),*lookAtPoint);
      *lookAtPoint = VectorAdd(*lookAtPoint,*cameraLocation);

  }
  if ( glutKeyIsDown('a') ) {
      *lookAtPoint = VectorSub(*lookAtPoint,*cameraLocation);
      *lookAtPoint = MultVec3(Ry(PI/50),*lookAtPoint);
      *lookAtPoint = VectorAdd(*lookAtPoint,*cameraLocation);

  }
  if (glutKeyIsDown('o')) {
      *lookAtPoint = VectorSub(*lookAtPoint,*cameraLocation);
      *lookAtPoint = MultVec3(Rx(-PI/50),*lookAtPoint);
      *lookAtPoint = VectorAdd(*lookAtPoint,*cameraLocation);

      //*upVector = VectorSub(*upVector,*cameraLocation);
      //*upVector = MultVec3(Rx(-PI/50),*upVector);
      //*upVector = VectorAdd(*upVector,*cameraLocation);
  }
  if ( glutKeyIsDown('l') ) {
      *lookAtPoint = VectorSub(*lookAtPoint,*cameraLocation);
      *lookAtPoint = MultVec3(Rx(PI/50),*lookAtPoint);
      *lookAtPoint = VectorAdd(*lookAtPoint,*cameraLocation);

      //*upVector = VectorSub(*upVector,*cameraLocation);
      //*upVector = MultVec3(Rx(PI/50),*upVector);
      //*upVector = VectorAdd(*upVector,*cameraLocation);
  }

  vec3 newDirection = VectorSub(*cameraLocation, *lookAtPoint);
  newDirection = Normalize(direction);
  vec3 rightDirection = MultVec3(Ry(PI/2),newDirection);

  *upVector = Normalize(CrossProduct(newDirection,rightDirection));
}

			void display(void)
			{
				printError("pre display");

				// clear the screen
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        phi = (phi < 2*PI) ? phi+PI/100 : phi-2*PI+PI/100;
        phiInner = (phiInner < 2*PI) ? phiInner+PI/500 : phiInner-2*PI+PI/500; // What is this I don't even?
        theta = (theta < 2*PI) ? theta+PI/168 : theta-2*PI+PI/168;
        xi = (xi < 2*PI) ? xi+PI/79 : xi-2*PI+PI/79;



        handleKeyEvents(&camPos, &lookAtPoint, &upVec, &speed);
        //handleKeyEvents(&camPos, &lookAtPoint, &upVec);

        glUseProgram(program);
        glUniformMatrix4fv(glGetUniformLocation(program, "projectionMatrix"), 1, GL_TRUE, projectionMatrix);
        printError("projectionMatrix");

        mat4 rotX = Rx(phi);
        mat4 rotY = Ry(0);
        mat4 rotZ = Rz(0);

        //mat4 rotationMill = Ry(phi);
        mat4 rotationMill = Ry(0);
        mat4 rotationBladeStandard;


        mat4 lookAtMat = lookAtv(camPos,lookAtPoint,upVec);
      	glUniformMatrix4fv(glGetUniformLocation(program, "lookAtMat"), 1, GL_TRUE, lookAtMat.m);
        printError("lookAtMatrix");
        mat4 transformationMatrix;

        vec3 translateWall = {0, -7, 15};
        mat4 transMatWall = T(translateWall.x,translateWall.y,translateWall.z);

        mat4 bunnyScale = S(4,4,4);
        vec3 translateBunny = {translateWall.x + 18,translateWall.y +2, translateWall.z + 5};
        mat4 transMatBunny = T(translateBunny.x,translateBunny.y,translateBunny.z);
        mat4 rotationBunnyIn = Ry(3*PI/4);
        mat4 rotationBunnyOut = Ry(phi);

        //*****************Draw skybox******************
        glUseProgram(skyShader);
        glDisable(GL_DEPTH_TEST);
        printError("Disable Z");
        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glBindTexture(GL_TEXTURE_2D,skyTexture);
        printError("Sky bind texture");
        glUniform1i(glGetUniformLocation(skyShader, "textUnit"), 0); // Texture unit 0
        printError("pre-skybox");
        transformationMatrix = rotationMill;
        printError("SKY: new transform");
        glUniformMatrix4fv(glGetUniformLocation(skyShader, "transformationMatrix"), 1, GL_TRUE, transformationMatrix.m);
        printError("SKY: Load transform");
        mat3 skyLookAtMat3 = mat4tomat3(lookAtMat);
        mat4 skyLookAtMat4 = mat3tomat4(skyLookAtMat3);
        glUniformMatrix4fv(glGetUniformLocation(skyShader, "lookAtMat"), 1, GL_TRUE, skyLookAtMat4.m);
        glUniformMatrix4fv(glGetUniformLocation(skyShader, "projectionMatrix"), 1, GL_TRUE, projectionMatrix);
        printError("SKY: projection");

        //glUniformMatrix4fv(glGetUniformLocation(skyShader, "lookAtMat"), 1, GL_TRUE, lookAtMat.m);
        //printError("Load lookAtMat sky");
        DrawModel(skyBox, skyShader, "in_Position", NULL , "inTexCoord");
        printError("Skybox draw");
        glEnable(GL_DEPTH_TEST);
        printError("Enable z");
        // **************************End skybox*******************

        // **************************Draw ground*******************
        glUseProgram(groundShader);
        glUniformMatrix4fv(glGetUniformLocation(groundShader, "lookAtMat"), 1, GL_TRUE, lookAtMat.m);
        printError("GROUND: lookAtMat");
        glUniformMatrix4fv(glGetUniformLocation(groundShader, "projectionMatrix"), 1, GL_TRUE, projectionMatrix);
        printError("GROUND: projectionMatrix");

        groundScaleMat = Mult(T(0, -7, 0), Mult(rotationMill,tmpGroundScaler));
        glUniformMatrix4fv(glGetUniformLocation(groundShader, "transformationMatrix"), 1, GL_TRUE, groundScaleMat.m);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glBindTexture(GL_TEXTURE_2D, groundTexture);
        glUniform1i(glGetUniformLocation(groundShader, "textUnit"), 1);
        DrawModel(ground, groundShader, "in_Position", "in_Normal" , "inTexCoord");


        glUseProgram(program);

        //*******************Light***************************

        glUniform3fv(glGetUniformLocation(program, "lightSourcesDirPosArr"), 4, &lightSourcesDirectionsPositions[0].x);

        glUniform3fv(glGetUniformLocation(program, "lightSourcesColorArr"), 4, &lightSourcesColorsArr[0].x);

        glUniform1f(glGetUniformLocation(program, "specularExponent"), specularExponent[0]);

        glUniform1iv(glGetUniformLocation(program, "isDirectional"), 4, isDirectional);

        //*******************End light***************************


        // *******************Draw objects*****************

        glUseProgram(program);
        glBindTexture(GL_TEXTURE_2D, dirtTexture);
        glUniform1i(glGetUniformLocation(program, "dirtTex"), 2);
        glBindTexture(GL_TEXTURE_2D, rutorTexture);
        glUniform1i(glGetUniformLocation(program, "rutorTex"), 3);

        //Draw wall
        transformationMatrix = Mult(transMatWall, rotationMill);
        glUniformMatrix4fv(glGetUniformLocation(program, "transformationMatrix"), 1, GL_TRUE, transformationMatrix.m);
        glUniformMatrix4fv(glGetUniformLocation(program, "projectionMatrix"), 1, GL_TRUE, projectionMatrix);
        DrawModel(wallModel, program, "in_Position", "in_Normal", "inTexCoord");
        printError("Wall");
        //Draw Balcony
        transformationMatrix = Mult(transMatWall, rotationMill);
        glUniformMatrix4fv(glGetUniformLocation(program, "transformationMatrix"), 1, GL_TRUE, transformationMatrix.m);
        DrawModel(balconyModel, program, "in_Position", "in_Normal", "inTexCoord");
        printError("Balcony");

        //Draw roof
        transformationMatrix = Mult(transMatWall, rotationMill);
        glUniformMatrix4fv(glGetUniformLocation(program, "transformationMatrix"), 1, GL_TRUE, transformationMatrix.m);
        DrawModel(roofModel, program, "in_Position", "in_Normal", "inTexCoord");
        printError("Roof");

        //Draw bunny
        transformationMatrix = Mult(rotationBunnyOut, Mult(transMatBunny, Mult(rotationBunnyIn, bunnyScale)));
        glUniformMatrix4fv(glGetUniformLocation(program, "transformationMatrix"), 1, GL_TRUE, transformationMatrix.m);
        DrawModel(bunnyModel, program, "in_Position", "in_Normal", "inTexCoord");
        printError("bunny");

        //vec3 translateBlade = {sin(phi), 0, cos(phi)};
        //mat4 transMatBlade = T(translateBlade.x, translateBlade.y, translateBlade.z);



        vec3 moveOutvec = {4.5, 9, 0};
        mat4 moveOut = T(moveOutvec.x, moveOutvec.y, moveOutvec.z);

        printError("Bladeloop");
        size_t blade;
        for (blade = 0; blade < 4; blade++)
        {
          rotX = Rx(phi + (PI/2)*blade);
          rotationBladeStandard = Mult(rotY,Mult(rotX,rotZ));

          mat4 First = Mult(moveOut, rotationBladeStandard);
          mat4 Second = Mult(rotationMill, First);
          transformationMatrix = Mult(transMatWall, Second);
          glUniformMatrix4fv(glGetUniformLocation(program, "transformationMatrix"), 1, GL_TRUE, transformationMatrix.m);
          DrawModel(bladeModel, program, "in_Position", "in_Normal", "inTexCoord");
        }


				printError("display");

				glutSwapBuffers();

			}

			void OnTimer(int value)

			{
				glutPostRedisplay();

				glutTimerFunc(20, &OnTimer, (value));
				/* Dat 50 fps */
			}

			int main(int argc, char *argv[])
			{
				glutInit(&argc, argv);
				glutInitContextVersion(3, 2);
				glutCreateWindow ("Windmill and skybox");
				init();
				glutDisplayFunc(display);
				OnTimer(0);
				glutMainLoop();
				return 0;
			}
