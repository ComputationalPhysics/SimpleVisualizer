#pragma once
#include "renderableobject.h"
#include <QtGui/QOpenGLShaderProgram>
#include <QOpenGLFunctions>
#include <QOpenGLTexture>
#include <vector>
using std::vector;
class Simulator;

struct BillboardData
{
    QVector2D position;
    QVector3D color;
    QVector2D textureCoord;
};

class Billboards : public RenderableObject
{
public:
    Billboards(std::function<void(RenderableObject *renderableObject)> copyDataFunction, QString textureFilename);
    ~Billboards();
    virtual void render(QMatrix4x4 &modelViewMatrix, QMatrix4x4 &projectionMatrix);
    virtual void uploadVBOs();
    virtual void initialize();
    void setPositions(std::vector<QVector2D> &positions);
    std::vector<QVector2D> &positions();
    std::vector<float> &rotations();

    float scale() const;
    void setScale(float scale);

private:
    QString m_textureFilename = "No file chosen";
    std::vector<BillboardData> m_vertices;
    std::vector<GLuint> m_indices;

    std::vector<QVector2D> m_positions;
    std::vector<float> m_rotations;
    float m_scale = 1.0;
    QOpenGLTexture *m_texture = 0;

    void createShaderProgram();
    void uploadTexture(QString filename);
    QVector3D vectorFromColor(const QColor &color);
};
