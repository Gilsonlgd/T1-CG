#ifndef FIGURA_H_INCLUDED
#define FIGURA_H_INCLUDED

class Figura {
protected:
    bool visible, selected;
    float r, g, b; //cor do retangulo
    float x, y;


public:
    Figura() {
        r = 1;
        g = 0;
        b = 0;
        x = 200;
        y = 410;
        visible = false;
        selected = false;
    }

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
    }

    bool isSelected() {
        return selected;
    }

    virtual void setVisible(float x, float y) {}
    virtual void setOffset(float x, float y) {}
    virtual void setMousePosition(float mx, float my) {}
    virtual bool hasCollided(int x, int y) {}
};

#endif // FIGURA_H_INCLUDED
