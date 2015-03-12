#include <billboards2d.h>
#include <QOpenGLFunctions>
#include <QFileInfo>
#include <QColor>
#include <QRgb>
#include <cmath>

using std::vector;

Billboards2D::Billboards2D(std::function<void(RenderableObject *renderableObject)> copyDataFunction, QString textureFilename)
{
    setNumberOfVBOs(2);
    m_textureFilename = textureFilename;
    copyData = copyDataFunction;
    m_color = QVector3D(1.0, 1.0, 1.0);
}

Billboards2D::~Billboards2D()
{

}

void Billboards2D::uploadTexture(QString filename)
{
    QFileInfo info(filename);
    if(!info.exists()) {
        qDebug() << "File " << filename << " does not exist. Aborting!";
        exit(1);
    }
    m_texture = new QOpenGLTexture(QImage(filename).mirrored());
}

QVector3D Billboards2D::vectorFromColor(const QColor &color)
{
    return QVector3D(color.redF(), color.greenF(), color.blueF());
}

std::vector<QVector2D> &Billboards2D::positions()
{
    return m_positions;
}

std::vector<float> &Billboards2D::rotations()
{
    return m_rotations;
}
float Billboards2D::scale() const
{
    return m_scale;
}

void Billboards2D::setScale(float scale)
{
    m_scale = scale;
}
QVector3D Billboards2D::color() const
{
    return m_color;
}

void Billboards2D::setColor(const QColor &color)
{
    m_color = vectorFromColor(color);
}



void Billboards2D::uploadVBOs()
{
    QVector2D right;
    right.setX(1.0);
    right.setY(0.0);
    QVector2D up;
    up.setX(0.0);
    up.setY(1.0);

    QVector2D ul = (0.5*up - 0.5*right)*m_scale;
    QVector2D ur = (0.5*up + 0.5*right)*m_scale;
    QVector2D dl = (-0.5*up - 0.5*right)*m_scale;
    QVector2D dr = (-0.5*up + 0.5*right)*m_scale;

    int numberOfVertices = m_positions.size()*4;
    m_vertices.resize(numberOfVertices);
    m_indices.resize(6*m_positions.size());

    QVector3D normalColor = m_color;

    for(unsigned int i=0; i<m_positions.size(); i++) {
        QVector2D &position = m_positions[i];
        float rotation = 0;
        if(m_rotations.size() > 0) {
            if(m_rotations.size() == 1) {
                rotation = m_rotations[0];
            } else {
                rotation = m_rotations[i];
            }
        }
        float cosTheta = cos(rotation);
        float sinTheta = sin(rotation);

        m_vertices[4*i + 0].position = position;
        m_vertices[4*i + 0].position[0] += dl[0]*cosTheta - dl[1]*sinTheta;
        m_vertices[4*i + 0].position[1] += dl[0]*sinTheta + dl[1]*cosTheta;
        m_vertices[4*i + 0].textureCoord= QVector2D(0,1);

        m_vertices[4*i + 1].position = position;
        m_vertices[4*i + 1].position[0] += dr[0]*cosTheta - dr[1]*sinTheta;
        m_vertices[4*i + 1].position[1] += dr[0]*sinTheta + dr[1]*cosTheta;
        m_vertices[4*i + 1].textureCoord= QVector2D(1,1);

        m_vertices[4*i + 2].position = position;
        m_vertices[4*i + 2].position[0] += ur[0]*cosTheta - ur[1]*sinTheta;
        m_vertices[4*i + 2].position[1] += ur[0]*sinTheta + ur[1]*cosTheta;
        m_vertices[4*i + 2].textureCoord= QVector2D(1,0);

        m_vertices[4*i + 3].position = position;
        m_vertices[4*i + 3].position[0] += ul[0]*cosTheta - ul[1]*sinTheta;
        m_vertices[4*i + 3].position[1] += ul[0]*sinTheta + ul[1]*cosTheta;
        m_vertices[4*i + 3].textureCoord= QVector2D(0,0);

        QVector3D color = normalColor;

        m_vertices[4*i + 0].color = color;
        m_vertices[4*i + 1].color = color;
        m_vertices[4*i + 2].color = color;
        m_vertices[4*i + 3].color = color;

        m_indices [6*i + 0] = 4*i+0;
        m_indices [6*i + 1] = 4*i+1;
        m_indices [6*i + 2] = 4*i+2;

        m_indices [6*i + 3] = 4*i+2;
        m_indices [6*i + 4] = 4*i+3;
        m_indices [6*i + 5] = 4*i+0;
    }

    // Transfer vertex data to VBO 0
    m_funcs->glBindBuffer(GL_ARRAY_BUFFER, m_vboIds[0]);
    m_funcs->glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Billboard2DData), &m_vertices[0], GL_STATIC_DRAW);

    // Transfer index data to VBO 1
    m_funcs->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vboIds[1]);
    m_funcs->glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(GLuint), &m_indices[0], GL_STATIC_DRAW);
}

void Billboards2D::initialize()
{
    uploadTexture(m_textureFilename);
}

void Billboards2D::setPositions(std::vector<QVector2D> &positions)
{
    m_positions = positions;
}

void Billboards2D::createShaderProgram() {
    if (!m_program) {
        m_program = new QOpenGLShaderProgram();

        m_program->addShaderFromSourceCode(QOpenGLShader::Vertex,
                                           "attribute highp vec2 a_position;\n"
                                           "attribute highp vec3 a_color;\n"
                                           "attribute highp vec2 a_texcoord;\n"
                                           "varying highp vec2 coords;\n"
                                           "varying highp float light;\n"
                                           "varying highp vec3 color;\n"
                                           "void main() {\n"
                                           "    gl_Position = vec4(a_position, 0.0, 1.0);\n"
                                           "    coords = a_texcoord;\n"
                                           "    color = a_color;\n"
                                           "}");

        m_program->addShaderFromSourceCode(QOpenGLShader::Fragment,
                                           "uniform sampler2D texture;\n"
                                           "varying highp vec2 coords;\n"
                                           "varying highp vec3 color;\n"
                                           "void main() {\n"
                                           "    gl_FragColor = texture2D(texture, coords.st)*vec4(color, 1.0);\n"
                                           "}");


        m_program->link();
    }
}

void Billboards2D::render(QMatrix4x4 &modelViewMatrix, QMatrix4x4 &projectionMatrix)
{
    if(m_vertices.size() == 0) return;
    createShaderProgram();

    m_program->bind();

    // Tell OpenGL which VBOs to use
    m_funcs->glBindBuffer(GL_ARRAY_BUFFER, m_vboIds[0]);
    m_funcs->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vboIds[1]);

    // Offset for position
    quintptr offset = 0;

    // Tell OpenGL programmable pipeline how to locate vertex position data
    int vertexLocation = m_program->attributeLocation("a_position");
    m_program->enableAttributeArray(vertexLocation);
    m_funcs->glVertexAttribPointer(vertexLocation, 2, GL_FLOAT, GL_FALSE, sizeof(Billboard2DData), (const void *)offset);

    // Offset for texture coordinate
    offset += sizeof(QVector2D);

    // Tell OpenGL programmable pipeline how to locate vertex color data
    int colorLocation = m_program->attributeLocation("a_color");
    m_program->enableAttributeArray(colorLocation);
    m_funcs->glVertexAttribPointer(colorLocation, 3, GL_FLOAT, GL_FALSE, sizeof(Billboard2DData), (const void *)offset);

    // Offset for texture coordinate
    offset += sizeof(QVector3D);

    // Tell OpenGL programmable pipeline how to locate vertex texture coordinate data
    int texcoordLocation = m_program->attributeLocation("a_texcoord");
    m_program->enableAttributeArray(texcoordLocation);
    m_funcs->glVertexAttribPointer(texcoordLocation, 2, GL_FLOAT, GL_FALSE, sizeof(Billboard2DData), (const void *)offset);

    // Draw cube geometry using indices from VBO 1
    // glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_SHORT, 0);
    m_texture->bind();
    glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);

    m_program->disableAttributeArray(vertexLocation);
    m_program->disableAttributeArray(colorLocation);
    m_program->disableAttributeArray(texcoordLocation);

    m_program->release();
}
