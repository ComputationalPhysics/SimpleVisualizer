#pragma once
#include "renderableobject.h"
#include <QColor>

namespace CompPhys {
struct Point2DData {
    QVector2D position;
    QVector3D color;
};

class Points2D : public RenderableObject
{
public:
    Points2D(std::function<void(Points2D *renderableObject)> copyDataFunction);
    ~Points2D();
    virtual void render(QMatrix4x4 &modelViewMatrix, QMatrix4x4 &projectionMatrix);
    virtual void uploadVBOs();
    virtual void initialize();
    virtual void copyDataFunction();
    std::function<void(Points2D *renderableObject)> myCopyData;

    float pointSize() const;
    void setPointSize(float pointSize);

    void setPositions(std::vector<QVector2D> &positions);
    std::vector<Point2DData> data() const;
    void setData(std::vector<QVector2D> &positions, std::vector<QVector3D> &colors);
    void setData(std::vector<QVector2D> &positions, QVector3D color = QVector3D(1.0, 0.0, 0.0));

private:
    std::vector<Point2DData> m_data;
    float m_pointSize;

    void createShaderProgram();
};
}
