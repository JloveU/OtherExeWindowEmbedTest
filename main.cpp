#include "MainWindow.h"
#include <QApplication>
#include <QTranslator>


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // 设置翻译文件
    QTranslator translator;
    if(translator.load(":/languages/zh_CN.qm"))
    {
         app.installTranslator(&translator);
    }

    if (argc < 2)
    {
        MainWindow window;
        window.show();

        return app.exec();
    }
    else
    {
        MainWindow window(NULL, QString(argv[1]));
        window.show();

        return app.exec();
    }
}
