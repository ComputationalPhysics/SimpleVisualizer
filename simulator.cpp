#include "simulator.h"
#include "billboards.h"
#include "points.h"

#include <cmath>
#include <iostream>
#include <functional>
#include <QDebug>
using std::cout; using std::endl; using std::function; using std::vector;

Simulator::Simulator(QObject *parent) :
    QObject(parent)
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

        positions.push_back(QVector2D(x,y));
        velocities.push_back(QVector2D(vx,vy));
    }

    Billboards *billboards = new Billboards(
                [&](RenderableObject *renderableObject) {
                  Billboards *obj = static_cast<Billboards*>(renderableObject);
                  obj->setPositions(positions);
                  }
            , ":/football.png");

    billboards->setScale(0.1);

    Points *points = new Points([&](RenderableObject *renderableObject) {
            Points *obj = static_cast<Points*>(renderableObject);
            obj->setData(positions, QVector3D(1.0, 0.0, 0.0));
            });
    points->setPointSize(10.0);

    m_renderableObjects.push_back(billboards);
    m_renderableObjects.push_back(points);
}

Simulator::~Simulator()
{

}

vector<RenderableObject *> &Simulator::renderableObjects()
{
    return m_renderableObjects;
}

void Simulator::setRenderableObjects(vector<RenderableObject *> &renderableObjects)
{
    m_renderableObjects = renderableObjects;
}

void Simulator::step()
{

    float springConstant = 1.0;
    float mass = 1.0;
    float k = springConstant;
    float dt = 0.01;

    for(unsigned int i=0; i<positions.size(); i++) {
        QVector2D force;
        force.setX(-k*positions[i][0]);
        force.setY(-k*positions[i][1]);

        velocities[i][0] += force[0]/mass*dt;
        velocities[i][1] += force[1]/mass*dt;

        positions[i][0] += velocities[i][0]*dt;
        positions[i][1] += velocities[i][1]*dt;
    }

    emit stepCompleted();
}
