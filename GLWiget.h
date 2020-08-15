#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLTexture>
#include <QTimer>
#include "Camera.h"
#include <memory>

class QOpenGLShaderProgram;
namespace Ui {
class MainWindow;
}

class GLWiget :  public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    explicit GLWiget(QWidget *parent = nullptr);
    ~GLWiget() Q_DECL_OVERRIDE;

protected:
    virtual void initializeGL() Q_DECL_OVERRIDE;
    virtual void paintGL() Q_DECL_OVERRIDE;
    virtual void resizeGL(int width, int height) Q_DECL_OVERRIDE;

    void keyPressEvent(QKeyEvent *event) Q_DECL_OVERRIDE;
    void keyReleaseEvent(QKeyEvent *event) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void wheelEvent(QWheelEvent *event) Q_DECL_OVERRIDE;
private:
    QOpenGLBuffer VBO{QOpenGLBuffer::VertexBuffer};
    QOpenGLVertexArrayObject VAO;
    QOpenGLTexture *texture1 = nullptr;
    QOpenGLTexture *texture2 = nullptr;
    QOpenGLShaderProgram ourShader;


    // camera
    std::unique_ptr<Camera> camera;

    QPoint lastPos;
    bool firstMouse = true;

    float deltaTime = 0.0f; // 当前帧与上一帧的时间差
    float lastFrame = 0.0f; // 上一帧的时间
    QTimer *timer; //扫描update();更新窗口，不更新窗口paintGL()不会自己循环
    int timeValue = 0;
    Ui::MainWindow *ui;

private:
    bool createShader();
};

#endif // MAINWINDOW_H
