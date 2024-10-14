#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_Qt_Opengl.h"
#include"axbopenglweight.h"
class Qt_Opengl : public QMainWindow
{
	Q_OBJECT

public:
	Qt_Opengl(QWidget *parent = Q_NULLPTR);

public slots:
	void actdraw();
	void actclear();
	void wireframe();

private:
	Ui::Main ui;
};
