#ifndef _IMPORT_H_
#define _IMPORT_H_

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
##### IMPORT #####
Classe utilizada para carregar o arquivo das figuras
bem simples apenas lê os dados de cada figura no arquivo
######################
*/

class Import{
public:
    list<Figura*> lista;

    Import(){
        vector<float> vx, vy;
        int nPoints;

        float radius;
        float angle;
        float r, g, b; 
        int colorScale, indexColor;

        float x;
        float y;

        int shapeType;

        std::ifstream arquivo("./Trab1GilsonGarcia/figuras.gr", std::ios::binary);
        if (!arquivo) {
            std::cerr << "Erro ao abrir arquivo\n";
        }
        if (arquivo.peek() == std::ifstream::traits_type::eof()) {
            std::cerr << "Arquivo vazio\n";
            arquivo.close();
            return;
        }

        int tamList;
        arquivo.read(reinterpret_cast<char*>(&tamList), sizeof(tamList));

        if (arquivo.is_open()) {
            for (int i = 0; i < tamList; i++){
                arquivo.read(reinterpret_cast<char*>(&shapeType), sizeof(shapeType));
                arquivo.read(reinterpret_cast<char*>(&angle), sizeof(angle));
                arquivo.read(reinterpret_cast<char*>(&nPoints), sizeof(nPoints));
                if (shapeType == CIRCLE_ID) {
                    arquivo.read(reinterpret_cast<char*>(&radius), sizeof(radius));
                }
                for (int i = 0; i < nPoints; i++) {
                    arquivo.read(reinterpret_cast<char*>(&x), sizeof(x));
                    arquivo.read(reinterpret_cast<char*>(&y), sizeof(y));
                    vx.push_back(x);
                    vy.push_back(y);
                }

                arquivo.read(reinterpret_cast<char*>(&colorScale), sizeof(colorScale));
                arquivo.read(reinterpret_cast<char*>(&r), sizeof(r));
                arquivo.read(reinterpret_cast<char*>(&g), sizeof(g));
                arquivo.read(reinterpret_cast<char*>(&b), sizeof(b));
                arquivo.read(reinterpret_cast<char*>(&indexColor), sizeof(indexColor));


                Figura *shape = NULL;
                if (shapeType == CIRCLE_ID) {
                    shape = new Circulo(vx, vy, radius, angle, colorScale, r, g, b, indexColor);
                } else if (shapeType == RECT_ID){
                    shape = new Retangulo(vx, vy, angle, colorScale, r, g, b, indexColor);
                } else if (shapeType == LINE_ID){
                    shape = new Linha(vx, vy, angle, colorScale, r, g, b, indexColor);
                }

                if(shape != NULL){
                    lista.push_back(shape);
                    vx.clear();
                    vy.clear();
                }
            }
            arquivo.close();  
            printf("\nArquivo Aberto com Sucesso!\n");
        } else {
            cout << "Erro ao abrir o arquivo a ser importado." << endl;
        }
    }

    list<Figura*> getShapesList(){
        return lista;
    }
};
#endif // _IMPORT_H_