#include "axbopenglweight.h"
#include<iostream>
#define TIMEOUTMESC 100
//��һ�ַ�������������

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

//��ʾ�����������
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
	//����ʱ�䣬����Ҫ����������༭��Ϳ���ֱ���������ֵ���Ϣ�����С���������������������˽��㡣
	setFocusPolicy(Qt::StrongFocus);//����ʱ�Ե���ת
	setMouseTracking(true);
	//�������ʱ�����ã��ͻ�һֱ������ת
	timer.start(TIMEOUTMESC);
	m_time.start();
	connect(&timer, SIGNAL(timeout()), this, SLOT(ontime()));

	m_camera.Position = QVector3D(0, 0, 3.0);
	
}
//������������
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
	//����VAO��VBO��EBO����
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	//��VAO��VBO����
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	//Ϊ��ǰ�󶨵�target�Ļ��������󴴽�һ���µ����ݴ洢
	//���data��Ϊ�գ���ʹ�����Դ�ָ������ݳ�ʼ�����ݴ洢
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices,GL_STATIC_DRAW);

	//��EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	//λ������
	//��֪�Կ���ν��������������ֵ
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	//����VAO����ĵ�һ������ֵ
	glEnableVertexAttribArray(0);
	////��ɫ����
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	//glEnableVertexAttribArray(1);
	//��������
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//-----------------------------------------------------


	//����������ɫ��
	//1. ������ɫ��

	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glCompileShader(vertexShader);

	//2. ƬԴ��ɫ��
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glCompileShader(fragmentShader);



	//3. ������ɫ��
	bool success;
	shaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex,"shapes.vert");
	shaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, "shapes.frag");
	success = shaderProgram.link();

	if (!success)
	{
		qDebug() << "ERROR:"<<shaderProgram.log();
	}


	//������Ȳ���
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);//͸����


	texturewall = new QOpenGLTexture(QImage("wall.jpg").mirrored());//����1
	texturesmile = new QOpenGLTexture(QImage("awesomeface.png").mirrored());//����2
	textureblending = new QOpenGLTexture(QImage("blending_transparent_window.png").mirrored());//����3

	shaderProgram.bind();//��ɫ����
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
//gl�л���
void axbopenglweight::paintGL()
{
	QMatrix4x4 model;
	QMatrix4x4 view;




	//const float radius = 10.0f;
	float time = m_time.elapsed() / 1000;
	
	QMatrix4x4 projection;
	projection.perspective(m_camera.Zoom, (float)(width() / height()), 0.1, 100);//��Ұ��С����
	//view.lookAt(m_camera.Position, m_camera.Front + m_camera.Position, m_camera.up);
	view = m_camera.GetViewMatrix();
	//glViewport(0, 0, width(), height());//���������ﶯ
	//float bordercolor[] = { 0.0f,0.0f,0.0f,1.0f };
	
	
	glClearColor(0.2f, 0.2f, 0.3f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT| GL_COLOR_BUFFER_BIT);

	//glUseProgram(shaderProgram);�滻Ϊ
	shaderProgram.bind();
	shaderProgram.setUniformValue("radio", radio);
	shaderProgram.setUniformValue("projection", projection);
	glBindVertexArray(VAO);
	switch (m_shape)
	{
	case Rect:
		//glActiveTexture(GL_TEXTURE0);//����˫������
		texturewall->bind(0);
		//glActiveTexture(GL_TEXTURE1);//����˫������
		texturesmile->bind(1);
		//glActiveTexture(GL_TEXTURE2);//����˫������
		textureblending->bind(2);

		//shaderProgram.setUniformValue("thematrix", matrix);

		shaderProgram.setUniformValue("view", view);
		

		for each (auto item in cubepostion)
		{
			model.setToIdentity();
			model.translate(item);

			model.rotate(time, 1.0f, 0.2f, 0.5f);//ʹͼ�η�����ת
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



