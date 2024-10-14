#pragma once
#include<QOpenGLWidget>
#include<QOpenGLFunctions_3_3_Core>
#include<QOpenGLShaderProgram>
#include<QDebug>
#include<QTimer>
#include<QOpenGLTexture>
#include<QKeyEvent>
#include<QTime>
#include<QEvent>
#include<QMouseEvent>
#include<QWheelEvent>
#include<camera.h>
class axbopenglweight:public QOpenGLWidget, QOpenGLFunctions_3_3_Core
{
	Q_OBJECT
public:

	enum Shape{None,Rect,Circle,Triangle};
	explicit axbopenglweight(QWidget * parent = nullptr);
	~axbopenglweight();

	void drawshape(Shape shape);
	void setWirefrme(bool wireframe);




protected:
	virtual void initializeGL();
	virtual void resizeGL(int w, int h);
	virtual void paintGL();
	void KeyPressEvent(QKeyEvent *event);
	void MouseEvent(QMouseEvent *event);
	void wheelEvent(QWheelEvent *event);

signals:


public slots:
	void ontime();
private:
	Shape m_shape;
	QOpenGLShaderProgram shaderProgram;
	QTimer timer;
	QTime m_time;
	unsigned int VAO, VBO, EBO;
	QOpenGLTexture * texturewall;
	QOpenGLTexture * texturesmile;
	QOpenGLTexture * textureblending;
	Camera m_camera;
};
