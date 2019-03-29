#include <showwidget.h>
ShowWidget::ShowWidget(QWidget *parent) : QWidget(parent)
{
	//�½�һ��ˮƽ�ָ�ڶ�����Ϊ�����ֿ�
	splitterMain = new QSplitter(Qt::Horizontal, 0);
	splitterMain->setOpaqueResize(true);
	splitterLeft = new QSplitter(Qt::Vertical, splitterMain);
	list = new QListWidget(splitterLeft);
	list->insertItem(0, QString::fromLocal8Bit("�������"));
	list->insertItem(1, QString::fromLocal8Bit("�������"));
	list->insertItem(2, QString::fromLocal8Bit("�ĸ����"));
	list->setMaximumSize(QSize(150, 600));
	hitLabel = new QLabel(QString::fromLocal8Bit("��ʾ��Ϣ"), splitterLeft);
	textEdit = new QTextEdit(QString::fromLocal8Bit("��ز�����ʾ"), splitterLeft);
	practice = new CenterWidget(splitterMain);
	QObject::connect(list, SIGNAL(currentRowChanged(int)), practice->stack, SLOT(setCurrentIndex(int)));
	fileview = new FileWidget(splitterMain);
	//���������ֿ�ˮƽ�ָ�ڵı���
	//splitterMain->setWindowTitle(QString::fromLocal8Bit("3Dͼ�����"));
	//���������ֿ�ˮƽ�ָ�ڵ���С�ߴ�
	//splitterMain->setMinimumSize(splitterMain->minimumSize());
	//���������ֿ�ˮƽ�ָ�ڵ����ߴ�
	//splitterMain->setMaximumSize(splitterMain->maximumSize());
	//splitterMain->show();
	horizontalLayout = new QHBoxLayout(this);
	horizontalLayout->addWidget(splitterMain);
}