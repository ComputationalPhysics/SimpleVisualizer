#ifndef MYCONTROLLER_H
#define MYCONTROLLER_H

#include "controller.h"
#include <QObject>

class MyController : public CompPhys::Controller
{
    Q_OBJECT
public:
    explicit MyController();

    void stepImplementation();
    std::vector<QVector2D> m_positions;
    std::vector<QVector2D> m_velocities;

signals:

public slots:
};

#endif // MYCONTROLLER_H
