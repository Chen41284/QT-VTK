#include <FileWidget.h>
FileWidget::FileWidget(QWidget *parent) : QWidget(parent)
{
	createBase();
}

void FileWidget::createBase()
{
	setWindowTitle(tr("File View"));
	fileLineEdit = new QLineEdit(tr("C:/"));
	fileListWidget = new QListWidget;
	RenderButton = new QPushButton(QString::fromLocal8Bit("�ػ�"));
	mainLayout = new QVBoxLayout(this);
	mainLayout->addWidget(fileLineEdit);
	mainLayout->addWidget(fileListWidget);
	mainLayout->addWidget(RenderButton);
	connect(fileLineEdit, SIGNAL(returnPressed()), this, SLOT(slotShow(QDir)));
	connect(fileLineEdit, SIGNAL(textChanged(const QString&)), this, SLOT(slotchanged()));
	connect(fileListWidget, SIGNAL(itemDoubleClicked(QListWidgetItem*)),
		this, SLOT(slotDirShow(QListWidgetItem*)));
	QString root = "C:/";
	QDir rootDir(root);
	QStringList string;
	string << "*";
	QFileInfoList list = rootDir.entryInfoList(string);
	showFileInfoList(list);
}

void FileWidget::slotchanged()
{
	QDir dir;
	dir.setPath(fileLineEdit->text());  //����QDir�����·��Ϊ��ǰĿ¼·��
	QStringList string;
	string << "*";
	QFileInfoList list = dir.entryInfoList(string, QDir::AllEntries, QDir::DirsFirst);										//(a)
	showFileInfoList(list);
}
void FileWidget::slotShow(QDir dir)
{
	QStringList string;
	string << "*";
	QFileInfoList list = dir.entryInfoList(string, QDir::AllEntries, QDir::DirsFirst);										//(a)
	showFileInfoList(list);
}

void FileWidget::showFileInfoList(QFileInfoList list)
{
	fileListWidget->clear();					//��������б�ؼ�
	for (int i = 0; i < list.count(); i++)	//(a)
	{
		QFileInfo tmpFileInfo = list.at(i);
		if (tmpFileInfo.isDir())
		{
			QIcon icon("Icons/dir.png");
			QString fileName = tmpFileInfo.fileName();
			QListWidgetItem *tmp = new QListWidgetItem(icon, fileName);
			tmp->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsSelectable);
			tmp->setCheckState(Qt::Unchecked);
			fileListWidget->addItem(tmp);
		}
		else if (tmpFileInfo.isFile())
		{
			QIcon icon("Icons/file.png");
			QString fileName = tmpFileInfo.fileName();
			QListWidgetItem *tmp = new QListWidgetItem(icon, fileName);
			tmp->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsSelectable);
			tmp->setCheckState(Qt::Unchecked);
			fileListWidget->addItem(tmp);
		}
	}
}

void FileWidget::slotDirShow(QListWidgetItem * item)
{
	QString str = item->text();			//����һ����Ŀ¼��������str��
	QDir dir;							//����һ��QDir����
	dir.setPath(fileLineEdit->text());  //����QDir�����·��Ϊ��ǰĿ¼·��
	dir.cd(str);                        //������һ��Ŀ¼����������QDir�����·��
	fileLineEdit->setText(dir.absolutePath());	//(a)
	slotShow(dir);						//��ʾ��ǰĿ¼�µ������ļ�
}