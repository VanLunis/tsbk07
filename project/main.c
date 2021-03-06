


// Project


// Should work as is on Linux and Mac. MS Windows needs GLEW or glee.
// See separate Visual Studio version of my demos.
#ifdef __APPLE__
#include <OpenGLe
/gl3.h>
// Linking hint for Lightweight IDE
// uses framework Cocoa
#endif
#include "MicroGlut.h"
#include "GL_utilities.h"
#include <math.h>
#include "loadobj.h"
#include "LoadTGA.h"
#include "VectorUtils3.h"
#include "simplefont/simplefont.c"
#include <string.h>




// Globals

#define near 0.5
#define far 2000.0
#define right 0.5
#define left -0.5
#define top 0.5
#define bottom -0.5
#define PI 3.14159265


  GLfloat projectionMatrix[] = {    2.0f*near/(right-left), 0.0f, (right+left)/(right-left), 0.0f,

  0.0f, 2.0f*near/(top-bottom), (top+bottom)/(top-bottom), 0.0f,

  0.0f, 0.0f, -(far + near)/(far - near), -2*far*near/(far - near),

  0.0f, 0.0f, -1.0f, 0.0f };

  GLfloat specularExponent = 10;

  Point3D lightSourcesColorsArr = {0.5f, 0.5f, 0.5f};
  Point3D lightSourcesColorsArrSun = {1.0f, 1.0f, 1.0f};
  Point3D lightSourcesDirectionsPositions = {0.0f, 0.0f, -1.0f};

  GLint isDirectional[] = {0,0,1,1};

  mat4 rot, trans, total;

  float speed = 1;

  GLfloat phi = 0;
  GLfloat phiInner = 0;
  GLfloat theta = 0;
  GLfloat xi = 0;
  GLfloat rotationSphereIn = 0;
  GLfloat rotationSphereOut = 0;
  GLfloat rotTellusIn = 1;
  GLfloat rotTellusOut;
  GLfloat pushConst = 0.01;
  GLfloat rotSunOut, rotMercurayOut, rotVenusOut, rotTellusOutTime, rotMarsOut, rotJupiterOut, rotSaturnOut, rotUranusOut, rotNeptuneOut;
  GLfloat rotSunIn, rotMercurayIn, rotVenusIn, rotTellusInTime, rotMarsIn, rotJupiterIn, rotSaturnIn, rotUranusIn, rotNeptuneIn;

  vec3 camPos = {3.0f, 0.0f, -3+3.0f};
  vec3 upVec = {0.0,1.0,0.0};
  vec3 camLocTemp;
  vec3 lookAtPointTemp;
  vec3 lookAtPoint = {0,0,-30};

  bool hasPressed = false;
  int xPos = 0;
  int yPos = 0;
  int tSpeed = 30;
  float scaleSize = 0.1;



  GLuint program, skyShader, groundShader, pickShader; // Reference to shader program
  GLuint skyTexture, groundTexture, sunTex, mercuryTex, venusTex, tellusTex, marsTex, jupiterTex, saturnTex, uranusTex, neptuneText;

  Model *skyBox;
  Model *ground;
  Model *sphereModel;

  char* toDisplay;

  mat4 groundScaleMat;
  mat4 tmpGroundScaler;


  void renderSelect();


  void reshape(GLsizei w, GLsizei h)
  {
    glViewport(0, 0, w, h);
    sfSetRasterSize(w, h);
  }

  // Prints out the name of the selected object
  void selectBody()
  {
    unsigned char res[4];
    GLint viewport[4];

    renderSelect();
    glGetIntegerv(GL_VIEWPORT, viewport);

    glReadPixels(xPos, viewport[3] - yPos, 1,1,GL_RGBA, GL_UNSIGNED_BYTE, &res);
    switch(res[0])
    {
      case 0:
      toDisplay = "Cold, dark void";
      break;
      case 1:
      toDisplay = "Sun";
      break;
      case 2:
      toDisplay = "Mercury";
      break;
      case 3:
      toDisplay = "Venus";
      break;
      case 4:
      toDisplay = "Tellus";
      break;
      case 5:
      toDisplay = "Mars";
      break;
      case 6:
      toDisplay = "Jupiter";
      break;
      case 7:
      toDisplay = "Saturn";
      break;
      case 8:
      toDisplay = "Uranus";
      break;
      case 9:
      toDisplay = "Neptune";
      break;
    }
    glutSwapBuffers();
  }

  // Checks if the left mouse button has been pressed down or not. If it has, then the selected body will be printed out.
  void handleMouseEvent(int button, int state, int x, int y)
  {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && hasPressed == false)
    {
      hasPressed = true;
      xPos = x;
      yPos = y;
      selectBody();
    }
    else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP && hasPressed == true)
    {
      hasPressed = false;
    }
  }

  // General inits
  void init(void)
  {
    dumpInfo();
    glutMouseFunc(&handleMouseEvent);
    sfMakeRasterFont();

    //Load models
    skyBox = LoadModelPlus("skybox.obj");
    ground = LoadModelPlus("ground.obj");
    sphereModel = LoadModelPlus("superSphere.obj");


    // GL inits
    glClearColor(0.0,0.0,0.0,0);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glEnable(GL_DEPTH_TEST);
    printError("GL inits");

    // Load and compile shaders
    program = loadShaders("main.vert", "main.frag");
    skyShader = loadShaders("sky3-5.vert", "sky3-5.frag");
    groundShader = loadShaders("ground3-5.vert", "ground3-5.frag");
    pickShader = loadShaders("picking.vert", "picking.frag");
    printError("init shader");

    // Skybox texture init
    glUseProgram(skyShader);
    glActiveTexture(GL_TEXTURE0);
    LoadTGATextureSimple("textures/stars.tga", &skyTexture);

    // Ground texture inits
    glUseProgram(groundShader);
    glActiveTexture(GL_TEXTURE1);
    LoadTGATextureSimple("textures/starNew.tga", &groundTexture);
    int groundScaler = 10;
    tmpGroundScaler = S(groundScaler, 0.0001,groundScaler);
    groundScaleMat = Mult(T(0, -7, 0), tmpGroundScaler);

    // Sun and planet texture inits
    glUseProgram(program);
    glActiveTexture(GL_TEXTURE2);
    LoadTGATextureSimple("textures/sun.tga", &sunTex);
    glActiveTexture(GL_TEXTURE3);
    LoadTGATextureSimple("textures/Mercury.tga", &mercuryTex);
    glActiveTexture(GL_TEXTURE4);
    LoadTGATextureSimple("textures/Venus.tga", &venusTex);
    glActiveTexture(GL_TEXTURE5);
    LoadTGATextureSimple("textures/Tellus.tga", &tellusTex);
    glActiveTexture(GL_TEXTURE6);
    LoadTGATextureSimple("textures/Mars.tga", &marsTex);
    glActiveTexture(GL_TEXTURE7);
    LoadTGATextureSimple("textures/Jupiter.tga", &jupiterTex);
    glActiveTexture(GL_TEXTURE8);
    LoadTGATextureSimple("textures/Saturn.tga", &saturnTex);
    glActiveTexture(GL_TEXTURE9);
    LoadTGATextureSimple("textures/Uranus.tga", &uranusTex);
    glActiveTexture(GL_TEXTURE10);
    LoadTGATextureSimple("textures/Neptune.tga", &neptuneText);

    trans = T(0, 0, -2);
    total = Mult(rot, trans);
    rotTellusOut = rotTellusIn/365;
  }

  // Renders all celestial bodies with picking
  void renderSelect(void)
  {
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(pickShader);

    rotationSphereIn = (rotationSphereIn < 2*PI) ? rotationSphereIn+PI/79 : rotationSphereIn-2*PI+PI/79;
    rotationSphereOut = (rotationSphereOut < 2*PI) ? rotationSphereOut+PI/79 : rotationSphereOut-2*PI+PI/79;
    mat4 transMatSphere;


    float r;
    r = 0.1;

    vec3 ScaleSunVec = {r*109.3, r*109.3, r*109.3};
    mat4 scaleSun = S(-ScaleSunVec.x, ScaleSunVec.y, ScaleSunVec.z);
    mat4 scaleMercury = S(scaleSize*0.3829, scaleSize*0.3829, scaleSize*0.3829);
    mat4 scaleVenus = S(scaleSize*0.9499, scaleSize*0.9499, scaleSize*0.9499);
    mat4 scaleTellus = S(scaleSize*1, scaleSize*1, scaleSize*1);
    mat4 scaleMars = S(scaleSize*0.5320, scaleSize*0.5320, scaleSize*0.5320);
    mat4 scaleJupiter = S(scaleSize*10.97, scaleSize*10.97, scaleSize*10.97);
    mat4 scaleSaturn = S(scaleSize*9.14, scaleSize*9.14, scaleSize*9.14);
    mat4 scaleUranus = S(scaleSize*3.981, scaleSize*3.981, scaleSize*3.981);
    mat4 scaleNeptune = S(scaleSize*3.865, scaleSize*3.865, scaleSize*3.865);

    mat4 scale[] = {scaleSun, scaleMercury, scaleVenus, scaleTellus, scaleMars, scaleJupiter, scaleSaturn, scaleUranus, scaleNeptune};

    float t;
    t = 30;

    mat4 translateSun = T(0, 15, 0);
    mat4 translateMercury = T(t*0.39, translateSun.m[7], 0);
    mat4 translateVenus = T(t*0.723, translateSun.m[7], 0);
    mat4 translateTellus = T(t*1, translateSun.m[7], 0);
    mat4 translateMars = T(t*1.524, translateSun.m[7], 0);
    mat4 translateJupiter = T(t*5.203, translateSun.m[7], 0);
    mat4 translateSaturn = T(t*9.539, translateSun.m[7], 0);
    mat4 translateUranus = T(t*19.18, translateSun.m[7], 0);
    mat4 translateNeptune = T(t*30.06, translateSun.m[7], 0);
    mat4 translation[] = {translateSun, translateMercury, translateVenus, translateTellus, translateMars, translateJupiter, translateSaturn, translateUranus, translateNeptune};


    rotTellusIn = tSpeed;


    GLfloat rotationIn[] = {0.0001, 0.01705 , -0.004115, 1, 0.9756, 2.4242, 2.2430, -1.3953, 1.4907}; //24/period of rotation

    GLfloat rotationOut[] = {0, 4.1477 , 1.6243, 1, 0.5313, 0.08428, 0.03396, 0.01193, 0.0061037}; //365/days per year

    rotSunOut = (rotSunOut < 2*PI) ? rotSunOut+rotTellusOut*rotationOut[0] : rotSunOut-2*PI+rotTellusOut*rotationOut[0];
    rotMercurayOut = (rotMercurayOut < 2*PI) ? rotMercurayOut+rotTellusOut*rotationOut[1] : rotMercurayOut-2*PI+rotTellusOut*rotationOut[1];
    rotVenusOut = (rotVenusOut < 2*PI) ? rotVenusOut+rotTellusOut*rotationOut[2] : rotVenusOut-2*PI+rotTellusOut*rotationOut[2];
    rotTellusOutTime = (rotTellusOutTime < 2*PI) ? rotTellusOutTime+rotTellusOut*rotationOut[3] : rotTellusOutTime-2*PI+rotTellusOut*rotationOut[3];
    rotMarsOut = (rotMarsOut < 2*PI) ? rotMarsOut+rotTellusOut*rotationOut[4] : rotMarsOut-2*PI+rotTellusOut*rotationOut[4];
    rotJupiterOut = (rotJupiterOut < 2*PI) ? rotJupiterOut+rotTellusOut*rotationOut[5] : rotJupiterOut-2*PI+rotTellusOut*rotationOut[5];
    rotSaturnOut = (rotSaturnOut < 2*PI) ? rotSaturnOut+rotTellusOut*rotationOut[6] : rotSaturnOut-2*PI+rotTellusOut*rotationOut[6];
    rotUranusOut = (rotUranusOut < 2*PI) ? rotUranusOut+rotTellusOut*rotationOut[7] : rotUranusOut-2*PI+rotTellusOut*rotationOut[7];
    rotNeptuneOut = (rotNeptuneOut < 2*PI) ? rotNeptuneOut+rotTellusOut*rotationOut[8] : rotNeptuneOut-2*PI+rotTellusOut*rotationOut[8];


    rotSunIn = (rotSunIn < 2*PI) ? rotSunIn+rotTellusIn*rotationIn[0] : rotSunIn-2*PI+rotTellusIn*rotationIn[0];
    rotMercurayIn = (rotMercurayIn < 2*PI) ? rotMercurayIn+rotTellusIn*rotationIn[1] : rotMercurayIn-2*PI+rotTellusIn*rotationIn[1];
    rotVenusIn = (rotVenusIn < 2*PI) ? rotVenusIn+rotTellusIn*rotationIn[2] : rotVenusIn-2*PI+rotTellusIn*rotationIn[2];
    rotTellusInTime = (rotTellusInTime < 2*PI) ? rotTellusInTime+rotTellusIn*rotationOut[3] : rotTellusInTime-2*PI+rotTellusIn*rotationOut[3];
    rotMarsIn = (rotMarsIn < 2*PI) ? rotMarsIn+rotTellusIn*rotationIn[4] : rotMarsIn-2*PI+rotTellusIn*rotationIn[4];
    rotJupiterIn = (rotJupiterIn < 2*PI) ? rotJupiterIn+rotTellusIn*rotationIn[5] : rotJupiterIn-2*PI+rotTellusIn*rotationIn[5];
    rotSaturnIn = (rotSaturnIn < 2*PI) ? rotSaturnIn+rotTellusIn*rotationIn[6] : rotSaturnIn-2*PI+rotTellusIn*rotationIn[6];
    rotUranusIn = (rotUranusIn < 2*PI) ? rotUranusIn+rotTellusIn*rotationIn[7] : rotUranusIn-2*PI+rotTellusIn*rotationIn[7];
    rotNeptuneIn = (rotNeptuneIn < 2*PI) ? rotNeptuneIn+rotTellusIn*rotationIn[8] : rotNeptuneIn-2*PI+rotTellusIn*rotationIn[8];

    GLfloat OuterRotList[] = {rotSunOut, rotMercurayOut, rotVenusOut, rotTellusOutTime, rotMarsOut, rotJupiterOut, rotSaturnOut, rotUranusOut, rotNeptuneOut};
    GLfloat InnerRotList[] = {rotSunIn, rotMercurayIn, rotVenusIn, rotTellusInTime, rotMarsIn, rotJupiterIn, rotSaturnIn, rotUranusIn, rotNeptuneIn};


    printError("planetloop");
    glUniformMatrix4fv(glGetUniformLocation(pickShader, "projectionMatrix"), 1, GL_TRUE, projectionMatrix);
    printError("projectionMatrix");
    mat4 lookAtMat = lookAtv(camPos,lookAtPoint,upVec);
    glUniformMatrix4fv(glGetUniformLocation(pickShader, "lookAtMat"), 1, GL_TRUE, lookAtMat.m);
    printError("lookAtMatrix");

    size_t planet;
    for (planet = 0; planet < 9; planet++)
    {
      glUniform1i(glGetUniformLocation(pickShader, "planetNumber"), planet+1);

      transMatSphere = translation[planet];
      mat4 rotationInner = Ry(InnerRotList[planet]);
      mat4 rotationOuter = Ry(OuterRotList[planet]);

      mat4 transformationMatrix = Mult(rotationOuter, Mult(transMatSphere, Mult(rotationInner, scale[planet])));
      mat4 actualPosMatrix = Mult(rotationOuter, Mult(transMatSphere, scale[planet]));

      glUniformMatrix4fv(glGetUniformLocation(pickShader, "transformationMatrix"), 1, GL_TRUE, transformationMatrix.m);
      glUniformMatrix4fv(glGetUniformLocation(pickShader, "actualPosMatrix"), 1, GL_TRUE, actualPosMatrix.m);
      DrawModel(sphereModel, pickShader, "in_Position", NULL, NULL);
    }
  }

  /* Makes movement possible, but only if there is no collision with any object.
  s = back
  w = forward
  a = left
  d = right
  o = up
  l = down
  m = slow down everything
  n = speed up everything
  */
  void handleKeyEvents(vec3* cameraLocation, vec3* lookAtPoint, vec3* upVector, float* movement_speed, const vec3* collisionPos, const GLfloat* radiuses)
  {
    // This is the direction the camera is looking
    vec3 direction;
    direction = VectorSub(*cameraLocation, *lookAtPoint);
    direction = Normalize(direction);
    direction = ScalarMult(direction,*movement_speed);

    camLocTemp = *cameraLocation;
    lookAtPointTemp = *lookAtPoint;

    if ( glutKeyIsDown('s') ) {
      camLocTemp = VectorAdd(camLocTemp, direction);
      lookAtPointTemp = VectorAdd(lookAtPointTemp, direction);

    }
    if ( glutKeyIsDown('w') ) {
      camLocTemp = VectorSub(camLocTemp, direction);
      lookAtPointTemp = VectorSub(lookAtPointTemp, direction);
    }

    if (glutKeyIsDown('o')) {
      camLocTemp = MultVec3(T(0,0.25,0),camLocTemp);
      lookAtPointTemp = MultVec3(T(0,0.25,0),lookAtPointTemp);
    }
    if ( glutKeyIsDown('l') ) {
      camLocTemp = MultVec3(T(0,-0.25,0),camLocTemp);
      lookAtPointTemp = MultVec3(T(0,-0.25,0),lookAtPointTemp);
    }

    if (glutKeyIsDown('n')){
      if (tSpeed < 100){
        tSpeed = tSpeed+1;
      }
    }
    if (glutKeyIsDown('m')){
      if (tSpeed > 0){
        tSpeed = tSpeed-1;
      }
    }

    if (glutKeyIsDown('t')){
      if (scaleSize < 10){
        scaleSize = scaleSize+0.01;
      }
    }
    if (glutKeyIsDown('g')){
      if (scaleSize > 0.1){
        scaleSize = scaleSize-0.01;
      }
    }

    if (glutKeyIsDown('q')){
      if (speed < 5){
        speed = speed+0.1;
      }
    }
    if (glutKeyIsDown('e')){
      if (speed > 0.2){
        speed = speed-0.1;
      }
    }

    bool collisionSafe = true;
    size_t body;
    for (body = 0; body <9; body++)
    {
      if (Norm(VectorSub(camLocTemp, collisionPos[body])) <= radiuses[body]){
        collisionSafe = false;
        body = 9;
      }
    }
    //collisionSafe = true;
    if (collisionSafe){
      *cameraLocation = camLocTemp;
      *lookAtPoint = lookAtPointTemp;
    }

    if (glutKeyIsDown('d')) {
      *lookAtPoint = VectorSub(*lookAtPoint,*cameraLocation);
      *lookAtPoint = MultVec3(Ry(-PI/100),*lookAtPoint);
      *lookAtPoint = VectorAdd(*lookAtPoint,*cameraLocation);
    }
    if ( glutKeyIsDown('a') ) {
      *lookAtPoint = VectorSub(*lookAtPoint,*cameraLocation);
      *lookAtPoint = MultVec3(Ry(PI/100),*lookAtPoint);
      *lookAtPoint = VectorAdd(*lookAtPoint,*cameraLocation);
    }

    vec3 newDirection = VectorSub(*cameraLocation, *lookAtPoint);
    newDirection = Normalize(direction);
    vec3 rightDirection = MultVec3(Ry(PI/2),newDirection);

    *upVector = Normalize(CrossProduct(newDirection,rightDirection));
  }

  // Renders and displays all objects and the background
  void display(void)
  {
    printError("pre display");

    // clear the screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    phi = (phi < 2*PI) ? phi+PI/100 : phi-2*PI+PI/100;
    phiInner = (phiInner < 2*PI) ? phiInner+PI/500 : phiInner-2*PI+PI/500; // What is this I don't even?
    theta = (theta < 2*PI) ? theta+PI/168 : theta-2*PI+PI/168;
    xi = (xi < 2*PI) ? xi+PI/79 : xi-2*PI+PI/79;


    mat4 lookAtMat = lookAtv(camPos,lookAtPoint,upVec);

    mat4 transformationMatrix;


    //*****************Draw space everywhere except the lower part******************
    glUseProgram(skyShader);
    glDisable(GL_DEPTH_TEST);
    printError("Disable Z");
    glBindTexture(GL_TEXTURE_2D,skyTexture);
    printError("Sky bind texture");
    glUniform1i(glGetUniformLocation(skyShader, "textUnit"), 0); // Texture unit 0
    printError("pre-skybox");
    transformationMatrix = IdentityMatrix();
    printError("SKY: new transform");
    glUniformMatrix4fv(glGetUniformLocation(skyShader, "transformationMatrix"), 1, GL_TRUE, transformationMatrix.m);
    printError("SKY: Load transform");
    mat3 skyLookAtMat3 = mat4tomat3(lookAtMat);
    mat4 skyLookAtMat4 = mat3tomat4(skyLookAtMat3);
    glUniformMatrix4fv(glGetUniformLocation(skyShader, "lookAtMat"), 1, GL_TRUE, skyLookAtMat4.m);
    glUniformMatrix4fv(glGetUniformLocation(skyShader, "projectionMatrix"), 1, GL_TRUE, projectionMatrix);
    printError("SKY: projection");
    DrawModel(skyBox, skyShader, "in_Position", NULL , "inTexCoord");
    printError("Skybox draw");
    glEnable(GL_DEPTH_TEST);
    printError("Enable z");
    // **************************End *******************

    // **************************Draw space on the lower part*******************

    glUseProgram(groundShader);
    glDisable(GL_DEPTH_TEST);
    glUniformMatrix4fv(glGetUniformLocation(groundShader, "lookAtMat"), 1, GL_TRUE, skyLookAtMat4.m);
    printError("GROUND: lookAtMat");
    glUniformMatrix4fv(glGetUniformLocation(groundShader, "projectionMatrix"), 1, GL_TRUE, projectionMatrix);
    printError("GROUND: projectionMatrix");

    groundScaleMat = Mult(T(0, -7, 0), tmpGroundScaler);
    glUniformMatrix4fv(glGetUniformLocation(groundShader, "transformationMatrix"), 1, GL_TRUE, groundScaleMat.m);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glBindTexture(GL_TEXTURE_2D, groundTexture);
    glUniform1i(glGetUniformLocation(groundShader, "textUnit"), 1);
    DrawModel(ground, groundShader, "in_Position", "in_Normal" , "inTexCoord");

    glEnable(GL_DEPTH_TEST);
    glUseProgram(program);
    // **************************End *******************

    //*******************Light***************************
    glUniform1f(glGetUniformLocation(program, "specularExponent"), specularExponent);

    glUniform1iv(glGetUniformLocation(program, "isDirectional"), 1, isDirectional);
    //*******************End light***************************


    // *******************Data for celestial bodies*****************
    rotationSphereIn = (rotationSphereIn < 2*PI) ? rotationSphereIn+PI/79 : rotationSphereIn-2*PI+PI/79;
    rotationSphereOut = (rotationSphereOut < 2*PI) ? rotationSphereOut+PI/79 : rotationSphereOut-2*PI+PI/79;
    mat4 transMatSphere;
    float r;
    r = 0.1; // a parameter to change the scaling if wanted

    vec3 ScaleSunVec = {r*109.3, r*109.3, r*109.3};
    mat4 scaleSun = S(-ScaleSunVec.x, ScaleSunVec.y, ScaleSunVec.z); //correct scaling
    mat4 scaleMercury = S(scaleSize*0.3829, scaleSize*0.3829, scaleSize*0.3829);
    mat4 scaleVenus = S(scaleSize*0.9499, scaleSize*0.9499, scaleSize*0.9499);
    mat4 scaleTellus = S(scaleSize*1, scaleSize*1, scaleSize*1);
    mat4 scaleMars = S(scaleSize*0.5320, scaleSize*0.5320, scaleSize*0.5320);
    mat4 scaleJupiter = S(scaleSize*10.97, scaleSize*10.97, scaleSize*10.97);
    mat4 scaleSaturn = S(scaleSize*9.14, scaleSize*9.14, scaleSize*9.14);
    mat4 scaleUranus = S(scaleSize*3.981, scaleSize*3.981, scaleSize*3.981);
    mat4 scaleNeptune = S(scaleSize*3.865, scaleSize*3.865, scaleSize*3.865);

    mat4 scale[] = {scaleSun, scaleMercury, scaleVenus, scaleTellus, scaleMars, scaleJupiter, scaleSaturn, scaleUranus, scaleNeptune};
    glUniform3fv(glGetUniformLocation(program, "SunPos"), 1, &ScaleSunVec.y);

    GLfloat objRadiiSun = 1;
    GLfloat objRadiiBigPlanets = 1.2;
    GLfloat objRadiiSmallPlanets = 1.6;
    GLfloat radii[] = {objRadiiSun*abs(scale[0].m[0]), objRadiiSmallPlanets*scale[1].m[0], objRadiiSmallPlanets*scale[2].m[0], objRadiiSmallPlanets*scale[3].m[0], objRadiiSmallPlanets*scale[4].m[0], objRadiiBigPlanets*scale[5].m[0],
                        objRadiiBigPlanets*scale[6].m[0], objRadiiBigPlanets*scale[7].m[0], objRadiiBigPlanets*scale[8].m[0]};

      float t;
      t = 30; // a parameter to change the planets's distance from the sun if wanted
      GLfloat distanceFromSun[] = {0, t*0.39, t*0.723, t*1, t*1.524, t*5.203, t*9.539, t*19.18, t*30.06};

      mat4 translateSun = T(0, 15, 0);
      mat4 translateMercury = T(t*0.39, translateSun.m[7], 0);
      mat4 translateVenus = T(t*0.723, translateSun.m[7], 0);
      mat4 translateTellus = T(t*1, translateSun.m[7], 0);
      mat4 translateMars = T(t*1.524, translateSun.m[7], 0);
      mat4 translateJupiter = T(t*5.203, translateSun.m[7], 0);
      mat4 translateSaturn = T(t*9.539, translateSun.m[7], 0);
      mat4 translateUranus = T(t*19.18, translateSun.m[7], 0);
      mat4 translateNeptune = T(t*30.06, translateSun.m[7], 0);
      mat4 translation[] = {translateSun, translateMercury, translateVenus, translateTellus, translateMars, translateJupiter, translateSaturn, translateUranus, translateNeptune};

      rotTellusIn = tSpeed;
      rotTellusOut = rotTellusIn/365;

      GLfloat rotationIn[] = {0.0001, 0.01705 , -0.004115, 1, 0.9756, 2.4242, 2.2430, -1.3953, 1.4907}; //argument 0 is actually 0.04087. Calculation: 24/period of rotation

      GLfloat rotationOut[] = {0, 4.1477 , 1.6243, 1, 0.5313, 0.08428, 0.03396, 0.01193, 0.0061037}; //365/days per year

      rotSunOut = (rotSunOut < 2*PI) ? rotSunOut+rotTellusOut*rotationOut[0] : rotSunOut-2*PI+rotTellusOut*rotationOut[0];
      rotMercurayOut = (rotMercurayOut < 2*PI) ? rotMercurayOut+rotTellusOut*rotationOut[1] : rotMercurayOut-2*PI+rotTellusOut*rotationOut[1];
      rotVenusOut = (rotVenusOut < 2*PI) ? rotVenusOut+rotTellusOut*rotationOut[2] : rotVenusOut-2*PI+rotTellusOut*rotationOut[2];
      rotTellusOutTime = (rotTellusOutTime < 2*PI) ? rotTellusOutTime+rotTellusOut*rotationOut[3] : rotTellusOutTime-2*PI+rotTellusOut*rotationOut[3];
      rotMarsOut = (rotMarsOut < 2*PI) ? rotMarsOut+rotTellusOut*rotationOut[4] : rotMarsOut-2*PI+rotTellusOut*rotationOut[4];
      rotJupiterOut = (rotJupiterOut < 2*PI) ? rotJupiterOut+rotTellusOut*rotationOut[5] : rotJupiterOut-2*PI+rotTellusOut*rotationOut[5];
      rotSaturnOut = (rotSaturnOut < 2*PI) ? rotSaturnOut+rotTellusOut*rotationOut[6] : rotSaturnOut-2*PI+rotTellusOut*rotationOut[6];
      rotUranusOut = (rotUranusOut < 2*PI) ? rotUranusOut+rotTellusOut*rotationOut[7] : rotUranusOut-2*PI+rotTellusOut*rotationOut[7];
      rotNeptuneOut = (rotNeptuneOut < 2*PI) ? rotNeptuneOut+rotTellusOut*rotationOut[8] : rotNeptuneOut-2*PI+rotTellusOut*rotationOut[8];


      rotSunIn = (rotSunIn < 2*PI) ? rotSunIn+rotTellusIn*rotationIn[0] : rotSunIn-2*PI+rotTellusIn*rotationIn[0];
      rotMercurayIn = (rotMercurayIn < 2*PI) ? rotMercurayIn+rotTellusIn*rotationIn[1] : rotMercurayIn-2*PI+rotTellusIn*rotationIn[1];
      rotVenusIn = (rotVenusIn < 2*PI) ? rotVenusIn+rotTellusIn*rotationIn[2] : rotVenusIn-2*PI+rotTellusIn*rotationIn[2];
      rotTellusInTime = (rotTellusInTime < 2*PI) ? rotTellusInTime+rotTellusIn*rotationOut[3] : rotTellusInTime-2*PI+rotTellusIn*rotationOut[3];
      rotMarsIn = (rotMarsIn < 2*PI) ? rotMarsIn+rotTellusIn*rotationIn[4] : rotMarsIn-2*PI+rotTellusIn*rotationIn[4];
      rotJupiterIn = (rotJupiterIn < 2*PI) ? rotJupiterIn+rotTellusIn*rotationIn[5] : rotJupiterIn-2*PI+rotTellusIn*rotationIn[5];
      rotSaturnIn = (rotSaturnIn < 2*PI) ? rotSaturnIn+rotTellusIn*rotationIn[6] : rotSaturnIn-2*PI+rotTellusIn*rotationIn[6];
      rotUranusIn = (rotUranusIn < 2*PI) ? rotUranusIn+rotTellusIn*rotationIn[7] : rotUranusIn-2*PI+rotTellusIn*rotationIn[7];
      rotNeptuneIn = (rotNeptuneIn < 2*PI) ? rotNeptuneIn+rotTellusIn*rotationIn[8] : rotNeptuneIn-2*PI+rotTellusIn*rotationIn[8];

      GLfloat OuterRotList[] = {rotSunOut, rotMercurayOut, rotVenusOut, rotTellusOutTime, rotMarsOut, rotJupiterOut, rotSaturnOut, rotUranusOut, rotNeptuneOut};
      GLfloat InnerRotList[] = {rotSunIn, rotMercurayIn, rotVenusIn, rotTellusInTime, rotMarsIn, rotJupiterIn, rotSaturnIn, rotUranusIn, rotNeptuneIn};

      GLuint* textArray = {sunTex, mercuryTex, venusTex, tellusTex, marsTex, jupiterTex, saturnTex, uranusTex, neptuneText};

      //*******************End: Data for celestial bodies***********************

      printError("planetloop");
      glUseProgram(program);
      glUniformMatrix4fv(glGetUniformLocation(program, "projectionMatrix"), 1, GL_TRUE, projectionMatrix);
      printError("projectionMatrix");
      glUniformMatrix4fv(glGetUniformLocation(program, "lookAtMat"), 1, GL_TRUE, lookAtMat.m);
      printError("lookAtMatrix");

      size_t planet;
      vec3 finalTranslation = {0, 0, 0};
      vec3 celestialTranslation[] = {finalTranslation, finalTranslation, finalTranslation, finalTranslation, finalTranslation, finalTranslation, finalTranslation, finalTranslation, finalTranslation};
      float sunBool = 1;

      for (planet = 0; planet < 9; planet++)
      {
        if (planet == 0) //if sun
        {
          glUniform3fv(glGetUniformLocation(program, "lightSourcesColorArr"), 1, &lightSourcesColorsArrSun.x);
          sunBool = 1;
        }
        else
        {
          glUniform3fv(glGetUniformLocation(program, "lightSourcesColorArr"), 1, &lightSourcesColorsArr.x);
          sunBool = 0;
        }
        glUniform1f(glGetUniformLocation(program, "sunBool"), sunBool);
        glBindTexture(GL_TEXTURE_2D, &textArray[planet]);
        glUniform1i(glGetUniformLocation(program, "planetTex"), planet+2);

        transMatSphere = translation[planet];
        mat4 rotationInner = Ry(InnerRotList[planet]);
        mat4 rotationOuter = Ry(OuterRotList[planet]);

        mat4 transformationMatrix = Mult(rotationOuter, Mult(transMatSphere, Mult(rotationInner, scale[planet])));
        mat4 actualPosMatrix = Mult(rotationOuter, Mult(transMatSphere, scale[planet]));
        vec3 finalTranslation = {actualPosMatrix.m[3], actualPosMatrix.m[7], actualPosMatrix.m[11]};
        celestialTranslation[planet] = finalTranslation;

        if (Norm(VectorSub(camPos, finalTranslation)) < radii[planet]){
          vec3 movingDirection = Normalize(VectorSub(camPos, celestialTranslation[planet]));
          GLfloat movingSpeed = pushConst * rotationOut[planet] * distanceFromSun[planet];
          vec3 toMove = ScalarMult(movingDirection, movingSpeed);
          camPos = MultVec3(T(toMove.x, toMove.y, toMove.z), camPos);
          lookAtPoint = MultVec3(T(toMove.x, toMove.y, toMove.z), lookAtPoint);
        }


        glUniformMatrix4fv(glGetUniformLocation(program, "transformationMatrix"), 1, GL_TRUE, transformationMatrix.m);
        glUniformMatrix4fv(glGetUniformLocation(program, "actualPosMatrix"), 1, GL_TRUE, actualPosMatrix.m);
        DrawModel(sphereModel, program, "in_Position", "in_Normal", "inTexCoord");
      }

      handleKeyEvents(&camPos, &lookAtPoint, &upVec, &speed, &celestialTranslation, &radii);

      // Checks if the mouse left button has been pressed and where it was pressed.
      if (hasPressed == true)
      {
        sfDrawString(xPos, yPos, toDisplay);
      }

      printError("display");
      glutSwapBuffers();
    }

    void OnTimer(int value)
    {
      glutPostRedisplay();

      glutTimerFunc(20, &OnTimer, (value));
    }

    int main(int argc, char *argv[])
    {
      glutInit(&argc, argv);
      glutInitContextVersion(3, 2);
      glutCreateWindow ("Solar system");
      glutReshapeFunc(reshape);
      init();
      glutDisplayFunc(display);
      OnTimer(0);
      glutMainLoop();
      return 0;
    }
