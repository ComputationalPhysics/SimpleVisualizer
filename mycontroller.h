#ifndef MYCONTROLLER_H
#define MYCONTROLLER_H

#include "controller.h"
#include <QObject>

class MySimulator;
class MyController : public CompPhys::Controller
{
    Q_OBJECT
    Q_PROPERTY(double springConstant READ springConstant WRITE setSpringConstant NOTIFY springConstantChanged)

public:
    explicit MyController();

    virtual CompPhys::Simulator* createSimulator() const;
    double springConstant() const;

signals:
    void springConstantChanged(double arg);

public slots:
    void setSpringConstant(double arg);

private:
    std::vector<QVector2D> m_positions;
    std::vector<QVector2D> m_velocities;
    double m_springConstant;

    friend class MySimulator;
};

#endif // MYCONTROLLER_H
