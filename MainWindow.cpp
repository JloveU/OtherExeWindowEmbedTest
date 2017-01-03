#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QProcess>
#include <QDebug>
#include <QWindow>
#include <Windows.h>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->button, SIGNAL(clicked(bool)), this, SLOT(onButtonClicked()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onButtonClicked()
{
    // 参考：http://www.cnblogs.com/gaoxudong0704/p/5986730.html 和 http://blog.csdn.net/jiaoyaziyang/article/details/49802993

    QString otherExePath;  // 待嵌入exe文件路径
    QString otherExeWindowClassName;  // 待嵌入exe窗口类名（使用Visual Studio提供的工具Spy++获取该窗口的类名和标题，作为FindWindow函数的参数。）
    QString otherExeWindowTitle;  // 待嵌入exe窗口标题（使用Visual Studio提供的工具Spy++获取该窗口的类名和标题，作为FindWindow函数的参数。）

    // 测试
    const int i = 2;
    switch (i)
    {
    case 0:  // Windows记事本  // TODO 不能显示菜单栏
    {
        otherExePath = QString("C:\\Windows\\System32\\notepad.exe");
        otherExeWindowClassName = QString("Notepad");
        otherExeWindowTitle = QString("");
        break;
    }
    case 1:  // Windows画图  // TODO FindWindow总是返回0，可能是类名的问题，手动打开画图时类名为“Afx:00007FF7266C0000:0:0000000000010003:0000000000000010:0000000000000000”，程序中自动打开时类名为“Afx:d10000:0:10003:10:0”。
    {
        otherExePath = QString("C:\\Windows\\System32\\mspaint.exe");
        otherExeWindowClassName = QString("Afx:00007FF7266C0000:0:0000000000010003:0000000000000010:0000000000000000");
        otherExeWindowTitle = QString("");
        break;
    }
    case 2:  // MeshLab  // TODO 刚开始嵌入时，菜单栏、工具栏、状态栏均不能显示，当resize之后就能正常显示了。
    {
        otherExePath = QString("D:\\ProgramFiles\\MeshLab\\meshlab.exe");
        otherExeWindowClassName = QString("Qt5QWindowIcon");
        otherExeWindowTitle = QString("MeshLab_64bit v1.3.3 - [Project_1]");
        break;
    }
    case 3:  // CMake GUI
    {
        otherExePath = QString("D:\\ProgramFiles\\PortablePrograms\\CMake_3.6.2_x64_Portable\\bin\\cmake-gui.exe");
        otherExeWindowClassName = QString("Qt5QWindowIcon");
        otherExeWindowTitle = QString("CMake 3.6.2 - E:/temp/DBSCAN");
        break;
    }
    case 4:  // Doxygen GUI  // TODO 作为嵌入窗口后，菜单和按钮不能点击，但可以通过键盘操作。
    {
        otherExePath = QString("D:\\ProgramFiles\\PortablePrograms\\Doxygen_1.8.12_x64_Portable\\bin\\doxywizard.exe");
        otherExeWindowClassName = QString("QWidget");
        otherExeWindowTitle = QString("Doxygen GUI frontend");
        break;
    }
    }

    // 启动待嵌入exe，打开其窗口
    QProcess *process = new QProcess(this);
    process->start(otherExePath);
    if(!process->waitForStarted(5000))
    {
        qDebug() << "Program start failed!";
        return;
    }
    Sleep(2000);  // 延时以保证该窗口已显示在顶层

    // 获取该窗口句柄
    WId wid = (WId) FindWindow((LPCWSTR) otherExeWindowClassName.utf16(), otherExeWindowTitle.isEmpty() ? NULL : (LPCWSTR) otherExeWindowTitle.utf16());
    if(wid == 0)
    {
        qDebug() << "Find window failed!";
        return;
    }

    // 生成指向该窗口的QWindow
    QWindow *window = QWindow::fromWinId(wid);
    if(!window)
    {
        qDebug() << "Create window failed!";
        return;
    }

    // 创建该窗口的容器
    QWidget *container = QWidget::createWindowContainer(window, this);

    // 将该窗口的容器显示到界面
    ui->embedLayout->addWidget(container);

    // 设置容器的大小（设置容器窗口大小相当于设置待嵌入窗口大小）
//    ui->embedLayoutWidget->setGeometry(50, 50, 200, 200);
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    // 根据窗口大小自动缩放待嵌入窗口容器的大小（设置容器窗口大小相当于设置待嵌入窗口大小）
    const int windowWidth = event->size().width();
    const int windowHeight = event->size().height();
    ui->embedLayoutWidget->setGeometry(50, 50, windowWidth - 100, windowHeight - 100);
}
