#include <GL/glut.h>
#include <cmath>
#include <vector>
#include<GL/freeglut.h>

#define MENU_SUB_15_TARGET 1
#define MENU_SUB_30_TARGET 2
#define MENU_SUB_TARGET_SIZE_SMALL 3
#define MENU_SUB_TARGET_SIZE_MEDIUM 4
#define MENU_SUB_TARGET_SIZE_LARGE 5
#define MENU_GAME_EXIT 6
#define MENU_GAME_HOME 7


class Bullet {
public:
    float positionX;
    float positionY;
    float positionZ;
    float directionX;
    float directionY;
    float directionZ;
    bool active;
    int bullet_count;

    Bullet(float posX, float posY, float posZ, float dirX, float dirY, float dirZ);
    void update(float deltatime);
    bool checkCollision(float targetX,float targetY,float targetZ,float targetRadius);
};


class functions{
    public:
        void keyboard_func(unsigned char key,int x, int y);
        void mouse_func(int button,int status,int x,int y);
        void mouse_motion_func(int x, int y);
        void display();
        void drawRoom();
        void reshape(int w,int h);
        void idle();
        void display_accuracy(int max_target,int bullet_count);
        void menu_options(int option);
        void totalmenu();
        void setup_2d_ortho();
        void draw_intro();
        void display_count_of_target_and_bullets(int mxt,int bullet_count);
};

float cameraX = 0.0f;
float cameraY =2.0f;
float cameraZ = 15.0f;
float cameraAngleX = 0.0f;
float cameraAngleY = 0.0f;
float cameraSpeed = 0.1f; // Adjust camera movement speed as needed
float aspectRatio;
int windowWidth;
int windowHeight;
float cameraMinX = -9.0f;
float cameraMaxX = 9.0f;
float cameraMinZ = 15.0f;
float cameraMaxZ = 29.0f;
float targetX = 0.0f;
float targetY = 5.0f;
float targetZ = -9.0f;
float targetRadius = 0.8f; // LARGEST RADIUS
bool gameset=false;
int max_targets = 30;
bool end_of_round=false;
int numtargets=0,bullet_count=0;
std::vector<Bullet> bullets;