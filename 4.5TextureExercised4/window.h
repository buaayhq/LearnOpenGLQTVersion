#ifndef WINDOW_H
#define WINDOW_H

#include <QOpenGLWindow>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QKeyEvent>

class QOpenGLShaderProgram;
class QOpenGLTexture;

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
    void keyPressEvent(QKeyEvent *e);

private:
  // OpenGL State Information
  QOpenGLBuffer *pVBO;
  QOpenGLBuffer *pEBO;
  QOpenGLVertexArrayObject *pVAO;
  QOpenGLShaderProgram *pShaderProgram;

  QOpenGLTexture *pTexture1;
  QOpenGLTexture *pTexture2;

  float mixValue;

};

#endif // WINDOW_H
