#include "cube.h"

RotatingCube::RotatingCube() {
    angleX = 0;
    angleY = 0;
    m_cubeColor = RGB(0.3, 0.3, 0.3);
    m_cubeSize = 150;
}

void RotatingCube::SetCubeColor(const RGB cubeColor) {
    m_cubeColor = cubeColor;
}

RGB RotatingCube::GetCubeColor() { return m_cubeColor; }

void RotatingCube::SetCubeSize(float size) { m_cubeSize = size; }

float RotatingCube::GetCubeSize() { return m_cubeSize; }

void RotatingCube::SetAngleX(float x) { angleX = x; }

void RotatingCube::SetAngleY(float y) { angleY = y; }

float RotatingCube::GetAngleX() { return angleX; }

float RotatingCube::GetAngleY() { return angleY; }

void RotatingCube::OnDraw(Context* cr) {
    cr->SetColor(GetBackColor());
    cr->FillRectangle(Point(0, 0), GetSize());

    cr->SetColor(m_cubeColor);

    UpdateVertices();

    for (const auto& edge : edges) {
        Point p1 = projectedVertices[edge.first];
        Point p2 = projectedVertices[edge.second];
        cr->SetColor(m_cubeColor);
        cr->Line(p1, p2);
    }
}

Point3D Rotate(const Point3D& p, float ax, float ay) {
    float ax_rad = ax * M_PI / 180.0f;
    float ay_rad = ay * M_PI / 180.0f;

    float cosX = cos(ax_rad), sinX = sin(ax_rad);
    float cosY = cos(ay_rad), sinY = sin(ay_rad);

    float y = p.y * cosX - p.z * sinX;
    float z = p.y * sinX + p.z * cosX;

    float x = p.x * cosY - y * sinY;
    y = p.x * sinY + y * cosY;

    return {x, y, z};
}

void RotatingCube::UpdateVertices() {
    std::vector<Point3D> baseVertices = {
        {-m_cubeSize, -m_cubeSize, -m_cubeSize},
        {m_cubeSize, -m_cubeSize, -m_cubeSize},
        {m_cubeSize, m_cubeSize, -m_cubeSize},
        {-m_cubeSize, m_cubeSize, -m_cubeSize},
        {-m_cubeSize, -m_cubeSize, m_cubeSize},
        {m_cubeSize, -m_cubeSize, m_cubeSize},
        {m_cubeSize, m_cubeSize, m_cubeSize},
        {-m_cubeSize, m_cubeSize, m_cubeSize},
    };

    vertices.clear();
    for (const auto& v : baseVertices) {
        vertices.push_back(Rotate(v, angleX, angleY));
    }

    int centerX = GetSize().GetWidth() / 2;
    int centerY = GetSize().GetHeight() / 2;

    // Проекция на 2D плоскость
    projectedVertices.clear();
    for (const auto& v : vertices) {
        projectedVertices.push_back(Point(v.x + centerX, v.y + centerY));
    }
}
