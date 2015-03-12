#include "points2d.h"
using std::vector;

namespace CompPhys {

Points2D::Points2D(std::function<void(Points2D *renderableObject)> copyDataFunction)
{
//    copyData = copyDataFunction;
    myCopyData = copyDataFunction;
    setNumberOfVBOs(1);

}

Points2D::~Points2D()
{

}

void Points2D::render(QMatrix4x4 &modelViewMatrix, QMatrix4x4 &projectionMatrix)
{
    if(m_data.size() == 0) return;

    createShaderProgram();

    m_program->bind();

    // Tell OpenGL which VBOs to use
    m_funcs->glBindBuffer(GL_ARRAY_BUFFER, m_vboIds[0]);

    // Offset for position
    quintptr offset = 0;

    // Tell OpenGL programmable pipeline how to locate vertex position data
    int vertexLocation = m_program->attributeLocation("a_position");
    m_program->enableAttributeArray(vertexLocation);
    m_funcs->glVertexAttribPointer(vertexLocation, 2, GL_FLOAT, GL_FALSE, sizeof(Point2DData),  (const void *)offset);

    // Offset for texture coordinate
    offset += sizeof(QVector2D);

    // Tell OpenGL programmable pipeline how to locate vertex color data
    int colorLocation = m_program->attributeLocation("a_color");
    m_program->enableAttributeArray(colorLocation);
    m_funcs->glVertexAttribPointer(colorLocation, 3, GL_FLOAT, GL_FALSE, sizeof(Point2DData), (const void *)offset);

    // Draw cube geometry using indices from VBO 1
    glPointSize(m_pointSize);
    m_funcs->glDrawArrays(GL_POINTS, 0, m_data.size());

    m_program->disableAttributeArray(vertexLocation);

    m_program->release();
}

void Points2D::uploadVBOs()
{
    // Transfer vertex data to VBO 0
    m_funcs->glBindBuffer(GL_ARRAY_BUFFER, m_vboIds[0]);
    m_funcs->glBufferData(GL_ARRAY_BUFFER, m_data.size() * sizeof(Point2DData), &m_data.front(), GL_STATIC_DRAW);
}

void Points2D::initialize()
{

}

void Points2D::copyDataFunction()
{
    myCopyData(this);
}

void Points2D::setPointSize(float pointSize)
{
    m_pointSize = pointSize;
}

void Points2D::setData(std::vector<QVector2D> &positions, std::vector<QVector3D> &colors)
{
    m_data.resize(positions.size());
    for(unsigned int i=0; i<positions.size(); i++) {
        m_data[i].position = positions[i];
        m_data[i].color = colors[i];
    }
}

void Points2D::setData(vector<QVector2D> &positions, QVector3D color)
{
    m_data.resize(positions.size());
    for(unsigned int i=0; i<positions.size(); i++) {
        m_data[i].position = positions[i];
        m_data[i].color = color;
    }
}



void Points2D::createShaderProgram()
{
    if (!m_program) {
        m_program = new QOpenGLShaderProgram();

        m_program->addShaderFromSourceCode(QOpenGLShader::Vertex,
                                           "attribute highp vec4 a_position;\n"
                                           "attribute highp vec3 a_color;\n"
                                           "varying highp vec3 color;\n"
                                           "void main() {\n"
                                           "    gl_Position = a_position;\n"
                                           "    color = a_color;"
                                           "}");

        m_program->addShaderFromSourceCode(QOpenGLShader::Fragment,
                                           "varying highp vec3 color;\n"
                                           "void main() {\n"
                                           "    gl_FragColor = vec4(color, 1.0);\n"
                                           "}");


        m_program->link();
    }
}

}
