#include <QtGui>
#include <math.h>
#include "scene3D.h"
#include <QTimer>


Scene3D::Scene3D(QWidget* parent) : QGLWidget(parent)
{
    defaultScene();

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateGL()));
    timer->start(10);
}

void Scene3D::initializeGL()
{
    glShadeModel( GL_SMOOTH );
	glDisable(GL_TEXTURE_2D);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClearDepth( 1.0f );

    glEnable(GL_DEPTH_TEST);
    glDepthFunc( GL_LEQUAL );
	glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f,(GLfloat)800/(GLfloat)600,0.1f,100.0f);
	glMatrixMode(GL_MODELVIEW);

}

void Scene3D::resizeGL(int nWidth, int nHeight)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    GLfloat ratio=(GLfloat)nHeight/(GLfloat)nWidth;

    /*if (nWidth>=nHeight)
        glOrtho(-1.0/ratio, 1.0/ratio, -1.0, 1.0, -10.0, 1.0);
    else
        glOrtho(-1.0, 1.0, -1.0*ratio, 1.0*ratio, -10.0, 1.0);*/

    glViewport(0, 0, (GLint)nWidth, (GLint)nHeight);
	gluPerspective( 45.0f, (GLfloat)nWidth/(GLfloat)nHeight, 0.1f, 100.0f );

}

void Scene3D::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    QFont font;
    font.setPointSize(15);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.0, 0.0, -3.0);

    glScalef(nSca, nSca, nSca);
    glRotatef(xRot, 1.0, 0.0, 0.0);
    glRotatef(yRot, 0.0, 1.0, 0.0);
    glRotatef(zRot, 0.0, 0.0, 1.0);

	glColor3f(1.0 ,0.0 ,1.0);
	drawLine(QVector3D(1,0,0));
    renderText(1, 0, 0, "X", font);
	glColor3f(1.0, 1.0, 0.0);
	drawLine(QVector3D(0,1,0));
    renderText(0, 1, 0, "Y", font);
	glColor3f(1.0, 0.0, 0.0);
	drawLine(QVector3D(0,0,1));
    renderText(0, 0, 1, "Z", font);

    drawCopter(lambda, 0.5, 0.5, 0.1);

	glColor3f(1, 0.5, 0.5);
	drawLine(lambdaAccel.vector());

    tilt = getAngles(lambda)*180/M_PI;
    tiltAccel = getAngles(lambdaAccel)*180/M_PI;


	glLoadIdentity();
    glTranslatef(0.0, 0.0, -3.0);
    glColor3f(1,1,1);

	PrintAngles(tilt, -1.1, 0.7);
	PrintQuaternion(lambda, -1.1, 0.4);

}

void Scene3D::PrintAngles(QVector3D t, double x, double y)
{
	char str[100];
    QFont font;
    font.setPointSize(15);

	sprintf(str,"Roll:   %lf", t.x());
    renderText(x, y, 0.0, str, font);
	sprintf(str,"Pitch: %lf", t.y());
    renderText(x, y-0.08, 0.0, str, font);
	sprintf(str,"Yaw:   %lf", t.z());
    renderText(x, y-0.16, 0.0, str, font);
}

void Scene3D::PrintQuaternion(QQuaternion t, double x, double y)
{
	char str[100];
    QFont font;
    font.setPointSize(15);

	sprintf(str,"Scal:   %lf", t.scalar());
    renderText(x, y, 0.0, str, font);
	sprintf(str,"X:   %lf", t.x());
    renderText(x, y-0.08, 0.0, str, font);
	sprintf(str,"Y:   %lf", t.y());
    renderText(x, y-0.16, 0.0, str, font);
	sprintf(str,"Z:   %lf", t.z());
    renderText(x, y-0.24, 0.0, str, font);
}

void Scene3D::mousePressEvent(QMouseEvent* pe)
{
    ptrMousePosition = pe->pos();
}

void Scene3D::mouseReleaseEvent(QMouseEvent* pe)
{

}

void Scene3D::mouseMoveEvent(QMouseEvent* pe)
{
    xRot += 180/nSca*(GLfloat)(pe->y()-ptrMousePosition.y())/height();
    zRot -= 180/nSca*(GLfloat)(pe->x()-ptrMousePosition.x())/width();

    ptrMousePosition = pe->pos();

    updateGL();
}

void Scene3D::wheelEvent(QWheelEvent* pe)
{
    if ((pe->delta())>0) scale_plus(); else if ((pe->delta())<0) scale_minus();

    updateGL();
}

void Scene3D::keyPressEvent(QKeyEvent* pe)
{
    switch (pe->key())
    {
        case Qt::Key_Equal:  scale_plus();   break;
        case Qt::Key_Minus:  scale_minus();  break;
        /*case Qt::Key_Up:     gyro_y(-1);     break;
        case Qt::Key_Down:   gyro_y(1);      break;
        case Qt::Key_Left:   gyro_x(-1);     break;
        case Qt::Key_Right:  gyro_x(1);      break;
        case Qt::Key_A:      gyro_z(-1);     break;
        case Qt::Key_D:      gyro_z(1);      break;*/
        case Qt::Key_Space:  defaultScene(); break;
        case Qt::Key_Escape: this->close();  break;
        //case Qt::Key_0:      lastLambda = QQuaternion(1,0,0,0); break;
    }

    updateGL();
}

void Scene3D::keyReleaseEvent(QKeyEvent* pe)
{
    switch (pe->key())
    {
        /*case Qt::Key_Up:    gyro_y(0); break;
        case Qt::Key_Down:  gyro_y(0); break;
        case Qt::Key_Left:  gyro_x(0); break;
        case Qt::Key_Right: gyro_x(0); break;
        case Qt::Key_A:     gyro_z(0); break;
        case Qt::Key_D:     gyro_z(0); break;*/
    }

    updateGL();
}

void Scene3D::scale_plus()
{
    nSca = nSca*1.1;
}

void Scene3D::scale_minus()
{
    nSca = nSca/1.1;
}

void Scene3D::defaultScene()
{
   xRot=120; yRot=0; zRot=-120; nSca=1;
}

void Scene3D::drawLine(QVector3D d)
{
    //glLineWidth(3.0f);
	glBegin(GL_LINES);
		glVertex3f( d.x()*1000,  d.y()*1000,  d.z()*1000);
		glVertex3f(-d.x()*1000, -d.y()*1000, -d.z()*1000);
	glEnd();
}


void Scene3D::drawCopter(QQuaternion l, double xlen, double ylen, double zlen)
{
	xlen /= 2;
	ylen /= 2;
	zlen /= 2;

	double ang = 2*acos(l.scalar());
	ang *= 180/M_PI;


	if (abs(ang)>0.01) {
		glColor3f(0.5, 1.0, 0.5);
		drawLine(l.vector());
	}

	glRotatef(ang, l.x(), l.y(), l.z());


	glBegin(GL_QUADS);
		glColor3f(0.0f,0.5f,1.0f);
		glVertex3f(-xlen, -ylen, -zlen);
		glVertex3f(-xlen, -ylen,  zlen);
		glVertex3f(-xlen,  ylen,  zlen);
		glVertex3f(-xlen,  ylen, -zlen);

		glVertex3f(xlen, -ylen, -zlen);
		glVertex3f(xlen, -ylen,  zlen);
		glVertex3f(xlen,  ylen,  zlen);
		glVertex3f(xlen,  ylen, -zlen);

		glColor3f(0.5f,0.5f,1.0f);
		glVertex3f(-xlen, -ylen, -zlen);
		glVertex3f(xlen,  -ylen, -zlen);
		glVertex3f(xlen,   ylen, -zlen);
		glVertex3f(-xlen,  ylen, -zlen);

		glVertex3f(-xlen, -ylen, zlen);
		glVertex3f(xlen,  -ylen, zlen);
		glVertex3f(xlen,   ylen, zlen);
		glVertex3f(-xlen,  ylen, zlen);

		glColor3f(0.5f,0.0f,1.0f);
		glVertex3f(-xlen, -ylen, -zlen);
		glVertex3f(-xlen, -ylen,  zlen);
		glVertex3f(xlen,  -ylen,  zlen);
		glVertex3f(xlen,  -ylen, -zlen);

		glVertex3f(-xlen,  ylen, -zlen);
		glVertex3f(-xlen,  ylen,  zlen);
		glVertex3f(xlen,   ylen,  zlen);
		glVertex3f(xlen,   ylen, -zlen);
	glEnd();
}


QVector3D Scene3D::getAngles(QQuaternion lambda) {
	qreal cos_xz = 2*lambda.x()*lambda.z() - 2*lambda.scalar()*lambda.y();
	qreal cos_xy = 2*lambda.x()*lambda.y() + 2*lambda.scalar()*lambda.z();

	qreal roll = acos( cos_xz ) - M_PI/2;
	qreal pitch = acos( 2*lambda.y()*lambda.z() + 2*lambda.scalar()*lambda.x()) - M_PI/2;

	cos_xz *= cos_xz;
	qreal yaw;
	if (cos_xz < 1-1e-6) yaw = acos( (2*lambda.x()*lambda.x() + 2*lambda.scalar()*lambda.scalar() - 1) / sqrt(1-cos_xz) );
	else yaw = 0;
	if (cos_xy > 0) yaw *= -1;

	return QVector3D(roll, pitch, yaw);
}

QVector3D Scene3D::getTilt()
{
    return tilt;
}

QVector3D Scene3D::getAccelTilt()
{
    return tiltAccel;
}

void Scene3D::setQuaternions(QQuaternion tlambda, QQuaternion taccel)
{
    lambda = tlambda;
    lambdaAccel = taccel;
}
