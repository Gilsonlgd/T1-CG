#ifndef RETANGULO_H_INCLUDED
#define RETANGULO_H_INCLUDED

#include "Figura.h"

class Retangulo : public Figura {
  float width;
  float height;
  float offsetX, offsetY;

public:
    using Figura::setVisible;

    Retangulo(float width, float height)
    {
        this->width = width;
        this->height = height;
        offsetX = 0;
        offsetY = 0;
    }

    Retangulo() {
        width = 40;
        height = 40;
        offsetX = 0;
        offsetY = 0;
    }

    void render() override {
        CV::color(0,0,0,100);
        CV::translate(x, y);
        if (selected) CV::rectFill(-2, -2, width + 2, height + 2);
        CV::color(r,g,b);
        CV::rectFill(Vector2(0,0), Vector2(width, height));
    }

    void setWidth(float width) {
        this->width = width;
    }
    void setHeight(float height) {
        this->height = height;
    }

    float getWidth(){
        return width;
    }
    float getHeight(){
        return height;
    }

    float getCenterX(){
        return width/2;
    }
    float getCenterY(){
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
};


#endif // RETANGULO_H_INCLUDED
