#ifndef GEOMETRIA_H
#define GEOMETRIA_H

#include <vector>
#include <cmath>
#define PI 3.14159265358979323846

using namespace std;

class Geometria {
public:
    // calcula a distância entre dois pontos
    static float dist(float x1, float y1, float x2, float y2);

    // retorna o angulo entre dois vetores (x1, y1) e (x2, y2) em rad
    static float angleRAD(float x1, float y1, float x2, float y2);

    // rotaciona um ponto de acordo com um angulo em graus e um pivot
    static void rotatePoint(float& x, float& y, float pivotX, float pivotY, float angle);

    // recebe um angulo em graus e rotaciona todos os pontos dos vetores de acordo com um pivot
    static void rotatePoints(vector<float>& vx, vector<float>& vy, float pivotX, float pivotY, float angle);
};
#endif // GEOMETRIA_H
