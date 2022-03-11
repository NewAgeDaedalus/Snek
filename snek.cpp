#include <time.h>
#include "include/main.h"
#include <iostream>
#include <math.h>
SnekElm::SnekElm(int x_pos, int y_pos, direction dir){
        x_position = x_pos;
        y_position = y_pos;
        smjer = dir;
}
SnekElm::SnekElm(int x_pos, int y_pos){
        x_position = x_pos;
        y_position = y_pos;
}
//sometimes retursn ATE when it shoudn't
status Snek::updatePositions(){
        status stat = NOTHING;
        for (int i = 0; i < body.size(); i++){
                SnekElm *elm = body[i];
                play_field[19- elm->y_position][19+ elm->x_position]-=1;
                //std::cout<<play_field[19++ elm->x_position][19++ elm->x_position]<<" ";
                switch (elm->smjer){
                        case UP:
                                elm->y_position++;
                                break;
                        case DOWN:
                                elm->y_position--;
                                break;
                        case LEFT:
                                elm->x_position--;
                                break;  
                        case RIGHT:
                                elm->x_position++;
                                break;
                }
                play_field[19-elm->y_position][19+elm->x_position]+=1;
                if (play_field[19-elm->y_position][19+elm->x_position] == 2)
                        stat = COLLISION;
                else if (play_field[19-elm->y_position][19+elm->x_position] == 0){
                        play_field[19-elm->y_position][19+elm->x_position] = 1;
                        stat = ATE;
                }
        }
        for (int i = body.size()-1; i > 0; i--){
                body[i]->smjer = body[i-1]->smjer;
        }
        return stat;
}
//needs more logic
void Snek::grow(){
        SnekElm *tail = body.back();
        SnekElm *newTail = new SnekElm(tail->x_position, tail->y_position, tail->smjer);
        switch (tail->smjer){
                case UP:
                        newTail->y_position -= 1;
                        break;
                case DOWN:
                        newTail->y_position += 1;
                        break;
                case LEFT:
                        newTail->x_position += 1;
                        break;
                case RIGHT:
                        newTail->x_position -= 1;
                        break;
        }
        play_field[19-newTail->y_position][19+newTail->x_position] = 1;
        body.push_back(newTail);
}
//Generates a munchie and returns its position on the grid
std::pair<int, int> setupMunchie(){
        time_t t;
        srand((unsigned)time(&t));
        int xpos = rand()%37-18;
        int ypos = rand()%37-18;
        while(play_field[19-ypos][19+xpos] != 0){
                xpos = rand()%37-18;
                ypos = rand()%37-18;
        }
        play_field[19-ypos][19+xpos] = -1;
        return *new std::pair<int, int> (xpos, ypos) ;      
}