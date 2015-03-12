#pragma once

#include <QObject>
#include <QVector2D>
#include <vector>

namespace CompPhys {
class Controller;
class RenderableObject;
class Simulator : public QObject
{
    Q_OBJECT
private:
    std::vector<RenderableObject*> m_renderableObjects;
public:
    Simulator(CompPhys::Controller* parent = 0);
    ~Simulator();
    CompPhys::Controller* m_controller;

    virtual void step() = 0;
    virtual void synchronize(Controller* controller) {Q_UNUSED(controller)}

public slots:
    void stepInThread();

signals:
    void stepCompleted();
};
}
