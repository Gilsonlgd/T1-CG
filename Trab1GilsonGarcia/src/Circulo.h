#ifndef CIRCULO_H_INCLUDED
#define CIRCULO_H_INCLUDED

#include <cmath>
#include "Figura.h"
#include "constants.h"

#define LEFT_SIDE 0
#define RIGHT_SIDE 1
#define TOP_SIDE 2
#define BOTTOM_SIDE 3
#define MIN_RADIUS 7

using namespace std;

/*
##### Círculo #####
Implementa um circulo simples
######################
*/

class Circulo : public Figura {
    float radius;

protected:
    // instancia os botões de borda 
    void createBoundingButtons() {
        for (int i = 0; i < 4; i++) {
            BoundingBtn* btn = new BoundingBtn(0, 0, BOUNDING_BTN_SIZE, INDEX14);
            btn->setColor(0);
            boundingButtons.push_back(btn);
        }
    }

    // desenha a caixa contorno (sinaliza que está selecionado)
    void drawBoundingBox() {
        float width = radius*2, height = radius*2;
        float bx = vx[0] - radius, by = vy[0] - radius;

        CV::color(0,0,0);
        CV::translate(bx, by);
        CV::rect(0, 0, width, height);
        CV::translate(0, 0);

       
        //resize buttons
        BoundingBtn* botao = boundingButtons[LEFT_SIDE];
        botao->setCoord(bx , by  + height / 2);
        botao->Render();

        botao = boundingButtons[RIGHT_SIDE];
        botao->setCoord(bx  + width, by  + height / 2);
        botao->Render();

        botao = boundingButtons[TOP_SIDE];
        botao->setCoord(bx  + width / 2, by );
        botao->Render();

        botao = boundingButtons[BOTTOM_SIDE];
        botao->setCoord(bx  + width / 2, by  + height);
        botao->Render();
    }

public:
    Circulo() : Figura(1) {
        ID = CIRCLE_ID;
        radius = 20;
        offsetX = 0;
        offsetY = 0;
        createBoundingButtons();
    }

    Circulo(float radius) : Figura(1) {
        ID = CIRCLE_ID;
        this->radius = radius;
        offsetX = 0;
        offsetY = 0;
        createBoundingButtons();
    }

     // construtor utilizado na ao se instanciar um objeto originário de um arquivo
    Circulo(vector<float> vx, vector<float> vy, float radius, float angle, 
    int colorScale, float r, float g, float b, int indexColor) : Figura(1) {
        
        ID = CIRCLE_ID;
        this->vx = vx;
        this->vy = vy;
        this->radius = radius;
        this->angle = angle;
        this->colorScale = colorScale;
        this->r = r;
        this->g = g;
        this->b = b;
        this->indexColor = indexColor;
        createBoundingButtons();
    }

    void render() override {
        CV::translate(vx[0], vy[0]);
        if (colorScale == RGBA) CV::color(r,g,b); 
        else if (colorScale == INDEX14)  CV::color(indexColor);
        
        CV::circleFill(0, 0, radius, NUM_SEGMENTS);
        CV::translate(0, 0);
        if(selected) drawBoundingBox();
    }

    float getCenterX() override{
        return vx[0];
    }
    float getCenterY() override{
        return vy[0];
    }

    float getRadius() {
        return radius;
    }

    // define um valor que subtraído a posição do mouse 
    // evita saltos
    void setOffset(float x, float y) override {
        offsetX = x - this->vx[0];
        offsetY = y - this->vy[0];
    }

    // seta a figura visível em deperminada posição
    void setVisible(float x, float y) override {
        this->vx[0] = x;
        this->vy[0] = y;
        visible = true;
    }

    // se a distancia entre o centro e o mouse for menor que o raio,
    // colidiu
    bool hasCollided(int mx, int my) override {
        if (selectedBoundingButton >= 0 && selected) return false;

        float mouseDist = sqrt( pow(mx - vx[0], 2) + pow(my - vy[0], 2));
        if (mouseDist < radius) return true;
        return false;
    }

    // utiliza a posição do mouse para mover a figura
    void setMousePosition(float mx, float my) override {
        vx[0] = mx - offsetX;
        vy[0] = my - offsetY;
    }

    // redefine a posição do circulo
    void setPosition(float x, float y) {
        this->vx[0] = x;
        this->vy[0] = y;
    }

    // altera o raio de acordo com a posição do mouse
    void resize(float mx, float my) override {
        float tempRadius = dist(mx, my, vx[0], vy[0]);
        if(tempRadius >= MIN_RADIUS) radius = tempRadius;
    }

    // faz o mesmo da anterior
    void resizeProportionally(float mx, float my) override {
        resize(mx, my);
    }
};


#endif // CIRCULO_H_INCLUDED
