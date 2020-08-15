#include "GLWiget.h"
#include "ui_mainwindow.h"
#include <QOpenGLShaderProgram>
#include "angle.h"
#include <QTimer>
#include "gltime.h"
GLWiget::GLWiget(QWidget *parent)
    : QOpenGLWidget(parent)
{
    camera = std::make_unique<Camera>(QVector3D(0.0f, 0.0f, 3.0f));
    //开启计时器，返回毫秒
    timer = new QTimer(this);
        connect(timer, &QTimer::timeout, this, [=]{
            timeValue += 1;
            update();
        });
    timer->start(1.0*1000/60);//60 fps
}

GLWiget::~GLWiget()
{
    makeCurrent();

    this->VAO.destroy();
    this->VBO.destroy();
    texture1->destroy();
    texture2->destroy();
    delete texture1;
    delete texture2;
    doneCurrent();
}


void GLWiget::initializeGL()
{
    this->initializeOpenGLFunctions();
    createShader();
    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

    VBO.create();
    VBO.bind();
    VBO.allocate(vertices,sizeof (vertices));


    {
        QOpenGLVertexArrayObject::Binder vaoBind(&VAO);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
//        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
//        glEnableVertexAttribArray(2);
    }


    // load textures (we now use a utility function to keep the code more organized)
    // -----------------------------------------------------------------------------
    texture1 = new QOpenGLTexture(QImage(":/texture/container.jpg"), QOpenGLTexture::GenerateMipMaps);
    if(!texture1->isCreated()){
        qDebug() << "Failed to load texture";
    }
    // set the texture wrapping parameters
    texture1->setWrapMode(QOpenGLTexture::DirectionS, QOpenGLTexture::Repeat);
    texture1->setWrapMode(QOpenGLTexture::DirectionT, QOpenGLTexture::Repeat);
    // set texture filtering parameters
    texture1->setMinificationFilter(QOpenGLTexture::Linear);
    texture1->setMagnificationFilter(QOpenGLTexture::Linear);

    // texture 2
    // ---------
    texture2 = new QOpenGLTexture(QImage(":/texture/awesomeface.png").mirrored(true, true), QOpenGLTexture::GenerateMipMaps);
    if(!texture2->isCreated()){
        qDebug() << "Failed to load texture";
    }
    // set the texture wrapping parameters
    texture2->setWrapMode(QOpenGLTexture::DirectionS, QOpenGLTexture::Repeat);
    texture2->setWrapMode(QOpenGLTexture::DirectionT, QOpenGLTexture::Repeat);
    // set texture filtering parameters
    texture2->setMinificationFilter(QOpenGLTexture::Linear);
    texture1->setMagnificationFilter(QOpenGLTexture::Linear);

    ourShader.bind();
    ourShader.setUniformValue("texture1", 0);
    ourShader.setUniformValue("texture2", 1);


    VBO.release();

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);
}

void GLWiget::resizeGL(int width, int height)
{
    glViewport(0, 0, width, height);
}

void GLWiget::paintGL()
{
    GLfloat currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
    // input
    // -----
    camera->processInput(deltaTime);//speed

    // render
    // ------
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // be sure to activate shader when setting uniforms/drawing objects
    ourShader.bind();

    // view/projection transformations
    QMatrix4x4 projection;
    projection.perspective(camera->Zoom, 1.0f * width() / height(), 0.1f, 100.0f);
    //view
    QMatrix4x4 view = camera->GetViewMatrix();
    ourShader.setUniformValue("projection", projection);
    ourShader.setUniformValue("view", view);
    // world transformation
    QMatrix4x4 model;
    ourShader.setUniformValue("model", model);


    // bind diffuse map
    glActiveTexture(GL_TEXTURE0);
    texture1->bind();

    // bind specular map
    glActiveTexture(GL_TEXTURE1);
    texture2->bind();

    {// render the cube
        QOpenGLVertexArrayObject::Binder vaoBind(&VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }
    ourShader.release();

    texture1->release();
    texture2->release();
}

bool GLWiget::createShader()
{
    bool success = ourShader.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shader/7.2.camera.vs");
    if (!success) {
        qDebug() << "shaderProgram addShaderFromSourceFile failed!" << ourShader.log();
        return success;
    }

    success = ourShader.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shader/7.2.camera.fs");
    if (!success) {
        qDebug() << "shaderProgram addShaderFromSourceFile failed!" << ourShader.log();
        return success;
    }

    success = ourShader.link();
    if(!success) {
        qDebug() << "shaderProgram link failed!" << ourShader.log();
    }

    return success;
}



void GLWiget::keyPressEvent(QKeyEvent *event)
{
    int key = event->key();
    if (key >= 0 && key < 1024)
        camera->keys[key] = true;
}

void GLWiget::keyReleaseEvent(QKeyEvent *event)
{
    int key = event->key();
    if (key >= 0 && key < 1024)
        camera->keys[key] = false;

}

void GLWiget::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton){
        firstMouse = true;
        lastPos = event->pos();
    }
}

void GLWiget::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);

    firstMouse = false;
}

void GLWiget::mouseMoveEvent(QMouseEvent *event)
{
    if (firstMouse) {

        int xpos = event->pos().x();
        int ypos = event->pos().y();

        int xoffset = xpos - lastPos.x();
        int yoffset = lastPos.y() - ypos;
        lastPos = event->pos();
        camera->ProcessMouseMovement(xoffset, yoffset);
    }
}

void GLWiget::wheelEvent(QWheelEvent *event)
{
    QPoint offset = event->angleDelta();
    camera->ProcessMouseScroll(offset.y());
}


