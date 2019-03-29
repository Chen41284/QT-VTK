#include "CenterWidget.h"

CenterWidget::CenterWidget(QWidget *parent)
	: QFrame(parent)
{
	stack = new QStackedWidget(this);
	//�Զ�ջ���ڵ���ʾ����������
	stack->setFrameStyle(QFrame::Panel | QFrame::Raised);
	/* ��������ҳ�� */
	onewidget = new OneWidget();
	twowidget = new TwoWidget();
	fourwidget = new FourWidget();
	stack->addWidget(onewidget);
	stack->addWidget(twowidget);
	stack->addWidget(fourwidget);
	/* ����������ť */
	ToolBtn = new QPushButton(QString::fromLocal8Bit("������"));
	CloseBtn = new QPushButton(QString::fromLocal8Bit("�ر�"));
	QHBoxLayout *BtnLayout = new QHBoxLayout;
	BtnLayout->addStretch(1);
	BtnLayout->addWidget(ToolBtn);
	BtnLayout->addWidget(CloseBtn);
	/* �������岼�� */
	QVBoxLayout *RightLayout = new QVBoxLayout(this);
	RightLayout->setMargin(10);
	RightLayout->setSpacing(6);
	RightLayout->addWidget(stack);
	RightLayout->addLayout(BtnLayout);
}
