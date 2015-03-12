#pragma once

#include <QObject>
#include <QVector2D>
#include <vector>

class RenderableObject;
namespace CompPhys {
class Controller;
}
class Simulator : public QObject
{
    Q_OBJECT
private:
    std::vector<RenderableObject*> m_renderableObjects;
public:
    Simulator(CompPhys::Controller* parent = 0);
    ~Simulator();
    CompPhys::Controller* m_controller;

public slots:
    void step();

signals:
    void stepCompleted();
};
