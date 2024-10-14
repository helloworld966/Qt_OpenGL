#include "Qt_Opengl.h"
Qt_Opengl::Qt_Opengl(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	//ÐÅºÅºÍ²Û±ØÐëÓÐ

	//»­Í¼
	connect(ui.action, &QAction::triggered, this, &Qt_Opengl::actdraw);
	//Çå³ý
	connect(ui.actionactclear, &QAction::triggered, this, &Qt_Opengl::actclear);

	//äÖÈ¾
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