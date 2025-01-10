#ifndef COMMON_INC
#define COMMON_INC
#include "window.h"
#include "text.h"
#include "list.h"
#include "scroll.h"
#include "image.h"
#include "button.h"
#include "GUI.h"
#endif

#include <cmath>
#include <vector>

struct Point3D {
    float x, y, z;
};

class RotatingCube : public Window {
   public:
    RotatingCube();
    virtual void OnDraw(Context *cr);

    void SetCubeColor(const RGB cubeColor);
    RGB GetCubeColor();
    void SetCubeSize(float size);
    float GetCubeSize();
    void SetAngleX(float x);
    void SetAngleY(float y);
    float GetAngleX();
    float GetAngleY();

   private:
    float m_cubeSize;
    RGB m_cubeColor;
    float angleX, angleY;

    void UpdateVertices();

    std::vector<std::pair<int, int>> edges = {
        {0, 1}, {1, 2}, {2, 3}, {3, 0},  // Задняя грань
        {4, 5}, {5, 6}, {6, 7}, {7, 4},  // Передняя грань
        {0, 4}, {1, 5}, {2, 6}, {3, 7}  // Соединения между гранями
    };
    std::vector<Point3D> vertices = {};
    std::vector<Point> projectedVertices = {};
};
