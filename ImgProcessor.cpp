#include <ImgProcessor.h>
#include <qfiledialog.h>
#include <qcolordialog.h>
#include <qcolor.h>
#include <qtextstream.h>
#include <QtPrintSupport\qprintdialog.h>
#include <QtPrintSupport\qprinter.h>
#include <qpainter.h>
ImgProcessor::ImgProcessor(QWidget *parent)
	: QMainWindow(parent)
{
	setWindowTitle(tr("Easy Word"));         //���ô������
	imagetext = new ImageAndText(this);
    setCentralWidget(imagetext);
	createFontBars();
	createlistBars();
	/* �����������˵����������ĺ��� */
	createActions();
	createMenus();
	createToolBars();
	if (img.load("Icons/image.png"))
	{
		//��imageLabel�����з���ͼ��
		imagetext->imageLabel->setPixmap(QPixmap::fromImage(img));
	}
	connect(imagetext->text, SIGNAL(ShowCurrentFormatChanged(QTextCharFormat&)), this,
		SLOT(ShowCurrentFormatChanged(QTextCharFormat &)));
	connect(imagetext->text->document(), SIGNAL(undoAvailable(bool)), redoAction, SLOT(setEnabled(bool)));
	connect(imagetext->text->document(), SIGNAL(redoAvailable(bool)), redoAction, SLOT(setEnabled(bool)));
	connect(imagetext->text, SIGNAL(ShowCursorPositionChanged()), this, SLOT(ShowCursorPositionChanged()));
}

void ImgProcessor::createlistBars()
{
	//����
	listLabel = new QLabel(QString::fromLocal8Bit("����"));
	listComboBox = new QComboBox();
	listComboBox->addItem("Standard");
	listComboBox->addItem("QTextListFormat::ListDisc");
	listComboBox->addItem("QTextListFormat::ListCircle");
	listComboBox->addItem("QTextListFormat::ListSquare");
	listComboBox->addItem("QTextListFormat::ListDecimal");
	listComboBox->addItem("QTextListFormat::ListLowerAlpha");
	listComboBox->addItem("QTextListFormat::ListUpperAlpha");
	listComboBox->addItem("QTextListFormat::ListLowerRoman");
	listComboBox->addItem("QTextListFormat::ListUpperRoman");
	connect(listComboBox, SIGNAL(activated(int)), this, SLOT(ShowList(int)));
}

void ImgProcessor::createFontBars()
{
	//��������
	fontLabel1 = new QLabel(QString::fromLocal8Bit("����:"));
	fontComboBox = new QFontComboBox();
	fontComboBox->setFontFilters(QFontComboBox::ScalableFonts);
	connect(fontComboBox, SIGNAL(activated(QString)), this, SLOT(ShowFontComboBox(QString)));
	fontLabel2 = new QLabel(QString::fromLocal8Bit("�ֺ�:"));
	sizeComboBox = new QComboBox();
	QFontDatabase db;
	foreach(int size, db.standardSizes())
		sizeComboBox->addItem(QString::number(size));
	connect(sizeComboBox, SIGNAL(activated(QString)), this, SLOT(ShowSizeSpinBox(QString)));
	boldBtn = new QToolButton();
	boldBtn->setIcon(QIcon("Icons/bold.png"));
	boldBtn->setCheckable(true);
	connect(boldBtn, SIGNAL(clicked()), this, SLOT(ShowBoldBtn()));
	italicBtn = new QToolButton();
	italicBtn->setIcon(QIcon("Icons/italic.png"));
	italicBtn->setCheckable(true);
	connect(italicBtn, SIGNAL(clicked()), this, SLOT(ShowItalicBtn()));
	underlineBtn = new QToolButton();
	underlineBtn->setIcon(QIcon("Icons/underline.png"));
	underlineBtn->setCheckable(true);
	connect(underlineBtn, SIGNAL(clicked()), this, SLOT(ShowUnderlineBtn()));
	colorBtn = new QToolButton();
	colorBtn->setIcon(QIcon("Icons/color.png"));
	colorBtn->setCheckable(true);
	connect(colorBtn, SIGNAL(clicked()), this, SLOT(ShowColorBtn()));
}


void ImgProcessor::createActions()
{
	//"��"����
	openFileAction = new QAction(QIcon("Icons/open.png"), QString::fromLocal8Bit("��"), this);
	openFileAction->setShortcut(tr("Ctrl+O"));
	openFileAction->setStatusTip(QString::fromLocal8Bit("��һ���ļ�"));
	connect(openFileAction, SIGNAL(triggered()), this, SLOT(ShowOpenFile()));
	//"�½�"����
	NewFileAction = new QAction(QIcon("Icons/new.png"), QString::fromLocal8Bit("�½�"), this);
	NewFileAction->setShortcut(tr("Ctrl+N"));
	NewFileAction->setStatusTip(QString::fromLocal8Bit("�½�һ���ļ�"));
	//"�˳�"����
	exitAction = new QAction(QString::fromLocal8Bit("�˳�"), this);
	exitAction->setShortcut(tr("Ctrl+Q"));
	exitAction->setStatusTip(QString::fromLocal8Bit("�˳�����"));
	connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));
	//"����"����
	copyAction = new QAction(QIcon("Icons/copy.png"), QString::fromLocal8Bit("����"), this);
	copyAction->setShortcut(tr("Ctrl+C"));
	copyAction->setStatusTip(QString::fromLocal8Bit("�����ļ�"));
	connect(copyAction, SIGNAL(triggered()), imagetext->text, SLOT(copy()));
	//"����"����
	cutAction = new QAction(QIcon("Icons/cut.png"), QString::fromLocal8Bit("����"), this);
	cutAction->setShortcut(tr("Ctrl+X"));
	cutAction->setStatusTip(QString::fromLocal8Bit("�����ļ�"));
	connect(cutAction, SIGNAL(triggered()), imagetext->text, SLOT(cut()));
	//"ճ��"����
	pasteAction = new QAction(QIcon("Icons/paste.png"), QString::fromLocal8Bit("ճ��"), this);
	pasteAction->setShortcut(tr("Ctrl+V"));
	pasteAction->setStatusTip(QString::fromLocal8Bit("ճ���ļ�"));
	connect(pasteAction, SIGNAL(triggered()), imagetext->text, SLOT(paste()));
	//"����"����
	aboutAction = new QAction(QString::fromLocal8Bit("����"), this);
	connect(aboutAction, SIGNAL(triggered()), this, SLOT(QApplication::aboutQt()));
	//"��ӡ�ı�"����
	PrintTextAction = new QAction(QIcon("Icons/printText.png"), QString::fromLocal8Bit("��ӡ�ı�"), this);
	PrintTextAction->setStatusTip(QString::fromLocal8Bit("��ӡһ���ı�"));
	connect(PrintTextAction, SIGNAL(triggered()), this, SLOT(ShowPrintText()));
	//"��ӡͼ��"����
	PrintImageAction = new QAction(QIcon("Icons/printImage.png"), QString::fromLocal8Bit("��ӡͼ��"), this);
	PrintImageAction->setStatusTip(QString::fromLocal8Bit("��ӡһ��ͼ��"));
	connect(PrintImageAction, SIGNAL(triggered()), this, SLOT(ShowPrintImage()));
	//"�Ŵ�"����
	zoomInAction = new QAction(QIcon("Icons/zoomin.png"), QString::fromLocal8Bit("�Ŵ�"), this);
	zoomInAction->setStatusTip(QString::fromLocal8Bit("�Ŵ�һ��ͼƬ"));
	connect(zoomInAction, SIGNAL(triggered()), this, SLOT(ShowZoomIn()));
	//"��С"����
	zoomOutAction = new QAction(QIcon("Icons/zoomout.png"), QString::fromLocal8Bit("��С"), this);
	zoomOutAction->setStatusTip(QString::fromLocal8Bit("��Сһ��ͼƬ"));
	connect(zoomOutAction, SIGNAL(triggered()), this, SLOT(ShowZoomOut()));
	//ʵ��ͼ����ת�Ķ���(Action)
	//��ת90��
	rotate90Action = new QAction(QIcon("Icons/rotate90.png"), QString::fromLocal8Bit("��ת90��"), this);
	rotate90Action->setStatusTip(QString::fromLocal8Bit("��һ��ͼ��ת90��"));
	connect(rotate90Action, SIGNAL(triggered()), this, SLOT(ShowRotate90()));
	//��ת180��
	rotate180Action = new QAction(QIcon("Icons/rotate180.png"), QString::fromLocal8Bit("��ת180��"), this);
	rotate180Action->setStatusTip(QString::fromLocal8Bit("��һ��ͼ��ת180��"));
	connect(rotate180Action, SIGNAL(triggered()), this, SLOT(ShowRotate180()));
	//��ת270��
	rotate270Action = new QAction(QIcon("Icons/rotate270.png"), QString::fromLocal8Bit("��ת270��"), this);
	rotate270Action->setStatusTip(QString::fromLocal8Bit("��һ��ͼ��ת270��"));
	connect(rotate270Action, SIGNAL(triggered()), this, SLOT(ShowRotate270()));
	//ʵ��ͼ����Ķ���(Action)
	//������
	mirrorVerticalAction = new QAction(QIcon("Icons/mirrorVertical.png"), QString::fromLocal8Bit("������"), this);
	mirrorVerticalAction->setStatusTip(QString::fromLocal8Bit("��һ��ͼ��������"));
	connect(mirrorVerticalAction, SIGNAL(triggered()), this, SLOT(ShowMirrorVertical()));
	//������
	mirrorHorizontalAction = new QAction(QIcon("Icons/mirrorHorizontal.png"), QString::fromLocal8Bit("������"), this);
	mirrorHorizontalAction->setStatusTip(QString::fromLocal8Bit("��һ��ͼ��������"));
	connect(mirrorHorizontalAction, SIGNAL(triggered()), this, SLOT(ShowMirrorHorizontal()));
	//ʵ�ֳ����ͻָ��Ķ���(Action)
	//�����ͻָ�
	undoAction = new QAction(QIcon("Icons/undo.png"), "����", this);
	undoAction->setStatusTip(QString::fromLocal8Bit("����"));
	connect(undoAction, SIGNAL(triggered()), imagetext->text, SLOT(undo()));
	redoAction = new QAction(QIcon("Icons/redo.png"), "����", this);
	redoAction->setStatusTip(QString::fromLocal8Bit("����"));
	connect(redoAction, SIGNAL(triggered()), imagetext->text, SLOT(redo()));
	//��������롢�Ҷ��롢���к����˶���
	actGrp = new QActionGroup(this);
	leftAction = new QAction(QIcon("Icons/left.png"), QString::fromLocal8Bit("�����"), actGrp);
	leftAction->setCheckable(true);
	rightAction = new QAction(QIcon("Icons/right.png"), QString::fromLocal8Bit("�Ҷ���"), actGrp);
	rightAction->setCheckable(true);
	centerAction = new QAction(QIcon("Icons/center.png"), QString::fromLocal8Bit("����"), actGrp);
	centerAction->setCheckable(true);
	justifyAction = new QAction(QIcon("Icons/justify.png"), QString::fromLocal8Bit("���˶���"), actGrp);
	justifyAction->setCheckable(true);
	connect(actGrp, SIGNAL(triggered(QAction *)), this, SLOT(ShowAlignment(QAction *)));
}

void ImgProcessor::createMenus()
{
	//�ļ��˵�
	fileMenu = menuBar()->addMenu(QString::fromLocal8Bit("�ļ�"));
	fileMenu->addAction(openFileAction);
	fileMenu->addAction(NewFileAction);
	fileMenu->addAction(PrintTextAction);
	fileMenu->addAction(PrintImageAction);
	fileMenu->addSeparator();
	fileMenu->addAction(exitAction);
	//���Ų˵�
	zoomMenu = menuBar()->addMenu(QString::fromLocal8Bit("�༭"));
	zoomMenu->addAction(copyAction);
	zoomMenu->addAction(cutAction);
	zoomMenu->addAction(pasteAction);
	zoomMenu->addAction(aboutAction);
	zoomMenu->addSeparator();
	zoomMenu->addAction(zoomInAction);
	zoomMenu->addAction(zoomOutAction);
	//ͼ�����Ӳ˵�--��ת�˵�
	rotateMenu = menuBar()->addMenu(QString::fromLocal8Bit("��ת"));
	rotateMenu->addAction(rotate90Action);
	rotateMenu->addAction(rotate180Action);
	rotateMenu->addAction(rotate270Action);
	//ͼ�����Ӳ˵�--����˵�
	mirrorMenu = menuBar()->addMenu(QString::fromLocal8Bit("����"));
	mirrorMenu->addAction(mirrorVerticalAction);
	mirrorMenu->addAction(mirrorHorizontalAction);
}

void ImgProcessor::createToolBars()
{
	//�ļ�������
	fileTool = addToolBar("File");
	fileTool->addAction(openFileAction);
	fileTool->addAction(NewFileAction);
	fileTool->addAction(PrintTextAction);
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
	//����༭������
	fontToolBar = addToolBar("Font");
	fontToolBar->addWidget(fontLabel1);
	fontToolBar->addWidget(fontComboBox);
	fontToolBar->addWidget(fontLabel2);
	fontToolBar->addWidget(sizeComboBox);
	fontToolBar->addSeparator();
	fontToolBar->addWidget(boldBtn);
	fontToolBar->addWidget(italicBtn);
	fontToolBar->addWidget(underlineBtn);
	fontToolBar->addSeparator();
	fontToolBar->addWidget(colorBtn);
	//���򹤾���
	listToolBar = addToolBar("list");
	listToolBar->addWidget(listLabel);
	listToolBar->addWidget(listComboBox);
	listToolBar->addSeparator();
	listToolBar->addActions(actGrp->actions());
}

void ImgProcessor::ShowOpenFile()
{
	fileName = QFileDialog::getOpenFileName(this);
	if (!fileName.isEmpty())
	{
		if (imagetext->text->document()->isEmpty())
		{
			loadFile(fileName);
		}
		else
		{
			ImgProcessor *newImgProcessor = new ImgProcessor;
			newImgProcessor->show();
			newImgProcessor->loadFile(fileName);
		}
	}
}

void ImgProcessor::loadFile(QString filename)
{
	printf("file name:%s \n", filename.data());
	QFile file(filename);
	if (file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		QTextStream textStream(&file);
		while (!textStream.atEnd())
		{
			imagetext->text->append(textStream.readLine());
			printf("read line \n");
		}
		printf("end \n");
	}
}

void ImgProcessor::ShowPrintText()
{
	QPrinter printer(QPrinter::HighResolution);
	QPrintDialog printDialog(&printer, this);
	if (printDialog.exec())
	{
		//���QTextEdit������ĵ�
		QTextDocument *doc = imagetext->text->document();
		doc->print(&printer);
	}
}

void ImgProcessor::ShowPrintImage()
{
	QPrinter printer;                //�½�һ��QPrinter����
	QPrintDialog printDialog(&printer, this);
	if (printDialog.exec())
	{
		QPainter painter(&printer);
		QRect rect = painter.viewport();
		QSize size = img.size();
		/* ����ͼ�εı�����С����������ͼ�������� */
		size.scale(rect.size(), Qt::KeepAspectRatio);
		painter.setViewport(rect.x(), rect.y(), size.width(), size.height());
		painter.setWindow(img.rect());
		painter.drawImage(0, 0, img);
	}
}

void ImgProcessor::ShowZoomIn()
{
	if (img.isNull())
		return;
	QMatrix matrix;
	matrix.scale(2, 2);
	img = img.transformed(matrix);
	//����������ʾͼ��
	imagetext->imageLabel->setPixmap(QPixmap::fromImage(img));
}

void ImgProcessor::ShowZoomOut()
{
	if (img.isNull())
		return;
	QMatrix matrix;
	matrix.scale(0.5, 0.5);
	img = img.transformed(matrix);
	imagetext->imageLabel->setPixmap(QPixmap::fromImage(img));
}

void ImgProcessor::ShowRotate90()
{
	if (img.isNull())
		return;
	QMatrix matrix;
	matrix.rotate(90);
	img = img.transformed(matrix);
	imagetext->imageLabel->setPixmap(QPixmap::fromImage(img));
}

void ImgProcessor::ShowRotate180()
{
	if (img.isNull())
		return;
	QMatrix matrix;
	matrix.rotate(180);
	img = img.transformed(matrix);
	imagetext->imageLabel->setPixmap(QPixmap::fromImage(img));
}

void ImgProcessor::ShowRotate270()
{
	if (img.isNull())
		return;
	QMatrix matrix;
	matrix.rotate(270);
	img = img.transformed(matrix);
	imagetext->imageLabel->setPixmap(QPixmap::fromImage(img));
}

void ImgProcessor::ShowMirrorVertical()
{
	if (img.isNull())
		return;
	img = img.mirrored(false, true);
	imagetext->imageLabel->setPixmap(QPixmap::fromImage(img));
}

void ImgProcessor::ShowMirrorHorizontal()
{
	if (img.isNull())
		return;
	img = img.mirrored(true, false);
	imagetext->imageLabel->setPixmap(QPixmap::fromImage(img));
}

//��������
void ImgProcessor::ShowFontComboBox(QString comboStr)
{
	QTextCharFormat fmt;            //����һ��QTextCharFormat����
	fmt.setFontFamily(comboStr);    //ѡ��������������ø�QTextCharFormat����
	mergeFormat(fmt);               //���µĸ�ʽӦ�õ����ѡ���ڵ��ַ�
}

void ImgProcessor::mergeFormat(QTextCharFormat format)
{
	QTextCursor cursor = imagetext->text->textCursor();        //��ñ༭���еĹ��
	if (!cursor.hasSelection())
		cursor.select(QTextCursor::WordUnderCursor);
	cursor.mergeCharFormat(format);
	imagetext->text->mergeCurrentCharFormat(format);
}

//�����ֺ�
void ImgProcessor::ShowSizeSpinBox(QString spinvalue)
{
	QTextCharFormat fmt;
	fmt.setFontPointSize(spinvalue.toFloat());
	imagetext->text->mergeCurrentCharFormat(fmt);
}

//����������ʾ�Ӵ�
void ImgProcessor::ShowBoldBtn()
{
	QTextCharFormat fmt;
	fmt.setFontWeight(boldBtn->isCheckable() ? QFont::Bold : QFont::Normal);
	imagetext->text->mergeCurrentCharFormat(fmt);
}

//����������ʾб��
void ImgProcessor::ShowItalicBtn()
{
	QTextCharFormat fmt;
	fmt.setFontItalic(italicBtn->isCheckable());
	imagetext->text->mergeCurrentCharFormat(fmt);
}

//�������ּ��»���
void ImgProcessor::ShowUnderlineBtn()
{
	QTextCharFormat fmt;
	fmt.setFontUnderline(underlineBtn->isCheckable());
	imagetext->text->mergeCurrentCharFormat(fmt);
}

void ImgProcessor::ShowColorBtn()
{
	QColor color = QColorDialog::getColor(Qt::red, this);
	if (color.isValid())
	{
		QTextCharFormat fmt;
		fmt.setForeground(color);
		imagetext->text->mergeCurrentCharFormat(fmt);
	}
}

void ImgProcessor::ShowCurrentFormatChanged(const QTextCharFormat &fmt)
{
	fontComboBox->setCurrentIndex(fontComboBox->findText(fmt.fontFamily()));
	sizeComboBox->setCurrentIndex(sizeComboBox->findText(QString::number(fmt.fontPointSize())));
	boldBtn->setChecked(fmt.font().bold());
	italicBtn->setChecked(fmt.fontItalic());
	underlineBtn->setChecked(fmt.fontUnderline());
}

void ImgProcessor::ShowAlignment(QAction *act)
{
	if (act == leftAction)
		imagetext->text->setAlignment(Qt::AlignLeft);
	if (act == rightAction)
		imagetext->text->setAlignment(Qt::AlignRight);
	if (act == centerAction)
		imagetext->text->setAlignment(Qt::AlignCenter);
	if (act == justifyAction)
		imagetext->text->setAlignment(Qt::AlignJustify);
}

void ImgProcessor::ShowCursorPositionChanged()
{
	if (imagetext->text->alignment() == Qt::AlignLeft)
		leftAction->setChecked(true);
	if (imagetext->text->alignment() == Qt::AlignRight)
		rightAction->setChecked(true);
	if (imagetext->text->alignment() == Qt::AlignCenter)
		centerAction->setChecked(true);
	if (imagetext->text->alignment() == Qt::AlignJustify)
		justifyAction->setChecked(true);
}

void ImgProcessor::ShowList(int index)
{
	//��ñ༭���QTextCursor����ָ��
	QTextCursor cursor = imagetext->text->textCursor();
	if (index != 0)
	{
		QTextListFormat::Style style = QTextListFormat::ListDisc;         //����style����ֵ
		switch (index)
		{
		default:
		case 1:
			style = QTextListFormat::ListDisc;
			break;
		case 2:
			style = QTextListFormat::ListCircle;
			break;
		case 3:
			style = QTextListFormat::ListSquare;
			break;
		case 4:
			style = QTextListFormat::ListDecimal;
			break;
		case 5:
			style = QTextListFormat::ListLowerAlpha;
			break;
		case 6:
			style = QTextListFormat::ListUpperAlpha;
			break;
		case 7:
			style = QTextListFormat::ListLowerRoman;
			break;
		case 8:
			style = QTextListFormat::ListUpperRoman;
			break;
		}
		/* ��������ֵ */
		cursor.beginEditBlock();
		QTextBlockFormat blockFmt = cursor.blockFormat();
		QTextListFormat listFmt;
		if (cursor.currentList())
		{
			listFmt = cursor.currentList()->format();
		}
		else
		{
			listFmt.setIndent(blockFmt.indent() + 1);
			blockFmt.setIndent(0);
			cursor.setBlockFormat(blockFmt);
		}
		listFmt.setStyle(style);
		cursor.createList(listFmt);
		cursor.endEditBlock();
	}
	else
	{
		QTextBlockFormat bfmt;
		bfmt.setObjectIndex(-1);
		cursor.mergeBlockFormat(bfmt);
	}
}