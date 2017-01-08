#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QResizeEvent>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    //! \param[in] configFilePathAndName 配置文件路径及文件名
    explicit MainWindow(QWidget *parent = 0, const QString &configFilePathAndName = QString("Config.ini"));

    ~MainWindow();

    //! \brief 打开待嵌入exe，并将其窗口嵌入到本窗口中
    //! \details 可使用Visual Studio提供的工具Spy++获取该窗口的类名和标题。
    //! \param[in] otherExeFilePathAndName 待嵌入exe文件路径
    //! \param[in] otherExeWindowClassName 待嵌入exe窗口类名
    //! \param[in] otherExeWindowTitle 待嵌入exe窗口标题
    //! \param[in] otherExeWindowEmbedContainerWidth 内嵌容器的宽度
    //! \param[in] otherExeWindowEmbedContainerHeight 内嵌容器的高度
    //! \param[in] millisecondsBetweenStartAndEmbed 在待嵌入exe打开之后，嵌入之前的延迟时间（延时以保证该窗口已显示在顶层）
    //! \return 是否执行成功
    bool startOtherExeAndEmbedIt(const QString &otherExeFilePathAndName,
                                 const QString &otherExeWindowClassName,
                                 const QString &otherExeWindowTitle,
                                 const int otherExeWindowEmbedContainerWidth,
                                 const int otherExeWindowEmbedContainerHeight,
                                 const int millisecondsBetweenStartAndEmbed=3000);

protected:
    virtual void resizeEvent(QResizeEvent *event);

private:
    Ui::MainWindow *ui;

    bool mAutoResizeEmbedContainer;  // 是否随窗口大小自动改变内嵌容器的大小

};

#endif // MAINWINDOW_H
