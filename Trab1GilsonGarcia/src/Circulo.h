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

class Circulo : public Figura {
    float radius;

protected:
    void createBoundingButtons() {
        for (int i = 0; i < 4; i++) {
            Botao* btn = new Botao(0, 0, BOUNDING_BTN_SIZE, BOUNDING_BTN_SIZE, "", RGBA);
            btn->setRGBA(0, 0, 0, 1);
            boundingButtons.push_back(btn);
        }
    }

    void drawBoundingBox() {
        float width = radius*2, height = radius*2;
        float bx = x - radius, by = y - radius;

        CV::color(0,0,0);
        CV::translate(bx, by);
        CV::rect(0, 0, width, height);
        CV::translate(0, 0);

        int offset = BOUNDING_BTN_SIZE / 2;
        //resize buttons
        Botao* botao = boundingButtons[LEFT_SIDE];
        botao->setCoord(bx - offset, by - offset + height / 2);
        botao->Render();

        botao = boundingButtons[RIGHT_SIDE];
        botao->setCoord(bx - offset + width, by - offset + height / 2);
        botao->Render();

        botao = boundingButtons[TOP_SIDE];
        botao->setCoord(bx - offset + width / 2, by - offset);
        botao->Render();

        botao = boundingButtons[BOTTOM_SIDE];
        botao->setCoord(bx - offset + width / 2, by - offset + height);
        botao->Render();
    }

public:
    Circulo(float radius)
    {
        this->radius = radius;
        offsetX = 0;
        offsetY = 0;
        createBoundingButtons();
    }

    Circulo() {
        radius = 20;
        offsetX = 0;
        offsetY = 0;
        createBoundingButtons();
    }

    void render() override {
        CV::translate(x, y);
        if (colorScale == RGBA) CV::color(r,g,b); 
        else if (colorScale == INDEX14)  CV::color(indexColor);
        CV::circleFill(0, 0, radius, NUM_SEGMENTS);
        CV::translate(0, 0);
        if(selected) drawBoundingBox();
    }

    float getCenterX() override{
        return x;
    }
    float getCenterY() override{
        return y;
    }

    void setOffset(float x, float y) override {
        offsetX = x - this->x;
        offsetY = y - this->y;
    }

    void setVisible(float x, float y) override {
        setPosition(x, y);
        visible = true;
    }

    bool hasCollided(int mx, int my) override {
        if (selectedBoundingButton >= 0 && selected) return false;

        float mouseDist = sqrt( pow(mx - x, 2) + pow(my - y, 2));
        if (mouseDist < radius) return true;
        return false;
    }

    void setMousePosition(float mx, float my) override {
        x = mx - offsetX;
        y = my - offsetY;
    }

    void resize(float mx, float my) override {
        float tempRadius = radius;
        if (selectedBoundingButton == RIGHT_SIDE) {
            tempRadius = mx - x;
        } else if (selectedBoundingButton == LEFT_SIDE) {
            tempRadius = x - mx;
        } else if (selectedBoundingButton == BOTTOM_SIDE) {
            tempRadius = my - y;
        } else if (selectedBoundingButton == TOP_SIDE) {
            tempRadius = y - my;
        }

        if(tempRadius >= MIN_RADIUS) radius = tempRadius;
    }

    void resizeProportionally(float mx, float my) override {
        resize(mx, my);
    }
};


#endif // CIRCULO_H_INCLUDED
