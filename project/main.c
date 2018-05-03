

//Do 5 arrays for the planets with translation, inner rotation, outer rotation, scaling and texture








// Project
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


    /*Point3D lightSourcesColorsArr[] = { {1.0f, 0.0f, 0.0f}, // Red light

    {0.0f, 1.0f, 0.0f}, // Green light

    {0.0f, 0.0f, 1.0f}, // Blue light

    {1.0f, 1.0f, 1.0f} }; // White light
    */

    Point3D lightSourcesColorsArr = {0.5f, 0.5f, 0.5f};
    Point3D lightSourcesColorsArrSun = {0.5f, 0.5f, 0.5f};

    GLint isDirectional[] = {0,0,1,1};
    GLfloat specularExponent[] = {10.0, 200.0, 60.0, 50.0, 300.0, 150.0};


    Point3D lightSourcesDirectionsPositions = {0.0f, 0.0f, -1.0f};

    /*Point3D lightSourcesDirectionsPositions[] = { {10.0f, 5.0f, 0.0f}, // Red light, positional

    {0.0f, 5.0f, 10.0f}, // Green light, positional

    {-1.0f, 0.0f, 0.0f}, // Blue light along X

    {0.0f, 0.0f, -1.0f} }; // White light along Z
    */




    mat4 rot, trans, total;
    //mat4 scaleSun, scaleMercury, scaleVenus, scaleTellus, scaleMars, scaleJupiter, scaleSaturn, scaleUranus, scaleNeptune;
    //mat4 translateSun, translateMercury, translateVenus, translateTellus, translateMars, translateJupiter, translateSaturn, translateUranus, translateNeptune;
    vec3 lookAtPoint = {0,0,-30};
    //mat4 scale[9], translation[9];
    static const float speed = 1;


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
    vec3 upVectorTemp;

    GLfloat tSpeed = 30; //change this

    //GLfloat rotationIn[9];

    //GLfloat rotationOut[9];










    // vertex array object
    unsigned int vertexArrayObjID;

    unsigned int SunVertexArrayObjID;
    unsigned int SunVertexBufferObjID;
    unsigned int SunIndexBufferObjID;
    unsigned int SunNormalBufferObjID;
    unsigned int SunTexCoordBufferObjID;
    // vertex buffer object, used for uploading the geometry
    unsigned int vertexBufferObjID;
    // color buffer
    unsigned int colorBufferObjID;

    // Reference to shader program
    GLuint program, skyShader, groundShader, pickShader;

    Model *skyBox;
    Model *ground;
    Model *sphereModel;

    GLuint skyTexture, groundTexture, sunTex, mercuryTex, venusTex, tellusTex, marsTex, jupiterTex, saturnTex, uranusTex, neptuneText;
    //GLuint rutorTexture;

    mat4 groundScaleMat;
    mat4 tmpGroundScaler;

    void init(void)
    {



        dumpInfo();



        //Load models

        skyBox = LoadModelPlus("skybox.obj");
        ground = LoadModelPlus("ground.obj");
        sphereModel = LoadModelPlus("superSphere.obj");


        // GL inits
        glClearColor(1.0,1.0,1.0,0);
        glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
        glEnable(GL_DEPTH_TEST);
        printError("GL inits");

        // Load and compile shader
        program = loadShaders("main.vert", "main.frag");
        skyShader = loadShaders("sky3-5.vert", "sky3-5.frag");
        groundShader = loadShaders("ground3-5.vert", "ground3-5.frag");
        pickShader = loadShaders("picking.vert", "picking.frag");
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
        LoadTGATextureSimple("textures/sunmap.tga", &sunTex);
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
        //glActiveTexture(GL_TEXTURE3);
        //LoadTGATextureSimple("maskros512.tga", &rutorTexture);



        trans = T(0, 0, -2);
        total = Mult(rot, trans);
        rotTellusOut = rotTellusIn/365;



    }

    void selectBody(int xPos, int yPos)
    {
        unsigned char res[9];
        GLint viewport[4];

        //TODO Make render selection here
        glGetIntegerv(GL_VIEWPORT, viewport);
        glReadPixels(xPos, viewport[3] - yPos, 1,1,GL_RGBA, GL_UNSIGNED_BYTE, &res);
        switch(res[0])
        {
            case 0: printf("Nothing selected\n"); break;
            case 1: printf("Selected Sun\n"); break;
            case 2: printf("Selected Mercury\n"); break;
            case 3: printf("Selected Venus\n"); break;
            case 4: printf("Selected Tellus\n"); break;
            case 5: printf("Selected Mars\n"); break;
            case 6: printf("Selected Jupiter\n"); break;
            case 7: printf("Selected Saturn\n"); break;
            case 8: printf("Selected Uranus\n"); break;
            case 9: printf("Selected Neptune\n"); break;
        }
    }

    void renderSelect(void)
    {
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(pickShader);

        rotationSphereIn = (rotationSphereIn < 2*PI) ? rotationSphereIn+PI/79 : rotationSphereIn-2*PI+PI/79;
        rotationSphereOut = (rotationSphereOut < 2*PI) ? rotationSphereOut+PI/79 : rotationSphereOut-2*PI+PI/79;
        mat4 transMatSphere;


        float r;
        r = 0.1; //change this

        vec3 ScaleSunVec = {r*109.3, r*109.3, r*109.3};
        mat4 scaleSun = S(ScaleSunVec.x, ScaleSunVec.y, ScaleSunVec.z); //correct scaling
        mat4 scaleMercury = S(r*0.3829, r*0.3829, r*0.3829);
        mat4 scaleVenus = S(r*0.9499, r*0.9499, r*0.9499);
        mat4 scaleTellus = S(r*1, r*1, r*1);
        mat4 scaleMars = S(r*0.5320, r*0.5320, r*0.5320);
        mat4 scaleJupiter = S(r*10.97, r*10.97, r*10.97);
        mat4 scaleSaturn = S(r*9.14, r*9.14, r*9.14);
        mat4 scaleUranus = S(r*3.981, r*3.981, r*3.981);
        mat4 scaleNeptune = S(r*3.865, r*3.865, r*3.865);

        mat4 scale[] = {scaleSun, scaleMercury, scaleVenus, scaleTellus, scaleMars, scaleJupiter, scaleSaturn, scaleUranus, scaleNeptune};

        GLfloat objRadii = 1.2;
        GLfloat radii[] = {objRadii*scale[0].m[0], objRadii*scale[1].m[0], objRadii*scale[2].m[0], objRadii*scale[3].m[0], objRadii*scale[4].m[0], objRadii*scale[5].m[0],
            objRadii*scale[6].m[0], objRadii*scale[7].m[0], objRadii*scale[8].m[0]};



            float t;
            t = 30; //change this
            GLfloat distanceFromSun[] = {0, t*0.39, t*0.723, t*1, t*1.524, t*5.203, t*9.539, t*19.18, t*30.06};

            mat4 translateSun = T(0, 15, 0);
            mat4 translateMercury = T(t*0.39, ScaleSunVec.y + translateSun.m[7]-10, 0);
            mat4 translateVenus = T(t*0.723, ScaleSunVec.y + translateSun.m[7]-10, 0);
            mat4 translateTellus = T(t*1, ScaleSunVec.y + translateSun.m[7]-10, 0);
            mat4 translateMars = T(t*1.524, ScaleSunVec.y + translateSun.m[7]-10, 0);
            mat4 translateJupiter = T(t*5.203, ScaleSunVec.y + translateSun.m[7]-10, 0);
            mat4 translateSaturn = T(t*9.539, ScaleSunVec.y + translateSun.m[7]-10, 0);
            mat4 translateUranus = T(t*19.18, ScaleSunVec.y + translateSun.m[7]-10, 0);
            mat4 translateNeptune = T(t*30.06, ScaleSunVec.y + translateSun.m[7]-10, 0);


            mat4 translation[] = {translateSun, translateMercury, translateVenus, translateTellus, translateMars, translateJupiter, translateSaturn, translateUranus, translateNeptune};


            rotTellusIn = tSpeed;


            GLfloat rotationIn[] = {0.04087, 0.01705 , -0.004115, 1, 0.9756, 2.4242, 2.2430, -1.3953, 1.4907}; //24/period of rotation

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

            printError("planetloop");
            glUniformMatrix4fv(glGetUniformLocation(pickShader, "projectionMatrix"), 1, GL_TRUE, projectionMatrix);
            printError("projectionMatrix");
            mat4 lookAtMat = lookAtv(camPos,lookAtPoint,upVec);
            glUniformMatrix4fv(glGetUniformLocation(pickShader, "lookAtMat"), 1, GL_TRUE, lookAtMat.m);
            printError("lookAtMatrix");

            size_t planet;
            vec3 finalTranslation = {0, 0, 0};
            vec3 celestialTranslation[] = {finalTranslation, finalTranslation, finalTranslation, finalTranslation, finalTranslation, finalTranslation, finalTranslation, finalTranslation, finalTranslation};


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
                DrawModel(sphereModel, pickShader, "in_Position", "in_Normal", "inTexCoord");
            }

    }


    void handleKeyEvents(vec3* cameraLocation, vec3* lookAtPoint, vec3* upVector, const float* movement_speed, const vec3* collisionPos, const GLfloat* radiuses)
    {
        // This is the direction the camera is looking
        vec3 direction;
        direction = VectorSub(*cameraLocation, *lookAtPoint);
        direction = Normalize(direction);
        direction = ScalarMult(direction,*movement_speed);

        camLocTemp = *cameraLocation;
        lookAtPointTemp = *lookAtPoint;
        upVectorTemp = *upVector;





        if ( glutKeyIsDown('s') ) {
            camLocTemp = VectorAdd(camLocTemp, direction);
            lookAtPointTemp = VectorAdd(lookAtPointTemp, direction);

        }
        if ( glutKeyIsDown('w') ) {
            camLocTemp = VectorSub(camLocTemp, direction);
            lookAtPointTemp = VectorSub(lookAtPointTemp, direction);
        }

        if (glutKeyIsDown('o')) {
            camLocTemp = MultVec3(T(0,0.5,0),camLocTemp);
            lookAtPointTemp = MultVec3(T(0,0.5,0),lookAtPointTemp);
        }
        if ( glutKeyIsDown('l') ) {
            camLocTemp = MultVec3(T(0,-0.5,0),camLocTemp);
            lookAtPointTemp = MultVec3(T(0,-0.5,0),lookAtPointTemp);
        }

        if (glutKeyIsDown('n')){
            if (tSpeed < 100){
                tSpeed = tSpeed+1;
            }
        }
        if (glutKeyIsDown('m')){
            if (tSpeed > -1){
                tSpeed = tSpeed-1;
            }
        }

        bool collisionSafe = true;
        size_t body;
        for (body = 0; body <9; body++)
        {
            if (Norm(VectorSub(camLocTemp, collisionPos[body])) < radiuses[body]){
                collisionSafe = false;
                body = 9;
            }
        }

        if (collisionSafe){
            *cameraLocation = camLocTemp;
            *lookAtPoint = lookAtPointTemp;
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




        mat4 rotationSun = Ry(0);
        //mat4 sunScale = S(10,10,10);



        mat4 lookAtMat = lookAtv(camPos,lookAtPoint,upVec);

        mat4 transformationMatrix;





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
        transformationMatrix = rotationSun;
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

        groundScaleMat = Mult(T(0, -7, 0), Mult(rotationSun,tmpGroundScaler));
        glUniformMatrix4fv(glGetUniformLocation(groundShader, "transformationMatrix"), 1, GL_TRUE, groundScaleMat.m);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glBindTexture(GL_TEXTURE_2D, groundTexture);
        glUniform1i(glGetUniformLocation(groundShader, "textUnit"), 1);
        DrawModel(ground, groundShader, "in_Position", "in_Normal" , "inTexCoord");


        glUseProgram(program);

        //*******************Light***************************

        //  glUniform3fv(glGetUniformLocation(program, "lightSourcesColorArr"), 1, &lightSourcesColorsArr.x);

        glUniform1f(glGetUniformLocation(program, "specularExponent"), specularExponent[0]);

        glUniform1iv(glGetUniformLocation(program, "isDirectional"), 1, isDirectional);

        //*******************End light***************************


        // *******************Draw objects*****************

        //glBindTexture(GL_TEXTURE_2D, rutorTexture);
        //glUniform1i(glGetUniformLocation(program, "rutorTex"), 3);



        //mat4 sphereScale = S(4,4,4);
        //vec3 translateSphere = {translateSun.x + 18,translateSun.y +2, translateSun.z + 5};
        //mat4 transMatSphere = T(translateSphere.x,translateSphere.y,translateSphere.z);
        rotationSphereIn = (rotationSphereIn < 2*PI) ? rotationSphereIn+PI/79 : rotationSphereIn-2*PI+PI/79;
        rotationSphereOut = (rotationSphereOut < 2*PI) ? rotationSphereOut+PI/79 : rotationSphereOut-2*PI+PI/79;
        mat4 transMatSphere;




        float r;
        r = 0.1; //change this

        vec3 ScaleSunVec = {r*109.3, r*109.3, r*109.3};
        mat4 scaleSun = S(ScaleSunVec.x, ScaleSunVec.y, ScaleSunVec.z); //correct scaling
        mat4 scaleMercury = S(r*0.3829, r*0.3829, r*0.3829);
        mat4 scaleVenus = S(r*0.9499, r*0.9499, r*0.9499);
        mat4 scaleTellus = S(r*1, r*1, r*1);
        mat4 scaleMars = S(r*0.5320, r*0.5320, r*0.5320);
        mat4 scaleJupiter = S(r*10.97, r*10.97, r*10.97);
        mat4 scaleSaturn = S(r*9.14, r*9.14, r*9.14);
        mat4 scaleUranus = S(r*3.981, r*3.981, r*3.981);
        mat4 scaleNeptune = S(r*3.865, r*3.865, r*3.865);

        mat4 scale[] = {scaleSun, scaleMercury, scaleVenus, scaleTellus, scaleMars, scaleJupiter, scaleSaturn, scaleUranus, scaleNeptune};
        glUniform3fv(glGetUniformLocation(program, "SunPos"), 1, &ScaleSunVec.y);

        GLfloat objRadii = 1.2;
        GLfloat radii[] = {objRadii*scale[0].m[0], objRadii*scale[1].m[0], objRadii*scale[2].m[0], objRadii*scale[3].m[0], objRadii*scale[4].m[0], objRadii*scale[5].m[0],
            objRadii*scale[6].m[0], objRadii*scale[7].m[0], objRadii*scale[8].m[0]};



            float t;
            t = 30; //change this
            GLfloat distanceFromSun[] = {0, t*0.39, t*0.723, t*1, t*1.524, t*5.203, t*9.539, t*19.18, t*30.06};

            mat4 translateSun = T(0, 15, 0);
            mat4 translateMercury = T(t*0.39, ScaleSunVec.y + translateSun.m[7]-10, 0);
            mat4 translateVenus = T(t*0.723, ScaleSunVec.y + translateSun.m[7]-10, 0);
            mat4 translateTellus = T(t*1, ScaleSunVec.y + translateSun.m[7]-10, 0);
            mat4 translateMars = T(t*1.524, ScaleSunVec.y + translateSun.m[7]-10, 0);
            mat4 translateJupiter = T(t*5.203, ScaleSunVec.y + translateSun.m[7]-10, 0);
            mat4 translateSaturn = T(t*9.539, ScaleSunVec.y + translateSun.m[7]-10, 0);
            mat4 translateUranus = T(t*19.18, ScaleSunVec.y + translateSun.m[7]-10, 0);
            mat4 translateNeptune = T(t*30.06, ScaleSunVec.y + translateSun.m[7]-10, 0);


            mat4 translation[] = {translateSun, translateMercury, translateVenus, translateTellus, translateMars, translateJupiter, translateSaturn, translateUranus, translateNeptune};


            rotTellusIn = tSpeed;


            GLfloat rotationIn[] = {0.04087, 0.01705 , -0.004115, 1, 0.9756, 2.4242, 2.2430, -1.3953, 1.4907}; //24/period of rotation

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

            printError("planetloop");
            glUseProgram(program);
            glUniformMatrix4fv(glGetUniformLocation(program, "projectionMatrix"), 1, GL_TRUE, projectionMatrix);
            printError("projectionMatrix");
            glUniformMatrix4fv(glGetUniformLocation(program, "lookAtMat"), 1, GL_TRUE, lookAtMat.m);
            printError("lookAtMatrix");

            size_t planet;
            vec3 finalTranslation = {0, 0, 0};
            vec3 celestialTranslation[] = {finalTranslation, finalTranslation, finalTranslation, finalTranslation, finalTranslation, finalTranslation, finalTranslation, finalTranslation, finalTranslation};


            for (planet = 0; planet < 9; planet++)
            {
                if (planet == 0) //if sun
                {
                    glUniform3fv(glGetUniformLocation(program, "lightSourcesColorArr"), 1, &lightSourcesColorsArrSun.x);
                }
                else
                {
                    glUniform3fv(glGetUniformLocation(program, "lightSourcesColorArr"), 1, &lightSourcesColorsArr.x);
                }

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
            init();
            glutDisplayFunc(display);
            OnTimer(0);
            glutMainLoop();
            return 0;
        }
