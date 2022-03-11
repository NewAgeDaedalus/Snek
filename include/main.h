#include <vector>
#include <GL/glew.h>

#define BOX_SIZE 0.05f
#define PLAY_FIELD_SIZE 19 // 0.95 divided by 0.05

enum direction {RIGHT, LEFT, UP, DOWN};
enum status {NOTHING, COLLISION, ATE};
extern int play_field[38][38];

extern float tick_time;
extern GLuint vbo;
extern GLuint vao;

class SnekElm{
        public:
                int x_position;
                int y_position;
                direction smjer;
                SnekElm(int, int, direction);
                SnekElm(int, int);
};
class Snek{
        public:
                std::vector<SnekElm*> body;
                status updatePositions();
                void grow();
};

void drawBox();
void drawSnek(Snek*);
std::pair<int, int> setupMunchie();
void drawMunchie(int, int);
