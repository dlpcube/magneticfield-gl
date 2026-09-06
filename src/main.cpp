#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <shaders/shader.h>
#include <classes/camera.h>
#include <glm/ext.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

// Include this in a separate c++ files
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include <cmath>
#include <iostream>



void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);
void addVertex(glm::vec3 pos, float phi, float theta);
unsigned int loadTexture(const char *path);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing

float deltaTime = 0.0f;
float lastFrame = 0.0f;

// lighting
// glm::vec3 lightPos(1.2f, 1.0f, 2.0f);


int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST);

    Shader lightingShader("dependencies/include/shaders/colors.vs", "dependencies/include/shaders/colors.fs");
    Shader lightCubeShader("dependencies/include/shaders/light_cube.vs", "dependencies/include/shaders/light_cube.fs");
    Shader lineShader("dependencies/include/shaders/line.vs", "dependencies/include/shaders/line.fs");
    Shader sphereShader("dependencies/include/shaders/sphere.vs", "dependencies/include/shaders/sphere.fs");
    Shader planeShader("dependencies/include/shaders/plane.vs", "dependencies/include/shaders/plane.fs");

    float planeVerticies[] = {
        4.0f, 2.0f, 0.0f,
        4.0f,-2.0f,0.0f,
        0.0f, -2.0f, 0.0f,
        0.0f, 2.0f, 0.0f,
        
    };
    unsigned int planeIndicies[] ={
        0,1,3,
        1,2,3
    };
    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float start = 0;
    float end = 0.2f;
    glm::vec3 p1(start, 0.0f, 0.0f);
    glm::vec3 p2(end, 0.0f, 0.0f);
    float lineDist = 1.0f;
    // Line Verticies
    glm::vec2 lverticies[] = {
        p1, p2   // end point
    };

    glm::vec3 linePos[300] = {};
    float width = 2.0f;
    float heightB = 2.0f;
    float numCol = 5.0f;
    float numRows = 4.0f;
    float lengthB = 10.0f;
    float dx = lineDist;
    float dz = width / numCol;
    float dy = heightB / numRows;
    float x, y, z; // x naught, y naught, z naught
    x = 0;
    y = 0;
    z = 0;
    int counter = 0;

    glm::vec3 originPos(0.0f, 0.0f, 0.0f);

    // outer dx loop
    for (int i = 0; i < 10; i++)
    {
        // inner yz-plane loop 
        // rows
        for (int i = 0; i < 3; i++)
        {
            
            linePos[counter] = glm::vec3(x,y,z);
            counter++;
            // so at each col iterate through the entire row
            // col loop
            // at each row, iterate 5 times 
            for (int i = 0; i < 4; i++)
            {   
                z = z + dz;
                linePos[counter] = glm::vec3(x,y,z);
                counter ++;
            }
            z = 0;
            y =  y + dy;
            // linePos[counter] = glm::vec3(x,y,z);
            // z = 0;
            // counter ++;
        }
        x = x + dx;
        y = 0;
        z = 0;
        linePos[counter] = glm::vec3(x,y,z);
        z = 0;
        counter ++;

        
    };

    //250 items
    // returning the position values (for debugging)
    for (int i = 0; i < 301; i++)
    {
        std::cout<<glm::to_string(linePos[i]);
        printf("i value of %d \n", i);
    }
    
    

    //--------------------------
    // CREATION OF SPHERE VERTICIES (REMEMBER VBO AND VAO FOR SPHERE)
    // int res = 50; // no of rows and cols
    // float circleStep = glm::two_pi<float>() / (float)res;
    // float heightStep = glm::pi<float>() / (float)res;

    // int row = 0;
    // float phi = 0;
    // float theta = 0;
    // float R = 1; // radius
    // x = R * sin(phi) * sin(theta);
    // y = R * cos(phi);
    // z = R * sin(phi) * cos(theta);
    // for(; phi < glm::pi<float>() + heightStep; phi += heightStep, row++)
    // {
    //     y = R * glm::sin(phi);
    //     int cell = 0;
    //     for (float theta = 0; theta < glm::two_pi<float>(); theta += circleStep, cell++)
    //     {
            
    //     }
        
    // }


    

    // Cube Verticies
    float vertices[] = {
        // positions         // normals        // texture coords
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
         0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 
         0.5f,  0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f, 
         0.5f,  0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f, 
        -0.5f,  0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
         0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 
         0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 
         0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 
        -0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

         0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 
         0.5f,  0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 
         0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 
         0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 
         0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 
         0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 

        -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,
         0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f, 
         0.5f, -0.5f,  0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 
         0.5f, -0.5f,  0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 
        -0.5f, -0.5f,  0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
         0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 
         0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 
         0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 
        -0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f
    };

    // plane VBO and VAO
    unsigned int planeVBO, planeVAO, planeEBO;
    glGenVertexArrays(1, &planeVAO);
    glGenBuffers(1, &planeVBO);
    glGenBuffers(1, &planeEBO);
    glBindVertexArray(planeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, planeEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(planeIndicies), planeIndicies, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0); 

    glBindVertexArray(0); 
    
    // ---------------------------------------------------------------------------


    // Line VBO and VAO
    unsigned int lVBO, lVAO;
    glGenVertexArrays(1, &lVAO);
    glGenBuffers(1, &lVBO);

    glBindVertexArray(lVAO);

    glBindBuffer(GL_ARRAY_BUFFER, lVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(lverticies), lverticies, GL_STATIC_DRAW);

    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE, 3* sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glClearColor(1.0f,1.0f,1.0f,1.0f);
    // ---------------------------------------------------------------------------
    // Cube VBO and VAO
    unsigned int VBO, cubeVAO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &VBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(cubeVAO);
    // position attribute
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture
    glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);




    // Light Source Cube VAO & VBO
    unsigned int lightCubeVAO;
    glGenVertexArrays(1, &lightCubeVAO);
    glBindVertexArray(lightCubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO); 


    // Forgot these two lines and the orange cube didn't render
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // loading texture
    unsigned int diffuseMap = loadTexture("imgs/container2.png");
    unsigned int specularMap = loadTexture("imgs/container2_specular.png");


    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind


    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.


    // uncomment this call to draw in wireframe polygons.
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic

        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        
        processInput(window);

        // render
        // ------
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // rendering the circle

        // GLfloat x, y, angleCircle;

        // glBegin(GL_POINTS);
        // for (angleCircle = 0.0f; angleCircle <= (2.0f * M_PI); angleCircle += 0.01f)
        // {
        //     x = 1.0f * sin(angleCircle);
        //     y = 1.0f * cos(angleCircle);
        //     glVertex3f(x,y,0.0f);
        // }
        // glEnd();

        


        // active shader when setting uniforms/drawing objects
        

        float angle = glm::radians(64*glfwGetTime());
        float xB = 3.0f;
        float yB = 0.5f;
        float zB = 1.0f*cos(angle);
        glm::vec3 lightPos(xB, yB, zB);
        float mag = glm::length(lightPos);
        float th = acosf(yB/mag)+M_PI/2;
        printf("The value of theta (cos) is currently %f\n", th);
        float phi = atan2f(xB, zB);
        printf("The value of phi (atan2) is currently %f\n", phi);
        lightingShader.use();
        lightingShader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
        lightingShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
        lightingShader.setVec3("lightPos", lightPos);
        lightingShader.setVec3("viewPos", camera.Position);
        lightingShader.setVec3("material.ambient", 1.0f, 0.5f, 0.31f);
        lightingShader.setInt("material.diffuse", 1);
        lightingShader.setInt("material.specular", 1);
        lightingShader.setFloat("material.shininess", 32.0f);
        lightingShader.setVec3("light.ambient", 0.2f, 0.2f, 0.2f);
        lightingShader.setVec3("light.diffuse", 0.5f, 0.5f, 0.5f); // darkened
        lightingShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);

        // view/projection transformation
        
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();

        // lightingShader.setMat4("projection", projection);
        // lightingShader.setMat4("view", view);

        // world transformation 

        glm::mat4 model = glm::mat4(1.0f);
        // lightingShader.setMat4("model", model);

        // texture commands

        // glActiveTexture(GL_TEXTURE0);
        // glBindTexture(GL_TEXTURE_2D, diffuseMap);
        // glActiveTexture(GL_TEXTURE1);
        // glBindTexture(GL_TEXTURE_2D, specularMap);
        // glBindVertexArray(cubeVAO);

        // //render cube
        // glBindVertexArray(cubeVAO);
        // glDrawArrays(GL_TRIANGLES, 0, 36);

        //draw xz plane
        planeShader.use();
        planeShader.setMat4("projection", projection);
        planeShader.setMat4("view", view);
        model = glm::mat4(1.0f);
        planeShader.setMat4("model", model);
        glBindVertexArray(planeVAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
        //glDrawArrays(GL_TRIANGLES, 0, 6);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        //draw lamp object

        lightCubeShader.use();
        lightCubeShader.setMat4("projection", projection);
        lightCubeShader.setMat4("view", view);
        model = glm::mat4(1.0f);
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.2f));
        lightCubeShader.setMat4("model", model);
        glBindVertexArray(lightCubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        
        // drawing line
        lineShader.use();
        lineShader.setMat4("projection", projection);
        lineShader.setMat4("view", view);


        for (int i = 0; i < 159; i++)
        {
            glm::vec3 center(0.0f,0.0f,0.0f);
            glm::vec3 axis1(glm::normalize(cross(p2 - p1, glm::vec3(0.0f,0.0f,1.0f))));
            glm::vec3 axis2(glm::normalize(cross(p2 - p1, glm::vec3(0.0f,1.0f,0.0f))));
            model = glm::mat4(1.0f);
            // model = glm::translate(model, -linePos[i]);
            model = glm::translate(model, linePos[i]);
            // model = glm::rotate(model, phi, glm::vec3(0.0f,1.0f,0.0f)); // theta z-axis
            model = glm::rotate(model, th, axis2); // theta z-axis
            model = glm::rotate(model, phi, axis1);
            // model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.0f,1.0f,0.0f));
            // model = glm::rotate(model, glm::radians(i), glm::vec3(0.0f,0.0f,1.0f));
            // model = glm::rotate(model, , glm::vec3(0.0f,1.0f,0.0f));
            lineShader.setMat4("model", model);
            glLineWidth(5);
            glDrawArrays(GL_LINES, 0,2);
        }
        // glm::vec3 center(p2-p1);
        // glm::vec3 center(0.0f,0.0f,0.0f);
        // glm::vec3 axis(cross(p2 - p1, glm::vec3(0.0f,0.0f,1.0f)));
        // model = glm::mat4(1.0f);
        // model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(1.0f,0.0f,0.0f)); // theta z-axis
        // model = glm::translate(model, -center);
        // model = glm::rotate(model, -phi, axis); // theta z-axis
        
        // // model = glm::rotate(model, th, glm::vec3(1.0f,0.0f,0.0f)); // theta z-axis
        // model = glm::translate(model, center);
            
        // lineShader.setMat4("model", model);
        // glBindVertexArray(lVAO);
        // glLineWidth(5);
        // glDrawArrays(GL_LINES, 0,2);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    //draw xz plane


    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteVertexArrays(1, &lightCubeVAO);
    glDeleteBuffers(1, &VBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(FORWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(LEFT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(RIGHT, deltaTime);
    }
    
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);


}
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

unsigned int loadTexture(char const * path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}

// void addVertex(glm::vec3 pos, float phi, float theta)
// {
//     glm::vec2 texCoord;
//     texCoord.x = theta / glm::two_pi<float>();
//     texCoord.y = (phi + glm::pi<float>()) / glm::pi<float>();

//     sphereVerticies.push_back({pos, texCoord});
// }
