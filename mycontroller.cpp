#include "mycontroller.h"
#include "simulator.h"
#include "billboards2d.h"
#include "points2d.h"
#include "controller.h"

#include <cmath>
#include <iostream>
#include <functional>

using std::cout; using std::endl; using std::function; using std::vector;

class MySimulator : public Simulator {

};

MyController::MyController()
{
    unsigned int numberOfBalls = 100;
    for(unsigned int i=0; i<numberOfBalls; i++) {
        float x =  2.0*(rand() / double(RAND_MAX)) - 1.0;
        float y =  2.0*(rand() / double(RAND_MAX)) - 1.0;
        float vx = 2.0*(rand() / double(RAND_MAX)) - 1.0;
        float vy = 2.0*(rand() / double(RAND_MAX)) - 1.0;


        x *= 0.5;
        y *= 0.5;
        vx *= 0.3;
        vy *= 0.3;

        m_positions.push_back(QVector2D(x,y));
        m_velocities.push_back(QVector2D(vx,vy));
    }

    Billboards2D *billboards = new Billboards2D(
                [&](Billboards2D *obj) {
                  obj->setPositions(m_positions);
                  }
            , ":/football.png");

    billboards->setScale(0.1);
    billboards->setColor(QColor("white"));

    Points2D *points = new Points2D([&](Points2D *obj) {
            obj->setData(m_positions, QVector3D(1.0, 0.0, 0.0));
            });
    points->setPointSize(10.0);

    addRenderableObject(billboards);
    addRenderableObject(points);
}

void MyController::stepImplementation() {
    qDebug() << QThread::currentThreadId() << " is mycontroller stepImplementation";
    float springConstant = 1.0;
    float mass = 1.0;
    float k = springConstant;
    float dt = 0.01;

    for(unsigned int i=0; i<m_positions.size(); i++) {
        QVector2D force;
        force.setX(-k*m_positions[i][0]);
        force.setY(-k*m_positions[i][1]);

        m_velocities[i][0] += force[0]/mass*dt;
        m_velocities[i][1] += force[1]/mass*dt;

        m_positions[i][0] += m_velocities[i][0]*dt;
        m_positions[i][1] += m_velocities[i][1]*dt;
    }
}
