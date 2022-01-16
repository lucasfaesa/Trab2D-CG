#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <iostream>
#include "player.h"
#include "tinyxml2.h"
#include "Cenario.h"
#include "tiro.h"
#include "enemy.h"
#include <algorithm>
#include <cmath>

#define INC_KEY 1
#define INC_KEYIDLE 0.02 //player X Speed

//Key status
int keyStatus[256];

bool pressingJumpKey = false;
bool isJumping = false;
bool test = false;

Boxes boxesArray[50] = {};
bool collidingOnRightSide;
bool collidingOnLeftSide;
bool collidingBottom;
bool collidingTop;

float previousPlayerLeft = 0;
float previousPlayerRight = 0;
float previousPlayerTop = 0;
float previousPlayerBottom = 0;

bool collidingBottomArray[50] ={};
bool collidingTopArray[50] ={};
bool collidingRightArray[50] ={};
bool collidingLeftArray[50] ={};
int contArray = 0;

float currentPlayerBottom = 0;
float currentPlayerRight =0;
float currentPlayerLeft = 0;
float currentPlayerTop = 0;

float yVel = 0.01; //player Y Speed
Player Player;
Cenario Cenario;
Enemy Enemy;

float camMove = 0;
int curY = 0;
Tiro * tiro = NULL; //Um tiro por vez
// Window dimensions
const GLint Width = 500;
const GLint Height = 500;

void renderScene(void)
{
    // Clear the screen.
    glClear(GL_COLOR_BUFFER_BIT);

    Cenario.Desenha();
    Player.Desenha();
    Enemy.Desenha();

    if(boxesArray[0].height == 0){ //apenas evitando retrabalho, afinal a fase não muda de posicoes
        Cenario.GetBoxesArray(boxesArray);
    }

    if (tiro) tiro->Desenha();

    glutSwapBuffers(); // Desenha the new frame of the game.
}

void keyPress(unsigned char key, int x, int y)
{
    switch (key)
    {
        case 'a':
        case 'A':
            keyStatus[(int)('a')] = 1; //Using keyStatus trick
            break;
        case 'd':
        case 'D':
            keyStatus[(int)('d')] = 1; //Using keyStatus trick
            break;

        case 27 :
            exit(0);
    }
    glutPostRedisplay();
}

void keyup(unsigned char key, int x, int y)
{
    keyStatus[(int)(key)] = 0;
    glutPostRedisplay();
}

void ResetKeyStatus()
{
    int i;
    //Initialize keyStatus
    for(i = 0; i < 256; i++)
        keyStatus[i] = 0;
}

void init(int w, int h)
{
    float playerPosX;
    float playerPosY;
    Player.GetPos(playerPosX,playerPosY);
    previousPlayerBottom = playerPosY  - 3.8;
    previousPlayerRight = playerPosX + 2 / 2;
    previousPlayerLeft = playerPosX - 2 / 2;
    previousPlayerTop = playerPosY + 5.6;

    ResetKeyStatus();
    // The color the windows will redraw. Its done to erase the previous frame.
    int R  = 1;
    int G = 0.2;
    int B = 1;
    glClearColor(R,G,B, 1);
    glMatrixMode(GL_PROJECTION); // Select the projection matrix

    glEnable(GL_BLEND); //Enable blending.
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); //Set blending function.

    gluOrtho2D(-45.9,45.9,-45.9,45.9);
    glMatrixMode(GL_MODELVIEW); // Select the projection matrix
    glLoadIdentity();
}

void idle(void)
{
    static GLdouble previousTime = glutGet(GLUT_ELAPSED_TIME);
    GLdouble currentTime, timeDiference;
    //Pega o tempo que passou do inicio da aplicacao
    currentTime = glutGet(GLUT_ELAPSED_TIME);
    // Calcula o tempo decorrido desde de a ultima frame.
    timeDiference = currentTime - previousTime;
    //Atualiza o tempo do ultimo frame ocorrido
    previousTime = currentTime;


    double inc = INC_KEYIDLE;
    //Treat keyPress
    if(keyStatus[(int)('a')])
    {
        if(!collidingOnLeftSide){
            camMove += inc * timeDiference;
            //cout << camMove << endl;
            Player.MoveEmX(-inc, timeDiference);
            if(collidingBottom){
                Player.RodaPernaD1(-inc);
                //Player.RodaPernaD2(-inc);
                Player.RodaPernaE1(-inc);
                //Player.RodaPernaE2(-inc);
            }
        }
    }
    if(keyStatus[(int)('d')])
    {
        if(!collidingOnRightSide) {
            camMove -= inc * timeDiference;
            //cout << camMove << endl;
            Player.MoveEmX(inc, timeDiference);
            if(collidingBottom){
                Player.RodaPernaD1(inc);
                //Player.RodaPernaD2(inc);
                Player.RodaPernaE1(inc);
                //Player.RodaPernaE2(inc);
            }
        }
    }

    float playerPosX;
    float playerPosY;
    Player.GetPos(playerPosX,playerPosY);

    currentPlayerBottom = playerPosY - 3.8;
    currentPlayerRight = playerPosX + 2 / 2;
    currentPlayerLeft = playerPosX - 2 / 2;
    currentPlayerTop = playerPosY + 5.6;

    int contBottom = 0;
    int contRight = 0;
    int contTop = 0;
    int contLeft = 0;

    for (Boxes box : boxesArray){
        if(box.height == 0) break;

        float boxLeft = box.xPos;
        float boxRight = box.xPos + box.width;
        float boxTop = box.yPos;
        float boxBottom = box.yPos + box.height;

        if(box.height == -10){
            int a = 10;
        }
        if(previousPlayerBottom > boxTop && currentPlayerBottom <= boxTop && currentPlayerRight > boxLeft && currentPlayerLeft < boxRight){
            Player.ResetJumpDistance();
            //collidingBottom = true;
            //collidingBottomArray[contArray] = true;
            contBottom++;
            //cout << "grounded" <<  endl;
        }else{
            //collidingBottom = false;
            collidingBottomArray[contArray] = false;
            //previousPlayerBottom = currentPlayerBottom;
            //cout << "not grounded" <<  endl;
        }

        if(previousPlayerRight < boxLeft && currentPlayerRight >= boxLeft &&  currentPlayerBottom < boxTop && currentPlayerTop > boxBottom){
            //collidingOnRightSide = true;
            collidingRightArray[contArray] = true;
            contRight++;
        }else{
            //collidingOnRightSide = false;
            collidingRightArray[contArray] = false;

        }

        if(previousPlayerLeft > boxRight && currentPlayerLeft <= boxRight && currentPlayerBottom < boxTop && currentPlayerTop > boxBottom){
            //collidingOnLeftSide = true;
            collidingLeftArray[contArray] = true;
            contLeft++;
        }else{
            //collidingOnLeftSide = false;
            collidingLeftArray[contArray] = false;

        }

        if(previousPlayerTop < boxBottom && currentPlayerTop >= boxBottom && currentPlayerRight > boxLeft && currentPlayerLeft < boxRight){
           // collidingTop = true;
            collidingTopArray[contArray] = true;
            contTop++;
        }else{
            //collidingTop = false;
            collidingTopArray[contArray] = false;
        }

        contArray++;
    }

    contArray = 0;

    if(contBottom > 0)
        collidingBottom = true;
    else{
        collidingBottom = false;
        previousPlayerBottom = currentPlayerBottom;
    }

    if(contRight > 0)
        collidingOnRightSide = true;
    else{
        collidingOnRightSide = false;
        previousPlayerRight = currentPlayerRight;
    }


    if(contLeft > 0)
        collidingOnLeftSide = true;
    else{
        collidingOnLeftSide = false;
        previousPlayerLeft = currentPlayerLeft;
    }


    if(contTop > 0)
        collidingTop = true;
    else{
        collidingTop = false;
        previousPlayerTop = currentPlayerTop;
    }

    if(collidingTop){
        isJumping = false;
    }

    /*for (int i=0; i < sizeof(collidingBottomArray); i++){
        if(collidingBottomArray[i]){
            contBottom++;
        }
        if(collidingRightArray[i]){
            collidingOnRightSide = true;
        }
        if(collidingLeftArray[i]){
            collidingOnLeftSide = true;
        }
        if(collidingTopArray[i]){
            collidingTop = true;
        }
    }*/


    /*if(previousPlayerBottom > boxTop && currentPlayerBottom <= boxTop && currentPlayerRight > boxLeft && currentPlayerLeft < boxRight){
        Player.ResetJumpDistance();
        collidingBottom = true;
        cout << "grounded" <<  endl;
    }else{

        collidingBottom = false;
        previousPlayerBottom = currentPlayerBottom;
        cout << "not grounded" <<  endl;
    }

    if(previousPlayerRight < boxLeft && currentPlayerRight >= boxLeft &&  currentPlayerBottom < boxTop && currentPlayerTop > boxBottom){
        collidingOnRightSide = true;
    }else{
        collidingOnRightSide = false;
        previousPlayerRight = currentPlayerRight;
    }

    if(previousPlayerLeft > boxRight && currentPlayerLeft <= boxRight && currentPlayerBottom < boxTop && currentPlayerTop > boxBottom){
        collidingOnLeftSide = true;
    }else{
        collidingOnLeftSide = false;
        previousPlayerLeft = currentPlayerLeft;
    }

    if(previousPlayerTop < boxBottom && currentPlayerTop >= boxBottom && currentPlayerRight > boxLeft && currentPlayerLeft < boxRight){
        collidingTop = true;
    }else{
        collidingTop = false;
        previousPlayerTop = currentPlayerTop;
    }*/

    /*if(currentPlayerRight >= boxLeft && currentPlayerRight < boxRight && currentPlayerBottom < boxTop && currentPlayerTop > boxBottom
        ){
        collidingOnRightSide = true;
    }else{
        collidingOnRightSide = false;
    }

    if(currentPlayerTop >= boxBottom && currentPlayerTop <= boxTop && currentPlayerRight > boxLeft && currentPlayerLeft < boxRight
       ){
        collidingTop = true;
    }else{
        collidingTop = false;
    }

    if(currentPlayerLeft <= boxRight && currentPlayerLeft > boxLeft && currentPlayerBottom < boxTop && currentPlayerTop > boxBottom
        ){
        collidingOnLeftSide = true;
    }else{
        collidingOnLeftSide = false;
    }*/


    //TODO ENTENDER ONDE ESTA PLAYERPOSY INICIALMENTE (descobri, está na base do tronco)
   /* if(playerPosY + 5.6 > -187.2 -10){
        var1 = true;
    }else{
        var1 = false;
    }
    if(playerPosY - 3.8 < -187.2){
        var2 = true;
    }else{
        var2 = false;
    }

    if(playerPosX < -163.5 + 364.1373){
        var3 = true;
    }else{
        var3 = false;
    }
    if(playerPosX + 2 > -163.5){
        var4 = true;
    }else{
        var4 = false;
    }

    //cout << "1:" << playerPosX << " " << -163.5 + 364.1373 << endl;
    //cout << "2:" << playerPosX + 2 << " " << -163.5  << endl;
    //cout << "3:" << playerPosY << " " << -187.2 - 10  << " "; //é flipado em Y por isso o menos
    //cout << "4:" << playerPosY + 3.8 << " " << -187.2  << endl;
    //cout << "1: " << var1 << " 2: " << var2 << " 3: " << var3 << " 4: " << var4 << "   " << playerPosY << endl;

    //escrever no papel depois para entender
    if(playerPosX < -163.5 + 364.1373 &&
    playerPosX + 2 > -163.5 &&
    playerPosY + 5.6 >= -187.2 -10 &&
    playerPosY - 3.8 <= -187.1){
        //cout << "Collisao" << endl;
        grounded = true;
        Player.ResetJumpDistance();
        //isJumping = false;
    }else{
        grounded = false;
    }*/


    //Trata o tiro (soh permite um tiro por vez)
    //Poderia usar uma lista para tratar varios tiros
    if(tiro){
        tiro->Move(timeDiference);

        //Trata colisao
        /*if (alvo.Atingido(tiro)){
            atingido++;
            alvo.Recria(rand()%500 - 250, 200);
        }*/

        if (!tiro->Valido()){
            delete tiro;
            tiro = NULL;
        }
    }


    if(isJumping && pressingJumpKey){
        Player.MoveEmY(yVel, isJumping);
    }

    /*if(pressingJumpKey){
        Player.MoveEmMenosY(-yVel, true);
    }*/
    if(test){
        Player.MoveEmMenosY(yVel, true);
    }
    if(!collidingBottom && previousPlayerBottom == currentPlayerBottom && !isJumping){ //gravity?
       Player.FreeFall(yVel);
    }

    //cout << "playerPos: " << playerPosY << " previousPlayerPos: " << previousPlayerPosY << endl;
    //std::cout << Player.ObtemDirection();

    glMatrixMode(GL_PROJECTION); // Select the projection matrix
    glLoadIdentity();

    glTranslatef(camMove/45.9 +3.427/*offset*/,3.08/*offset*/,0);
    //glTranslatef((camMove/45.9) + 3.427/*offset*/,4/*offset*/,0);

    gluOrtho2D(-45.9,45.9,-45.9,45.9);

    glMatrixMode(GL_MODELVIEW); // Select the projection matrix
    glLoadIdentity();

    glutPostRedisplay();

    //Player.GetPos(playerPosX,playerPosY);
    //previousPlayerBottom = currentPlayerBottom;

}
void passive(int x1,int y1) {

    int centerY = 250;
    int deltaY = y1 - centerY;
    deltaY = deltaY / 3 /* sense inversa*/;
    if(deltaY < -45){
        deltaY = -45;
    }
    if(deltaY > 45){
        deltaY = 45;
    }
    Player.RodaBraco(-deltaY);

}

void MyMouse(int button, int state, int x, int y)
{
    switch (button)
    {
        case GLUT_LEFT_BUTTON:

            if(state == GLUT_UP)
            {
                if (!tiro)
                    tiro = Player.Atira();
            }
            break;

        case GLUT_RIGHT_BUTTON:
            if(state == GLUT_DOWN){
                pressingJumpKey = true;
                if(collidingBottom){
                    isJumping = true;
                }
            }

            if(state == GLUT_UP){
                pressingJumpKey = false;
                isJumping = false;
            }
            break;

        case GLUT_MIDDLE_BUTTON:
            if(state == GLUT_DOWN){
                if(!collidingBottom)
                    test = true;
            }

            if(state == GLUT_UP){
                test = false;
            }
            break;
    }
}

int main(int argc, char *argv[])
{
    // Initialize openGL with Double buffer and RGB color without transparency.
    // Its interesting to try GLUT_SINGLE instead of GLUT_DOUBLE.
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

    // Create the window.
    glutInitWindowSize(Width, Height);
    glutInitWindowPosition(150,150);
    glutCreateWindow("Trab 2D - Lucas Rios");

    // Define callbacks.
    glutDisplayFunc(renderScene);
    glutKeyboardFunc(keyPress);
    glutPassiveMotionFunc(passive);
    glutIdleFunc(idle);
    glutMouseFunc(MyMouse);
    glutKeyboardUpFunc(keyup);

    glutReshapeFunc(init);

    glutMainLoop();

    return 0;
}

