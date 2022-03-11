#include <GL/glew.h>
#include "include/shaders.h"
#include "include/main.h"
#include <iostream>
const char* fragmentShaderCode = R"gsls(
        #version 150 core
        in vec3 color;
        out vec4 outColor;
        void main(){
                outColor = vec4(color, 1.0);
        }
)gsls";

const char* vertexShaderCode = R"gsls(
        #version 150 core
        in vec2 Position;
        in vec3 Color;
        out vec3 color;
        void main(){
                gl_Position = vec4(Position, 0.0, 1.0);
                color = Color;
        }
)gsls";

//draws the box
GLuint setupProgram(GLuint& vertexShader,GLuint& fragmentShader ){
        vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertexShaderCode, NULL);
        glCompileShader(vertexShader);

        fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragmentShaderCode, NULL);
        glCompileShader(fragmentShader);

        GLuint shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader); 
        
        glBindFragDataLocation(shaderProgram, 0, "outColor");
        glLinkProgram(shaderProgram);
        glUseProgram(shaderProgram);
        return shaderProgram;
}
void deleteProgram(GLuint& vertexShader,GLuint& fragmentShader, GLuint& shaderProgram){
        glDeleteProgram(shaderProgram);
        glDeleteShader(fragmentShader);
        glDeleteShader(vertexShader);
}
void drawBox(){
        // every vertex is defined as 5-tuple: (x_pos, y_pos, red, green, blue)
        GLfloat verticies[] = {
                -1.0f, -1.0f, 1.0f, 1.0f, 1.0f,
                -0.95f, -1.0f, 1.0f, 1.0f, 1.0f,
                -1.0f, -0.95f, 1.0f, 1.0f, 1.0f,
                //bot right
                 1.0f, -1.0f, 1.0f, 1.0f, 1.0f,
                 0.95f, -1.0f, 1.0f, 1.0f, 1.0f,
                 1.0f,  -0.95f, 1.0f, 1.0f, 1.0f,
                //top right
                 1.0f,  1.0f, 1.0f, 1.0f, 1.0f,
                 0.95f,  1.0f, 1.0f, 1.0f, 1.0f,
                 1.0f,  0.95f, 1.0f, 1.0f, 1.0f,
                //top left
                -1.0f,  1.0f, 1.0f, 1.0f, 1.0f,
                -0.95f,  1.0f, 1.0f, 1.0f, 1.0f,
                -1.0f,  0.95f, 1.0f, 1.0f, 1.0f
        };
        glBufferData(GL_ARRAY_BUFFER, sizeof(verticies), verticies, GL_STATIC_DRAW);

        GLuint ebo;
        GLuint elements[] = {
                0,3,2,
                5,3,2,
                0,9,10,
                0,1,10,
                9,11,6,
                6,8,11,
                6,7,3,
                3,4,7
        };
        glGenBuffers(1,&ebo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);

       
        GLuint vertexShader, fragmentShader;
        GLuint shaderProgram = setupProgram(vertexShader, fragmentShader);
        
        GLint posAttrib = glGetAttribLocation(shaderProgram, "Position");
        glEnableVertexAttribArray(posAttrib);
        glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), 0);

        GLint colorAttrib = glGetAttribLocation(shaderProgram, "Color");
        glEnableVertexAttribArray(colorAttrib);
        glVertexAttribPointer(colorAttrib, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)(2*sizeof(float)));
        glDrawElements(GL_TRIANGLES, 24, GL_UNSIGNED_INT, 0);

        deleteProgram(vertexShader, fragmentShader, shaderProgram);
        glDeleteBuffers(1, &ebo);
}
void drawSnekElm(SnekElm *elm){
        float x_transform = (elm->x_position)/19.0;
        float y_transform = (elm->y_position)/19.0; 
        GLfloat verticies[] = {
                (x_transform) + BOX_SIZE/2, (y_transform) + BOX_SIZE/2, 1.0f, 1.0f, 1.0f,
                (x_transform) + BOX_SIZE/2, (y_transform) - BOX_SIZE/2, 1.0f, 1.0f, 1.0f,
                (x_transform) - BOX_SIZE/2, (y_transform) + BOX_SIZE/2, 1.0f, 1.0f, 1.0f,
                (x_transform) - BOX_SIZE/2, (y_transform) - BOX_SIZE/2, 1.0f, 1.0f, 1.0f
        };
        glBufferData(GL_ARRAY_BUFFER, sizeof(verticies), verticies, GL_STATIC_DRAW);

        GLuint ebo;
        GLuint elements[] = {
                0,1,2,
                2,3,1
        };
        glGenBuffers(1,&ebo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);
        
        GLuint vertexShader, fragmentShader;
        GLuint shaderProgram = setupProgram(vertexShader, fragmentShader);
        GLint posAttrib = glGetAttribLocation(shaderProgram, "Position");
        glEnableVertexAttribArray(posAttrib);
        glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), 0);

        GLint colorAttrib = glGetAttribLocation(shaderProgram, "Color");
        glEnableVertexAttribArray(colorAttrib);
        glVertexAttribPointer(colorAttrib, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)(2*sizeof(float)));

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glDeleteBuffers(1, &ebo);
        deleteProgram(vertexShader,fragmentShader, shaderProgram);

}
//draws the snake
void drawSnek(Snek *snek){
        GLuint vertexShader, fragmentShader;
        for (auto elm:(snek->body)){
                drawSnekElm(elm);
        }
}
//draws a munchie
void drawMunchie(int x_pos, int y_pos){
        GLfloat verticies[] = { //I LOVE MAGIC NUMBERS! rgb = (0.33, 0.33, 0.33) should give grey
                x_pos/19.0f + 0.025f, y_pos/19.0f + 0.025f, 0.33f,0.33f,0.33f,
                x_pos/19.0f + 0.025f, y_pos/19.0f - 0.025f, 0.33f,0.33f,0.33f, 
                x_pos/19.0f - 0.025f, y_pos/19.0f + 0.025f, 0.33f,0.33f,0.33f, 
                x_pos/19.0f - 0.025f, y_pos/19.0f - 0.025f, 0.33f,0.33f,0.33f
        };
        glBufferData(GL_ARRAY_BUFFER, sizeof(verticies), verticies, GL_STATIC_DRAW);

        GLuint ebo;
        GLuint elements[] = {
                0,1,2,
                2,3,1
        };
        glGenBuffers(1,&ebo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);
        
        GLuint vertexShader, fragmentShader;
        GLuint shaderProgram = setupProgram(vertexShader, fragmentShader);
        GLint posAttrib = glGetAttribLocation(shaderProgram, "Position");
        glEnableVertexAttribArray(posAttrib);
        glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), 0);

        GLint colorAttrib = glGetAttribLocation(shaderProgram, "Color");
        glEnableVertexAttribArray(colorAttrib);
        glVertexAttribPointer(colorAttrib, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)(2*sizeof(float)));

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glDeleteBuffers(1, &ebo);
        deleteProgram(vertexShader,fragmentShader, shaderProgram);
}