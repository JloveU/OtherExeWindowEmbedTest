#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QProcess>
#include <QDebug>
#include <QWindow>
#include <QSettings>
#include <QMessageBox>
#include <QApplication>
#include <QDesktopWidget>
#include <QFile>
#include <Windows.h>


MainWindow::MainWindow(QWidget *parent, const QString &configFilePathAndName) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    mAutoResizeEmbedContainer(false)
{
    ui->setupUi(this);

    // 读取配置文件
    QFile file(configFilePathAndName);
    if (!file.exists())
    {
        QMessageBox::critical(this, tr("Error"), tr("Config file not found!"));
        exit(-1);
    }
    QSettings settings(configFilePathAndName, QSettings::IniFormat);
    QString otherExeFilePathAndName = settings.value("OtherExe/filePathAndName", "").toString();
    QString otherExeWindowClassName = settings.value("OtherExe/windowClassName", "").toString();
    QString otherExeWindowTitle     = settings.value("OtherExe/windowTitle", "").toString();
    int otherExeWindowEmbedContainerWidth  = settings.value("ThisWindow/embedContainerWidth", 640).toInt();
    int otherExeWindowEmbedContainerHeight = settings.value("ThisWindow/embedContainerHeight", 480).toInt();
    bool thisWindowResizable               = settings.value("ThisWindow/resizable", false).toBool();  // 是否可调整窗口大小
    mAutoResizeEmbedContainer              = settings.value("ThisWindow/autoResizeEmbedContainer", false).toBool();
    QString thisWindowTitle                = settings.value("ThisWindow/title", tr("EmbedContainerWindow")).toString();  // 本窗口标题
    int millisecondsBetweenStartAndEmbed = settings.value("Others/millisecondsBetweenStartAndEmbed", 3000).toInt();

    // 设置本窗口参数
    const int thisWindowWidth = otherExeWindowEmbedContainerWidth;
    const int thisWindowHeight = otherExeWindowEmbedContainerHeight;
    const int screenWidth = QApplication::desktop()->width();
    const int screenHeight = QApplication::desktop()->height();
    setGeometry((screenWidth - thisWindowWidth) / 2, (screenHeight - thisWindowHeight) / 2, thisWindowWidth, thisWindowHeight);  // 设置窗口大小以适应待嵌入exe窗口
    if (!thisWindowResizable)
    {
        setMinimumSize(thisWindowWidth, thisWindowHeight);
        setMaximumSize(thisWindowWidth, thisWindowHeight);
    }
    setWindowTitle(thisWindowTitle.isEmpty() ? (otherExeWindowTitle.isEmpty() ? (otherExeWindowClassName) : otherExeWindowTitle) : thisWindowTitle);

    bool ok = startOtherExeAndEmbedIt(otherExeFilePathAndName,
                                      otherExeWindowClassName,
                                      otherExeWindowTitle,
                                      otherExeWindowEmbedContainerWidth,
                                      otherExeWindowEmbedContainerHeight,
                                      millisecondsBetweenStartAndEmbed);
    if (!ok)
    {
        exit(-1);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::startOtherExeAndEmbedIt(const QString &otherExeFilePathAndName,
                                         const QString &otherExeWindowClassName,
                                         const QString &otherExeWindowTitle,
                                         const int otherExeWindowEmbedContainerWidth,
                                         const int otherExeWindowEmbedContainerHeight,
                                         const int millisecondsBetweenStartAndEmbed)
{
    // 参考：http://www.cnblogs.com/gaoxudong0704/p/5986730.html 和 http://blog.csdn.net/jiaoyaziyang/article/details/49802993

    // 测试
    /*const int i = 5;
    switch (i)
    {
    case 0:  // Windows记事本  // TODO 不能显示菜单栏
    {
        otherExeFilePathAndName = QString("C:\\Windows\\System32\\notepad.exe");
        otherExeWindowClassName = QString("Notepad");
        otherExeWindowTitle = QString("");
        break;
    }
    case 1:  // Windows画图  // TODO FindWindow总是返回0，可能是类名的问题，手动打开画图时类名为“Afx:00007FF7266C0000:0:0000000000010003:0000000000000010:0000000000000000”，程序中自动打开时类名为“Afx:d10000:0:10003:10:0”。
    {
        otherExeFilePathAndName = QString("C:\\Windows\\System32\\mspaint.exe");
        otherExeWindowClassName = QString("Afx:00007FF7266C0000:0:0000000000010003:0000000000000010:0000000000000000");
        otherExeWindowTitle = QString("");
        break;
    }
    case 2:  // MeshLab  // TODO 刚开始嵌入时，菜单栏、工具栏、状态栏均不能显示，当resize之后就能正常显示了。
    {
        otherExeFilePathAndName = QString("D:\\ProgramFiles\\MeshLab\\meshlab.exe");
        otherExeWindowClassName = QString("Qt5QWindowIcon");
        otherExeWindowTitle = QString("MeshLab_64bit v1.3.3 - [Project_1]");
        break;
    }
    case 3:  // CMake GUI
    {
        otherExeFilePathAndName = QString("D:\\ProgramFiles\\PortablePrograms\\CMake_3.6.2_x64_Portable\\bin\\cmake-gui.exe");
        otherExeWindowClassName = QString("Qt5QWindowIcon");
        otherExeWindowTitle = QString("CMake 3.6.2 - E:/temp/DBSCAN");
        break;
    }
    case 4:  // Doxygen GUI  // TODO 作为嵌入窗口后，菜单和按钮不能点击，但可以通过键盘操作。
    {
        otherExeFilePathAndName = QString("D:\\ProgramFiles\\PortablePrograms\\Doxygen_1.8.12_x64_Portable\\bin\\doxywizard.exe");
        otherExeWindowClassName = QString("QWidget");
        otherExeWindowTitle = QString("Doxygen GUI frontend");
        break;
    }
    case 5:  // oCam
    {
        otherExeFilePathAndName = QString("D:\\ProgramFiles\\PortablePrograms\\oCam_344.0_Portable\\oCam.exe");
        otherExeWindowClassName = QString("Hi! oCam");
        otherExeWindowTitle = QString("");
        break;
    }
    }*/

    // 启动待嵌入exe，打开其窗口
    QProcess *process = new QProcess(this);
    process->start(otherExeFilePathAndName);
    if(!process->waitForStarted(5000))
    {
        QMessageBox::critical(this, tr("Error"), tr("Program start failed!") + "\n" + otherExeFilePathAndName);
        return false;
    }
    Sleep(millisecondsBetweenStartAndEmbed);  // 延时以保证该窗口已显示在顶层

    // 获取该窗口句柄
    WId wid = (WId) FindWindow((LPCWSTR) otherExeWindowClassName.utf16(), otherExeWindowTitle.isEmpty() ? NULL : (LPCWSTR) otherExeWindowTitle.utf16());
    if(wid == 0)
    {
        QMessageBox::critical(this, tr("Error"), tr("Find window failed!"));
        return false;
    }

    // 生成指向该窗口的QWindow
    QWindow *window = QWindow::fromWinId(wid);
    if(!window)
    {
        QMessageBox::critical(this, tr("Error"), tr("Create window failed!"));
        return false;
    }

    // 创建该窗口的容器
    QWidget *container = QWidget::createWindowContainer(window, this);

    // 将该窗口的容器显示到界面
    ui->embedLayout->addWidget(container);

    // 设置容器的大小（设置容器窗口大小相当于设置待嵌入窗口大小）
    ui->embedLayoutWidget->resize(otherExeWindowEmbedContainerWidth, otherExeWindowEmbedContainerHeight);

    return true;
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    if (mAutoResizeEmbedContainer)
    {
        // 根据窗口大小自动缩放待嵌入窗口容器的大小（设置容器窗口大小相当于设置待嵌入窗口大小）
        const int windowWidth = event->size().width();
        const int windowHeight = event->size().height();
        ui->embedLayoutWidget->setGeometry(0, 0, windowWidth, windowHeight);
    }
}
