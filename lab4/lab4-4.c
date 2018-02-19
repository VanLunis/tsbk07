// Lab 4, terrain generation

#ifdef __APPLE__
	#include <OpenGL/gl3.h>
	// Linking hint for Lightweight IDE
	// uses framework Cocoa
#endif
#include "MicroGlut.h"
#include "GL_utilities.h"
#include "VectorUtils3.h"
#include "loadobj.h"
#include "LoadTGA.h"


mat4 projectionMatrix;

//vec3 camPos = {3.0f, 0.0f, -3+3.0f};
vec3 upVec = {0.0,1.0,0.0};
//vec3 lookAtPoint = {0,0,-30};

static const float speed = 0.5;
static const float PI = 3.14159265;

GLfloat timeCounter= 0;
GLfloat startX = 0;
GLfloat startZ = 0;

vec3 cam = {0, 5, 8};
vec3 lookAtPoint = {2, 0, 2};
mat4 modelView, total, camMatrix, sphereTranslate;

vec3 getNormal(Model* model, int x, int z, int texWidth)
{
		return (vec3){model->normalArray[(x + z * texWidth)*3 + 0], model->normalArray[(x + z * texWidth)*3 + 1], model->normalArray[(x + z * texWidth)*3 + 2]};
}

vec3 getVertex(Model* model, int x, int z, int texWidth)
{
	return (vec3){model->vertexArray[(x + z * texWidth)*3 + 0], model->vertexArray[(x + z * texWidth)*3 + 1], model->vertexArray[(x + z * texWidth)*3 + 2]};
}

float getHeight(Model* model, float x, float z, int texWidth)
{
	int intX = (int)x;
	int intZ = (int)z;
	float deltaX = x - (float)intX;
	float deltaZ = z - (float)intZ;

	//get 4 vertices = 1 quad
	vec3 normal[4];
	vec3 vertice[4];
	normal[0] = getNormal(model, intX, intZ, texWidth);
	vertice[0] = getVertex(model, intX, intZ, texWidth);
	normal[1] = getNormal(model, intX, intZ, texWidth);
	vertice[1] = getVertex(model, intX, intZ, texWidth);
	normal[2] = getNormal(model, intX, intZ, texWidth);
	vertice[2] = getVertex(model, intX, intZ, texWidth);
	normal[3] = getNormal(model, intX, intZ, texWidth);
	vertice[3] = getVertex(model, intX, intZ, texWidth);

	vec3 normalVec = ScalarMult(normal[0], sqrt(pow(1-deltaX,2) + pow(1-deltaZ,2)));
	normalVec = VectorAdd(normalVec, ScalarMult(normal[1], sqrt(pow(1-deltaX,2) + pow(deltaZ,2))));
	normalVec = VectorAdd(normalVec, ScalarMult(normal[2], sqrt(pow(deltaX,2) + pow(1-deltaZ,2))));
	normalVec = VectorAdd(normalVec, ScalarMult(normal[3], sqrt(pow(deltaX,2) + pow(deltaZ,2))));
	normalVec = Normalize(normalVec);

	vec3 posVertice = VectorAdd(vertice[0], vertice[1]);
	posVertice = VectorAdd(posVertice, vertice[2]);
	posVertice = VectorAdd(posVertice, vertice[3]);
	posVertice = ScalarMult(posVertice, 0.25);

	float planeConst = DotProduct(normalVec,posVertice); //Plane eq: Ax+By+Cz=planeConst

	return (planeConst - normalVec.x*x - normalVec.z*z)/normalVec.y; //y = (planeConst - Ax - Cz)/B
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



Model* GenerateTerrain(TextureData *tex)
{
	int vertexCount = tex->width * tex->height;
	int triangleCount = (tex->width-1) * (tex->height-1) * 2;
	int x, z;

	GLfloat *vertexArray = malloc(sizeof(GLfloat) * 3 * vertexCount);
	GLfloat *normalArray = malloc(sizeof(GLfloat) * 3 * vertexCount);
	GLfloat *texCoordArray = malloc(sizeof(GLfloat) * 2 * vertexCount);
	GLuint *indexArray = malloc(sizeof(GLuint) * triangleCount*3);

	printf("bpp %d\n", tex->bpp);
	for (x = 0; x < tex->width; x++)
		for (z = 0; z < tex->height; z++)
		{
// Vertex array. You need to scale this properly
			vertexArray[(x + z * tex->width)*3 + 0] = x / 1.0;
			vertexArray[(x + z * tex->width)*3 + 1] = tex->imageData[(x + z * tex->width) * (tex->bpp/8)] / 100.0;
			vertexArray[(x + z * tex->width)*3 + 2] = z / 1.0;
//Calculating normal vectors
			vec3 vertex1 = {vertexArray[(x + z * tex->width)*3 + 0], vertexArray[(x + z * tex->width)*3 + 1], vertexArray[(x + z * tex->width)*3 + 2]};
			vec3 vertex2;
			vec3 vertex3;

			vertex2 = (x-1 > 0) ? (vec3){vertexArray[((x-1) + z * tex->width)*3 + 0], vertexArray[((x-1) + z * tex->width)*3 + 1], vertexArray[((x-1) + z * tex->width)*3 + 2]} : (vec3){0,1,0};
    	vertex3 = (z-1 > 0) ? (vec3){vertexArray[(x + (z-1) * tex->width)*3 + 0], vertexArray[(x + (z-1) * tex->width)*3 + 1], vertexArray[(x + (z-1) * tex->width)*3 + 2]} : (vec3){0,1,0};

			vec3 firstSide = VectorSub(vertex3, vertex2);
			vec3 secondSide = VectorSub(vertex2, vertex1);
			vec3 normal = Normalize(CrossProduct(firstSide, secondSide));

			if (normal.y < 0)
			{
				normal = ScalarMult(normal, -1);
			}

// Normal vectors. You need to calculate these.
			normalArray[(x + z * tex->width)*3 + 0] = normal.x;
			normalArray[(x + z * tex->width)*3 + 1] = normal.y;
			normalArray[(x + z * tex->width)*3 + 2] = normal.z;
// Texture coordinates. You may want to scale them.
			texCoordArray[(x + z * tex->width)*2 + 0] = x; // (float)x / tex->width;
			texCoordArray[(x + z * tex->width)*2 + 1] = z; // (float)z / tex->height;
		}
	for (x = 0; x < tex->width-1; x++)
		for (z = 0; z < tex->height-1; z++)
		{
		// Triangle 1
			indexArray[(x + z * (tex->width-1))*6 + 0] = x + z * tex->width;
			indexArray[(x + z * (tex->width-1))*6 + 1] = x + (z+1) * tex->width;
			indexArray[(x + z * (tex->width-1))*6 + 2] = x+1 + z * tex->width;
		// Triangle 2
			indexArray[(x + z * (tex->width-1))*6 + 3] = x+1 + z * tex->width;
			indexArray[(x + z * (tex->width-1))*6 + 4] = x + (z+1) * tex->width;
			indexArray[(x + z * (tex->width-1))*6 + 5] = x+1 + (z+1) * tex->width;
		}

	// End of terrain generation

	// Create Model and upload to GPU:

	Model* model = LoadDataToModel(
			vertexArray,
			normalArray,
			texCoordArray,
			NULL,
			indexArray,
			vertexCount,
			triangleCount*3);

	return model;
}


// vertex array object
Model *m, *m2, *tm, *sphere;
// Reference to shader program
GLuint program;
GLuint tex1, tex2;
GLuint sphereShader;
TextureData ttex; // terrain

void init(void)
{
	// GL inits
	glClearColor(0.2,0.2,0.5,0);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	printError("GL inits");

	projectionMatrix = frustum(-0.1, 0.1, -0.1, 0.1, 0.2, 50.0);

	// Load and compile shader
	program = loadShaders("terrain4-4.vert", "terrain4-4.frag");
	sphereShader = loadShaders("sphere4-4.vert", "sphere4-4.frag");
	glUseProgram(program);
	printError("init shader");

	glUniformMatrix4fv(glGetUniformLocation(program, "projMatrix"), 1, GL_TRUE, projectionMatrix.m);
	glUniform1i(glGetUniformLocation(program, "tex"), 0); // Texture unit 0
	LoadTGATextureSimple("maskros512.tga", &tex1);

// Load terrain data

	LoadTGATextureData("fft-terrain.tga", &ttex);
	tm = GenerateTerrain(&ttex);
	printError("init terrain");

	//Load groundsphere
	sphere = LoadModelPlus("groundsphere.obj");
}

void display(void)
{
	// clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	printError("pre display");

	glUseProgram(program);




	// Build matrix
	handleKeyEvents(&cam, &lookAtPoint, &upVec, &speed);
	camMatrix = lookAt(cam.x, cam.y, cam.z,
				lookAtPoint.x, lookAtPoint.y, lookAtPoint.z,
				0.0, 1.0, 0.0);

	modelView = IdentityMatrix();
	total = Mult(camMatrix, modelView);
	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total.m);

	glBindTexture(GL_TEXTURE_2D, tex1);		// Bind Our Texture tex1
	DrawModel(tm, program, "inPosition", "inNormal", "inTexCoord");
	printError("display ground");

	glUseProgram(sphereShader);
	timeCounter = timeCounter+ 0.01;
	GLfloat yHeight = getHeight(tm, startX+timeCounter, startZ+timeCounter, ttex.width);
	sphereTranslate = T(startX+timeCounter, yHeight, startZ+timeCounter);
	glUniformMatrix4fv(glGetUniformLocation(sphereShader, "sphereTranslate"), 1, GL_TRUE, sphereTranslate.m);
	glUniformMatrix4fv(glGetUniformLocation(sphereShader, "projMatrix"), 1, GL_TRUE, projectionMatrix.m);
	glUniformMatrix4fv(glGetUniformLocation(sphereShader, "mdlMatrix"), 1, GL_TRUE, total.m);
	DrawModel(sphere, sphereShader, "inPosition", "inNormal", "inTexCoord");
	//printf("y: %.6f\n", yHeight); //Debug print of spheres y-position
	glutSwapBuffers();
}

void timer(int i)
{
	glutTimerFunc(20, &timer, i);
	glutPostRedisplay();
}

void mouse(int x, int y)
{
	printf("%d %d\n", x, y);
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);
	glutInitContextVersion(3, 2);
	glutInitWindowSize (600, 600);
	glutCreateWindow ("TSBK07 Lab 4");
	glutDisplayFunc(display);
	init ();
	glutTimerFunc(20, &timer, 0);

	glutPassiveMotionFunc(mouse);

	glutMainLoop();
	exit(0);
}
