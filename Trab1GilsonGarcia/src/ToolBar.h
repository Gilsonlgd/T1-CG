#ifndef TOOLBAR_H_INCLUDED
#define TOOLBAR_H_INCLUDED

#include "Figura.h"
#include "Retangulo.h"
#include "Circulo.h"
#include "Linha.h"
#include "Botao.h"
#include "ColorPicker.h"
#include <list>
#include <vector>
#include <cmath>

#define BACKGROUND_COLOR 21, 26, 35
#define LIMIT_LINE_COLOR 255, 255, 255, 1
#define MARGIN_LEFT  20
#define MARGIN_TOP   10
#define PADDING       5

// Botoes:
#define BTN_COLOR       111, 125, 155, 1
#define BTN_WIDTH_MD    110
#define BTN_HEIGHT_MD   35

#define BTN_WIDTH_LG    200
#define BTM_HEIGHT_LG    60

/*
##### TOOLBAR #####
Barra de ferramentas customizável.
É possível adicionar botões em cada uma das 3 sessões
sem que haja problemas de sobreposição
basta adicionar um botão à lista de labels da sessão
######################
*/

using namespace std;

class ToolBar {
    float x, y, width, height;
    float r, g, b;
    vector<char*> shapeButtonsLabel = {"Linha", "Retangulo", "Circulo"};
    vector<char*> managementButtonsLabel = {"Salvar Arquivo", "Excluir Tudo"};

    list<Botao*> shapeButtonsList;
    list<Botao*> managementButtonsList;

    ColorPicker* colorPicker;
protected:
    // calcula o espaço entre as colunas e linhas da grade de cores
    int calculateXPadding(int index) {
        if (index < 2) return 0;
        int coluna = floor(index / 2);
        return PADDING*coluna;
    }

    // posiciona todos os botões
    void createButtons(vector<char*> labelsList, list<Botao*>& buttonsList, float translationX, float _width) {
        float btn_width = _width;
        float btn_height = BTN_HEIGHT_MD;
        int deltaX = 0, deltaY = 0;

        for (unsigned int i = 0; i < labelsList.size(); i++) {
            i % 2 != 0 ? deltaY = 1 : deltaY = 0;

            float btnX = translationX + MARGIN_LEFT + deltaX*btn_width + calculateXPadding(i);
            float btnY = MARGIN_TOP + deltaY*btn_height + deltaY*PADDING;
            char* label = labelsList[i];
            Botao* btn = new Botao(btnX, btnY, btn_width, btn_height, label, RGBA);
            btn->setColor(BTN_COLOR);
            buttonsList.push_back(btn);

            if(i % 2 != 0) deltaX++;
        }
    }

    // denha uma linha de limite de sessão
    void drawLimitLine(float sessionOffset, float r, float g, float b, float a) {
        CV::color(r/255, g/255, b/255, a);
        CV::line(sessionOffset + 8, 5, sessionOffset + 8, height -5);
    }

    // retorna a largura da sessão de adm do arquivo
    float getMngSessionWidth() {
        const int mngColumns = trunc((managementButtonsLabel.size() + 1) / 2);
        return mngColumns * BTN_WIDTH_LG + mngColumns * PADDING + MARGIN_LEFT;
    }

    // retorna a largura da sessão de figuras
    float getShapesSessionWidth() {
        const int mngColumns = trunc((shapeButtonsList.size() + 1) / 2);
        return mngColumns * BTN_WIDTH_MD + mngColumns * PADDING + MARGIN_LEFT;
    }

public:
    ToolBar(float x, float y, float width, float height) {
        this->x = x;
        this->y = y;
        this->width = width;
        this->height = height;

        createButtons(managementButtonsLabel, managementButtonsList, 0, BTN_WIDTH_LG);
        createButtons(shapeButtonsLabel, shapeButtonsList, getMngSessionWidth(), BTN_WIDTH_MD);

        float translationX = getMngSessionWidth() + getShapesSessionWidth();
        colorPicker = new ColorPicker(translationX);

        setRGB(BACKGROUND_COLOR);
    }

    void render() {
        CV::translate(0, 0);
        CV::color(r,g,b);
        CV::rectFill(Vector2(x,y), Vector2(width, height));

        for (auto button : managementButtonsList) {
            button->Render();
        }

        const float mngWidth = getMngSessionWidth();
        drawLimitLine(mngWidth, LIMIT_LINE_COLOR);

        for (auto button : shapeButtonsList) {
            button->Render();
        }

        const float sessionsSum = mngWidth + getShapesSessionWidth() ;
        drawLimitLine(sessionsSum, LIMIT_LINE_COLOR);

        colorPicker->render();
    }

    // se um botão de criar figura foi clicado, retorna uma instância desta figura
    // NULL se nada foi clicado
    Figura* checkShapeButtonClicked(float mx, float my) {
        for (auto button : shapeButtonsList) {
            if (button->hasCollided(mx, my)) {
                char* label = button->getLabel();

                if (!strcmp(label, "Linha")) return new Linha();
                else if ( !strcmp(label, "Retangulo") ) return new Retangulo();
                else if ( !strcmp(label, "Circulo")   ) return new Circulo();
                else return NULL;
            }
        }
        return NULL;
    }

    //se um botão management foi clicado, retorna o id deste botão
    int checkMngButtonClicked(float mx, float my) {
    
        for (auto button : managementButtonsList) {
            if (button->hasCollided(mx, my)) {
                char* label = button->getLabel();
                
                if (!strcmp(label, "Salvar Arquivo")) return SAVE_ALL;
                else if ( !strcmp(label, "Excluir Tudo")) return DELETE_ALL;
                else return NO_SELECTION;
            }
        }
        return NO_SELECTION;
    }

    //se um botão de cor foi clicado, retorna a cor selecionada
    int checkColorButtonClicked(float mx, float my) {
        return colorPicker->getColorIndex(mx, my);
    }

    list<Botao*> getButtonsList() {
        return shapeButtonsList;
    }

    void setRGB(float r, float g, float b) {
        this->r = r/255;
        this->g = g/255;
        this->b = b/255;
    }

    float getHeight() {
        return height;
    }

    float getWidth() {
        return width;
    }
};


#endif // TOOLBAR_H_INCLUDED
