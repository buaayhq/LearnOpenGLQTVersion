#ifndef WINDOW_H
#define WINDOW_H

#include <QOpenGLWindow>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QTime>

class QOpenGLShaderProgram;

class Window : public QOpenGLWindow,
               protected QOpenGLFunctions
{
  Q_OBJECT

public:
    Window();
    ~Window();
// OpenGL Events
public:
  void initializeGL();
  void resizeGL(int width, int height);
  void paintGL();
protected slots:
  void teardownGL(); 

private:
  // OpenGL State Information
  QOpenGLBuffer *pVBO;
  QOpenGLVertexArrayObject *pVAO;
  QOpenGLShaderProgram *pShaderProgram;

private:
  QTime currentTime;

};

#endif // WINDOW_H
