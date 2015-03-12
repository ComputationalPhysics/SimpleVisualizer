/****************************************************************************
**
** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the demonstration applications of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.  For licensing terms and
** conditions see http://qt.digia.com/licensing.  For further information
** use the contact form at http://qt.digia.com/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights.  These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "controller.h"
#include "renderableobject.h"
#include "renderer.h"

#include <functional>
#include <QtQuick/qquickwindow.h>
#include <QtGui/QOpenGLShaderProgram>
#include <QtGui/QOpenGLContext>
#include <QGLFormat>
#include <QOpenGLContext>
#include <iostream>
#include <cmath>
#include <QOpenGLFramebufferObjectFormat>
using namespace std;
namespace CompPhys {
Controller::Controller() :
      m_running(true),
      m_stepRequested(false),
      m_previousStepCompleted(true),
      m_simulatorOutputDirty(false),
      m_lastStepWasBlocked(false),
      m_simulator(0)
{
}

Controller::~Controller()
{
    m_simulatorWorkerThread.quit();
    m_simulatorWorkerThread.wait();
}

CompPhys::Renderer* Controller::createRenderer() const
{
    return new CompPhys::Renderer();
}

void Controller::setRunning(bool arg)
{
    if (m_running == arg)
        return;

    m_running = arg;
    emit runningChanged(arg);
    update();
}

void Controller::step()
{
    if(!m_simulator) {
        m_simulator = createSimulator();
        m_simulator->moveToThread(&m_simulatorWorkerThread);
        connect(this, &Controller::requestStep, m_simulator, &Simulator::stepInThread);
        connect(m_simulator, &Simulator::stepCompleted, this, &Controller::finalizeStep);
        m_simulatorWorkerThread.start();
        m_timer.start();
    }

    qDebug() << QThread::currentThreadId() << " is controller requesting step";
    if(!m_running) {
        return;
    }

    if(m_simulatorRunningMutex.tryLock()) {
        m_lastStepWasBlocked = false;
        m_simulator->synchronize(this);
        emit requestStep();
    } else {
        m_lastStepWasBlocked = true;
    }
}

void Controller::finalizeStep()
{
    m_simulatorOutputMutex.lock();

    // Update renderable objects from simulator
//    m_renderableObjects = m_simulatorWorker.renderableObjects();
    for(RenderableObject *obj : m_renderableObjects) {
//        obj->copyData(obj);
        obj->copyDataFunction();
    }

    m_previousStepCompleted = true;
    m_simulatorOutputDirty = true;
    m_simulatorOutputMutex.unlock();
    m_simulatorRunningMutex.unlock();
    update();

    if(m_lastStepWasBlocked) {
        m_lastStepWasBlocked = false;
        step();
    }
}

void Controller::handleWindowChanged(QQuickWindow *win)
{
    if (win) {
        //        connect(win, SIGNAL(beforeSynchronizing()), this, SLOT(sync()), Qt::DirectConnection);
        //        connect(win, SIGNAL(sceneGraphInvalidated()), this, SLOT(cleanup()), Qt::DirectConnection);
        // If we allow QML to do the clearing, they would clear what we paint
        // and nothing would show.
        win->setClearBeforeRendering(false);
    }
}

vector<RenderableObject *> &Controller::renderableObjects()
{
    return m_renderableObjects;
}

void Controller::addRenderableObject(RenderableObject *renderableObject)
{
    m_renderableObjects.push_back(renderableObject);
}

// ********************************************
// ******* Basic setters and getters **********
// ********************************************

void Controller::setPreviousStepCompleted(bool arg)
{
    if (m_previousStepCompleted != arg) {
        m_previousStepCompleted = arg;
        emit previousStepCompletedChanged(arg);
    }
}

void Controller::setSimulatorOutputDirty(bool arg)
{
    m_simulatorOutputDirty = arg;
}

bool Controller::running() const
{
    return m_running;
}

bool Controller::previousStepCompleted() const
{
    return m_previousStepCompleted;
}

bool Controller::simulatorOutputDirty() const
{
    return m_simulatorOutputDirty;
}
}
