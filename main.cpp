#include <iostream>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// GLM
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

// Classes
#include "ShaderProgram.hpp"
#include "Camera.h"
#include "MeshObject.h"
#include "Texture.h"

/*******************************************
 ****** FUNCTION/VARIABLE DECLARATIONS *****
 *******************************************/

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

GLuint createMasterHairs(const MeshObject& object);

// Window dimensions
const GLuint WIDTH = 1000, HEIGHT = 600;

// Camera variables
Camera camera(glm::vec3(0.0f, 0.0f, 10.0f));
float lastX = WIDTH / 2.0f;
float lastY = HEIGHT / 2.0f;
bool firstMouse = true;

// Time variables
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

// Hair/fur variables
int maxNoofTesselatorInstances = 64;
int hairBladeInstancesPerUnitArea = 8;
int noofHairSegments = 4;
float hairSegmentLength = 5.0f;
float hairWidthScale = 0.01f;
float hairDisplacementScale = 5.0f;
float hairRadius = 1;

int noOfMasterHairs;


/*******************************************
 **************    MAIN     ****************
 *******************************************/

// The MAIN function, from here we start the application and run the rendering loop
int main()
{
    std::cout << "Starting GLFW context, OpenGL 4.0 or higher" << std::endl;
    // Init GLFW
    if(!glfwInit()) {
        std::cout << "Failed to initialise GLFW" << std::endl;
        return 1;
    }

    // Set all the required options for GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // Create a GLFWwindow object that we can use for GLFW's functions
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Real-time rendering of fur", NULL, NULL);

    if (window == nullptr){
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // GLFW Options
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
    glewExperimental = GL_TRUE;

    // Initialize GLEW to setup the OpenGL Function pointers
    if (glewInit() != GLEW_OK) {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return -1;
    }

    /************** Callback functions *************/
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    /***************** Shaders ********************/
    // Build and compile the shader programs

    // Plain shader for rendering the object as it is
    std::string plainVertexFilename = "../shaders/plainShader.vert";
    std::string plainFragmentFilename = "../shaders/plainShader.frag";
    ShaderProgram plainShader(plainVertexFilename, "", "", "", plainFragmentFilename);
    plainShader();

    // Fur shader for rendering the hair strands on the object
    std::string furVertexFilename = "../shaders/furShader.vert";
    std::string furTessControlFilename = "../shaders/furShader.tc";
    std::string furTessEvaluateFilename = "../shaders/furShader.te";
    std::string furGeometryFilename = "../shaders/furShader.gs";
    std::string furFragmentFilename = "../shaders/furShader.frag";
    ShaderProgram furShader(furVertexFilename, furTessControlFilename, furTessEvaluateFilename, furGeometryFilename, furFragmentFilename);
    furShader();

    /****************** Models ********************/

    MeshObject triangle;
    triangle.createTriangle();

    //MeshObject box;
    //box.createBox(2.0, 2.0, 2.0);

    //MeshObject sphere;
    //sphere.createSphere(2.0, 40);

    // MeshObject bunny;
    // bunny.readOBJ("../objects/bunny.obj");

    // MeshObject trex;
    // trex.readOBJ("../objects/trex.obj");

    Texture mainTexture = Texture("../textures/lightbrown.tga");
    // Texture textureTrex = Texture("../textures/trex.tga");

    /****************** Hair/Fur ******************/

    // TODO: Create master hairs using a compute shader instead. This makes it possible to also simulate the hairs.
    GLuint hairDataTextureID = createMasterHairs(triangle);

    /**************** Uniform variables **********************/
    GLint viewLocPlain = glGetUniformLocation(plainShader, "view");
    GLint projLocPlain = glGetUniformLocation(plainShader, "projection");

    GLint viewLocFur = glGetUniformLocation(furShader, "view");
    GLint projLocFur = glGetUniformLocation(furShader, "projection");

    GLint mainTextureLoc = glGetUniformLocation(furShader, "mainTexture");
    GLint hairDataTextureLoc = glGetUniformLocation(furShader, "hairDataTexture");
    glUniform1i(mainTextureLoc,0);
    glUniform1i(hairDataTextureLoc, 1);

    GLint noOfHairSegmentLoc = glGetUniformLocation(furShader, "noOfHairSegments");
    GLint noOfVerticesLoc = glGetUniformLocation(furShader, "noOfVertices");
    glUniform1f(noOfHairSegmentLoc, (float)noofHairSegments);
    glUniform1f(noOfVerticesLoc, (float)noOfMasterHairs);

    /****************************************************/
    /******************* RENDER LOOP ********************/
    /****************************************************/

    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);

        // OpenGL settings
        glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);

        // Create camera transformation
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)WIDTH/(float)HEIGHT, 0.1f, 100.0f);

        /****************************************************/
        /******************* RENDER STUFF *******************/

        /***************** Render it plain ******************/
        /*
        plainShader();
        glUniformMatrix4fv(viewLocPlain, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLocPlain, 1, GL_FALSE, glm::value_ptr(projection));
        triangle.render(false);
         */

        /***************** Render with fur ******************/
        furShader();

        glUniformMatrix4fv(viewLocFur, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLocFur, 1, GL_FALSE, glm::value_ptr(projection));

        // Main texture (for colour)
        glActiveTexture(GL_TEXTURE0 + 0); // Texture unit 0
        glBindTexture(GL_TEXTURE_2D, mainTexture.textureID);

        // Hair data saved in texture
        glActiveTexture(GL_TEXTURE0 + 1); // Texture unit 1
        glBindTexture(GL_TEXTURE_2D, hairDataTextureID);

        triangle.render(true);

        /****************************************************/
        /****************************************************/

        // Swap front and back buffers
        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    // Terminate GLFW, clearing any resources allocated by GLFW.
    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glfwGetWindowSize( window, &width, &height );
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    camera.ProcessMouseScroll(yoffset);
}

GLuint createMasterHairs(const MeshObject& object){
    GLfloat* vertexArray = object.getVertexArray();
    noOfMasterHairs = object.getNoOfVertices();

    GLfloat* hairData = new GLfloat[noOfMasterHairs * (noofHairSegments * 9)];

    int masterHairIndex = 0;
    int stride = 8; // 8 because the vertexArray consists of (vertex (3), normal (3), tex (2))
    for(int i = 0; i < noOfMasterHairs*stride; i = i+stride){
        glm::vec3 rootPos = glm::vec3(vertexArray[i], vertexArray[i+1], vertexArray[i+2]);
        glm::vec3 rootNormal = glm::vec3(vertexArray[i+3], vertexArray[i+4], vertexArray[i+5]);

        // Add hairVertices
        for(int hairSegment = 1; hairSegment <= noofHairSegments; hairSegment++){
            // Add position
            glm::vec3 newPos = rootPos + hairSegment*hairSegmentLength*rootNormal;
            hairData[masterHairIndex++] = newPos.x;
            hairData[masterHairIndex++] = newPos.y;
            hairData[masterHairIndex++] = newPos.z;

            // Add normal TODO: CHANGE THIS TO CORRECT NORMAL
            hairData[masterHairIndex++] = vertexArray[i+3];
            hairData[masterHairIndex++] = vertexArray[i+4];
            hairData[masterHairIndex++] = vertexArray[i+5];

            // Add texture coordinates
            hairData[masterHairIndex++] = vertexArray[i+6];
            hairData[masterHairIndex++] = vertexArray[i+7];
            hairData[masterHairIndex++] = 0.0f;
        }
    }

    GLuint hairDataTextureID;
    glGenTextures(1, &hairDataTextureID);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, hairDataTextureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, noofHairSegments * 3, noOfMasterHairs, 0, GL_RGB, GL_FLOAT, hairData);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    return hairDataTextureID;
}
