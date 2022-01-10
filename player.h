#ifndef MAIN_CPP_PLAYER_H
#define MAIN_CPP_PLAYER_H
#include <GL/gl.h>
#include <GL/glu.h>


class Player {
    GLfloat gX;
    GLfloat gY;
    GLfloat bTheta = 0;
    GLfloat pETheta1 = 0;
    GLfloat pETheta2 = 0;
    GLfloat pDTheta1 = 0;
    GLfloat pDTheta2 = 0;

private:
void DesenhaRect(GLfloat height, GLfloat width, GLfloat R, GLfloat G, GLfloat B);
void DesenhaCabeca(GLfloat x, GLfloat y, GLint radius, GLfloat R,GLfloat G, GLfloat B);
void DesenhaPerna(GLfloat x, GLfloat y, GLfloat theta1, GLfloat theta2, GLfloat theta3, GLfloat theta4);
void DesenhaBraco(GLfloat x, GLfloat y, GLfloat theta1);
void DesenhaPlayer(GLfloat x, GLfloat y, GLfloat bTheta, GLfloat pETheta1, GLfloat pETheta2, GLfloat pDTheta1, GLfloat pDTheta2);

public:
    Player(){
        gX = -157;
        gY = -183.2;
        bTheta = 0;
        pETheta1 = 0;
        pETheta2 = 0;
        pDTheta1 = 0;
        pDTheta2 = 0;
    };
    void Desenha(){
        DesenhaPlayer(gX, gY, bTheta, pETheta1, pETheta2, pDTheta1, pDTheta2);
    };

    void RodaBraco(GLfloat inc);
    void RodaPernaE1(GLfloat inc);
    void RodaPernaE2(GLfloat inc);
    void RodaPernaD1(GLfloat inc);
    void RodaPernaD2(GLfloat inc);
    void MoveEmX(GLfloat dx);
    void MoveEmY(GLfloat dy);

};
#endif //MAIN_CPP_PLAYER_H
