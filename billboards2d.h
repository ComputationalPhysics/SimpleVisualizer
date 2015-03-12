#pragma once
#include "renderableobject.h"
#include <QtGui/QOpenGLShaderProgram>
#include <QOpenGLFunctions>
#include <QOpenGLTexture>
#include <vector>
using std::vector;
class Simulator;

struct Billboard2DData
{
    QVector2D position;
    QVector3D color;
    QVector2D textureCoord;
};

class Billboards2D : public RenderableObject
{
public:
    Billboards2D(std::function<void(RenderableObject *renderableObject)> copyDataFunction, QString textureFilename);
    ~Billboards2D();
    virtual void render(QMatrix4x4 &modelViewMatrix, QMatrix4x4 &projectionMatrix);
    virtual void uploadVBOs();
    virtual void initialize();
    void setPositions(std::vector<QVector2D> &positions);
    std::vector<QVector2D> &positions();
    std::vector<float> &rotations();

    float scale() const;
    void setScale(float scale);

    QVector3D color() const;
    void setColor(const QColor &color);

private:
    QString m_textureFilename = "No file chosen";
    std::vector<Billboard2DData> m_vertices;
    std::vector<GLuint> m_indices;
    QVector3D m_color;

    std::vector<QVector2D> m_positions;
    std::vector<float> m_rotations;
    float m_scale = 1.0;
    QOpenGLTexture *m_texture = 0;

    void createShaderProgram();
    void uploadTexture(QString filename);
    QVector3D vectorFromColor(const QColor &color);
};
