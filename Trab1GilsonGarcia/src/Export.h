#ifndef _EXPORT_H_
#define _EXPORT_H_

using namespace std;
#include "gl_canvas2d.h"
#include "Figura.h"
#include "Circulo.h"
#include <iostream>
#include <fstream>
#include <iterator>
#include <list>
#include <vector>
#include <string>

/*
##### EXPORT #####
Classe utilizada para exportar o arquivo das figuras
bem simples apenas escreve os dados de cada figura no arquivo
######################
*/

class Export{
public:

    Export(list<Figura*> shapesList){
        vector<float> vx, vy;
        int nPoints;

        float radius;
        float angle;
        float r, g, b; 
        int colorScale, indexColor;

        int tamList = shapesList.size();

        ofstream arquivo("./Trab1GilsonGarcia/figuras.gr", ios::out | ios::binary | ios::trunc);
        if (arquivo.is_open()) {
            arquivo.write(reinterpret_cast<char*>(&tamList), sizeof(tamList));
            for (auto it = shapesList.begin(); it != shapesList.end(); ++it){
                int shapeType = (*it)->getType();
                angle = (*it)->getAngle();
                vx = (*it)->getVx();
                vy = (*it)->getVy();
                nPoints = vx.size();
                if (shapeType == CIRCLE_ID) {
                    Figura *fig = (*it);
                    Circulo* circ = dynamic_cast<Circulo*>(fig);
                    radius = circ->getRadius();
                }

                arquivo.write(reinterpret_cast<char*>(&shapeType), sizeof(shapeType));
                arquivo.write(reinterpret_cast<char*>(&angle), sizeof(angle));
                arquivo.write(reinterpret_cast<char*>(&nPoints), sizeof(nPoints));
                if (shapeType == CIRCLE_ID) {
                    arquivo.write(reinterpret_cast<char*>(&radius), sizeof(radius));
                }
                for (int i = 0; i < nPoints; i++) {
                    arquivo.write(reinterpret_cast<char*>(&vx[i]), sizeof(vx[i]));
                    arquivo.write(reinterpret_cast<char*>(&vy[i]), sizeof(vy[i]));
                }
                
                r = (*it)->getColorR();
                g = (*it)->getColorG();
                b = (*it)->getColorB();
                indexColor = (*it)->getIndexColor();
                colorScale = (*it)->getColorScale();
                arquivo.write(reinterpret_cast<char*>(&colorScale), sizeof(colorScale));
                arquivo.write(reinterpret_cast<char*>(&r), sizeof(r));
                arquivo.write(reinterpret_cast<char*>(&g), sizeof(g));
                arquivo.write(reinterpret_cast<char*>(&b), sizeof(b));
                arquivo.write(reinterpret_cast<char*>(&indexColor), sizeof(indexColor));
            }
            arquivo.close();
            printf("\nArquivo Salvo com Sucesso!\n");
        } else {
            cout << "Erro ao abrir o arquivo para exportar." << endl;
        }
    }





};
#endif // _EXPORT_H_