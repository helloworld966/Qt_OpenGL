#include "Qt_Opengl.h"
Qt_Opengl::Qt_Opengl(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	//�źźͲ۱�����

	//��ͼ
	connect(ui.action, &QAction::triggered, this, &Qt_Opengl::actdraw);
	//���
	connect(ui.actionactclear, &QAction::triggered, this, &Qt_Opengl::actclear);

	//��Ⱦ
	connect(ui.action1, &QAction::triggered, this, &Qt_Opengl::wireframe);
}


void Qt_Opengl::actdraw()
{
	ui.AXBopenglweight->drawshape(axbopenglweight::Rect);
}

void Qt_Opengl::actclear()
{
	ui.AXBopenglweight->drawshape(axbopenglweight::None);
}

void Qt_Opengl::wireframe()
{
	ui.AXBopenglweight->setWirefrme(ui.action1->isChecked());
}