#ifndef COLORPICKER_H_INCLUDED
#define COLORPICKER_H_INCLUDED
#include <iostream>
#include <vector>
#include "Botao.h"
#include "constants.h"
#include <cmath>

#define LIMIT_LINE_COLOR 255, 255, 255, 1
#define MARGIN_LEFT  20
#define MARGIN_TOP   10
#define PADDING      10
#define COLORS_LEN   14
#define BTN_SIZE     35

/*
##### COLORPICKER #####
Classe que define a sessão de escolha de cor para
preenchimento das figuras. É utilizada na toolbar
######################
*/

using namespace std;

class ColorPicker {
private:
    struct Color {
        int r;
        int g;
        int b;

    };
    // calcula o espaço entre as colunas e linhas da grade de cores
    int calculateXPadding(int index) {
        if (index < 2) return 0;
        int coluna = floor(index / 2);
        return PADDING*coluna;
    }

    vector<Botao*> colors;
public:
    // recebe um cociente de translação da coordenada X
    ColorPicker(float translationX) {
        float btn_width = BTN_SIZE;
        float btn_height = BTN_SIZE;
        int deltaX = 0, deltaY = 0;

        for (int i = 0; i < COLORS_LEN; i++) {
            i % 2 != 0 ? deltaY = 1 : deltaY = 0;

            float btnX = translationX + MARGIN_LEFT + deltaX*btn_width + calculateXPadding(i);
            float btnY = MARGIN_TOP + deltaY*btn_height + deltaY*PADDING;
            Botao* btn = new Botao(btnX, btnY, btn_width, btn_height, "", INDEX14);
            btn->setColor(i);
            colors.push_back(btn);

            if(i % 2 != 0) deltaX++;
        }
    }

    void render() {
        for (auto btn : colors) {
            btn->Render();
        }
    }

    // de acordo com o btão clicado retorna a cor selecioanda
    int getColorIndex(float x, float y) {
        for (auto btn : colors) {
            if (btn->hasCollided(x, y)) {
                return btn->getColorIndex();
                break;
            }
        }
        return NO_SELECTION;
    }
};

#endif // COLORPICKER_H_INCLUDED

