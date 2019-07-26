#pragma once
#include <qmainwindow.h>
#include <qimage.h>
#include <qlabel.h>
#include <qmenu.h>
#include <qmenubar.h>
#include <MarchingCubes.h>
#include <qaction.h>
#include <qcombobox.h>
#include <qspinbox.h>
#include <qtoolbar.h>
#include <qfontcombobox.h>
#include <qtoolbutton.h>
#include <QTextCharFormat>
#include <ImageAndText.h>
#include <showwidget.h>
#include <vtkJPEGReader.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkNamedColors.h>
#include <vtkinclude.h>


class VTKMainWindow : public QMainWindow
{
	Q_OBJECT
public:
	VTKMainWindow(QWidget *parent = 0);
	void createActions();            //��������
	void createMenus();              //�����˵�
	void createToolBars();           //����������
	void Show(QString &QSfilePath);                     //��ʾ�ļ�
	QList <QString> filespath;
	QList <QString> dirspath;
protected:
	vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor;
	vtkSmartPointer<vtkJPEGReader> jpegReader;
	vtkSmartPointer<vtkActor> firstActor;
	//QVTKWidget *mainwidget;
	QVTKWidget *usedwidget;
private:
	void WritePointTxt();
	enum saveType {none,OBJ, PCD, PointTxt};
	saveType save;
	MarchingCubes *marching;
	QMenu *fileMenu;
	QMenu *pictureMenu;
	QMenu *vtkMenu;
	QMenu *featureMenu;
	//QMenu *picturetypeMenu;
	QMenu *pclMenu;
	QMenu *zoomMenu;
	QMenu *rotateMenu;
	QMenu *mirrorMenu;
	QMenu *OtherMenu;
	QImage img;
	QString fileName;
	ImageAndText *imagetext;                    //ͼ���ı�����
	ShowWidget *showwidget;
	QAction *openFileAction;
	QAction *filesaveAction;
	QAction *NewFileAction;
	QAction *Print3DModelAction;
	QAction *PrintImageAction;
	QMenu *PickMenu;                           //ʰȡ�˵�
	QAction *SinglePointPickAction; 
	QAction *MutiplePointPickAction;
	QAction *CellPickAction;
	QAction *AreaPickAction;
	QAction *PropPickAction;
	QAction *exitAction;
	QAction *copyAction;                       //�༭�˵�
	QAction *cutAction;
	QAction *pasteAction;
	QAction *aboutAction;
	QAction *zoomInAction;
	QAction *zoomOutAction;
	QAction *rotate90Action;                  //��ת�˵�
	QAction *rotate180Action;
	QAction *rotate270Action;
	QAction *mirrorVerticalAction;            //����˵�
	QAction *mirrorHorizontalAction;
	QAction *undoAction;
	QAction *redoAction;
	QToolBar *fileTool;                       //������
	QToolBar *zoomTool;
	QToolBar *rotateTool;
	QToolBar *mirrorTool;
	QToolBar *doToolBar;
	//----------------������С����
	QAction *WordsEditAction;
	QAction *FuctionlogAction;                //������־���
	//vtk�Ĺ��ܺ���
	QAction *CPUVolumeAction;                 //CPU���
	QAction *GPUVolumeAction;                 //GPU���
	QAction *ctkVTKGPUVolumeAction;           //ʹ�ÿ�������ƵĲ���
	QAction *MarchingCubesAction;             //��ͼƬ�������
	QAction *MarchingDirCTAction;             //����Ŀ¼�����
	QAction *OBJOutputAction;                 //OBJģ�����
	//pcl�Ĺ���
	QAction *PCDInputAction;                  //PCDģ�͵�����
	QAction *obj2pcdAction;                   //obj��pcdģ�͵�ת��
	//Python�Ĺ���
	QAction *PyImageSegmentAction;            //Pythonҽѧͼ��ָ�
	//��������Ĺ���
	QAction *STL2PLYAction;                   //STLת��PLY
	QAction *OBJ2PLYAction;                   //OBJת��PLY
	QAction *PLY2BPAction;                    //PLYģ�͵Ķ���
	QAction *PLYSimplyAction;                 //PLYģ�ͼ�
	QAction *PLYInformationAction;            //ͳ��PLYģ�͵Ķ�����Ŀ�����������Ŀ
	QAction *PLYFeatureAction;                //���PLYģ�͵�������

	//�˵����ܺ���
public slots:
	void ShowWordsEdit();           //��ʾ�ı��༭��
	void slotWriteLog();
	void slotShow();
	void slotItemClicked();
	void slotSelectionChanged();
	void slotItemChanged(QListWidgetItem * item);
	void slotRender();
	void slotClear();
	void slotPCD();
	void slotobj2pcd();
	void slotSTL2PLY();
	void slotOBJ2PLY();
	void slotPLY2BP();
	void slotPLYSimply();
	void slotPLYFeature();
	void slotSaveFile();
	void slotShowCubes();
	void slotDirCTCubes();
	void slotGPUVolumeRender();
	void slotCPUVolumeRender();
	void slotctkVTKGPUVolumeRender();
	void slotSinglePointPick();            //ʰȡ����
	void slotAreaPick();
	void slotActorPick();
	void slotRemind();
	void slotAbout();
	void slotPyImageSegment();
	void slotPLYInformation();
};
 