#ifndef MAIN_CPP_CENARIO_H
#define MAIN_CPP_CENARIO_H

#include <GL/gl.h>
#include <GL/glu.h>
#include <string>
using namespace std;

class Cenario {
private:
    void DesenhaRect(GLfloat x, GLfloat y, GLfloat height, GLfloat width, string color);
    void DesenhaCirc(GLfloat x, GLfloat y, GLfloat radius, string color);
    void DesenhaCenario();
public:
    void Desenha(){
        DesenhaCenario();
    };
};


#endif //MAIN_CPP_CENARIO_H
