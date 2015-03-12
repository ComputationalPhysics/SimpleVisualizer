#include "simulator.h"
#include "billboards2d.h"
#include "points2d.h"
#include "controller.h"

#include <cmath>
#include <iostream>
#include <functional>
#include <QDebug>
#include <QThread>

using std::cout; using std::endl; using std::function; using std::vector;

namespace CompPhys {

Simulator::Simulator(CompPhys::Controller *parent) :
    QObject()
{
    m_controller = parent;
}

Simulator::~Simulator()
{

}

void Simulator::stepInThread()
{
    qDebug() << QThread::currentThreadId() << " is simulator";
    step();
    emit stepCompleted();
}

}
