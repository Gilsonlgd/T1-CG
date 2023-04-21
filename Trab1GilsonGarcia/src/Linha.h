#ifndef LINHA_H_INCLUDED
#define LINHA_H_INCLUDED

#include "Figura.h"
#include "constants.h"

#define TOP_SIDE         0
#define BOTTOM_SIDE      1
#define MIN_LEN          5
#define START_LEN       40
#define X_MARGIN         2
#define TOL              2

using namespace std;

/*
##### LINHA #####
Foi implementada a partir de dois pontos
######################
*/

class Linha : public Figura {
    BoundingBtn* rotateBtn;

protected:
    // desenha a caixa contorno (sinaliza que está selecionado)
    void drawBoundingBox() {
        //resize buttons
        BoundingBtn* botao = boundingButtons[TOP_SIDE];
        botao->setCoord(vx[0], vy[0]);
        botao->Render();

        botao = boundingButtons[BOTTOM_SIDE];
        botao->setCoord(vx[1], vy[1]);
        botao->Render();

        rotateBtn->Render();
    }
    //atualiza a coordenada do botão de rotação de acordo com a posição da figura
    void attRotateBtnCoordinate() {
        float dx = vx[1] - vx[0];
        float dy = vy[1] - vy[0];
        float len = sqrt(dx*dx + dy*dy);
        dx /= len;
        dy /= len;

        float px_new = vx[1] + dx * 20;
        float py_new = vy[1] + dy * 20;
        rotateBtn->setCoord(px_new, py_new);
    }

    
public:
    using Figura::setVisible;

    Linha() : Figura(2) {
        ID = LINE_ID;
        offsetX = 0;
        offsetY = 0;
        rotateBtn = new BoundingBtn(0, 0, BOUNDING_BTN_SIZE, INDEX14);
    }

    // construtor utilizado na ao se instanciar um objeto originário de um arquivo
    Linha(vector<float> vx, vector<float> vy, float angle, 
    int colorScale, float r, float g, float b, int indexColor) : Figura(2) {
        
        ID = LINE_ID;
        this->vx = vx;
        this->vy = vy;
        this->angle = angle;
        this->colorScale = colorScale;
        this->r = r;
        this->g = g;
        this->b = b;
        this->indexColor = indexColor;
        rotateBtn = new BoundingBtn(0, 0, BOUNDING_BTN_SIZE, INDEX14);
        attRotateBtnCoordinate();
    }

    void render() override {
        if (colorScale == RGBA) CV::color(r,g,b); 
        else if (colorScale == INDEX14)  CV::color(indexColor);
        CV::translate(0,0);
        CV::line(vx[0], vy[0], vx[1], vy[1]);
        if(selected) drawBoundingBox();
    }

    // define um valor que subtraído a posição do mouse 
    // evita saltos
    void setOffset(float x, float y) override {
        offsetX = x - this->vx[0];
        offsetY = y - this->vy[0];
    }

    // seta a figura visível em deperminada posição
    void setVisible(float x, float y) override {
        float start_len = START_LEN;
        vx[0] = x;
        vy[0] = y - start_len/2;
        vx[1] = x;
        vy[1] = y + start_len/2;
        rotateBtn->setCoord(vx[1], vy[1] + 20);
        visible = true;
    }

    // Colidiu?
    bool hasCollided(int mx, int my) override {
        if (selectedBoundingButton >= 0 && selected) return false;

        float d1 = dist(mx,my, vx[0],vy[0]);
        float d2 = dist(mx,my, vx[1],vy[1]);
        float lineLen = dist(vx[0],vy[0], vx[1],vy[1]);   
        
        if (d1 + d2 >= lineLen - TOL && d1 + d2 <= lineLen + TOL) {
            return true;
        }
        return false;
    }

    // utiliza a posição do mouse para mover a figura
    void setMousePosition(float mx, float my) override {
        float xDif = vx[1] - vx[0];
        float yDif = vy[1] - vy[0];
        vx[0] = mx - offsetX;
        vy[0] = my - offsetY;
        vx[1] = mx + xDif - offsetX;
        vy[1] = my + yDif - offsetY;
        attRotateBtnCoordinate();
    }

    void resize(float mx, float my) override {
        if (selectedBoundingButton == BOTTOM_SIDE) {
            if(dist(mx, my, vx[0], vy[0]) > MIN_LEN) {
                vx[1] = mx;
                vy[1] = my;
            }
        } else if (selectedBoundingButton == TOP_SIDE) {
            if(dist(mx, my, vx[1], vy[1]) > MIN_LEN) {
                vx[0] = mx;
                vy[0] = my;
            }
        }

        attRotateBtnCoordinate();
    }

    // utiliza a posição do mouse para mover a figura.
    // - calcula o ângulo entre o botão de rotação da figura
    // e a nova posição do mouse para definir a rotação dos pontos da figura
    void rotate(float mx, float my) override {
        float pivotX = getCenterX();
        float pivotY = getCenterY();

        float CBx = rotateBtn->getCenterX() - pivotX;
        float CBy = rotateBtn->getCenterY() - pivotY;

        float CMx = mx - pivotX;
        float CMy = my - pivotY;

        float newAngle = angleDEG(CMx, CMy, CBx, CBy);

        if (abs(newAngle) > 180.0) {
            newAngle = newAngle > 0 ? newAngle - 360.0 : newAngle + 360.0;
        }
        angle += newAngle;
        rotatePoints(pivotX, pivotY, newAngle);
        attRotateBtnCoordinate();
    }

    // ao mudar uma coordenada da linha, a outra será alterada na mesma proporção
    void resizeProportionally(float mx, float my) override{
        if (selectedBoundingButton == BOTTOM_SIDE) {
            if(dist(mx, my, vx[0], vy[0]) > MIN_LEN) {
                vx[0] += -(mx - vx[1]);
                vy[0] += -(my - vy[1]);
                vx[1] = mx;
                vy[1] = my;
            }
        } else if (selectedBoundingButton == TOP_SIDE) {
            if(dist(mx, my, vx[1], vy[1]) > MIN_LEN) {
                vx[1] += -(mx - vx[0]);
                vy[1] += -(my - vy[0]);
                vx[0] = mx;
                vy[0] = my;
            }
        }

        attRotateBtnCoordinate();
    }

    bool hasRotateButtonCollided(float mx, float my) override{
        return rotateBtn->hasCollided(mx, my);
    }
};


#endif // LINHA_H_INCLUDED
