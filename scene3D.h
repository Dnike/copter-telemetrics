#ifndef SCENE3D_H
#define SCENE3D_H

#include <QGLWidget>
#include <GL/glu.h>

#include <QQuaternion>
#include <QTime>

#define KEY_DOWN(key) (GetAsyncKeyState(key) & 0x8000)
#define KEY_PUSH(key) (GetAsyncKeyState(key) & 0x0001)

class Scene3D : public QGLWidget
{
    private:
        bool keys[256];
        GLfloat xRot;
        GLfloat yRot;
        GLfloat zRot;
        GLfloat nSca;
        QQuaternion lambda, lambdaAccel;
        QVector3D  tilt, tiltAccel;

        QPoint ptrMousePosition;

        void scale_plus();
        void scale_minus();
        void defaultScene();

        void drawLine(QVector3D d);
        void drawCopter(QQuaternion l, double xlen, double ylen, double zlen);

        QVector3D getAngles(QQuaternion lambda);

        void PrintAngles(QVector3D t, double x, double y);
        void PrintQuaternion(QQuaternion t, double x, double y);

    protected:
        void initializeGL();
        void resizeGL(int nWidth, int nHeight);
        void paintGL();
        void mousePressEvent(QMouseEvent* pe);
        void mouseMoveEvent(QMouseEvent* pe);
        void mouseReleaseEvent(QMouseEvent* pe);
        void wheelEvent(QWheelEvent* pe);
        void keyPressEvent(QKeyEvent* pe);
        void keyReleaseEvent(QKeyEvent* pe);

    public:
        Scene3D(QWidget* parent = 0);
        void setQuaternions(QQuaternion tlambda, QQuaternion taccel);
        QVector3D getTilt();
        QVector3D getAccelTilt();
};
#endif
