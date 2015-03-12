#pragma once
#include <vector>
#include <QtGui/QOpenGLShaderProgram>
#include <QOpenGLFunctions>
#include <functional>
class Simulator;

namespace CompPhys {
class RenderableObject
{
public:
    RenderableObject();
    ~RenderableObject();
//    std::function<void(RenderableObject *renderableObject)> copyData;
    virtual void copyDataFunction() = 0;
    virtual void uploadVBOs() = 0;
    virtual void render(QMatrix4x4 &modelViewMatrix, QMatrix4x4 &projectionMatrix) = 0;
    virtual void initialize() = 0;
    void initializeSuper();
    bool isInitialized() const;
    unsigned int numberOfVBOs() const;
    void setNumberOfVBOs(unsigned int numberOfVBOs);

protected:
    std::vector<GLuint> m_vboIds;
    QOpenGLFunctions *m_funcs = 0;
    QOpenGLShaderProgram *m_program = 0;
    virtual void createShaderProgram() = 0;
    void generateVBOs();
    void ensureInitialized();
private:
    bool m_isInitialized = false;
    unsigned int m_numberOfVBOs = 1;
};
}
