#ifndef RETANGULO_H_INCLUDED
#define RETANGULO_H_INCLUDED

#include "Figura.h"
#include "constants.h"

#define LEFT_SIDE 0
#define RIGHT_SIDE 1
#define TOP_SIDE 2
#define BOTTOM_SIDE 3

using namespace std;

class Retangulo : public Figura {
    float width;
    float height;

protected:
    void createBoundingButtons() {
        for (int i = 0; i < 4; i++) {
            Botao* btn = new Botao(0, 0, BOUNDING_BTN_SIZE, BOUNDING_BTN_SIZE, "");
            btn->setRGBA(0, 0, 0, 1);
            boundingButtons.push_back(btn);
        }
    }

    void drawBoundingBox() {
        CV::color(0,0,0);
        CV::translate(x, y);
        CV::rect(0, 0, width, height);
        CV::translate(0, 0);

        int offset = BOUNDING_BTN_SIZE / 2;
        //resize buttons
        Botao* botao = boundingButtons[LEFT_SIDE];
        botao->setCoord(x - offset, y - offset + height / 2);
        botao->Render();

        botao = boundingButtons[RIGHT_SIDE];
        botao->setCoord(x - offset + width, y - offset + height / 2);
        botao->Render();

        botao = boundingButtons[TOP_SIDE];
        botao->setCoord(x - offset + width / 2, y - offset);
        botao->Render();

        botao = boundingButtons[BOTTOM_SIDE];
        botao->setCoord(x - offset + width / 2, y - offset + height);
        botao->Render();
    }

public:
    using Figura::setVisible;

    Retangulo(float width, float height)
    {
        this->width = width;
        this->height = height;
        offsetX = 0;
        offsetY = 0;
        createBoundingButtons();
    }

    Retangulo() {
        width = 40;
        height = 40;
        offsetX = 0;
        offsetY = 0;
        createBoundingButtons();
    }

    void render() override {
        CV::translate(x, y);
        CV::color(r,g,b);
        CV::rectFill(Vector2(0,0), Vector2(width, height));
        if(selected) drawBoundingBox();
    }

    float getCenterX() override{
        return width/2;
    }
    float getCenterY() override{
        return height/2;
    }

    void setOffset(float x, float y) override {
        offsetX = x - this->x;
        offsetY = y - this->y;
    }

    void setVisible(float x, float y) override {
        setPosition(x - getCenterX(), y - getCenterY());
        visible = true;
    }

    bool hasCollided(int x, int y) override {
        if (selectedBoundingButton >= 0 && selected) return false;

        if( x >= this->x && x <= (this->x + width) && y >= this->y && y <= (this->y + height) )
        {
            return true;
        }
        return false;
    }

    void setMousePosition(float mx, float my) override {
        x = mx - offsetX;
        y = my - offsetY;
    }

    void resize(float mx, float my) override {
        if (selectedBoundingButton == RIGHT_SIDE) {
            width = mx - x;
        } else if (selectedBoundingButton == LEFT_SIDE) {
            width = width + x - mx;
            x = mx;
        } else if (selectedBoundingButton == BOTTOM_SIDE) {
            height = my - y;
        } else if (selectedBoundingButton == TOP_SIDE) {
            height = height + y - my;
            y = my;
        }
    }
};


#endif // RETANGULO_H_INCLUDED
