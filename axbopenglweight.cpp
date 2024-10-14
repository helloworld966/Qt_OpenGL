#include "axbopenglweight.h"
#include<iostream>
#define TIMEOUTMESC 100
//第一种方法绘制三角形

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

//显示更多的立方体
QVector <QVector3D> cubepostion=
{
  QVector3D(0.0f,  0.0f,  0.0f),
  QVector3D(2.0f,  5.0f, -15.0f),
  QVector3D(-1.5f, -2.2f, -2.5f),
  QVector3D(-3.8f, -2.0f, -12.3f),
  QVector3D(2.4f, -0.4f, -3.5f),
  QVector3D(-1.7f,  3.0f, -7.5f),
  QVector3D(1.3f, -2.0f, -2.5f),
  QVector3D(1.5f,  2.0f, -2.5f),
  QVector3D(1.5f,  0.2f, -1.5f),
  QVector3D(-1.3f,  1.0f, -1.5f)
};
unsigned int indices[] = {  // note that we start from 0!
	0, 1, 3,  // first Triangle
	1, 2, 3   // second Triangle
};
float radio = 0.5;
QPoint deltaPos;
axbopenglweight::axbopenglweight(QWidget *parent):QOpenGLWidget(parent)
{
	//焦点时间，不需要先用鼠标点击编辑框就可以直接输入文字等信息到其中。这就是由于搜索框设置了焦点。
	setFocusPolicy(Qt::StrongFocus);//有延时性的旋转
	setMouseTracking(true);
	//如果加入时间设置，就会一直发生旋转
	timer.start(TIMEOUTMESC);
	m_time.start();
	connect(&timer, SIGNAL(timeout()), this, SLOT(ontime()));

	m_camera.Position = QVector3D(0, 0, 3.0);
	
}
//构建析构函数
axbopenglweight::~axbopenglweight()
{
	makeCurrent();
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteVertexArrays(1, &VAO);
	doneCurrent();
}

void axbopenglweight::drawshape(axbopenglweight::Shape shape)
{
	m_shape = shape;
	update();
}

void axbopenglweight::setWirefrme(bool wireframe)
{
	makeCurrent();
	if (wireframe)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	update();
	doneCurrent();
}
void axbopenglweight::initializeGL()
{
	initializeOpenGLFunctions();
	//-----------------------------------------------------
	//创建VAO和VBO、EBO对象
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	//绑定VAO和VBO对象
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	//为当前绑定到target的缓冲区对象创建一个新的数据存储
	//如果data不为空，则使用来自此指针的数据初始化数据存储
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices,GL_STATIC_DRAW);

	//绑定EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	//位置属性
	//告知显卡如何解析缓冲里的属性值
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	//开启VAO管理的第一个属性值
	glEnableVertexAttribArray(0);
	////颜色属性
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	//glEnableVertexAttribArray(1);
	//纹理属性
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//-----------------------------------------------------


	//编译链接着色器
	//1. 顶点着色器

	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glCompileShader(vertexShader);

	//2. 片源着色器
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glCompileShader(fragmentShader);



	//3. 链接着色器
	bool success;
	shaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex,"shapes.vert");
	shaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, "shapes.frag");
	success = shaderProgram.link();

	if (!success)
	{
		qDebug() << "ERROR:"<<shaderProgram.log();
	}


	//开启深度测试
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);//透明度


	texturewall = new QOpenGLTexture(QImage("wall.jpg").mirrored());//纹理1
	texturesmile = new QOpenGLTexture(QImage("awesomeface.png").mirrored());//纹理2
	textureblending = new QOpenGLTexture(QImage("blending_transparent_window.png").mirrored());//纹理3

	shaderProgram.bind();//着色器绑定
	shaderProgram.setUniformValue("texturewall", 0);
	shaderProgram.setUniformValue("texturesmile", 1);
	shaderProgram.setUniformValue("textureblending", 2);
	textureblending->generateMipMaps();
	glBindVertexArray(0);

	
}
void axbopenglweight::resizeGL(int w, int h)
{
	//Q_UNUSED(w); Q_UNUSED(h);
	glViewport(0, 0, w, h);
}
//gl中绘制
void axbopenglweight::paintGL()
{
	QMatrix4x4 model;
	QMatrix4x4 view;




	//const float radius = 10.0f;
	float time = m_time.elapsed() / 1000;
	
	QMatrix4x4 projection;
	projection.perspective(m_camera.Zoom, (float)(width() / height()), 0.1, 100);//视野大小缩放
	//view.lookAt(m_camera.Position, m_camera.Front + m_camera.Position, m_camera.up);
	view = m_camera.GetViewMatrix();
	//glViewport(0, 0, width(), height());//调整在哪里动
	//float bordercolor[] = { 0.0f,0.0f,0.0f,1.0f };
	
	
	glClearColor(0.2f, 0.2f, 0.3f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT| GL_COLOR_BUFFER_BIT);

	//glUseProgram(shaderProgram);替换为
	shaderProgram.bind();
	shaderProgram.setUniformValue("radio", radio);
	shaderProgram.setUniformValue("projection", projection);
	glBindVertexArray(VAO);
	switch (m_shape)
	{
	case Rect:
		//glActiveTexture(GL_TEXTURE0);//激活双层纹理
		texturewall->bind(0);
		//glActiveTexture(GL_TEXTURE1);//激活双层纹理
		texturesmile->bind(1);
		//glActiveTexture(GL_TEXTURE2);//激活双层纹理
		textureblending->bind(2);

		//shaderProgram.setUniformValue("thematrix", matrix);

		shaderProgram.setUniformValue("view", view);
		

		for each (auto item in cubepostion)
		{
			model.setToIdentity();
			model.translate(item);

			model.rotate(time, 1.0f, 0.2f, 0.5f);//使图形发生旋转
			shaderProgram.setUniformValue("model", model);
			
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		
		

		break;
	default:
		break;
	}
	
}

void axbopenglweight::KeyPressEvent(QKeyEvent *event)
{
	float deltaTime = TIMEOUTMESC / 1000.0;
	switch (event->key())
	{
	case Qt::Key_Up:radio += 0.1;break;
	case Qt::Key_Down:radio -= 0.1;break;
	case Qt::Key_W:m_camera.ProssKeyboard(FORWARD, deltaTime); break;
	case Qt::Key_S:m_camera.ProssKeyboard(BACKWARD, deltaTime); break;
	case Qt::Key_D:m_camera.ProssKeyboard(RIGHT, deltaTime); break;
	case Qt::Key_A:m_camera.ProssKeyboard(LEFT, deltaTime); break;
		
	default:
		break;
	}
	if (radio > 1) radio = 1;
	if (radio < 0) radio = 0;
	

	shaderProgram.bind();
	shaderProgram.setUniformValue("radio", radio);
	update();
}

void axbopenglweight::MouseEvent(QMouseEvent *event)
{

	static QPoint lastPos(width() / 2, height() / 2);
	auto CurrentPos = event->pos();
	deltaPos = CurrentPos - lastPos;
	lastPos = CurrentPos;


	m_camera.ProssKeyMouseMovement(deltaPos.x(), -deltaPos.y());
	update();
}


void axbopenglweight::wheelEvent(QWheelEvent *event)
{
	m_camera.ProssMouseScroll(event->angleDelta().y()/120);

	update();
}

void axbopenglweight::ontime()
{
	
	update();
}



