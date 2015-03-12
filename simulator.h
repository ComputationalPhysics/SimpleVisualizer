#pragma once

#include <QObject>
#include <QVector2D>
#include <vector>

class RenderableObject;
class Simulator : public QObject
{
    Q_OBJECT
private:
    std::vector<RenderableObject*> m_renderableObjects;
public:
    Simulator(QObject* parent = 0);
    ~Simulator();
    std::vector<QVector2D> positions;
    std::vector<QVector2D> velocities;

    std::vector<RenderableObject *> &renderableObjects();
    void setRenderableObjects(std::vector<RenderableObject *> &renderableObjects);

public slots:
    void step();

signals:
    void stepCompleted();
};
