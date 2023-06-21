#include <GL/glut.h>
#include <math.h>
#include<stdlib.h>
#include<iostream>
#include <vector>
#include<GL/freeglut.h>
#include<GL/glu.h>
#include<GL/gl.h>

using namespace std;

#define MENU_SUB_15_TARGET 1
#define MENU_SUB_30_TARGET 2
#define MENU_SUB_TARGET_SIZE_SMALL 3
#define MENU_SUB_TARGET_SIZE_MEDIUM 4
#define MENU_SUB_TARGET_SIZE_LARGE 5
#define MENU_GAME_EXIT 6
#define MENU_GAME_HOME 7
#define TARGET_COLOR_TEAL 8
#define TARGET_COLOR_ORANGE 9
#define TARGET_COLOR_CREAM 10
#define TARGET_COLOR_PURPLE 11
#define TARGET_COLOR_GREEN 12

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
        void game_entry();
        void draw_intro();
};

char aimlabs_text[] = "AimLabsGL";
char game_start_text[] = "Press G to start";
char targets_left_text[] = "Targets left : ";
char buffer[10];

float cameraX = 0.0f;
float cameraY =2.0f;
float cameraZ = 15.0f;
float cameraAngleX = 0.0f;
float cameraAngleY = 0.0f;
float cameraSpeed = 0.1f; // Adjust camera movement speed as needed
float aspectRatio;
float cameraMinX = -9.0f;
float cameraMaxX = 9.0f;
float cameraMinZ = 15.0f;
float cameraMaxZ = 29.0f;
float targetX = 0.0f;
float targetY = 5.0f;
float targetZ = -9.0f;
float targetRadius = 0.8f;
float target_color[3] = {0,0.392,0};

bool gameset=false;
bool end_of_round=false;
bool allow_shooting =false;

int max_targets = 30;
int windowWidth;
int windowHeight;
int numtargets=0;
int bullet_count=0;


std::vector<Bullet> bullets;
functions f;

// bullet functions
void Bullet::update(float deltaTime) {
    if (active) {
        // Move the bullet based on its direction and speed
        float speed = 100.0f; // Adjust bullet speed as needed
        positionX += directionX * speed * deltaTime;
        positionY += directionY * speed * deltaTime;
        positionZ += directionZ * speed * deltaTime;
    }
}
Bullet::Bullet(float posX,float posY,float posZ,float dirX,float dirY,float dirZ) {
    positionX=posX;
    positionY=posY;
    positionZ=posZ;
    directionX=dirX;
    directionY=dirY;
    directionZ=dirZ; 
    active=false;
}

bool Bullet::checkCollision(float targetX, float targetY, float targetZ, float targetRadius) {
    float distance = sqrt(pow(positionX - targetX, 2) + pow(positionY - targetY, 2) + pow(positionZ - targetZ, 2));
    return (distance < targetRadius);
}

// All handler functions and room related functions
void functions::mouse_func(int button,int status,int x, int y){
    if (button == GLUT_LEFT_BUTTON && status == GLUT_DOWN) {
        // Fire a bullet
        // Calculate bullet direction based on camera angles
        if(allow_shooting){
            float bulletStartX = cameraX + sin(cameraAngleY) * 0.5f;
            float bulletStartY = cameraY + sin(cameraAngleX) * 0.5f;
            float bulletStartZ = cameraZ - cos(cameraAngleY) * 0.5f;
            float bulletDirX = sin(cameraAngleY);
            float bulletDirY = sin(cameraAngleX);
            float bulletDirZ = -cos(cameraAngleY);
            bullets.emplace_back(bulletStartX + bulletDirX * 20.0f, bulletStartY + bulletDirY * 20.0f, bulletStartZ + bulletDirZ * 20.0f, bulletDirX, bulletDirY, bulletDirZ);
            bullets.back().active = true;
            bullet_count+=1;
        }
    }
}
void functions::display(){
    glClearColor(1,1,1,1);
    glClear(GL_COLOR_BUFFER_BIT);
    if(gameset){
        glClearColor(0.8,0.8,0.8,1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(40.0, aspectRatio, 0.01, 100.0);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

            // Set up the camera
        gluLookAt(cameraX, cameraY, cameraZ,           // eye position
                cameraX + sin(cameraAngleY), cameraY + sin(cameraAngleX), cameraZ - cos(cameraAngleY),   // look-at position
                0.0f, 1.0f, 0.0f);          // up direction

        // Draw the scene objects
        game_entry();
    }
    else{
        glClearColor(0,0,0,1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(40.0, aspectRatio, 0.01, 100.0);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

            // Set up the camera
        gluLookAt(cameraX, cameraY, cameraZ,           // eye position
                0, 0, 0,   // look-at position
                0.0f, 1.0f, 0.0f);
        glColor3f(1,1,0);
        // print text to prompt user to press G to start the game
        glPushMatrix();
            glTranslatef(-6.5, 0, 0);
            glScalef(0.02, 0.02, 0.02);
            for (char *p = aimlabs_text; *p; p++)
                glutStrokeCharacter(GLUT_STROKE_ROMAN, *p);
        glPopMatrix();
        glColor3f(1,0,1);
        glPushMatrix();
            glTranslatef(-3.5, -1, 0);
            glScalef(0.004, 0.004, 0.002);
            for (char *p = game_start_text; *p; p++)
                glutStrokeCharacter(GLUT_STROKE_ROMAN, *p);
        glPopMatrix();

    }
    glutSwapBuffers();
    
}

void functions::game_entry(){
    if(max_targets - numtargets>0){
        drawRoom();
    // Draw the gun
        glColor3f(target_color[0],target_color[1],target_color[2]);
        glPushMatrix();
        glTranslatef(targetX,targetY,targetZ);
        glutSolidSphere(targetRadius,20,20);
        glPopMatrix();


        glPushMatrix();
        glColor3f(1,0,0);
        glPointSize(5);
        glBegin(GL_POINTS);
            glVertex3f(cameraX + sin(cameraAngleY) * 0.5f, cameraY + sin(cameraAngleX) * 0.5f, cameraZ - cos(cameraAngleY) * 0.5f);
        glEnd();
        glPopMatrix();
        glPushMatrix();
        glTranslatef(cameraX + sin(cameraAngleY) * 0.5f, cameraY + sin(cameraAngleX) * 0.5f, cameraZ - cos(cameraAngleY) * 0.5f);
    // Assuming the gun size as a cube with side length 0.1
        glPopMatrix();
        // Draw the bullets
        glPushMatrix();
        glColor3f(1.0f, 1.0f, 0.0f); // white color for bullets
        // display number of targets left
        char *target_count = itoa(max_targets-numtargets,buffer,10);
        glPushMatrix();
        glTranslatef(0,9,-9);
        glScalef(0.004,0.004,0.002);
        for(char *p = target_count;*p;p++){
            glutStrokeCharacter(GLUT_STROKE_ROMAN,*p);
        }
        glPopMatrix();
        if(allow_shooting){
            for (auto& bullet : bullets) {
                if (bullet.active ) {
                    // increasing count of bullets at each bullet shot
                    glPushMatrix();
                    glTranslatef(bullet.positionX, bullet.positionY, bullet.positionZ);
                    glutSolidSphere(0.15f, 10, 10); // Assuming bullet radius is 0.1
                    glPopMatrix();
                    if(bullet.checkCollision(targetX,targetY,targetZ,targetRadius)){
                        bullet.active = false;
                        targetX = static_cast<float>(rand()%11 - 5);
                        targetY = static_cast<float>(rand()%6 +2);
                        
                        numtargets+=1;
                    }
                    
                }
            }
        }
        glPopMatrix();
        if(max_targets == numtargets){allow_shooting = false;} // flag to stop shooting even after round ends
    }
    else{
        drawRoom();
        allow_shooting = false;
        char round_over_text[] = "Round Over!";
        char display_accuracy_text[] = "Accuracy : ";
        char *accuracy_value_text = ltoa((max_targets*100/bullet_count),buffer,10);
        // display round over
        glPushMatrix();
        glTranslatef(-5,5,-9);
        glScalef(0.01,0.01,0.01);
        glColor3f(1,0,1);
        for(char *p=round_over_text;*p;p++){
            glutStrokeCharacter(GLUT_STROKE_ROMAN,*p);
        }
        glPopMatrix();
        // display accuracy text
        glPushMatrix();
        glTranslatef(-5,3,-9);
        glScalef(0.006,0.006,0.006);
        glColor3f(1,0,0);
        for(char *p=display_accuracy_text;*p;p++){
            glutStrokeCharacter(GLUT_STROKE_ROMAN,*p);
        }
        glPopMatrix();
        // display the round accuracy
        glPushMatrix();
        glTranslatef(0,3,-9);
        glScalef(0.006,0.006,0.006);
        glColor3f(0,1,1);
        for(char *p=accuracy_value_text;*p;p++){
            glutStrokeCharacter(GLUT_STROKE_ROMAN,*p);
        }
        glPopMatrix();
    }
}
void functions::drawRoom(){
    // Draw the floor
    glPushMatrix();
    glColor3f(0.3f, 0.3f, 0.3f); // Gray color for the floor
    glBegin(GL_QUADS);
    glVertex3f(-10.0f, 0.0f, -10.0f);
    glVertex3f(-10.0f, 0.0f, 30.0f);
    glVertex3f(10.0f, 0.0f, 30.0f);
    glVertex3f(10.0f, 0.0f, -10.0f);
    glEnd();
    glPopMatrix();
    // Draw the walls // Gray color for the walls
    glBegin(GL_QUADS);
    // Front wall
        glPushMatrix();
        glColor3f(0,0,0);
    glVertex3f(-10.0f, 0.0f, -10.0f);
    glVertex3f(-10.0f, 10.0f, -10.0f);
    glVertex3f(10.0f, 10.0f, -10.0f);
    glVertex3f(10.0f, 0.0f, -10.0f);
        glPopMatrix();
        //back wall
        glColor3f(0.2,0.2,0.2);
    glPushMatrix();
    glVertex3f(-10.0f, 0.0f, 30.0f);
    glVertex3f(-10.0f, 10.0f, 30.0f);
    glVertex3f(10.0f, 10.0f, 30.0f);
    glVertex3f(10.0f, 0.0f, 30.0f);
        glPopMatrix();

    glColor3f(0.5f, 0.5f, 0.5f);
    // Left wall
        glPushMatrix();
    glVertex3f(-10.0f, 0.0f, -10.0f);
    glVertex3f(-10.0f, 10.0f, -10.0f);
    glVertex3f(-10.0f, 10.0f, 30.0f);
    glVertex3f(-10.0f, 0.0f, 30.0f);
    // Right wall
        glPopMatrix();
        glPushMatrix();
    glVertex3f(10.0f, 0.0f, -10.0f);
    glVertex3f(10.0f, 10.0f, -10.0f);
    glVertex3f(10.0f, 10.0f, 30.0f);
    glVertex3f(10.0f, 0.0f, 30.0f);
        glPopMatrix();
    //roof
    glColor3f(0.2,0.2,0.2); 
    glPushMatrix();
    glVertex3f(-10.0f, 10.0f, -10.0f);
    glVertex3f(10.0f, 10.0f, -10.0f);
    glVertex3f(10.0f, 10.0f, 30.0f);
    glVertex3f(-10.0f, 10.0f, 30.0f);
    glPopMatrix();
    glEnd();

}
void functions::mouse_motion_func(int x,int y){
    float sensitivity = 0.001f; // Adjust mouse sensitivity as needed

    // Update camera angles based on mouse movement
    cameraAngleY -= (-x + windowWidth / 2) * sensitivity;
    cameraAngleX += (-y + windowHeight / 2) * sensitivity; // cameraAnglex += (y-windowHeignt/2) for inverted v axis

    // Limit the camera angle within certain bounds
    if (cameraAngleX > M_PI / 2.0f)
        cameraAngleX = M_PI / 2.0f;
    else if (cameraAngleX < -M_PI / 2.0f)
        cameraAngleX = -M_PI / 2.0f;

    glutWarpPointer(windowWidth / 2, windowHeight / 2); 
}
void functions::reshape(int width,int height){
    if (height == 0) height = 1;
    aspectRatio = static_cast<float>(width) / height;
    windowWidth = width;
    windowHeight = height;
    glViewport(0, 0, width, height);
}

void functions::keyboard_func(unsigned char key, int x,int y){
    float cameraMoveSpeed = 0.5f;

    if(key == 'w' || key == 'W'){
        cameraX += sin(cameraAngleY) * cameraMoveSpeed;
        cameraZ -= cos(cameraAngleY) * cameraMoveSpeed;
    }
    if(key == 's' || key == 'S'){
        cameraX -= sin(cameraAngleY) * cameraMoveSpeed;
        cameraZ += cos(cameraAngleY) * cameraMoveSpeed;
    }
    if(key == 'a' || key == 'A'){
        cameraX -= sin(cameraAngleY + M_PI / 2.0f) * cameraMoveSpeed;
        cameraZ += cos(cameraAngleY + M_PI / 2.0f) * cameraMoveSpeed;
    }
    if(key == 'd' || key == 'D'){
        cameraX += sin(cameraAngleY + M_PI / 2.0f) * cameraMoveSpeed;
        cameraZ -= cos(cameraAngleY + M_PI / 2.0f) * cameraMoveSpeed;
    }
    if(key == 'g' || key == 'G'){
        gameset = true;
        allow_shooting = true;
    }
    if (cameraX < cameraMinX) cameraX = cameraMinX;
    if (cameraX > cameraMaxX) cameraX = cameraMaxX;
    if (cameraZ < cameraMinZ) cameraZ = cameraMinZ;
    if (cameraZ > cameraMaxZ) cameraZ = cameraMaxZ;
}

void updateBullets(float deltaTime) {
    for (auto& bullet : bullets) {
        bullet.update(deltaTime);
    }
}
void functions::idle(){
    static float lastTime = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;
    float currentTime = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;
    float deltaTime = currentTime - lastTime;
    lastTime = currentTime;

    // Update the bullets
    updateBullets(deltaTime);

    glutPostRedisplay();
}

void functions::menu_options(int option){
    switch(option){
        case MENU_SUB_15_TARGET:
            max_targets = 15;
            numtargets = 0;
            bullet_count =0;
            allow_shooting = true;
            break;
        case MENU_SUB_30_TARGET:
            max_targets = 30;
            numtargets = 0;
            bullet_count=0;
            allow_shooting = true;
            break;
        case MENU_SUB_TARGET_SIZE_SMALL:
            targetRadius=0.4f;
            break;
        case MENU_SUB_TARGET_SIZE_MEDIUM:
            targetRadius=0.6f;
            break;
        case MENU_SUB_TARGET_SIZE_LARGE:
            targetRadius=0.8f;
            break;
        case MENU_GAME_EXIT:
            glutLeaveMainLoop();
        break;
        case MENU_GAME_HOME:
            gameset=false;
            break;
        case TARGET_COLOR_GREEN:
            target_color[0] = 0;
            target_color[1] = 0.392f;
            target_color[2] = 0;
            break;
        case TARGET_COLOR_PURPLE:
            target_color[0] = 0.733;
            target_color[1] = 0.161;
            target_color[2] = 0.733;
            break;
        case TARGET_COLOR_TEAL:
            target_color[0] = 0.188;
            target_color[1] = 0.808;
            target_color[2] = 0.847;
            break;
        case TARGET_COLOR_ORANGE:
            target_color[0] = 1;
            target_color[1] = 0.51;
            target_color[2] = 0;
            break;
        case TARGET_COLOR_CREAM:
            target_color[0] = 0.996f;
            target_color[1] = 0.988f;
            target_color[2] = 0.843f;
            break;
    }
}
void menu_function(int option){f.menu_options(option);}
// add this into functions class
void functions::totalmenu(){
    // game mode sub menu
    int game_mode_menu = glutCreateMenu(menu_function);
    glutAddMenuEntry("15 TARGETS",MENU_SUB_15_TARGET);
    glutAddMenuEntry("30 TARGETS",MENU_SUB_30_TARGET);
    // target size sub menu
    int target_size_menu = glutCreateMenu(menu_function);
    glutAddMenuEntry("SMALL",MENU_SUB_TARGET_SIZE_SMALL);
    glutAddMenuEntry("MEDIUM",MENU_SUB_TARGET_SIZE_MEDIUM);
    glutAddMenuEntry("LARGE",MENU_SUB_TARGET_SIZE_LARGE);
    // target color sub menu
    int target_color_menu = glutCreateMenu(menu_function);
    glutAddMenuEntry("CREAM", TARGET_COLOR_CREAM);
    glutAddMenuEntry("GREEN",TARGET_COLOR_GREEN);
    glutAddMenuEntry("PURPLE",TARGET_COLOR_PURPLE);
    glutAddMenuEntry("ORANGE",TARGET_COLOR_ORANGE);
    glutAddMenuEntry("TEAL",TARGET_COLOR_TEAL);
    // create the entire menu
    glutCreateMenu(menu_function);
    glutAddSubMenu("Game Modes",game_mode_menu);
    glutAddSubMenu("Target Size",target_size_menu);
    glutAddSubMenu("Target Color",target_color_menu);
    glutAddMenuEntry("Go To Home",MENU_GAME_HOME);
    glutAddMenuEntry("Exit Game",MENU_GAME_EXIT);
    
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}

// defining all functions to be registered.

void idle() {f.idle();}
void keyboard(unsigned char key, int x, int y) {f.keyboard_func(key,x,y);}
void mouse(int button, int state, int x, int y) {f.mouse_func(button,state,x,y);}
void mouseMotion(int x, int y) {f.mouse_motion_func(x,y);}
void reshape(int width, int height) {f.reshape(width,height);}
void display(){f.display();} 

int main(int argc, char** argv) {
    //basic initialisations
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH | GLUT_MULTISAMPLE);
    glutInitWindowSize(glutGet(GLUT_SCREEN_WIDTH), glutGet(GLUT_SCREEN_HEIGHT));
    glutCreateWindow("AimLabsGL");
    glEnable(GL_DEPTH_TEST);

    // registering all the functions
    glutDisplayFunc(display);
    glutIdleFunc(idle);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    glutPassiveMotionFunc(mouseMotion);
    glutReshapeFunc(reshape);
    
    //set cursor position and hiding it
    glutWarpPointer(windowWidth / 2, windowHeight / 2); // Initially center the mouse pointer 
    glutSetCursor(GLUT_CURSOR_NONE); // hides the mouse cursor
   
    // menu options
    f.totalmenu();
    glutMainLoop();
    return 0;
}