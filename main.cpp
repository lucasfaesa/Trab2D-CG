#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <iostream>
#include "player.h"
#include "tinyxml2.h"
#include "Cenario.h"
#include "tiro.h"
#include <algorithm>
#define INC_KEY 1
#define INC_KEYIDLE 0.02 //player X Speed

//Key status
int keyStatus[256];

bool pressingJumpKey = false;
bool grounded = true;
bool isJumping = false;
bool test = false;
float previousPlayerPosY = 0;

float yVel = 0.01; //player Y Speed
Player Player;
Cenario Cenario;
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
        camMove += inc * timeDiference;
        //cout << camMove << endl;
        Player.MoveEmX(-inc, timeDiference);
    }
    if(keyStatus[(int)('d')])
    {
        camMove -= inc * timeDiference;
        //cout << camMove << endl;
        Player.MoveEmX(inc, timeDiference);
        //mover personagem e camera
    }

    float playerPosX;
    float playerPosY;
    Player.GetPos(playerPosX,playerPosY);

    string var1 = "";
    string var2 = "";
    string var3 = "";
    string var4 = "";


    //TODO ENTENDER ONDE ESTA PLAYERPOSY INICIALMENTE
    if(playerPosY + 5.6 > -187.2 -10){
        var1 = "true";
    }else{
        var1 = "false";
    }
    if(playerPosY - 3.8 < -187.2){
        var2 = "true";
    }else{
        var2 = "false";
    }

    if(playerPosX < -163.5 + 364.1373){
        var3 = "true";
    }else{
        var3 = "false";
    }
    if(playerPosX + 2 > -163.5){
        var4 = "true";
    }else{
        var4 = "false";
    }
    //cout << "1:" << playerPosX << " " << -163.5 + 364.1373 << endl;
    //cout << "2:" << playerPosX + 2 << " " << -163.5  << endl;
    //cout << "3:" << playerPosY << " " << -187.2 - 10  << " "; //é flipado em Y por isso o menos
    //cout << "4:" << playerPosY + 3.8 << " " << -187.2  << endl;
    //cout << "1: " << var1 << " 2: " << var2 << " 3: " << var3 << " 4: " << var4 << "   " << playerPosY << endl;

    //escrever no papel depois para entender
    if(playerPosX < -163.5 + 364.1373 &&
    playerPosX + 2 > -163.5 &&
    playerPosY + 5.6 > -187.2 -10 &&
    playerPosY - 3.8 < -187.2){
        //cout << "Collisao" << endl;
        grounded = true;
        Player.ResetJumpDistance();
        //isJumping = false;
    }else{
        grounded = false;
    }


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
        Player.MoveEmY(yVel);
    }

    if(!grounded && previousPlayerPosY == playerPosY){ //gravity?
        Player.FreeFall(yVel);
    }

    //cout << "playerPos: " << playerPosY << " previousPlayerPos: " << previousPlayerPosY << endl;
    //std::cout << Player.ObtemDirection();

    glMatrixMode(GL_PROJECTION); // Select the projection matrix
    glLoadIdentity();

    //glTranslatef(camMove/45.9 +3.427/*offset*/,3.08/*offset*/,0);
    glTranslatef((camMove/45.9) + 3.427/*offset*/,4/*offset*/,0);

    gluOrtho2D(-45.9,45.9,-45.9,45.9);

    glMatrixMode(GL_MODELVIEW); // Select the projection matrix
    glLoadIdentity();

    glutPostRedisplay();

    previousPlayerPosY = playerPosY;
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
                if(grounded){
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
                if(!grounded)
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

