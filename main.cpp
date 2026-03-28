#include <print>
#include <cmath>
#include <iostream>
#include <cassert>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

struct Vector2 {
    float x, y;
};

struct Vector4 {
    float r, g, b, a;
};

struct Vertex {
    Vector2 pos;
    Vector4 color;
};

// this is a vertex shader
const char* vx = R"glsl(
// version of glsl (opengl)
// 330 = 3.3.0
#version 330 core

layout (location = 0) in vec2 pos; // this is the data the VAO sends to gpu
layout (location = 1) in vec4 color; // this is the data the VAO sends to gpu

out vec4 pixC; // sending out vec4 called pixC

void main() { // there are other drawing modes? GL_POINTS, and i think there's GL_QUADS
    pixC = color; // try now
    // ran but nothing drawn now the depressing part of opengl
// oh so we dont need to declare it lmao
// whath appens in this part
    // gl_Position is a variable given to us by opengl
    gl_Position = vec4(pos, 0, 1);
}
)glsl";


const char* pix = R"glsl(
#version 330 core

in vec4 pixC;

// output pixel layout
out vec4 color;

void main() {
    color = pixC; // try now // vec4(0, 0, 0, 1);
}
)glsl";
// wwjat the fuck :Sob: felt like jumpsc
// i pas// so opengl is like a submissive little girl

std::string error;
unsigned int glCheckError_(const char* file, int line) {
    unsigned int errorCode;
    while ((errorCode = glGetError()) != GL_NO_ERROR) {
        switch (errorCode) {
            case GL_INVALID_ENUM: error = "INVALID_ENUM"; break;
            case GL_INVALID_VALUE: error = "INVALID_VALUE"; break;
            case GL_INVALID_OPERATION: error = "INVALID_OPERATION"; break;
            case GL_OUT_OF_MEMORY: error = "OUT_OF_MEMORY"; break;
            case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
            default: error = "UNKNOWN_ERROR"; break;
        }
        std::println("OpenGL Error [{}] {} | {}:{}", errorCode, error, file, line);
        assert(!errorCode);
    }
    return errorCode;
}

// alright installed
int main() { 
    /* 
        glfwInit() - used to initialize GLFW
        @returns GLFW_TRUE (1/true) if successful and GLFW_FALSE (0/false) if not
    */
    if (!glfwInit()) return 1;
    
    /*
        glfwCreateWindow(int w, int h, char* name, int* monitor, int* shared) - Window creation function
        @returns GLFWwindow*
        @note keep the last two nullptr, they're usually useless
    */
    GLFWwindow* window = glfwCreateWindow(300,300,"Hello world",NULL,NULL);

    if (!window){
        glfwTerminate();
        return 1;
    }

    glfwMakeContextCurrent(window);

    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    // glCreateShader allocates a place for our shader
    int vertexCompiled; // opengl uses int pointers, but inside this is just a bool 0 1? yup alright 
    auto vertex = glCreateShader(GL_VERTEX_SHADER); // allocates a place for our vertex shader
    glShaderSource(vertex, 1, &vx, nullptr); // sets the code for our vertex shader
    glCompileShader(vertex); // compiles the shader
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &vertexCompiled); // we check if it compiled successfully

    if(!vertexCompiled){
        int length; // error msg length
        glGetShaderiv(vertex, GL_INFO_LOG_LENGTH, &length); // getting the length from opengl
        // why not std::string ? whats STL oh but why though
        // what abt char* ? unsafe in cpp? doesnt say anything js the comment u wrote
        // alright
        char infoLog[512]; // this is a C layer, it doesnt use smart STL containers 
        glGetShaderInfoLog(vertex, length + 1, nullptr, infoLog); // compile error message, basically allocating it on the go? or maybe using memcpy
        std::println("Error compiling vertex shader: {}", infoLog);
    }

    // same thing but this is the pixel shader
    int fragmentCompiled;
    auto fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &pix, nullptr);
    glCompileShader(fragment);
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &fragmentCompiled);

    
    // alright
    if(!fragmentCompiled){
        int length; // error msg length
        glGetShaderiv(fragment, GL_INFO_LOG_LENGTH, &length); // getting the length from opengl
        char infoLog[512]; // i see
        glGetShaderInfoLog(fragment, length + 1, nullptr, infoLog); // compile error message, basically allocating it on the go? or maybe using memcpy
        std::println("Error compiling fragment shader: {}", infoLog);
    } // i was ina hurry when u wrote that part of code

    // now that they both compiled
    // whhats autoutomatically choose type // o alright
    GLuint program;
    if(vertexCompiled && fragmentCompiled){
        program = glCreateProgram(); // create new shader program (gpu executable)

        glAttachShader(program, vertex); // attach vertex shader we want for this program
        glAttachShader(program, fragment); // attach pixel shader we want for program
        glLinkProgram(program); // we link both of them (same thing as doing gcc for c programs or ld)
        glValidateProgram(program); // check for errors
        int success; // i see
        glGetProgramiv(program, GL_LINK_STATUS, &success);

        if(!success) {
            char infoLog[512];

            glGetProgramInfoLog(program, 512, nullptr, infoLog);

            std::println("Failed to link program: {}", infoLog);
            return 1;
        }
    } else return 1; // we built the shaders now
    uint32_t vao;
    glGenVertexArrays(1, &vao); // empty vao i guess
    
    // alright now we need to tell the VAO how the vertices look like
    
    // so our vertex is data the gpu doesn't know how to use, we're basically gonan help it know how to use it
    // here we tell it, at layout position 0, there will be a float, not normalized (ndc = native device coordinates aka -1 -> 1) alright
    // the issue most likely here
    // lemme quickly docs.gl ts alrightt
    // try now
    // it failed to compile
    // wait
    // just warnings
    unsigned int offset = 0;
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<const void*>(offset)); // this sets the vertex attributes 
    offset += sizeof(GLfloat) * 2;
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<const void*>(offset)); // this sets the vertex attributes 
    glEnableVertexAttribArray(0); // lets go back to the top
    glEnableVertexAttribArray(1); // lets go back to the top
    glBindVertexArray(vao); // lets set up the VAO to not be just empty
    glCheckError_(__FILE__,__LINE__);
    //OpenGL Error [1281] INVALID_VALUE | /home/yas/Documents/trash/cpp-project/main.cpp:178
    // same
    // i can see the console btw
    std::println("Vertex Array made!"); // try rebuilding
    
    uint32_t vbo;
    glGenBuffers(1, &vbo);
    
    Vertex verts[3] = {
        { {  0.0,  0.5 }, { 1, 0, 0, 1 } },
        { {  0.5, -0.5 }, { 0, 1, 0, 1 } },
        { { -0.5, -0.5 }, { 0, 0, 1, 1 } }
    };
    
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * 3, verts, GL_STATIC_DRAW);
    // idk how to make one i never drank one 
    //i feel so sleepy then rebuild hmm ts is wild
    // can u upload this to github wait
    // same thing
    // wanna do t tmr im so sleepy hm alright sure 1s
    // vertex arryay made but nothing printed on scren only the rainbow
    while(!glfwWindowShouldClose(window)){ // time for shaders
        // alright bet
        
        glClear(GL_COLOR_BUFFER_BIT);
        float dt = glfwGetTime()/15;
        glClearColor(1, 1, 1, 1); // test again
        // glClearColor(fmod(dt * 8, 2),fmod(dt * 5, 2),fmod(dt * 7, 2),1); // this is technically valid     
        // white screen
        glUseProgram(program); // sets curr
        glDrawArrays(GL_TRIANGLES, 0, 3); // try now fire lets continue
        // r u sure we r drawing htem
        glfwSwapBuffers(window);
        glfwPollEvents(); 
        glCheckError_(__FILE__, __LINE__); // if there was an issue ts would've fired, hmm
        // run ts
        // same thing
    }// alright
    // black screen it crashed
    //OpenGL Error [1281] INVALID_VALUE | /home/yas/Documents/trash/cpp-project/main.cpp:196
    //main: /home/yas/Documents/trash/cpp-project/main.cpp:71: unsigned int glCheckError_(const char*, int): Assertion `!errorCode' failed.
    //fish: Job 1, './build/main' terminated by signal SIGABRT (Abort)
     /*
        glfwTerminate() - used to deinitialize GLFW
        @returns nothing.
    */
    glfwTerminate();
    return 0;
}

