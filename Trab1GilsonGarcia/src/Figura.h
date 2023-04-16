#ifndef FIGURA_H_INCLUDED
#define FIGURA_H_INCLUDED

#include <vector>
#include <cmath>
#include "Botao.h"

using namespace std;

class Figura {
protected:
    bool visible, selected, resizing;
    float r, g, b; //cor da figura
    float x, y;
    float offsetX, offsetY;

    vector<Botao*> boundingButtons;
    int selectedBoundingButton;

    float dist(float x1, float y1, float x2, float y2) {
        return sqrt(pow((x2 - x1), 2) + pow((y2 - y1), 2));
    }

public:
    Figura() {
        r = 1;
        g = 0;
        b = 0;
        x = 200;
        y = 410;
        visible = false;
        selected = false;
        resizing = false;
        selectedBoundingButton = NO_SELECTION;
    }

    virtual ~Figura() {}

    virtual void render() {}

    float getX() {
        return x;
    }

    float getY() {
        return y;
    }

    void setX(float x) {
        this->x = x;
    }

    void setY(float y) {
        this->y = y;
    }

    void setPosition(float x, float y) {
        this->x = x;
        this->y = y;
    }

    void incrementX(float value) {
        x += value;
    }
    void incrementX() {
        x++;
    }

    void incrementY(float value) {
        y += value;
    }
    void incrementY() {
        y++;
    }

    void setColor(float r, float g, float b) {
        this->r = r;
        this->g = g;
        this->b = b;
    }

    float getColorR() {
        return r;
    }
    float getColorG() {
        return g;
    }
    float getColorB() {
        return b;
    }

    void setVisible() {
        visible = true;
    }
    void setUnvisible() {
        visible = false;
    }

    bool isVisible() {
        return visible;
    }

    void setSelected() {
        selected = true;
    }
    void setUnselected() {
        selected = false;
        selectedBoundingButton = NO_SELECTION;
    }

    void setResizing(bool value) {
        if (value == false) {
            selectedBoundingButton = NO_SELECTION;
        }
        resizing = value;
    }
    bool isResizing() {
        return resizing;
    }

    bool isSelected() {
        return selected;
    }

    int hasBoundingBtnCollided(float mx, float my) {
        for (unsigned int i = 0; i < boundingButtons.size(); i++) {
            auto btn = boundingButtons[i];
            if (btn->hasCollided(mx, my)) {
                selectedBoundingButton = i;
                return true;
            }
        }
        return false;
    }

    virtual void resize(float mx, float my) {}
    virtual void resizeProportionally(float mx, float my) {}
    virtual void setVisible(float x, float y) {}
    virtual void setOffset(float x, float y) {}
    virtual void setMousePosition(float mx, float my) {}
    virtual bool hasCollided(int x, int y) {}
    virtual float getCenterX() {}
    virtual float getCenterY() {}

};

#endif // FIGURA_H_INCLUDED
