#include "renderableobject.h"
#include <QFileInfo>

using std::vector;

namespace CompPhys {

RenderableObject::RenderableObject()
{

}

RenderableObject::~RenderableObject()
{
    if(m_funcs) {
        delete m_funcs;
        m_funcs = 0;
    }
    if(m_program) {
        delete m_program;
        m_program = 0;
    }
    m_vboIds.clear();
}

void RenderableObject::generateVBOs()
{
    m_vboIds.resize(m_numberOfVBOs);
    m_funcs->glGenBuffers(m_numberOfVBOs, &m_vboIds.front());
}
unsigned int RenderableObject::numberOfVBOs() const
{
    return m_numberOfVBOs;
}

void RenderableObject::setNumberOfVBOs(unsigned int numberOfVBOs)
{
    m_numberOfVBOs = numberOfVBOs;
    m_isInitialized = false;
}


void RenderableObject::initializeSuper()
{
    if(!m_funcs) m_funcs = new QOpenGLFunctions(QOpenGLContext::currentContext());
    generateVBOs();
    initialize();
    m_isInitialized = true;
}

bool RenderableObject::isInitialized() const
{
    return m_isInitialized;
}

}
