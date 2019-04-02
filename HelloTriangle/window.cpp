#include "window.h"
#include <QDebug>
#include <QString>
#include <QOpenGLShaderProgram>

const char *vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";
const char *fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n\0";



static const float vertices[] = {
    -0.5f, -0.5f, 0.0f, // left  
    0.5f, -0.5f, 0.0f, // right 
    0.0f,  0.5f, 0.0f  // top   
};


Window::Window()
: pVBO(nullptr), pVAO(nullptr), pShaderProgram(nullptr)
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
  pShaderProgram->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSource);
  //添加片段着色器
  pShaderProgram->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSource);
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

  // Enables the vertex array at location in this shader program so that the value set by 
  // setAttributeArray() on location will be used by the shader program.
  pShaderProgram->enableAttributeArray(0);
  // Sets an array of vertex values on the attribute at location in this shader program, 
  // starting at a specific offset in the currently bound vertex buffer. 
  // The stride indicates the number of bytes between vertices.
  pShaderProgram->setAttributeBuffer(0, GL_FLOAT, 0, 3, 0);

  // unbind VBO
  pVBO->release();
  // unbind VAO
  pVAO->release();
  
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

  // Render using our shader
  pShaderProgram->bind();
  // seeing as we only have a single VAO there's no need to bind it every time, 
  // but we'll do so to keep things a bit more organized
  pVAO->bind();
  glDrawArrays(GL_TRIANGLES, 0, 3);
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

}


