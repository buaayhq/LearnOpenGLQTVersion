#include "window.h"
#include <QDebug>
#include <QString>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QImage>
#include <QSTring>


float vertices[] = {
    // positions          // colors           // texture coords
    0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
    0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
    -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
    -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
};
unsigned int indices[] = {
    0, 1, 3, // first triangle
    1, 2, 3  // second triangle
};

Window::Window()
: pVBO(nullptr), pVAO(nullptr), pEBO(nullptr), pShaderProgram(nullptr)
,pTexture(nullptr)
{

}


Window::~Window()
{
    makeCurrent();
    teardownGL();
}

/*******************************************************************************
 * OpenGL Events
 ******************************************************************************/

void Window::initializeGL()
{
  // Initialize OpenGL Backend
  initializeOpenGLFunctions();
//  connect(context(), SIGNAL(aboutToBeDestroyed()), this, SLOT(teardownGL()), Qt::DirectConnection);

  // Set global information
  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

  //创建shader
  pShaderProgram = new QOpenGLShaderProgram();
  //添加顶点着色器
  pShaderProgram->addShaderFromSourceFile(QOpenGLShader::Vertex, "4.1.texture.vs");
  //添加片段着色器
  pShaderProgram->addShaderFromSourceFile(QOpenGLShader::Fragment, "4.1.texture.fs");
  //Links together the shaders that were added to this program
  bool linkSuccess = pShaderProgram->link();
  if(!linkSuccess)
      qDebug() << "Error in linking shaders:" << pShaderProgram->log();

  //创建VAO
  pVAO = new QOpenGLVertexArrayObject();
  // Creates the underlying OpenGL vertex array object. 
  // There must be a valid OpenGL context that supports vertex array objects 
  // current for this function to succeed. Returns true if the OpenGL vertex array 
  // object was successfully created.
  pVAO->create();
  // Binds this vertex array object to the OpenGL binding point
  // Binds VAO first;then bind and set vertex buffer(s), and then configure vertex attributes(s).
  pVAO->bind();

  // create VBO
  pVBO = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
  //Creates the vertex buffer object in the OpenGL server. Returns true if the object was created; 
  //false otherwise
  pVBO->create();
  //Binds the buffer associated with this object to the current OpenGL context.
  pVBO->bind();
  // Sets the usage pattern for this buffer object to value. 
  // This function must be called before allocate() or write().
  pVBO->setUsagePattern(QOpenGLBuffer::StaticDraw);
  // allocate(const void *data, int count)
  // Allocates count bytes of space to the buffer, initialized to the contents of data. 
  // Any previous contents will be removed.
  // It is assumed that create() has been called on this buffer and that 
  // it has been bound to the current context.
  pVBO->allocate(vertices, sizeof(vertices));

  pEBO = new QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
  pEBO->create();
  pEBO->bind();
  pEBO->setUsagePattern(QOpenGLBuffer::StaticDraw);
  pEBO->allocate(indices, sizeof(indices));

  // Enables the vertex array at location in this shader program so that the value set by 
  // setAttributeArray() on location will be used by the shader program.
  pShaderProgram->enableAttributeArray(0);
  pShaderProgram->enableAttributeArray(1);
  pShaderProgram->enableAttributeArray(2);
  // Sets an array of vertex values on the attribute at location in this shader program, 
  // starting at a specific offset in the currently bound vertex buffer. 
  // The stride indicates the number of bytes between vertices.
  // offset: 它表示位置数据在缓冲中起始位置的偏移量(Offset)。由于位置数据在数组的开头，所以这里是0
  // 而颜色数据是位置数据之后，需要偏移三个float的大小，所以这里是3*sizeof(float)
  // tupleSize: 顶点属性的大小，从GLSL代码可以看出位置和颜色都是vec3类型的，它由3个值组成, 所以大小是3
  // stride: 它告诉我们在连续的顶点属性组之间的间隔。由于下个组位置数据在6个float之后，我们把步长设置为6 * sizeof(float)。
  // 简单来说就是一个顶点属性组的字节大小
  pShaderProgram->setAttributeBuffer(0, GL_FLOAT, 0, 3, 8*sizeof(float));
  pShaderProgram->setAttributeBuffer(1, GL_FLOAT, 3*sizeof(float), 3, 8*sizeof(float));
  pShaderProgram->setAttributeBuffer(2, GL_FLOAT, 6*sizeof(float), 2, 8*sizeof(float));

  // unbind VBO
  //pVBO->release();
  // unbind VAO
  //pVAO->release();

  // texture
  pTexture = new QOpenGLTexture(QOpenGLTexture::Target2D);
  pTexture->create();
  pTexture->bind();
  pTexture->setWrapMode(QOpenGLTexture::DirectionS, QOpenGLTexture::Repeat);
  pTexture->setWrapMode(QOpenGLTexture::DirectionT, QOpenGLTexture::Repeat);
  pTexture->setMinMagFilters(QOpenGLTexture::Linear, QOpenGLTexture::Linear);
  pTexture->setData(QImage(QString("container.jpg")).mirrored());

  
}

void Window::resizeGL(int width, int height)
{
  // Currently we are not handling width/height changes.
  (void)width;
  (void)height;
}

void Window::paintGL()
{
  // Clear
  glClear(GL_COLOR_BUFFER_BIT);

  pTexture->bind();
  // Render using our shader
  pShaderProgram->bind();
  // seeing as we only have a single VAO there's no need to bind it every time, 
  // but we'll do so to keep things a bit more organized
  pVAO->bind();
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
  pVAO->release();
  
  //Releases the active shader program from the current QGLContext. 
  //This is equivalent to calling glUseProgram(0).
  pShaderProgram->release();
}

void Window::teardownGL()
{
  // Actually destroy our OpenGL information
  pVBO->destroy();
  pVAO->destroy();
  if(pVBO!=NULL)
      delete pVBO;
  if(pVAO!=NULL)
      delete pVAO;
  if(pEBO!=NULL)
      delete pEBO;

}


