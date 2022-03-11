#define GLEW_STATIC
#include <iostream>
#include <chrono>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <GL/glew.h>
#include <thread>
#include <vector>
#include "include/shaders.h"
#include "include/main.h"


//grid with discrete locations 1 mean there is an object
float tick_time;
GLuint vbo;
GLuint vao;
int play_field[38][38];
bool running = true;
direction cannot = RIGHT;
std::pair<int, int>  munchiePositions;

void printField(){
        for (int i = 0; i < 38; i++){
                for (int j = 0; j <38; j++){
                        std::cout << play_field[i][j] << " "; 
                }
                std::cout<<"\n";
        }
        std::cout << "\n";
}
void Tick(Snek *snek, sf::Window *window){
        while (1337){
                status stat = NOTHING;
                stat = snek->updatePositions(); // also returns a status which tells us if there were any collisions
                switch (snek->body[0]->smjer){
                        case UP:
                                cannot = DOWN;
                                break;
                        case DOWN:
                                cannot = UP;
                                break;
                        case LEFT:
                                cannot = RIGHT;
                                break;
                        case RIGHT:
                                cannot = LEFT;
                                break;
                }
                //std::cout<<"\n";
                glClear(GL_COLOR_BUFFER_BIT);
                glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
                drawBox();

                drawSnek(snek);
                drawMunchie(munchiePositions.first, munchiePositions.second);
                window->display();
                switch (stat){
                        case COLLISION:
                                running = false;
                                return;
                        case NOTHING:
                                break;
                        case ATE:
                                //Increase snek size and add +1 points
                                snek->grow();
                                //Setup new munchie
                                munchiePositions = setupMunchie();
                                drawMunchie(munchiePositions.first, munchiePositions.second);
                                //printField();
                                break;
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(200));                
        }
}

int main(void){
        //setting up window settings
        sf::ContextSettings settings;
        settings.depthBits = 24;
        settings.stencilBits = 8;
        settings.antialiasingLevel = 2;
        settings.majorVersion = 3;
        settings.minorVersion = 2;
        //opening a window
        sf::Window window(sf::VideoMode(0, 0, 32), "OpenGL", sf::Style::Titlebar | sf::Style::Close | sf::Style::Fullscreen, settings);



        glewExperimental = GL_TRUE;
        glewInit();
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);

        
        Snek snek;
        snek.body.push_back(new SnekElm(0,0,RIGHT));
        snek.body.push_back(new SnekElm(-1,0,RIGHT));
        snek.body.push_back(new SnekElm(-2,0,RIGHT));
        snek.body.push_back(new SnekElm(-3,0,RIGHT));
        snek.body.push_back(new SnekElm(-4,0,RIGHT));

        play_field[19][19] = play_field[19][18] = play_field[19][17] =play_field[19][16] = play_field[19][15]=1; 
        munchiePositions = setupMunchie();
        for (int i = 0; i < 38; i++){
                play_field[i][0] = 1;
                play_field[0][i] = 1;
                play_field[i][37] = 1;
                play_field[37][i] = 1;
        }
        std::thread Tick_thread(Tick, &snek, &window);
        while (running){
                sf::Event windowEvent;
                while (window.pollEvent(windowEvent)){
                        switch (windowEvent.type){
                        case sf::Event::Closed:
                                running = false;
                                break;
                        case sf::Event::KeyPressed:
                                if (windowEvent.key.code == sf::Keyboard::Escape)
                                        running = false;
                                else if(windowEvent.key.code == sf::Keyboard::Up && UP != cannot)
                                        snek.body[0]->smjer = UP;
                                else if(windowEvent.key.code == sf::Keyboard::Down && DOWN != cannot)
                                        snek.body[0]->smjer = DOWN;
                                else if(windowEvent.key.code == sf::Keyboard::Left && LEFT != cannot)
                                        snek.body[0]->smjer = LEFT;
                                else if(windowEvent.key.code == sf::Keyboard::Right && RIGHT != cannot)
                                        snek.body[0]->smjer = RIGHT;
                                break;
                        }
                }
        }
        glDeleteBuffers(1, &vao);
        glDeleteBuffers(1, &vbo);
        window.display();        
}