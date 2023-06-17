#include "game.hpp"
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
        float bulletStartX = cameraX + sin(cameraAngleY) * 0.5f;
        float bulletStartY = cameraY + sin(cameraAngleX) * 0.5f;
        float bulletStartZ = cameraZ - cos(cameraAngleY) * 0.5f;
        float bulletDirX = sin(cameraAngleY);
        float bulletDirY = sin(cameraAngleX);
        float bulletDirZ = -cos(cameraAngleY);
        bullets.emplace_back(bulletStartX + bulletDirX * 20.0f, bulletStartY + bulletDirY * 20.0f, bulletStartZ + bulletDirZ * 20.0f, bulletDirX, bulletDirY, bulletDirZ);
        bullets.back().active = true;
    }
}
void functions::display(){
    if(gameset && end_of_round){
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
        drawRoom();
    // Draw the gun
        glColor3f(0,0.392,0);
        glPushMatrix();
        glTranslatef(targetX,targetY,targetZ);
        glutSolidSphere(targetRadius,20,20);
        glPopMatrix();
        glutSwapBuffers();
        numtargets=0,bullet_count=0;
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
        if(numtargets == max_targets){
            end_of_round=false;
        }
        // Draw the bullets
        glColor3f(1.0f, 1.0f, 0.0f); // red color for the bullets
        for (auto& bullet : bullets) {
            if (bullet.active) {
                glPushMatrix();
                glTranslatef(bullet.positionX, bullet.positionY, bullet.positionZ);
                glutSolidSphere(0.15f, 10, 10); // Assuming bullet radius is 0.1
                glPopMatrix();
                if(bullet.checkCollision(targetX,targetY,targetZ,targetRadius)){
                    bullet.active = false;
                    targetX = static_cast<float>(rand()%11 - 5);
                    targetY = static_cast<float>(rand()%8 + 2);
                    bullet_count+=1;
                    numtargets+=1;
                }
                else{bullet.bullet_count+=1;} // increasing count of bullets at each bullet shot
            }
        }
        display_count_of_target_and_bullets(max_targets-numtargets,bullet_count);
    }
    else if(!end_of_round){
        display_accuracy(numtargets,max_targets);
    }
    else{
        setup_2d_ortho();
        draw_intro();
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
            break;
        case MENU_SUB_30_TARGET:
            max_targets = 30;
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
    }
}
void menu_function(int option){f.menu_options(option);}
// add this into functions class
void functions::totalmenu(){
    int game_mode_menu = glutCreateMenu(menu_function);
    glutAddMenuEntry("15 TARGETS",MENU_SUB_15_TARGET);
    glutAddMenuEntry("30 TARGETS",MENU_SUB_30_TARGET);
    int target_size_menu = glutCreateMenu(menu_function);
    glutAddMenuEntry("SMALL",MENU_SUB_TARGET_SIZE_SMALL);
    glutAddMenuEntry("MEDIUM",MENU_SUB_TARGET_SIZE_MEDIUM);
    glutAddMenuEntry("LARGE",MENU_SUB_TARGET_SIZE_LARGE);
    glutCreateMenu(menu_function);
    glutAddMenuEntry("Game Modes",game_mode_menu);
    glutAddMenuEntry("Target Size",target_size_menu);
    glutAddMenuEntry("Go To Home",MENU_GAME_HOME);
    glutAddMenuEntry("Exit Game",MENU_GAME_EXIT);
    
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}
void functions::setup_2d_ortho(){
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0,500,0,500);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

}
void functions::display_accuracy(int mxt,int bltc){
    glClearColor(0.8,0.8,0.8,1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // string fro number of targets in the game round,i.e max target
    glPushMatrix();
    glRasterPos3f(-5,-5,9);
    glColor3f(1,1,1);
    glutBitmapString(GLUT_BITMAP_HELVETICA_12,(unsigned char*)"Number Of Targets : ");
    glPopMatrix();
        glPushMatrix();
    glRasterPos3f(-5,-7,9);
    glColor3f(1,1,1);
    glutBitmapString(GLUT_BITMAP_HELVETICA_12,(unsigned char*)mxt);
    glPopMatrix();
        glPushMatrix();
    glRasterPos3f(0,-5,9);
    glColor3f(1,1,1);
    glutBitmapString(GLUT_BITMAP_HELVETICA_12,(unsigned char*)"Number of Bullets Fired : ");
    glPopMatrix();
        glPushMatrix();
    glRasterPos3f(-5,-7,9);
    glColor3f(1,1,1);
    glutBitmapString(GLUT_BITMAP_HELVETICA_12,(unsigned char*)bltc);
    glPopMatrix();
        glPushMatrix();
    glRasterPos3f(5,-5,9);
    glColor3f(1,1,1);
    glutBitmapString(GLUT_BITMAP_HELVETICA_12,(unsigned char*)"Accuracy : ");
    glPopMatrix();
        glPushMatrix();
    glRasterPos3f(5,-7,9);
    glColor3f(1,1,1);
    glutBitmapString(GLUT_BITMAP_HELVETICA_12,(unsigned char*)((mxt/bltc)*100));
    glPopMatrix();

}

void functions::display_count_of_target_and_bullets(int mxt,int bltc){
    glClearColor(0.8,0.8,0.8,1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPushMatrix();
    glRasterPos3f(-8,-5,9);
    glColor3f(1,1,1);
    glutBitmapString(GLUT_BITMAP_HELVETICA_12,(unsigned char*)"Targets Left : ");
    glPopMatrix();
    glPushMatrix();
    glRasterPos3f(5,-7,9);
    glColor3f(1,1,1);
    glutBitmapString(GLUT_BITMAP_HELVETICA_12,(unsigned char*)mxt);
    glPopMatrix();
    glPushMatrix();
    glRasterPos3f(8,-5,9);
    glColor3f(1,1,1);
    glutBitmapString(GLUT_BITMAP_HELVETICA_12,(unsigned char*)"BulletsFired : ");
    glPopMatrix();
    glPushMatrix();
    glRasterPos3f(5,-7,9);
    glColor3f(1,1,1);
    glutBitmapString(GLUT_BITMAP_HELVETICA_12,(unsigned char*)bltc);
    glPopMatrix();
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