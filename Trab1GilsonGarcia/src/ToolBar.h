#ifndef TOOLBAR_H_INCLUDED
#define TOOLBAR_H_INCLUDED

#include "Figura.h"
#include "Retangulo.h"
#include "Botao.h"
#include <list>
#include <vector>
#include <cmath>

#define BACKGROUND_COLOR 21, 26, 35, 100
#define MARGIN_LEFT  20
#define MARGIN_TOP   10
#define PADDING 5

// Botoes:
#define BTN_WIDTH_MD    110
#define BTN_HEIGHT_MD   35

#define BTN_WIDTH_LG   200
#define BTM_HEIGHT_LG   60

using namespace std;

class ToolBar {
    float x, y, width, height;
    vector<char*> shapeButtonsLabel = {"Linha", "Retangulo", "Circulo", "Poligono"};
    vector<char*> managementButtonsLabel = {"Salvar Arquivo", "Excluir Tudo"};

    list<Botao*> shapeButtonsList;
    list<Botao*> managementButtonsList;

protected:
    int calculateXPadding(int index) {
        if (index < 2) return 0;
        if (index % 4 == 0) return 2*PADDING;
        if ((index - 1) % 4 == 0) return 2*PADDING;
        if (index % 4 != 0) return PADDING;
        return 0;
    }

    void createButtons(vector<char*> labelsList, list<Botao*>& buttonsList, float translationX, float _width) {
        float btn_width = _width;
        float btn_height = BTN_HEIGHT_MD;
        int deltaX = 0, deltaY = 0;

        for (unsigned int i = 0; i < labelsList.size(); i++) {
            i % 2 != 0 ? deltaY = 1 : deltaY = 0;

            float btnX = translationX + MARGIN_LEFT + deltaX*btn_width + calculateXPadding(i);
            float btnY = MARGIN_TOP + deltaY*btn_height + deltaY*PADDING;
            char* label = labelsList[i];
            buttonsList.push_back(new Botao(btnX, btnY, btn_width, btn_height, label));

            if(i % 2 != 0) deltaX++;
        }
    }

public:
    ToolBar(float x, float y, float width, float height) {
        this->x = x;
        this->y = y;
        this->width = width;
        this->height = height;
        createButtons(managementButtonsLabel, managementButtonsList, 0, BTN_WIDTH_LG);
        createButtons(shapeButtonsLabel, shapeButtonsList, getMngSessionWidth(), BTN_WIDTH_MD);
    }

    void render() {
        CV::color(0,26,35);
        CV::rectFill(Vector2(x,y), Vector2(width, height));

        for (auto button : managementButtonsList) {
            button->Render();
        }

        float mngWidth = getMngSessionWidth();
        CV::color(0,0,0);
        CV::line(mngWidth + 8, 0, mngWidth + 8, height);

        for (auto button : shapeButtonsList) {
            button->Render();
        }
    }

    Figura* checkShapeButtonClicked(float mx, float my) {
        for (auto button : shapeButtonsList) {
            if (button->hasCollided(mx, my)) {
                char* label = button->getLabel();

                if (!strcmp(label, "Linha")) return NULL;
                else if ( !strcmp(label, "Retangulo") ) return new Retangulo;
                else if ( !strcmp(label, "Circulo")   ) return NULL;
                else if ( !strcmp(label, "Poligono")  ) return NULL;
                else return NULL;
            }
        }
        return NULL;
    }

    float getMngSessionWidth() {
        const int mngColumns = trunc((managementButtonsLabel.size() + 1) / 2);
        return mngColumns * BTN_WIDTH_LG + mngColumns * PADDING + MARGIN_LEFT;
    }

    list<Botao*> getButtonsList() {
        return shapeButtonsList;
    }


};


#endif // TOOLBAR_H_INCLUDED