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
      GLuint groundtexture;

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
        //ground = LoadModelPlus("skybox.obj");
        ground = LoadModelPlus("ground.obj");
        bunnyModel = LoadModelPlus("bunny.obj");


        // GL inits
        glClearColor(1.0,1.0,1.0,0);
        glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
        glEnable(GL_DEPTH_TEST);
        printError("GL inits");

        // Load and compile shader
        program = loadShaders("lab3-3.vert", "lab3-3.frag");
        skyShader = loadShaders("sky3-3.vert", "sky3-3.frag");
        groundShader = loadShaders("ground3-3.vert", "ground3-3.frag");
        printError("init shader");

        // Skybox texture init
        glUseProgram(skyShader);
        glActiveTexture(GL_TEXTURE0);
        LoadTGATextureSimple("SkyBox512.tga", &skyTexture);

        // Ground texture inits
        glUseProgram(groundShader);
        glActiveTexture(GL_TEXTURE0);
        LoadTGATextureSimple("grass.tga", &groundtexture);
        int groundScaler = 10;
        tmpGroundScaler = S(groundScaler, 0.0001,groundScaler);
        groundScaleMat = Mult(T(0, -7, 0), tmpGroundScaler);



trans = T(0, 0, -2);


total = Mult(rot, trans);

						}

/*
      void handleKeyEvents(vec3* camPos, vec3* lookAtPoint, vec3* upVec)
      {
        vec3 translation;
        vec3 yVector = {0,1,0};
        vec3 direction = Normalize(VectorSub(*camPos, *lookAtPoint));
        vec3 projectedDirection = {direction.x, 0, direction.z};
        projectedDirection = Normalize(projectedDirection);

        if (glutKeyIsDown('w')) {
          translation = ScalarMult(projectedDirection, -speed);
          *camPos =  VectorAdd(*camPos, ScalarMult(projectedDirection, -speed));
          *lookAtPoint = VectorAdd(*lookAtPoint, translation);
        }

        if (glutKeyIsDown('a')) {
          translation = ScalarMult(CrossProduct(projectedDirection, yVector),-speed);
          *camPos =  VectorAdd(*camPos, ScalarMult(projectedDirection, -speed));
          *lookAtPoint = VectorAdd(*lookAtPoint, translation);
        }

        if (glutKeyIsDown('s')) {
          translation = ScalarMult(projectedDirection,speed);
          *camPos =  VectorAdd(*camPos, ScalarMult(projectedDirection, speed));
          *lookAtPoint = VectorAdd(*lookAtPoint, translation);
        }

        if (glutKeyIsDown('d')) {
          translation = ScalarMult(CrossProduct(projectedDirection, yVector),speed);
          *camPos =  VectorAdd(*camPos, ScalarMult(projectedDirection, speed));
          *lookAtPoint = VectorAdd(*lookAtPoint, translation);
        }
      }
*/


/*
void handleKeyEvents(vec3* cameraLocation, vec3* lookAtPoint, vec3* upVector, const float* movement_speed)
{
  // This is the direction the camera is looking
  vec3 translator;
  vec3 y_vector = {0, 1, 0};
  vec3 direction = Normalize(VectorSub(*cameraLocation, *lookAtPoint));
  vec3 temp = {direction.x, 0, direction.z};
  vec3 projected_direction = Normalize(temp);

  if ( glutKeyIsDown('w') ) {
    translator = ScalarMult(projected_direction,*movement_speed);
    *cameraLocation = VectorAdd(*cameraLocation, ScalarMult(projected_direction, *movement_speed));
    // *lookAtPoint = VectorAdd(*lookAtPoint, translator);
    *lookAtPoint = VectorAdd(*lookAtPoint, ScalarMult(projected_direction, *movement_speed));
  }
  if (glutKeyIsDown('d')) {
    translator = ScalarMult(Normalize(CrossProduct(projected_direction, y_vector)), *movement_speed);
    *cameraLocation = VectorAdd(*cameraLocation, ScalarMult(projected_direction, *movement_speed));
    // *cameraLocation = VectorAdd(*cameraLocation, translator);
    // *lookAtPoint = VectorAdd(*lookAtPoint, translator);
    *lookAtPoint = VectorAdd(*lookAtPoint, ScalarMult(projected_direction, *movement_speed));
  }
  if ( glutKeyIsDown('a') ) {
    translator = ScalarMult(Normalize(CrossProduct(projected_direction, y_vector)), -*movement_speed);
    *cameraLocation = VectorAdd(*cameraLocation, ScalarMult(projected_direction, -*movement_speed));
    // *cameraLocation = VectorAdd(*cameraLocation, translator);
    // *lookAtPoint = VectorAdd(*lookAtPoint, translator);
    *lookAtPoint = VectorAdd(*lookAtPoint, ScalarMult(projected_direction, -*movement_speed));
  }
  if ( glutKeyIsDown('s') ) {
    translator = ScalarMult(projected_direction,-*movement_speed);
    *cameraLocation = VectorAdd(*cameraLocation, ScalarMult(projected_direction, -*movement_speed));
    // *cameraLocation = VectorAdd(*cameraLocation, translator);
    // *lookAtPoint = VectorAdd(*lookAtPoint, translator);
    *lookAtPoint = VectorAdd(*lookAtPoint, ScalarMult(projected_direction, -*movement_speed));
  }
}
*/

void handleKeyEvents(vec3* cameraLocation, vec3* lookAtPoint, vec3* upVector, const float* movement_speed)
{
  // This is the direction the camera is looking
  vec3 direction;
  direction = VectorSub(*cameraLocation, *lookAtPoint);
  direction = Normalize(direction);
  direction = ScalarMult(direction,*movement_speed);

  if ( glutKeyIsDown('w') ) {
      *cameraLocation = VectorAdd(*cameraLocation, direction);
      *lookAtPoint = VectorAdd(*lookAtPoint, direction);

  }
  if (glutKeyIsDown('d')) {
      *lookAtPoint = VectorSub(*lookAtPoint,*cameraLocation);
      *lookAtPoint = MultVec3(Ry(-PI/50),*lookAtPoint);
      *lookAtPoint = VectorAdd(*lookAtPoint,*cameraLocation);
      *cameraLocation = MultVec3(Ry(-PI/50),*cameraLocation);
  }
  if ( glutKeyIsDown('a') ) {
      *lookAtPoint = VectorSub(*lookAtPoint,*cameraLocation);
      *lookAtPoint = MultVec3(Ry(PI/50),*lookAtPoint);
      *lookAtPoint = VectorAdd(*lookAtPoint,*cameraLocation);
      *cameraLocation = MultVec3(Ry(PI/50),*cameraLocation);
  }
  if ( glutKeyIsDown('s') ) {
      *cameraLocation = VectorSub(*cameraLocation, direction);
      *lookAtPoint = VectorSub(*lookAtPoint, direction);
  }
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

        vec3 camTrans = {0,0,0};


        handleKeyEvents(&camPos, &lookAtPoint, &upVec, &speed);
        //handleKeyEvents(&camPos, &lookAtPoint, &upVec);

        glUseProgram(program);
        glUniformMatrix4fv(glGetUniformLocation(program, "projectionMatrix"), 1, GL_TRUE, projectionMatrix);

        mat4 rotX = Rx(phi);
        mat4 rotY = Ry(0);
        mat4 rotZ = Rz(0);

        //mat4 rotationMill = Ry(phi);
        mat4 rotationMill = Ry(0);
        mat4 rotationBladeStandard;


        mat4 lookAtMat = lookAtv(camPos,camTrans,upVec);
      	glUniformMatrix4fv(glGetUniformLocation(program, "lookAtMat"), 1, GL_TRUE, lookAtMat.m);
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
        printError("new transform");
        glUniformMatrix4fv(glGetUniformLocation(skyShader, "transformationMatrix"), 1, GL_TRUE, transformationMatrix.m);
        printError("Load transform");
        mat3 skyLookAtMat3 = mat4tomat3(lookAtMat);
        mat4 skyLookAtMat4 = mat3tomat4(skyLookAtMat3);
        glUniformMatrix4fv(glGetUniformLocation(skyShader, "lookAtMat"), 1, GL_TRUE, skyLookAtMat4.m);
        glUniformMatrix4fv(glGetUniformLocation(skyShader, "projectionMatrix"), 1, GL_TRUE, projectionMatrix);
        printError("Load projection");

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
        glUniformMatrix4fv(glGetUniformLocation(groundShader, "projectionMatrix"), 1, GL_TRUE, projectionMatrix);

        groundScaleMat = Mult(T(0, -7, 0), Mult(rotationMill,tmpGroundScaler));
        glUniformMatrix4fv(glGetUniformLocation(groundShader, "transformationMatrix"), 1, GL_TRUE, groundScaleMat.m);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glBindTexture(GL_TEXTURE_2D, groundtexture);
        glUniform1i(glGetUniformLocation(groundShader, "textUnit"), 0);
        DrawModel(ground, groundShader, "in_Position", "in_Normal" , "inTexCoord");


        // *******************Draw windmill*****************
        glUseProgram(program);

        //Draw wall
        transformationMatrix = Mult(transMatWall, rotationMill);
        glUniformMatrix4fv(glGetUniformLocation(program, "transformationMatrix"), 1, GL_TRUE, transformationMatrix.m);
        glUniformMatrix4fv(glGetUniformLocation(program, "projectionMatrix"), 1, GL_TRUE, projectionMatrix);
        DrawModel(wallModel, program, "in_Position", "in_Normal", NULL);
        printError("Wall");
        //Draw Balcony
        transformationMatrix = Mult(transMatWall, rotationMill);
        glUniformMatrix4fv(glGetUniformLocation(program, "transformationMatrix"), 1, GL_TRUE, transformationMatrix.m);
        DrawModel(balconyModel, program, "in_Position", "in_Normal", NULL);
        printError("Balcony");

        //Draw roof
        transformationMatrix = Mult(transMatWall, rotationMill);
        glUniformMatrix4fv(glGetUniformLocation(program, "transformationMatrix"), 1, GL_TRUE, transformationMatrix.m);
        DrawModel(roofModel, program, "in_Position", "in_Normal", NULL);
        printError("Roof");

        //Draw bunny
        transformationMatrix = Mult(rotationBunnyOut, Mult(transMatBunny, Mult(rotationBunnyIn, bunnyScale)));
        glUniformMatrix4fv(glGetUniformLocation(program, "transformationMatrix"), 1, GL_TRUE, transformationMatrix.m);
        DrawModel(bunnyModel, program, "in_Position", "in_Normal", NULL);
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
          DrawModel(bladeModel, program, "in_Position", "in_Normal", NULL);
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
