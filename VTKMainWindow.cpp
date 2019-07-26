#include <VTKMainWindow.h>
#include "vtkSmartPointer.h" 
#include <ImgProcessor.h>
#include <qfiledialog.h>
#include <qmessagebox.h>
#include <qinputdialog.h>
#include <VTK3D.h>
#include <volumeRender.h>
#include <stdio.h>
#include <stdlib.h>
#include <InputDialog.h>
#include <Data.h>
#include <FileWidget.h>
#include <PCLPoint.h>
#include <VTKPick.h>
#include <TestFunction.h>
#include "PythonImageSegment.h"
#include "AToBTransform.h"
#include <qtextstream.h>
#include <qiodevice.h>
#include "PlySimply.h"
#include "Mesh_Saliency.h"

#define VTK_CREATE(type, name) \
  vtkSmartPointer<type> name = vtkSmartPointer<type>::New()
VTKMainWindow::VTKMainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	QIcon icon;
	icon.addFile(QStringLiteral("Icons/VTK.png"), QSize(), QIcon::Normal, QIcon::Off);
	setWindowIcon(icon);
	//setIconSize(QSize(22, 22));
	setWindowTitle(QString::fromLocal8Bit("3Dģ�ʹ���"));         //���ô������
	showwidget = new ShowWidget(this);
	setCentralWidget(showwidget);
	firstActor = NULL;
	//��ʼ�����������
	save = none;
	/* �����������˵����������ĺ��� */
	createActions();
	createMenus();
	createToolBars();
	connect(showwidget->practice->CloseBtn, SIGNAL(clicked()), this, SLOT(close()));
	connect(showwidget->practice->ToolBtn, SIGNAL(clicked()), this, SLOT(slotRemind()));
}

void VTKMainWindow::createActions()
{
	//"��"����
	openFileAction = new QAction(QIcon("Icons/open.png"), QString::fromLocal8Bit("��"), this);
	openFileAction->setShortcut(tr("Ctrl+O"));
	openFileAction->setStatusTip(QString::fromLocal8Bit("��һ���ļ�"));
	connect(openFileAction, SIGNAL(triggered()), this, SLOT(slotShow()));
	//�����ļ�
	filesaveAction = new QAction(QIcon("Icons/filesave.png"), QString::fromLocal8Bit("����"), this);
	filesaveAction->setShortcut(tr("Ctrl + S"));
	filesaveAction->setStatusTip(QString::fromLocal8Bit("�����ļ�"));
	connect(filesaveAction, SIGNAL(triggered()), this, SLOT(slotSaveFile()));
	//"�½�"����
	NewFileAction = new QAction(QIcon("Icons/new.png"), QString::fromLocal8Bit("�½�"), this);
	NewFileAction->setShortcut(tr("Ctrl+N"));
	NewFileAction->setStatusTip(QString::fromLocal8Bit("�½�һ���ļ�"));
	connect(NewFileAction, SIGNAL(triggered()), this, SLOT(slotRemind()));      //������
	//"�˳�"����
	exitAction = new QAction(QString::fromLocal8Bit("�˳�"), this);
	exitAction->setShortcut(tr("Ctrl+Q"));
	exitAction->setStatusTip(QString::fromLocal8Bit("�˳�����"));
	connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));
	//"����"����
	copyAction = new QAction(QIcon("Icons/copy.png"), QString::fromLocal8Bit("����"), this);
	copyAction->setShortcut(tr("Ctrl+C"));
	copyAction->setStatusTip(QString::fromLocal8Bit("�����ļ�"));
	connect(copyAction, SIGNAL(triggered()), this, SLOT(slotRemind()));       //������
	//"����"����
	cutAction = new QAction(QIcon("Icons/cut.png"), QString::fromLocal8Bit("����"), this);
	cutAction->setShortcut(tr("Ctrl+X"));
	cutAction->setStatusTip(QString::fromLocal8Bit("�����ļ�"));
	connect(cutAction, SIGNAL(triggered()), this, SLOT(slotRemind()));      //������
	//"ճ��"����
	pasteAction = new QAction(QIcon("Icons/paste.png"), QString::fromLocal8Bit("ճ��"), this);
	pasteAction->setShortcut(tr("Ctrl+V"));
	pasteAction->setStatusTip(QString::fromLocal8Bit("ճ���ļ�"));
	connect(pasteAction, SIGNAL(triggered()), this, SLOT(slotRemind()));      //������
	//"����"����
	aboutAction = new QAction(QString::fromLocal8Bit("����"), this);
	connect(aboutAction, SIGNAL(triggered()), this, SLOT(slotAbout()));
	//"��ӡ3Dģ��"����
	Print3DModelAction = new QAction(QIcon("Icons/printText.png"), QString::fromLocal8Bit("��ӡ3Dģ��"), this);
	Print3DModelAction->setStatusTip(QString::fromLocal8Bit("��ӡһ��3Dģ��"));
	connect(Print3DModelAction, SIGNAL(triggered()), this, SLOT(slotRemind()));      //������
	//"��ӡͼ��"����
	PrintImageAction = new QAction(QIcon("Icons/printImage.png"), QString::fromLocal8Bit("��ӡͼ��"), this);
	PrintImageAction->setStatusTip(QString::fromLocal8Bit("��ӡһ��ͼ��"));
	connect(PrintImageAction, SIGNAL(triggered()), this, SLOT(slotRemind()));      //������
	//"�Ŵ�"����
	zoomInAction = new QAction(QIcon("Icons/zoomin.png"), QString::fromLocal8Bit("�Ŵ�"), this);
	zoomInAction->setStatusTip(QString::fromLocal8Bit("�Ŵ�һ��ͼƬ"));
	connect(zoomInAction, SIGNAL(triggered()), this, SLOT(slotRemind()));      //������
	//"��С"����
	zoomOutAction = new QAction(QIcon("Icons/zoomout.png"), QString::fromLocal8Bit("��С"), this);
	zoomOutAction->setStatusTip(QString::fromLocal8Bit("��Сһ��ͼƬ"));
	connect(zoomOutAction, SIGNAL(triggered()), this, SLOT(slotRemind()));      //������
	//ʵ��ͼ����ת�Ķ���(Action)
	//��ת90��
	rotate90Action = new QAction(QIcon("Icons/rotate90.png"), QString::fromLocal8Bit("��ת90��"), this);
	rotate90Action->setStatusTip(QString::fromLocal8Bit("��һ��ͼ��ת90��"));
	connect(rotate90Action, SIGNAL(triggered()), this, SLOT(slotRemind()));      //������
	//��ת180��
	rotate180Action = new QAction(QIcon("Icons/rotate180.png"), QString::fromLocal8Bit("��ת180��"), this);
	rotate180Action->setStatusTip(QString::fromLocal8Bit("��һ��ͼ��ת180��"));
	connect(rotate180Action, SIGNAL(triggered()), this, SLOT(slotRemind()));      //������
	//��ת270��
	rotate270Action = new QAction(QIcon("Icons/rotate270.png"), QString::fromLocal8Bit("��ת270��"), this);
	rotate270Action->setStatusTip(QString::fromLocal8Bit("��һ��ͼ��ת270��"));
	connect(rotate270Action, SIGNAL(triggered()), this, SLOT(slotRemind()));      //������
	//ʵ��ͼ����Ķ���(Action)
	//������
	mirrorVerticalAction = new QAction(QIcon("Icons/mirrorVertical.png"), QString::fromLocal8Bit("������"), this);
	mirrorVerticalAction->setStatusTip(QString::fromLocal8Bit("��һ��ͼ��������"));
	connect(mirrorVerticalAction, SIGNAL(triggered()), this, SLOT(slotRemind()));      //������
	//������
	mirrorHorizontalAction = new QAction(QIcon("Icons/mirrorHorizontal.png"), QString::fromLocal8Bit("������"), this);
	mirrorHorizontalAction->setStatusTip(QString::fromLocal8Bit("��һ��ͼ��������"));
	connect(mirrorHorizontalAction, SIGNAL(triggered()), this, SLOT(slotRemind()));      //������
	//ʵ�ֳ����ͻָ��Ķ���(Action)
	//�����ͻָ�
	undoAction = new QAction(QIcon("Icons/undo.png"), "����", this);
	undoAction->setStatusTip(QString::fromLocal8Bit("����"));
	connect(undoAction, SIGNAL(triggered()), this, SLOT(slotRemind()));      //������
	redoAction = new QAction(QIcon("Icons/redo.png"), "����", this);
	redoAction->setStatusTip(QString::fromLocal8Bit("����"));
	connect(redoAction, SIGNAL(triggered()), this, SLOT(slotRemind()));      //������

	//VTK�Ĺ��� -- ���/���
	CPUVolumeAction = new QAction(QString::fromLocal8Bit("CPU���"));
	connect(CPUVolumeAction, SIGNAL(triggered()), this, SLOT(slotCPUVolumeRender()));
	GPUVolumeAction = new QAction(QString::fromLocal8Bit("GPU���"));
	connect(GPUVolumeAction, SIGNAL(triggered()), this, SLOT(slotGPUVolumeRender()));
	GPUVolumeAction->setStatusTip(QString::fromLocal8Bit("ʹ�ö����Կ�����"));
	ctkVTKGPUVolumeAction = new QAction(QString::fromLocal8Bit("ʹ��ctk��������Ʋ���"));
	connect(ctkVTKGPUVolumeAction, SIGNAL(triggered()), this, SLOT(slotctkVTKGPUVolumeRender()));
	MarchingCubesAction = new QAction(QString::fromLocal8Bit("���"));
	connect(MarchingCubesAction, SIGNAL(triggered()), this, SLOT(slotShowCubes()));
	MarchingDirCTAction = new QAction(QString::fromLocal8Bit("��Ŀ¼��棨DICOMͼ��"));
	connect(MarchingDirCTAction, SIGNAL(triggered()), this, SLOT(slotDirCTCubes()));

	OBJOutputAction = new QAction(QString::fromLocal8Bit("���OBJģ��"));

	//VTK�Ĺ��� -- ��/���ʰȡ
	SinglePointPickAction = new QAction(QString::fromLocal8Bit("������ı��"));
	connect(SinglePointPickAction, SIGNAL(triggered()), this, SLOT(slotSinglePointPick()));
	MutiplePointPickAction = new QAction(QString::fromLocal8Bit("�����ı��"));
	CellPickAction = new QAction(QString::fromLocal8Bit("��Ԫ�ı��"));
	AreaPickAction = new QAction(QString::fromLocal8Bit("����ı��"));
	connect(AreaPickAction, SIGNAL(triggered()), this, SLOT(slotAreaPick()));
	PropPickAction = new QAction(QString::fromLocal8Bit("���������ʰȡ"));
	connect(PropPickAction, SIGNAL(triggered()), this, SLOT(slotActorPick()));

	//PCL�Ĺ���
	PCDInputAction = new QAction(QString::fromLocal8Bit("PCDģ�͵�����"));
	connect(PCDInputAction, SIGNAL(triggered()), this, SLOT(slotPCD()));
	obj2pcdAction = new QAction(QString::fromLocal8Bit("OBJģ��תΪPCDģ��"));
	connect(obj2pcdAction, SIGNAL(triggered()), this, SLOT(slotobj2pcd())); 

	//Python�Ĺ���
	PyImageSegmentAction = new QAction(QString::fromLocal8Bit("Pythonͼ��ָ�"));
	connect(PyImageSegmentAction, SIGNAL(triggered()), this, SLOT(slotPyImageSegment()));

	//��������Ĺ���
	STL2PLYAction = new QAction(QString::fromLocal8Bit("STLģ��תΪPLYģ��"));
	connect(STL2PLYAction, SIGNAL(triggered()), this, SLOT(slotSTL2PLY()));
	OBJ2PLYAction = new QAction(QString::fromLocal8Bit("OBJģ��תΪPLYģ��"));
	connect(OBJ2PLYAction, SIGNAL(triggered()), this, SLOT(slotOBJ2PLY()));
	PLY2BPAction = new QAction(QString::fromLocal8Bit("PLYģ��תΪ����"));
	connect(PLY2BPAction, SIGNAL(triggered()), this, SLOT(slotPLY2BP()));
	PLYSimplyAction = new QAction(QString::fromLocal8Bit("PLYģ�ͼ�"));
	connect(PLYSimplyAction, SIGNAL(triggered()), this, SLOT(slotPLYSimply()));
	PLYInformationAction = new QAction(QString::fromLocal8Bit("PLYģ�͵���Ϣ"));
	connect(PLYInformationAction, SIGNAL(triggered()), this, SLOT(slotPLYInformation()));
	PLYFeatureAction = new QAction(QString::fromLocal8Bit("PLYģ�͵�������"));
	connect(PLYFeatureAction, SIGNAL(triggered()), this, SLOT(slotPLYFeature()));

	//-----------������С����--------------------------------
	WordsEditAction = new QAction(QString::fromLocal8Bit("���ֱ༭��"), this);
	connect(WordsEditAction, SIGNAL(triggered()), this, SLOT(ShowWordsEdit()));
	FuctionlogAction = new QAction(QString::fromLocal8Bit("������־����"), this);
	connect(FuctionlogAction, SIGNAL(triggered()), this, SLOT(slotWriteLog()));

	//�ļ��������Ӧ
	connect(showwidget->fileview->fileListWidget, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(slotItemClicked()));
	connect(showwidget->fileview->fileListWidget, SIGNAL(itemSelectionChanged()),
		this, SLOT(slotSelectionChanged()));
	connect(showwidget->fileview->fileListWidget, SIGNAL(itemChanged(QListWidgetItem*)), this, 
		SLOT(slotItemChanged(QListWidgetItem*)));
	connect(showwidget->fileview->RenderButton, SIGNAL(clicked(bool)), this, SLOT(slotRender()));
	connect(showwidget->fileview->fileLineEdit, SIGNAL(textChanged(const QString&)), this, SLOT(slotClear()));
}

void VTKMainWindow::createMenus()
{
	//�ļ��˵�
	fileMenu = menuBar()->addMenu(QString::fromLocal8Bit("�ļ�"));
	fileMenu->addAction(openFileAction);
	fileMenu->addAction(filesaveAction);
	fileMenu->addAction(NewFileAction);
	fileMenu->addAction(Print3DModelAction);
	fileMenu->addAction(PrintImageAction);
	fileMenu->addSeparator();
	fileMenu->addAction(exitAction);
	//���Ų˵�
	zoomMenu = menuBar()->addMenu(QString::fromLocal8Bit("�༭"));
	zoomMenu->addAction(copyAction);
	zoomMenu->addAction(cutAction);
	zoomMenu->addAction(pasteAction);
	zoomMenu->addSeparator();
	zoomMenu->addAction(zoomInAction);
	zoomMenu->addAction(zoomOutAction);
	//ͼ����˵�
	pictureMenu = menuBar()->addMenu(QString::fromLocal8Bit("ͼ����"));
	//ͼ�����Ӳ˵�--��ת�˵�
	rotateMenu = pictureMenu->addMenu(QString::fromLocal8Bit("��ת"));
	rotateMenu->addAction(rotate90Action);
	rotateMenu->addAction(rotate180Action);
	rotateMenu->addAction(rotate270Action);
	//ͼ�����Ӳ˵�--����˵�
	mirrorMenu = pictureMenu->addMenu(QString::fromLocal8Bit("����"));
	mirrorMenu->addAction(mirrorVerticalAction);
	mirrorMenu->addAction(mirrorHorizontalAction);
	//Pythonͼ��ָ�
	pictureMenu->addAction(PyImageSegmentAction);
	//VTK����
	vtkMenu = menuBar()->addMenu(QString::fromLocal8Bit("3Dģ���ؽ�"));
	vtkMenu->addAction(CPUVolumeAction);
	vtkMenu->addAction(GPUVolumeAction);
	vtkMenu->addAction(MarchingCubesAction);
	vtkMenu->addAction(MarchingDirCTAction);
	vtkMenu->addAction(OBJOutputAction);
	vtkMenu->addAction(ctkVTKGPUVolumeAction);
	PickMenu = vtkMenu->addMenu(QString::fromLocal8Bit("ʰȡ����"));
	PickMenu->addAction(SinglePointPickAction);
	//PickMenu->addAction(MutiplePointPickAction);
	//PickMenu->addAction(CellPickAction);
	PickMenu->addAction(AreaPickAction);
	PickMenu->addAction(PropPickAction);
	//PCL����
	pclMenu = menuBar()->addMenu(QString::fromLocal8Bit("����ģ��"));
	pclMenu->addAction(PCDInputAction);
	pclMenu->addAction(obj2pcdAction);
	//�������⹦��
	featureMenu = menuBar()->addMenu(QString::fromLocal8Bit("��������"));
	featureMenu->addAction(STL2PLYAction);
	featureMenu->addAction(OBJ2PLYAction);
	featureMenu->addAction(PLY2BPAction);
	featureMenu->addAction(PLYSimplyAction);
	featureMenu->addAction(PLYInformationAction);
	featureMenu->addAction(PLYFeatureAction);
	//�������ܵĲ˵�
	OtherMenu = menuBar()->addMenu(QString::fromLocal8Bit("��������"));
	OtherMenu->addAction(WordsEditAction);
	//OtherMenu->addAction(FuctionlogAction);
	OtherMenu->addAction(aboutAction);
}

void VTKMainWindow::createToolBars()
{
	//�ļ�������
	fileTool = addToolBar("File");
	fileTool->addAction(openFileAction);
	fileTool->addAction(filesaveAction);
	fileTool->addAction(NewFileAction);
	fileTool->addAction(Print3DModelAction);
	fileTool->addAction(PrintImageAction);
	//�༭������
	zoomTool = addToolBar("Edit");
	zoomTool->addAction(copyAction);
	zoomTool->addAction(cutAction);
	zoomTool->addAction(pasteAction);
	zoomTool->addSeparator();
	zoomTool->addAction(zoomInAction);
	zoomTool->addAction(zoomOutAction);
	//��ת������
	rotateTool = addToolBar("rotate");
	rotateTool->addAction(rotate90Action);
	rotateTool->addAction(rotate180Action);
	//����������������
	doToolBar = addToolBar("doEdit");
	doToolBar->addAction(undoAction);
	doToolBar->addAction(redoAction);
}

void VTKMainWindow::ShowWordsEdit()
{
	ImgProcessor *newImgProcessor = new ImgProcessor;
	newImgProcessor->show();
}

//��־���
void VTKMainWindow::slotWriteLog()
{
	//freopen("C:\\Users\\chenjiaxing\\Desktop\\CenterWidget\\Deps\\log\\log.txt", "w", stdout); 
	//freopen("CON", "w", stdout); //recover stdout(Windows)
	//freopen("C:\\Users\\chenjiaxing\\Desktop\\CenterWidget\\Deps\\log\\log.txt", "w", stdout);
	//ColorPickDialog *test = new ColorPickDialog(this);
	//if (!test->exec())
	//{
	//	printf("Cancel");
	//	return;
	//}
	//double radius = test->getRadius();
	//printf("radius:%f\n", radius);
	//QRgb rgb = test->getRGB();
	//
	//double RGB[3] = { qRed(rgb), qBlue(rgb), qGreen(rgb) };
	//printf("Red:%f\n", RGB[0]);
	//printf("Blue:%f\n", RGB[1]);
	//printf("Green:%f\n", RGB[2]);
	//freopen("CON", "w", stdout); //recover stdout(Windows)
	//TestFunction(showwidget->practice->onewidget->qvtkwidget1);
	ColorDialog *test = new ColorDialog(0);
	if (!test->exec())
	{
		QMessageBox::information(this, QString::fromLocal8Bit("������Ϣ"),
			QString::fromLocal8Bit("�ѵ��ȡ��"));
		return;
	}
	else
	{
		QMessageBox::information(this, QString::fromLocal8Bit("������Ϣ"),
			QString::fromLocal8Bit("�ѵ��ȷ��"));
	}
}

void VTKMainWindow::slotShow()
{
	QString QSfilePath = QFileDialog::getOpenFileName(this);
	Show(QSfilePath);
}
void VTKMainWindow::Show(QString &QSfilePath)
{
	if (QSfilePath == NULL)
	{
		return;
	}
	QByteArray QBfilePath = QSfilePath.toLocal8Bit();
	const char* filepath = QBfilePath.data();
	//��ȡ��չ��
	/*int exetension = QSfilePath.lastIndexOf(".");
	QString fileExt = QSfilePath.right(QSfilePath.length() - exetension - 1);
	QByteArray Extba = fileExt.toLocal8Bit();
	const char* fileExten = Extba.data();*/
	std::string extension = vtksys::SystemTools::GetFilenameLastExtension(std::string(filepath));
	
	int currentrow = showwidget->list->currentRow();
	if (currentrow == 0)      //�������
	{
		//if (!_stricmp(fileExten, "jpg") || !_stricmp(fileExten, "bmp") || !_stricmp(fileExten, "png"))
		if (extension == ".jpg" || extension == ".bmp" || extension == ".png")
		{
			if (showwidget->practice->onewidget->initialCount == 0)
			{
				showwidget->practice->onewidget->initialImage();
			}
			showwidget->practice->onewidget->ShowImage(filepath);
		}
		else if (extension == ".dcm")
		{
			if (showwidget->practice->onewidget->initialCount == 0)
			{
				showwidget->practice->onewidget->initialImage();
			}
			showwidget->practice->onewidget->ShowDICOM(filepath);
		}
		else if (extension == ".stl" || extension == ".obj" || extension == ".ply" ||
			     extension == ".vtp" || extension == ".g" || extension == ".vtk" ||
			     extension == ".fp"  || extension == ".bp")
		{
			VTK3D *vtk3d = new VTK3D(showwidget->practice->onewidget->qvtkwidget1);
			vtk3d->ShowVTK3D(filepath);
		}
		else if (extension == ".pcd")
		{
			PCLPoint *pcl = new PCLPoint(showwidget->practice->onewidget->qvtkwidget1);
			pcl->ShowPointCloud(filepath);
		}
		else if(extension.empty())
		{
			QMessageBox::information(this, QString::fromLocal8Bit("������Ϣ"),
				QString::fromLocal8Bit("��������к�׺�����ļ�"));
		}
		else
		{
			QMessageBox::information(this, QString::fromLocal8Bit("������Ϣ"),
				QString::fromLocal8Bit("�ݲ�֧�����ָ�ʽ���ļ�"));
		}
	}
	else if (currentrow == 1)  //˫���
	{
		bool ok;
		int i = QInputDialog::getInt(this, QString::fromLocal8Bit("ѡ����ʾ�����"),
			QString::fromLocal8Bit("���������ı��"), 1, 1, 2, 1, &ok);
		if (!ok)
		{
			return;
		}
		switch (i)
		{
		case 1:
			usedwidget = showwidget->practice->twowidget->qvtkwidge1;
			break;
		case 2:
			usedwidget = showwidget->practice->twowidget->qvtkwidge2;
			break;
		}
		if (extension == ".jpg" || extension == ".bmp" || extension == ".png")
		{
			if (showwidget->practice->twowidget->initialCount[i - 1] == 0)
			{
				showwidget->practice->twowidget->initialImage(i);
			}
			showwidget->practice->twowidget->ShowImage(filepath, i);
		}
		else if (extension == ".dcm")
		{
			if (showwidget->practice->twowidget->initialCount[i - 1] == 0)
			{
				showwidget->practice->twowidget->initialImage(i);
			}
			showwidget->practice->twowidget->ShowDICOM(filepath, i);
		}
		else if (extension == ".stl" || extension == ".obj" || extension == ".ply" ||
			     extension == ".vtp" || extension == ".g" || extension == ".vtk" ||
			     extension == ".fp" || extension == ".bp")
		{
			VTK3D *vtk3d = new VTK3D(usedwidget);
			vtk3d->ShowVTK3D(filepath);
		}
		else if (extension == ".pcd")
		{
			PCLPoint *pcl = new PCLPoint(usedwidget);
			pcl->ShowPointCloud(filepath);
		}
		else if (extension.empty())
		{
			QMessageBox::information(this, QString::fromLocal8Bit("������Ϣ"),
				QString::fromLocal8Bit("��������к�׺�����ļ�"));
		}
		else
		{
			QMessageBox::information(this, QString::fromLocal8Bit("������Ϣ"),
				QString::fromLocal8Bit("�ݲ�֧�����ָ�ʽ���ļ�"));
		}
	}
	else if (currentrow == 2)  //�ĸ����
	{
		//showwidget->practice->fourwidget->testImage();
		bool ok;
		int i = QInputDialog::getInt(this, QString::fromLocal8Bit("ѡ����ʾ�����"),
			QString::fromLocal8Bit("���������ı��"), 1, 1, 4, 1, &ok);
		if (!ok)
		{
			return;
		}
		switch (i)
		{
		case 1:
			usedwidget = showwidget->practice->fourwidget->qvtkwidge1;
			break;
		case 2:
			usedwidget = showwidget->practice->fourwidget->qvtkwidge2;
			break;
		case 3:
			usedwidget = showwidget->practice->fourwidget->qvtkwidge3;
			break;
		case 4:
			usedwidget = showwidget->practice->fourwidget->qvtkwidge4;
			break;
		}
		if (extension == ".jpg" || extension == ".bmp" || extension == ".png")
		{
			if (showwidget->practice->fourwidget->initialCount[i-1] == 0)
			{
				showwidget->practice->fourwidget->initialImage(i);
			}
			showwidget->practice->fourwidget->ShowImage(filepath, i);
		}
		else if (extension == ".dcm")
		{
			if (showwidget->practice->fourwidget->initialCount[i - 1] == 0)
			{
				showwidget->practice->fourwidget->initialImage(i);
			}
			showwidget->practice->fourwidget->ShowDICOM(filepath, i);
		}
		else if (extension == ".stl" || extension == ".obj" || extension == ".ply" ||
			     extension == ".vtp" || extension == ".g" || extension == ".vtk" ||
			     extension == ".fp" || extension == ".bp")
		{
			VTK3D *vtk3d = new VTK3D(usedwidget);
			vtk3d->ShowVTK3D(filepath);
		}
		else if (extension == ".pcd")
		{
			PCLPoint *pcl = new PCLPoint(usedwidget);
			pcl->ShowPointCloud(filepath);
		}
		else if (extension.empty())
		{
			QMessageBox::information(this, QString::fromLocal8Bit("������Ϣ"),
				QString::fromLocal8Bit("��������к�׺�����ļ�"));
		}
		else
		{
			QMessageBox::information(this, QString::fromLocal8Bit("������Ϣ"),
				QString::fromLocal8Bit("�ݲ�֧�����ָ�ʽ���ļ�"));
		}
	}
}

void VTKMainWindow::slotSaveFile()
{
	const char* filepath = NULL;
	if (save == OBJ)
	{
		QString Savename = QFileDialog::getSaveFileName(this,
			QString::fromLocal8Bit("�����ļ�"),
			"");//,
				//tr("obj Files (*.obj);; pcd Files(*.pcd)"));
		if (Savename.length() == 0)
		{
			return;
		}
		// ֧�ִ�����·�����ļ�����ȡ
		QByteArray ba = Savename.toLocal8Bit();
		filepath = ba.data();
	}
	switch (save)
	{
	case none:
		showwidget->textEdit->setText(QString::fromLocal8Bit("Ŀǰ��֧��OBJ��PCD���ݵı���"));
		QMessageBox::information(this, QString::fromLocal8Bit("������Ϣ"),
			QString::fromLocal8Bit("�ݲ��ܱ����������͵�����"));
		break;
	case OBJ:
		marching->WriteOBJ(filepath);
		break;	
	case PCD:
		break;
	case PointTxt:
		WritePointTxt();
		break;
	default:
		break;
	}
}

void VTKMainWindow::WritePointTxt()
{
	QString pointPath = QFileDialog::getOpenFileName(this, tr("Open File"), " ", tr("txt file(*.txt)"));
	QFile file(pointPath);
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		QMessageBox::critical(NULL, QString::fromLocal8Bit("��ʾ"), QString::fromLocal8Bit("�޷����ļ�"));
		return;
	}
	QTextStream intxt(&file);
	intxt << showwidget->textEdit->toPlainText().remove(QString::fromLocal8Bit("���Ա���Ϊtxt�ļ�"));
	intxt.flush();
	file.close();
}

void VTKMainWindow::slotShowCubes()
{
	QString QSpicturePath = QFileDialog::getOpenFileName(this);
	if (QSpicturePath == NULL)
	{
		return;
	}
	QByteArray QBpicturePath = QSpicturePath.toLocal8Bit();
	const char* picturepath = QBpicturePath.data();
	//��ȡ��չ��
	int exetension = QSpicturePath.lastIndexOf(".");
	QString fileExt = QSpicturePath.right(QSpicturePath.length() - exetension - 1);
	QByteArray Extba = fileExt.toLocal8Bit();
	const char* fileExten = Extba.data();
	//��ȡĿ¼��
	int dirindex = QSpicturePath.lastIndexOf("/");
	QString dirpath = QSpicturePath.left(dirindex);
	QByteArray dirba = dirpath.toLocal8Bit();
	const char* dirpath2 = dirba.data();
	QDir dir(dirpath2);
	QStringList filter;
	//filter << "*.jpg";
	//dir.setNameFilters(filter);
	//QFileInfoList list = dir.entryInfoList();//��ȡ�ļ���Ϣ�б�  //����

	int currentrow = showwidget->list->currentRow();
	if (currentrow == 0)      //�������
	{
		if (!_stricmp(fileExten, "jpg"))
		{
			filter << "*.jpg";
			dir.setNameFilters(filter);
			QFileInfoList list = dir.entryInfoList();
			MarchingDialogSingle *test = new MarchingDialogSingle(this, 1, list.length()-1);  //��0��ʼ
			int rec = test->exec();
			if (rec ==	QDialog::Rejected)
			{
				return;
			}
			int skinvalue = test->getskinvalue();
			int startvalue = test->getstartvalue();
			int endvalue = test->getendvalue();
			marching = new MarchingCubes(showwidget->practice->onewidget->qvtkwidget1);
			marching->JPEGMarchingCubes(dirpath2, skinvalue, startvalue, endvalue);
			save = OBJ;
			showwidget->textEdit->setText(QString::fromLocal8Bit("���Խ���ǰ�ļ�����ΪOBJ�ļ�"));
		}
		else if (!_stricmp(fileExten, "dcm"))
		{
			filter << "*.dcm";
			dir.setNameFilters(filter);
			QFileInfoList list = dir.entryInfoList();
			MarchingDialogSingle *test = new MarchingDialogSingle(this, 0, list.length() - 1);  //��0��ʼ
			int rec = test->exec();
			if (rec == QDialog::Rejected)
			{
				return;
			}
			int skinvalue = test->getskinvalue();
			int bonevalue = test->getbonevalue();
			int startvalue = test->getstartvalue();
			int endvalue = test->getendvalue();
			marching = new MarchingCubes(showwidget->practice->onewidget->qvtkwidget1);
			marching->DICOMMarchingCubes(dirpath2, skinvalue, bonevalue, startvalue, endvalue);
			save = OBJ;
			showwidget->textEdit->setText(QString::fromLocal8Bit("���Խ���ǰ�ļ�����ΪOBJ�ļ�"));
		}
		else if (exetension == -1)
		{
			QMessageBox::information(this, QString::fromLocal8Bit("������Ϣ"),
				QString::fromLocal8Bit("��������к�׺�����ļ�"));
		}
		else
		{
			QMessageBox::information(this, QString::fromLocal8Bit("������Ϣ"),
				QString::fromLocal8Bit("�ݲ�֧�����ָ�ʽ���ļ�"));
		}
	}
	else if (currentrow == 1)  //˫���
	{
		bool ok;
		int i = QInputDialog::getInt(this, QString::fromLocal8Bit("ѡ����ʾ�����"),
			QString::fromLocal8Bit("���������ı��"), 1, 1, 2, 1, &ok);
		if (!ok)
		{
			return;
		}
		switch (i)
		{
		case 1:
			usedwidget = showwidget->practice->twowidget->qvtkwidge1;
			break;
		case 2:
			usedwidget = showwidget->practice->twowidget->qvtkwidge2;
			break;
		}
		if (!_stricmp(fileExten, "jpg"))
		{
			filter << "*.jpg";
			dir.setNameFilters(filter);
			QFileInfoList list = dir.entryInfoList();
			MarchingDialogSingle *test = new MarchingDialogSingle(this, 1, list.length()-1);
			int rec = test->exec();
			if (rec == QDialog::Rejected)
			{
				return;
			}
			int skinvalue = test->getskinvalue();
			int startvalue = test->getstartvalue();
			int endvalue = test->getendvalue();
			marching = new MarchingCubes(usedwidget);
			marching->JPEGMarchingCubes(dirpath2, skinvalue, startvalue, endvalue);
		}
		else if (!_stricmp(fileExten, "dcm"))
		{
			filter << "*.dcm";
			dir.setNameFilters(filter);
			QFileInfoList list = dir.entryInfoList();
			MarchingDialogSingle *test = new MarchingDialogSingle(this, 0, list.length() - 1);  //��0��ʼ
			int rec = test->exec();
			if (rec == QDialog::Rejected)
			{
				return;
			}
			int skinvalue = test->getskinvalue();
			int bonevalue = test->getbonevalue();
			int startvalue = test->getstartvalue();
			int endvalue = test->getendvalue();
			marching = new MarchingCubes(usedwidget);
			marching->DICOMMarchingCubes(dirpath2, skinvalue, bonevalue, startvalue, endvalue);
		}
		else if (exetension == -1)
		{
			QMessageBox::information(this, QString::fromLocal8Bit("������Ϣ"),
				QString::fromLocal8Bit("��������к�׺�����ļ�"));
		}
		else
		{
			QMessageBox::information(this, QString::fromLocal8Bit("������Ϣ"),
				QString::fromLocal8Bit("�ݲ�֧�����ָ�ʽ���ļ�"));
		}
	}
	else if (currentrow == 2)  //�ĸ����
	{
		bool ok;
		int i = QInputDialog::getInt(this, QString::fromLocal8Bit("ѡ����ʾ�����"),
			QString::fromLocal8Bit("���������ı��"), 1, 1, 4, 1, &ok);
		if (!ok)
		{
			return;
		}
		switch (i)
		{
		case 1:
			usedwidget = showwidget->practice->fourwidget->qvtkwidge1;
			break;
		case 2:
			usedwidget = showwidget->practice->fourwidget->qvtkwidge2;
			break;
		case 3:
			usedwidget = showwidget->practice->fourwidget->qvtkwidge3;
			break;
		case 4:
			usedwidget = showwidget->practice->fourwidget->qvtkwidge4;
			break;
		}
		if (!_stricmp(fileExten, "jpg"))
		{
			filter << "*.jpg";
			dir.setNameFilters(filter);
			QFileInfoList list = dir.entryInfoList();
			MarchingDialogSingle *test = new MarchingDialogSingle(this, 1, list.length()-1);
			int rec = test->exec();
			if (rec == QDialog::Rejected)
			{
				return;
			}
			int skinvalue = test->getskinvalue();
			int startvalue = test->getstartvalue();
			int endvalue = test->getendvalue();
			marching = new MarchingCubes(usedwidget);
			marching->JPEGMarchingCubes(dirpath2, skinvalue, startvalue, endvalue);
		}
		else if (!_stricmp(fileExten, "dcm"))
		{
			filter << "*.dcm";
			dir.setNameFilters(filter);
			QFileInfoList list = dir.entryInfoList();
			MarchingDialogSingle *test = new MarchingDialogSingle(this, 0, list.length() - 1);  //��0��ʼ
			int rec = test->exec();
			if (rec == QDialog::Rejected)
			{
				return;
			}
			int skinvalue = test->getskinvalue();
			int bonevalue = test->getbonevalue();
			int startvalue = test->getstartvalue();
			int endvalue = test->getendvalue();
			marching = new MarchingCubes(usedwidget);
			marching->DICOMMarchingCubes(dirpath2, skinvalue, bonevalue, startvalue, endvalue);
		}
		else if (exetension == -1)
		{
			QMessageBox::information(this, QString::fromLocal8Bit("������Ϣ"),
				QString::fromLocal8Bit("��������к�׺�����ļ�"));
		}
		else
		{
			QMessageBox::information(this, QString::fromLocal8Bit("������Ϣ"),
				QString::fromLocal8Bit("�ݲ�֧�����ָ�ʽ���ļ�"));
		}
	}
}

void VTKMainWindow::slotGPUVolumeRender()
{
	QString QSpicturePath = QFileDialog::getOpenFileName(this);
	if (QSpicturePath == NULL)
	{
		return;
	}
	QByteArray QBpicturePath = QSpicturePath.toLocal8Bit();
	const char* picturepath = QBpicturePath.data();
	//��ȡ��չ��
	int exetension = QSpicturePath.lastIndexOf(".");
	QString fileExt = QSpicturePath.right(QSpicturePath.length() - exetension - 1);
	QByteArray Extba = fileExt.toLocal8Bit();
	const char* fileExten = Extba.data();
	//��ȡĿ¼��
	int dirindex = QSpicturePath.lastIndexOf("/");
	QString dirpath = QSpicturePath.left(dirindex);
	QByteArray dirba = dirpath.toLocal8Bit();
	const char* dirpath2 = dirba.data();
	QDir dir(dirpath2);
	QStringList filter;

	int currentrow = showwidget->list->currentRow();
	if (currentrow == 0)      //�������
	{
		if (!_stricmp(fileExten, "jpg"))
		{
			filter << "*.jpg";
			dir.setNameFilters(filter);
			QFileInfoList list = dir.entryInfoList();
			VolumeInputDialog *inputvolume = new VolumeInputDialog(this, list.count() - 1);
			int rec = inputvolume->exec();
			if (rec == QDialog::Rejected)
			{
				return;
			}
			VolumeData *volumedata = inputvolume->getdata();
			//VolumeRender *volume = new VolumeRender(showwidget->practice->onewidget->qvtkwidget1, volumedata);
			VolumeRender *volume = new VolumeRender(showwidget->practice->onewidget->qvtkwidget1, 
				volumedata, showwidget->volumePropertywidget);
			volume->GPU(dirpath2, fileExten);
		}
		else if (!_stricmp(fileExten, "dcm"))
		{
			filter << "*.dcm";
			dir.setNameFilters(filter);
			QFileInfoList list = dir.entryInfoList();
			VolumeInputDialog *inputvolume = new VolumeInputDialog(this, list.count() - 1);
			int rec = inputvolume->exec();
			if (rec == QDialog::Rejected)
			{
				return;
			}
			VolumeData *volumedata = inputvolume->getdata();
			//VolumeRender *volume = new VolumeRender(showwidget->practice->onewidget->qvtkwidget1, volumedata);
			VolumeRender *volume = new VolumeRender(showwidget->practice->onewidget->qvtkwidget1, volumedata
				,showwidget->volumePropertywidget);
			volume->GPU(dirpath2, fileExten);
		}
		else if (exetension == -1)
		{
			QMessageBox::information(this, QString::fromLocal8Bit("������Ϣ"),
				QString::fromLocal8Bit("��������к�׺�����ļ�"));
		}
		else
		{
			QMessageBox::information(this, QString::fromLocal8Bit("������Ϣ"),
				QString::fromLocal8Bit("�ݲ�֧�����ָ�ʽ���ļ�"));
		}
	}
	else if (currentrow == 1)  //˫���
	{
		bool ok;
		int i = QInputDialog::getInt(this, QString::fromLocal8Bit("ѡ����ʾ�����"),
			QString::fromLocal8Bit("���������ı��"), 1, 1, 2, 1, &ok);
		if (!ok)
		{
			return;
		}
		switch (i)
		{
		case 1:
			usedwidget = showwidget->practice->twowidget->qvtkwidge1;
			break;
		case 2:
			usedwidget = showwidget->practice->twowidget->qvtkwidge2;
			break;
		}
		if (!_stricmp(fileExten, "jpg"))
		{
			filter << "*.jpg";
			dir.setNameFilters(filter);
			QFileInfoList list = dir.entryInfoList();
			VolumeInputDialog *inputvolume = new VolumeInputDialog(this, list.count() - 1);
			int rec = inputvolume->exec();
			if (rec == QDialog::Rejected)
			{
				return;
			}
			VolumeData *volumedata = inputvolume->getdata();
			VolumeRender *volume = new VolumeRender(usedwidget, volumedata, showwidget->volumePropertywidget);
			volume->GPU(dirpath2, fileExten);
		}
		else if (!_stricmp(fileExten, "dcm"))
		{
			filter << "*.dcm";
			dir.setNameFilters(filter);
			QFileInfoList list = dir.entryInfoList();
			VolumeInputDialog *inputvolume = new VolumeInputDialog(this, list.count() - 1);
			int rec = inputvolume->exec();
			if (rec == QDialog::Rejected)
			{
				return;
			}
			VolumeData *volumedata = inputvolume->getdata();
			VolumeRender *volume = new VolumeRender(usedwidget, volumedata, showwidget->volumePropertywidget);
			volume->GPU(dirpath2, fileExten);
		}
		else if (exetension == -1)
		{
			QMessageBox::information(this, QString::fromLocal8Bit("������Ϣ"),
				QString::fromLocal8Bit("��������к�׺�����ļ�"));
		}
		else
		{
			QMessageBox::information(this, QString::fromLocal8Bit("������Ϣ"),
				QString::fromLocal8Bit("�ݲ�֧�����ָ�ʽ���ļ�"));
		}
	}
	else if (currentrow == 2)  //�ĸ����
	{
		bool ok;
		int i = QInputDialog::getInt(this, QString::fromLocal8Bit("ѡ����ʾ�����"),
			QString::fromLocal8Bit("���������ı��"), 1, 1, 4, 1, &ok);
		if (!ok)
		{
			return;
		}
		switch (i)
		{
		case 1:
			usedwidget = showwidget->practice->fourwidget->qvtkwidge1;
			break;
		case 2:
			usedwidget = showwidget->practice->fourwidget->qvtkwidge2;
			break;
		case 3:
			usedwidget = showwidget->practice->fourwidget->qvtkwidge3;
			break;
		case 4:
			usedwidget = showwidget->practice->fourwidget->qvtkwidge4;
			break;
		}
		if (!_stricmp(fileExten, "jpg"))
		{
			filter << "*.jpg";
			dir.setNameFilters(filter);
			QFileInfoList list = dir.entryInfoList();
			VolumeInputDialog *inputvolume = new VolumeInputDialog(this, list.count() - 1);
			int rec = inputvolume->exec();
			if (rec == QDialog::Rejected)
			{
				return;
			}
			VolumeData *volumedata = inputvolume->getdata();
			VolumeRender *volume = new VolumeRender(usedwidget, volumedata, showwidget->volumePropertywidget);
			volume->GPU(dirpath2, fileExten);
		}
		else if (!_stricmp(fileExten, "dcm"))
		{
			filter << "*.dcm";
			dir.setNameFilters(filter);
			QFileInfoList list = dir.entryInfoList();
			VolumeInputDialog *inputvolume = new VolumeInputDialog(this, list.count() - 1);
			int rec = inputvolume->exec();
			if (rec == QDialog::Rejected)
			{
				return;
			}
			VolumeData *volumedata = inputvolume->getdata();
			VolumeRender *volume = new VolumeRender(usedwidget, volumedata, showwidget->volumePropertywidget);
			volume->GPU(dirpath2, fileExten);
		}
		else if (exetension == -1)
		{
			QMessageBox::information(this, QString::fromLocal8Bit("������Ϣ"),
				QString::fromLocal8Bit("��������к�׺�����ļ�"));
		}
		else
		{
			QMessageBox::information(this, QString::fromLocal8Bit("������Ϣ"),
				QString::fromLocal8Bit("�ݲ�֧�����ָ�ʽ���ļ�"));
		}
	}
} 
void VTKMainWindow::slotCPUVolumeRender()
{
	QString QSpicturePath = QFileDialog::getOpenFileName(this);
	if (QSpicturePath == NULL)
	{
		return;
	}
	QByteArray QBpicturePath = QSpicturePath.toLocal8Bit();
	const char* picturepath = QBpicturePath.data();
	//��ȡ��չ��
	int exetension = QSpicturePath.lastIndexOf(".");
	QString fileExt = QSpicturePath.right(QSpicturePath.length() - exetension - 1);
	QByteArray Extba = fileExt.toLocal8Bit();
	const char* fileExten = Extba.data();
	//��ȡĿ¼��
	int dirindex = QSpicturePath.lastIndexOf("/");
	QString dirpath = QSpicturePath.left(dirindex);
	QByteArray dirba = dirpath.toLocal8Bit();
	const char* dirpath2 = dirba.data();
	QDir dir(dirpath2);
	QStringList filter;

	int currentrow = showwidget->list->currentRow();
	if (currentrow == 0)      //�������
	{
		if (!_stricmp(fileExten, "jpg"))
		{
			filter << "*.jpg";
			dir.setNameFilters(filter);
			QFileInfoList list = dir.entryInfoList();
			VolumeInputDialog *inputvolume = new VolumeInputDialog(this, list.count() - 1);
			int rec = inputvolume->exec();
			if (rec == QDialog::Rejected)
			{
				return;
			}
			VolumeData *volumedata = inputvolume->getdata();
			VolumeRender *volume = new VolumeRender(showwidget->practice->onewidget->qvtkwidget1, volumedata);
			volume->CPU(dirpath2, fileExten);
		}
		else if (!_stricmp(fileExten, "dcm"))
		{
			filter << "*.dcm";
			dir.setNameFilters(filter);
			QFileInfoList list = dir.entryInfoList();
			VolumeInputDialog *inputvolume = new VolumeInputDialog(this, list.count() - 1);
			int rec = inputvolume->exec();
			if (rec == QDialog::Rejected)
			{
				return;
			}
			VolumeData *volumedata = inputvolume->getdata();
			VolumeRender *volume = new VolumeRender(showwidget->practice->onewidget->qvtkwidget1, volumedata);
			volume->CPU(dirpath2, fileExten);
		}
		else if (exetension == -1)
		{
			QMessageBox::information(this, QString::fromLocal8Bit("������Ϣ"),
				QString::fromLocal8Bit("��������к�׺�����ļ�"));
		}
		else
		{
			QMessageBox::information(this, QString::fromLocal8Bit("������Ϣ"),
				QString::fromLocal8Bit("�ݲ�֧�����ָ�ʽ���ļ�"));
		}
	}
	else if (currentrow == 1)  //˫���
	{
		bool ok;
		int i = QInputDialog::getInt(this, QString::fromLocal8Bit("ѡ����ʾ�����"),
			QString::fromLocal8Bit("���������ı��"), 1, 1, 2, 1, &ok);
		if (!ok)
		{
			return;
		}
		switch (i)
		{
		case 1:
			usedwidget = showwidget->practice->twowidget->qvtkwidge1;
			break;
		case 2:
			usedwidget = showwidget->practice->twowidget->qvtkwidge2;
			break;
		}
		if (!_stricmp(fileExten, "jpg"))
		{
			filter << "*.jpg";
			dir.setNameFilters(filter);
			QFileInfoList list = dir.entryInfoList();
			VolumeInputDialog *inputvolume = new VolumeInputDialog(this, list.count() - 1);
			int rec = inputvolume->exec();
			if (rec == QDialog::Rejected)
			{
				return;
			}
			VolumeData *volumedata = inputvolume->getdata();
			VolumeRender *volume = new VolumeRender(usedwidget, volumedata);
			volume->CPU(dirpath2, fileExten);
		}
		else if (!_stricmp(fileExten, "dcm"))
		{
			filter << "*.dcm";
			dir.setNameFilters(filter);
			QFileInfoList list = dir.entryInfoList();
			VolumeInputDialog *inputvolume = new VolumeInputDialog(this, list.count() - 1);
			int rec = inputvolume->exec();
			if (rec == QDialog::Rejected)
			{
				return;
			}
			VolumeData *volumedata = inputvolume->getdata();
			VolumeRender *volume = new VolumeRender(usedwidget, volumedata);
			volume->CPU(dirpath2, fileExten);
		}
		else if (exetension == -1)
		{
			QMessageBox::information(this, QString::fromLocal8Bit("������Ϣ"),
				QString::fromLocal8Bit("��������к�׺�����ļ�"));
		}
		else
		{
			QMessageBox::information(this, QString::fromLocal8Bit("������Ϣ"),
				QString::fromLocal8Bit("�ݲ�֧�����ָ�ʽ���ļ�"));
		}
	}
	else if (currentrow == 2)  //�ĸ����
	{
		bool ok;
		int i = QInputDialog::getInt(this, QString::fromLocal8Bit("ѡ����ʾ�����"),
			QString::fromLocal8Bit("���������ı��"), 1, 1, 4, 1, &ok);
		if (!ok)
		{
			return;
		}
		switch (i)
		{
		case 1:
			usedwidget = showwidget->practice->fourwidget->qvtkwidge1;
			break;
		case 2:
			usedwidget = showwidget->practice->fourwidget->qvtkwidge2;
			break;
		case 3:
			usedwidget = showwidget->practice->fourwidget->qvtkwidge3;
			break;
		case 4:
			usedwidget = showwidget->practice->fourwidget->qvtkwidge4;
			break;
		}
		if (!_stricmp(fileExten, "jpg"))
		{
			filter << "*.jpg";
			dir.setNameFilters(filter);
			QFileInfoList list = dir.entryInfoList();
			VolumeInputDialog *inputvolume = new VolumeInputDialog(this, list.count() - 1);
			int rec = inputvolume->exec();
			if (rec == QDialog::Rejected)
			{
				return;
			}
			VolumeData *volumedata = inputvolume->getdata();
			VolumeRender *volume = new VolumeRender(usedwidget, volumedata);
			volume->CPU(dirpath2, fileExten);
		}
		else if (!_stricmp(fileExten, "dcm"))
		{
			filter << "*.dcm";
			dir.setNameFilters(filter);
			QFileInfoList list = dir.entryInfoList();
			VolumeInputDialog *inputvolume = new VolumeInputDialog(this, list.count() - 1);
			int rec = inputvolume->exec();
			if (rec == QDialog::Rejected)
			{
				return;
			}
			VolumeData *volumedata = inputvolume->getdata();
			VolumeRender *volume = new VolumeRender(usedwidget, volumedata);
			volume->CPU(dirpath2, fileExten);
		}
		else if (exetension == -1)
		{
			QMessageBox::information(this, QString::fromLocal8Bit("������Ϣ"),
				QString::fromLocal8Bit("��������к�׺�����ļ�"));
		}
		else
		{
			QMessageBox::information(this, QString::fromLocal8Bit("������Ϣ"),
				QString::fromLocal8Bit("�ݲ�֧�����ָ�ʽ���ļ�"));
		}
	}
}

void VTKMainWindow::slotItemClicked()
{
	QListWidgetItem *clickedItem = showwidget->fileview->fileListWidget->currentItem();
	if (clickedItem == NULL)
	{
		return;
	}
	QFileInfo fileinfo(clickedItem->text());
	QString filename(clickedItem->text());
	if (fileinfo.suffix() != NULL)
	{
		QString dirs(showwidget->fileview->fileLineEdit->text());
		dirs += "/";
		QString absolutepath = dirs + filename;
		if (dirspath.count() == 0 && filespath.count() > 1)
		{
			slotRender();
		}
		else
		{
			Show(absolutepath);
		}
	}
}

void VTKMainWindow::slotSelectionChanged()
{
	QListWidgetItem *clickedItem = showwidget->fileview->fileListWidget->currentItem();
	if (clickedItem == NULL)
	{
		return;
	}
	QFileInfo fileinfo(clickedItem->text());
	QString filename(clickedItem->text());
	if (fileinfo.suffix() != NULL)
	{
		QString dirs(showwidget->fileview->fileLineEdit->text());
		dirs += "/";
		QString absolutepath = dirs + filename;
		
		if (dirspath.count() == 0 && filespath.count() > 1)
		{
			slotRender();
		}
		else
		{
			Show(absolutepath);
		}
	}
}

void VTKMainWindow::slotItemChanged(QListWidgetItem * item)
{
	QFileInfo fileinfo(item->text());
	if (Qt::Checked == item->checkState())
	{
		if (fileinfo.suffix() == NULL)
		{
			QString filepath(showwidget->fileview->fileLineEdit->text());
			filepath += "/";
			filepath += item->text();
			dirspath.push_back(filepath);
		}
		else
		{
			QString filepath(showwidget->fileview->fileLineEdit->text());
			filepath += "/";
			filepath += item->text();
			filespath.push_back(filepath);
		}
	}
	else if (Qt::Unchecked == item->checkState())
	{
		if (fileinfo.suffix() == NULL)
		{
			QString filepath(showwidget->fileview->fileLineEdit->text());
			filepath += "/";
			filepath += item->text();
			int index = dirspath.indexOf(filepath);
			dirspath.removeAt(index);
		}
		else
		{
			QString filepath(showwidget->fileview->fileLineEdit->text());
			filepath += "/";
			filepath += item->text();
			int index = filespath.indexOf(filepath);
			filespath.removeAt(index);
		}
	}
}

void VTKMainWindow::slotRender()
{
	if (dirspath.count() == 0 && filespath.count() == 0)
	{
		QMessageBox::information(this, QString::fromLocal8Bit("����"), QString::fromLocal8Bit("��ѡ��Ҫ���Ƶ��ļ�"));
		return;
	}
	if (dirspath.count() > 0)
	{
		QMessageBox::information(this, QString::fromLocal8Bit("����"), QString::fromLocal8Bit("��֧���ļ��Ļ���"));
		return;
	}
	int currentrow = showwidget->list->currentRow();
	if (currentrow == 0)      //�������
	{	
	   VTK3D *vtk3d = new VTK3D(showwidget->practice->onewidget->qvtkwidget1);
	   vtk3d->ShowMulti(filespath);
	}
	else if (currentrow == 1)  //˫���
	{
		bool ok;
		int i = QInputDialog::getInt(this, QString::fromLocal8Bit("ѡ����ʾ�����"),
			QString::fromLocal8Bit("���������ı��"), 1, 1, 2, 1, &ok);
		if (!ok)
		{
			return;
		}
		switch (i)
		{
		case 1:
			usedwidget = showwidget->practice->twowidget->qvtkwidge1;
			break;
		case 2:
			usedwidget = showwidget->practice->twowidget->qvtkwidge2;
			break;
		}
		VTK3D *vtk3d = new VTK3D(usedwidget);
		vtk3d->ShowMulti(filespath);
	}
	else if (currentrow == 2)  //�ĸ����
	{
		bool ok;
		int i = QInputDialog::getInt(this, QString::fromLocal8Bit("ѡ����ʾ�����"),
			QString::fromLocal8Bit("���������ı��"), 1, 1, 4, 1, &ok);
		if (!ok)
		{
			return;
		}
		switch (i)
		{
		case 1:
			usedwidget = showwidget->practice->fourwidget->qvtkwidge1;
			break;
		case 2:
			usedwidget = showwidget->practice->fourwidget->qvtkwidge2;
			break;
		case 3:
			usedwidget = showwidget->practice->fourwidget->qvtkwidge3;
			break;
		case 4:
			usedwidget = showwidget->practice->fourwidget->qvtkwidge4;
			break;
		}
		VTK3D *vtk3d = new VTK3D(usedwidget);
		vtk3d->ShowMulti(filespath);
	}
}

void VTKMainWindow::slotClear()
{
	dirspath.clear();
	filespath.clear();
}

void VTKMainWindow::slotPCD()
{
	QString QSfilePath = QFileDialog::getOpenFileName(this);
	if (QSfilePath == NULL)
	{
		return;
	}
	QByteArray QBfilePath = QSfilePath.toLocal8Bit();
	const char* filepath = QBfilePath.data();
	int currentrow = showwidget->list->currentRow();
	if (currentrow == 0)      //�������
	{	
		PCLPoint *pcl = new PCLPoint(showwidget->practice->onewidget->qvtkwidget1);
		pcl->ShowPointCloud(filepath);
	}
	else if (currentrow == 1)  //˫���
	{
		bool ok;
		int i = QInputDialog::getInt(this, QString::fromLocal8Bit("ѡ����ʾ�����"),
			QString::fromLocal8Bit("���������ı��"), 1, 1, 2, 1, &ok);
		if (!ok)
		{
			return;
		}
		switch (i)
		{
		case 1:
			usedwidget = showwidget->practice->twowidget->qvtkwidge1;
			break;
		case 2:
			usedwidget = showwidget->practice->twowidget->qvtkwidge2;
			break;
		}
		PCLPoint *pcl = new PCLPoint(usedwidget);
		pcl->ShowPointCloud(filepath);
	}
	else if (currentrow == 2)  //�ĸ����
	{
		bool ok;
		int i = QInputDialog::getInt(this, QString::fromLocal8Bit("ѡ����ʾ�����"),
			QString::fromLocal8Bit("���������ı��"), 1, 1, 4, 1, &ok);
		if (!ok)
		{
			return;
		}
		switch (i)
		{
		case 1:
			usedwidget = showwidget->practice->fourwidget->qvtkwidge1;
			break;
		case 2:
			usedwidget = showwidget->practice->fourwidget->qvtkwidge2;
			break;
		case 3:
			usedwidget = showwidget->practice->fourwidget->qvtkwidge3;
			break;
		case 4:
			usedwidget = showwidget->practice->fourwidget->qvtkwidge4;
			break;
		}	
		PCLPoint *pcl = new PCLPoint(usedwidget);
		pcl->ShowPointCloud(filepath);
	}
}

void VTKMainWindow::slotobj2pcd()
{
	obj2pcdDialog test = new obj2pcdDialog();
	if (!test.exec())
	{
		return;
	}
	QStringList filepaths = test.getfilespath();
	QString QSobjpath = filepaths.at(0);
	QByteArray QBobjpath = QSobjpath.toLocal8Bit();
	const char* objpath = QBobjpath.data();
	QString QSpcdpath = filepaths.at(1);
	QByteArray QBpcdpath = QSpcdpath.toLocal8Bit();
	const char* pcdpath = QBpcdpath.data();
	PCLPoint *pcl = new PCLPoint();
	pcl->obj2pcd(objpath, pcdpath);
}

void VTKMainWindow::slotSinglePointPick()
{
	int currentrow = showwidget->list->currentRow();
	if (currentrow == 0)      //�������
	{
		ColorPointPickDialog *test = new ColorPointPickDialog(0);
		if (!test->exec())
		{
			//printf("Cancel");
			return;
		}
		double radius = test->getRadius();
		//printf("radius:%f\n", radius);
		QRgb rgb = test->getRGB();

		double RGB[3] = { qRed(rgb), qGreen(rgb), qBlue(rgb)};
		// Set the custom stype to use for interaction.
		vtkSmartPointer<SinglePointPickingStyle> style =
			vtkSmartPointer<SinglePointPickingStyle>::New();
		
	    style->SetDefaultRenderer(showwidget->practice->onewidget->qvtkwidget1->
			GetRenderWindow()->GetRenderers()->GetFirstRenderer());
		//style->SetDefaultRenderer(showwidget->practice->onewidget->getRender());
		style->setRadius(radius);
		style->setRGB(RGB);
		style->setTextEdit(showwidget->textEdit);
		showwidget->practice->onewidget->qvtkwidget1->GetInteractor()->SetInteractorStyle(style);
		//showwidget->practice->onewidget->getInteractor()->SetInteractorStyle(style);
		//showwidget->practice->onewidget->getInteractor()->Initialize();
		//showwidget->practice->onewidget->getInteractor()->Start();
		showwidget->textEdit->setText(QString::fromLocal8Bit("���Ա���Ϊtxt�ļ�"));
		save = PointTxt;
	}
	else
	{
		QMessageBox::information(this, QString::fromLocal8Bit("������Ϣ"),
			QString::fromLocal8Bit("ʰȡ������֧���ڵ��������ʹ��"));
	}
}

void VTKMainWindow::slotAreaPick()
{
	int currentrow = showwidget->list->currentRow();
	if (currentrow == 0)      //�������
	{
		ColorPointPickDialog *test = new ColorPointPickDialog(0);
		if (!test->exec())
		{
			return;
		}
		double radius = test->getRadius();
		//printf("radius:%f\n", radius);
		QRgb rgb = test->getRGB();
		double RGB[3] = { qRed(rgb), qGreen(rgb), qBlue(rgb) };
		// Set the custom stype to use for interaction.
		vtkSmartPointer<AreaPickingStyle> style =
			vtkSmartPointer<AreaPickingStyle>::New();
		style->SetDefaultRenderer(showwidget->practice->onewidget->qvtkwidget1->
			GetRenderWindow()->GetRenderers()->GetFirstRenderer());
		style->setRadius(radius);
		style->setRGB(RGB);
		style->setTextEdit(showwidget->textEdit);
		/*vtkSmartPointer<vtkActor> actor =
			showwidget->practice->onewidget->qvtkwidget1->
			GetRenderWindow()->GetRenderers()->GetFirstRenderer()->GetActors()->GetLastActor();*/
		if (firstActor == NULL)
		{
			firstActor = showwidget->practice->onewidget->qvtkwidget1->
				GetRenderWindow()->GetRenderers()->GetFirstRenderer()->GetActors()->GetLastActor();
		}
		vtkSmartPointer<vtkGeometryFilter> gf = vtkSmartPointer<vtkGeometryFilter>::New();
		vtkSmartPointer<vtkAreaPicker> areaPicker = vtkSmartPointer<vtkAreaPicker>::New();
		showwidget->practice->onewidget->qvtkwidget1->GetInteractor()->SetPicker(areaPicker);
		gf->SetInputConnection(firstActor->GetMapper()->GetInputConnection(0, 0));
		gf->Update();
		vtkSmartPointer<vtkPolyData> polydata = vtkSmartPointer<vtkPolyData>::New();
		polydata->DeepCopy(gf->GetOutput());
		style->SetPolyData(polydata);
		showwidget->practice->onewidget->qvtkwidget1->GetInteractor()->SetInteractorStyle(style);
		showwidget->textEdit->setText(QString::fromLocal8Bit("Ӣ�����뷨�¼���R������ʰȡģʽ"));
		//save = PointTxt;
	}
	else
	{
		QMessageBox::information(this, QString::fromLocal8Bit("������Ϣ"),
			QString::fromLocal8Bit("ʰȡ������֧���ڵ��������ʹ��"));
	}
}

void VTKMainWindow::slotActorPick()
{
	int currentrow = showwidget->list->currentRow();
	if (currentrow == 0)      //�������
	{
		ColorDialog *test = new ColorDialog(0);
		if (!test->exec())
		{
			return;
		}
		QRgb rgb = test->getRGB();
		double RGB[3] = { qRed(rgb), qGreen(rgb), qBlue(rgb) };
		// Set the custom stype to use for interaction.
		vtkSmartPointer<ActorPickingStyle> style =
			vtkSmartPointer<ActorPickingStyle>::New();
		style->SetDefaultRenderer(showwidget->practice->onewidget->qvtkwidget1->
			GetRenderWindow()->GetRenderers()->GetFirstRenderer());
		style->setRGB(RGB);
		showwidget->practice->onewidget->qvtkwidget1->GetInteractor()->SetInteractorStyle(style);
		//showwidget->textEdit->setText(QString::fromLocal8Bit("Ӣ�����뷨�¼���R������ʰȡģʽ"));
	}
	else
	{
		QMessageBox::information(this, QString::fromLocal8Bit("������Ϣ"),
			QString::fromLocal8Bit("ʰȡ������֧���ڵ��������ʹ��"));
	}
}

void VTKMainWindow::slotPyImageSegment()
{
	PyImageSegmentDialog test = new PyImageSegmentDialog();
	if (!test.exec())
	{
		return;
	}
	QStringList filepaths = test.getfilespath();
	QString QSSrcpath = filepaths.at(0);
	QSSrcpath.replace("/", "\\");
	QSSrcpath.append("\\");
	QByteArray QBSrcpath = QSSrcpath.toLocal8Bit();
	const char* Srcpath = QBSrcpath.data();
	QString QSGrowingpath = filepaths.at(1);
	QSGrowingpath.replace("/", "\\");
	QSGrowingpath.append("\\");
	QByteArray QBGrowingpath = QSGrowingpath.toLocal8Bit();
	const char* Growingpath = QBGrowingpath.data();
	QString QSEdgepath = filepaths.at(2);
	QSEdgepath.replace("/", "\\");
	QSEdgepath.append("\\");
	QByteArray QBEdgepath = QSEdgepath.toLocal8Bit();
	const char* Edgepath = QBEdgepath.data();
	PythonImageSegment(Srcpath, Growingpath, Edgepath);
	QMessageBox::information(this, QString::fromLocal8Bit("������Ϣ"),
		QString::fromLocal8Bit("�ָ����"));
}

void VTKMainWindow::slotRemind()
{
	QMessageBox::information(this, QString::fromLocal8Bit("������Ϣ"),
		QString::fromLocal8Bit("����δ����"));
}

void VTKMainWindow::slotAbout()
{
	QMessageBox::information(this, QString::fromLocal8Bit("������Ϣ"),
		QString::fromLocal8Bit("��ǰ�汾Ϊ1.3.1\n"));
}

void VTKMainWindow::slotDirCTCubes()
{
	int currentrow = showwidget->list->currentRow();
	usedwidget = showwidget->practice->onewidget->qvtkwidget1;
	if (currentrow == 0)      //�������
	{
		CTDirPLYDialog test = new CTDirPLYDialog();
		if (!test.exec())
		{
			return;
		}
		QStringList filepaths = test.getfilespath();
		QString QSCTDirpath = filepaths.at(0);
		QByteArray QBCTDirpath = QSCTDirpath.toLocal8Bit();
		const char* CTDirpath = QBCTDirpath.data();
		QString QSplypath = filepaths.at(1);
		QByteArray QBplypath = QSplypath.toLocal8Bit();
		const char* plypath = QBplypath.data();
		int grayValue = test.getGrayValue();
		marching = new MarchingCubes(usedwidget);
		marching->CTDirMarchingCubes(CTDirpath, grayValue, plypath);
	}
	else
	{
		QMessageBox::information(this, QString::fromLocal8Bit("������Ϣ"),
			QString::fromLocal8Bit("ʰȡ������֧���ڵ��������ʹ��"));
	}
}

void VTKMainWindow::slotSTL2PLY()
{
	const char* stlName = "stl";
	const char* plyName = "ply";
	AToBDialog *test = new AToBDialog(0, stlName, plyName);
	if (!test->exec())
	{
		return;
	}
	QStringList filepaths = test->getfilespath();
	QString QSstlpath = filepaths.at(0);
	QByteArray QBstlpath = QSstlpath.toLocal8Bit();
	const char* stlpath = QBstlpath.data();
	QString QSplypath = filepaths.at(1);
	QByteArray QBplypath = QSplypath.toLocal8Bit();
	const char* plypath = QBplypath.data();
	STL2PLY(stlpath, plypath);
	QMessageBox::information(this, QString::fromLocal8Bit("������Ϣ"),
		QString::fromLocal8Bit("ת�����"));
}

void VTKMainWindow::slotOBJ2PLY()
{
	const char* objName = "obj";
	const char* plyName = "ply";
	AToBDialog *test = new AToBDialog(0, objName, plyName);
	if (!test->exec())
	{
		return;
	}
	QStringList filepaths = test->getfilespath();
	QString QSstlpath = filepaths.at(0);
	QByteArray QBstlpath = QSstlpath.toLocal8Bit();
	const char* stlpath = QBstlpath.data();
	QString QSplypath = filepaths.at(1);
	QByteArray QBplypath = QSplypath.toLocal8Bit();
	const char* plypath = QBplypath.data();
	OBJ2PLY(stlpath, plypath);
	QMessageBox::information(this, QString::fromLocal8Bit("������Ϣ"),
		QString::fromLocal8Bit("ת�����"));
}

void VTKMainWindow::slotPLYSimply()
{
	PlySimplyDialog test = new PlySimplyDialog(0);
	if (!test.exec())
	{
		return;
	}
	QStringList filepaths = test.getfilespath();
	QString QSstlpath = filepaths.at(0);
	QByteArray QBstlpath = QSstlpath.toLocal8Bit();
	const char* sourcePath = QBstlpath.data();
	QString QSplypath = filepaths.at(1);
	QByteArray QBplypath = QSplypath.toLocal8Bit();
	const char* destPath = QBplypath.data();
	float factor = test.getFactor();
	PlySimply *simply = new PlySimply(sourcePath, destPath, factor);
	simply->Compute();
	delete simply;
	QMessageBox::information(this, QString::fromLocal8Bit("������Ϣ"),
		QString::fromLocal8Bit("�����"));
}

void VTKMainWindow::slotPLYInformation()
{
	PLYInformationDialog test = new PLYInformationDialog(0);
	if (!test.exec())
	{
		return;
	}
	QStringList filepaths = test.getfilespath();
	QString QSplypath = filepaths.at(0);
	QByteArray QBplypath = QSplypath.toLocal8Bit();
	const char* sourcePath = QBplypath.data();

	vtkSmartPointer<vtkPLYReader> reader =
		vtkSmartPointer<vtkPLYReader>::New();
	reader->SetFileName(sourcePath);
	reader->Update();
	vtkSmartPointer<vtkPolyData> polydata = reader->GetOutput();

	std::string information = "������Ŀ  " + std::to_string(polydata->GetNumberOfPoints())
		+ " \n" + "�����Ŀ  " + std::to_string(polydata->GetNumberOfCells());
	
	QMessageBox::information(this, QString::fromLocal8Bit("������Ϣ"),
		QString::fromLocal8Bit(information.c_str()));
}

void VTKMainWindow::slotPLYFeature()
{
	const char* plyName = "ply";
	const char* fpName = "fp";
	AToBDialog *test = new AToBDialog(0, plyName, fpName);
	if (!test->exec())
	{
		return;
	}
	QStringList filepaths = test->getfilespath();
	QString QSplypath = filepaths.at(0);
	QByteArray QBplypath = QSplypath.toLocal8Bit();
	const char* plypath = QBplypath.data();
	QString QSfppath = filepaths.at(1);
	QByteArray QBfppath = QSfppath.toLocal8Bit();
	const char* fppath = QBfppath.data();
	
	MeshSaliency mesh(plypath);
	//mesh.setFaceReduceOne();  //Armadilo
	mesh.ComputeSaliency_OneLevel();
	mesh.WriteOneLevel(fppath);
	//mesh.WriteOneLevelAndLocalMax(fppath);

	QMessageBox::information(this, QString::fromLocal8Bit("������Ϣ"),
		QString::fromLocal8Bit("�����������"));
}

void VTKMainWindow::slotPLY2BP()
{
	const char* plyName = "ply";
	const char* bpName = "bp";
	AToBDialog *test = new AToBDialog(0, plyName, bpName);
	if (!test->exec())
	{
		return;
	}
	QStringList filepaths = test->getfilespath();
	QString QSplypath = filepaths.at(0);
	QByteArray QBplypath = QSplypath.toLocal8Bit();
	const char* plypath = QBplypath.data();
	QString QSBPpath = filepaths.at(1);
	QByteArray QBBPpath = QSBPpath.toLocal8Bit();
	const char* BPpath = QBBPpath.data();
	PLY2BP bp(plypath, BPpath);
	bp.Write();
	QMessageBox::information(this, QString::fromLocal8Bit("������Ϣ"),
		QString::fromLocal8Bit("ת�����"));
}

//ʹ��ctk��ܿ�������Ʋ���
void VTKMainWindow::slotctkVTKGPUVolumeRender()
{
	// show file dialog. change filename only when the new filename is not empty.
	QString filter("Meta image file (*.mhd *.mha)");
	QString filename = "";
	filename = QFileDialog::getOpenFileName(this, QString(tr("Open a volume data set")), filename, filter);
	if (filename.trimmed().isEmpty())
	{
		qDebug("�ļ���Ϊ��!");
		return;
	}
	

	// show filename on window title
	this->setWindowTitle(QString::fromUtf8("Volume Renderer - ") + filename);

	// get local 8-bit representation of the string in locale encoding (in case the filename contains non-ASCII characters) 
	QByteArray ba = filename.toLocal8Bit();
	const char *filename_str = ba.data();


	// read Meta Image (.mhd or .mha) files
	auto reader = vtkSmartPointer<vtkMetaImageReader>::New();
	reader->SetFileName(filename_str);

	//// read a series of raw files in the specified folder
	//auto reader = vtkSmartPointer<vtkVolume16Reader>::New();
	//reader->SetDataDimensions(512, 512);
	//reader->SetImageRange(1, 361);
	//reader->SetDataByteOrderToBigEndian();
	//reader->SetFilePrefix(filename_str);
	//reader->SetFilePattern("%s%d");
	//reader->SetDataSpacing(1, 1, 1);

	//// read NRRD files
	//vtkNew<vtkNrrdReader> reader;
	//if (!reader->CanReadFile(filename_str))
	//{
	//	std::cerr << "Reader reports " << filename_str << " cannot be read.";
	//	exit(EXIT_FAILURE);
	//}
	//reader->SetFileName(filename_str);
	//reader->Update();


	// scale the volume data to unsigned char (0-255) before passing it to volume mapper
	auto shiftScale = vtkSmartPointer<vtkImageShiftScale>::New();
	shiftScale->SetInputConnection(reader->GetOutputPort());
	shiftScale->SetOutputScalarTypeToUnsignedChar();

	// Create transfer mapping scalar value to opacity.
	auto opacityTransferFunction = vtkSmartPointer<vtkPiecewiseFunction>::New();
	opacityTransferFunction->AddPoint(0.0, 0.0);
	opacityTransferFunction->AddPoint(36.0, 0.125);
	opacityTransferFunction->AddPoint(72.0, 0.25);
	opacityTransferFunction->AddPoint(108.0, 0.375);
	opacityTransferFunction->AddPoint(144.0, 0.5);
	opacityTransferFunction->AddPoint(180.0, 0.625);
	opacityTransferFunction->AddPoint(216.0, 0.75);
	opacityTransferFunction->AddPoint(255.0, 0.875);

	// Create transfer mapping scalar value to color.
	auto colorTransferFunction = vtkSmartPointer<vtkColorTransferFunction>::New();
	colorTransferFunction->AddRGBPoint(0.0, 0.0, 0.0, 0.0);
	colorTransferFunction->AddRGBPoint(36.0, 1.0, 0.0, 0.0);
	colorTransferFunction->AddRGBPoint(72.0, 1.0, 1.0, 0.0);
	colorTransferFunction->AddRGBPoint(108.0, 0.0, 1.0, 0.0);
	colorTransferFunction->AddRGBPoint(144.0, 0.0, 1.0, 1.0);
	colorTransferFunction->AddRGBPoint(180.0, 0.0, 0.0, 1.0);
	colorTransferFunction->AddRGBPoint(216.0, 1.0, 0.0, 1.0);
	colorTransferFunction->AddRGBPoint(255.0, 1.0, 1.0, 1.0);

	// set up volume property
	auto volumeProperty = vtkSmartPointer<vtkVolumeProperty>::New();
	volumeProperty->SetColor(colorTransferFunction);
	volumeProperty->SetScalarOpacity(opacityTransferFunction);
	volumeProperty->ShadeOff();
	volumeProperty->SetInterpolationTypeToLinear();

	// assign volume property to the volume property widget
	showwidget->volumePropertywidget->setVolumeProperty(volumeProperty);
	if (showwidget->volumePropertywidget == NULL)
	{
		return;
	}

	auto volumeMapper = vtkSmartPointer<vtkGPUVolumeRayCastMapper>::New();
	
	volumeMapper->SetInputConnection(shiftScale->GetOutputPort());

	// The volume holds the mapper and the property and can be used to position/orient the volume.
	auto volume = vtkSmartPointer<vtkVolume>::New();
	volume->SetMapper(volumeMapper);
	volume->SetProperty(volumeProperty);

	// add the volume into the renderer
	auto renderer = vtkSmartPointer<vtkRenderer>::New();
	renderer->AddVolume(volume);
	renderer->SetBackground(1, 1, 1);

	
	usedwidget = showwidget->practice->onewidget->qvtkwidget1;
	auto window = vtkSmartPointer<vtkRenderWindow>::New();
	usedwidget->SetRenderWindow(window);
	window->AddRenderer(renderer);
	window->Render();
	usedwidget->GetInteractor()->Initialize();
	
}