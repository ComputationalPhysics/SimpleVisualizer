#pragma once
#include "renderableobject.h"
#include <QColor>

struct PointData {
    QVector2D position;
    QVector3D color;
};

class Points : public RenderableObject
{
public:
    Points(std::function<void(RenderableObject *renderableObject)> copyDataFunction);
    ~Points();
    virtual void render(QMatrix4x4 &modelViewMatrix, QMatrix4x4 &projectionMatrix);
    virtual void uploadVBOs();
    virtual void initialize();

    float pointSize() const;
    void setPointSize(float pointSize);

    void setPositions(std::vector<QVector2D> &positions);
    std::vector<PointData> data() const;
    void setData(std::vector<QVector2D> &positions, std::vector<QVector3D> &colors);
    void setData(std::vector<QVector2D> &positions, QVector3D color = QVector3D(1.0, 0.0, 0.0));

private:
    std::vector<PointData> m_data;
    float m_pointSize;

    void createShaderProgram();
};
