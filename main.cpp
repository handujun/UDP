#include "mainwindow.h"
#include <QApplication>
#include <QString>
#include <QStyleFactory>
#include <QTextCodec>

int main(int argc, char *argv[]) {
#if QT_VERSION >= QT_VERSION_CHECK(5,6,0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    QApplication a(argc, argv);
#ifdef __linux
    QStringList styles = QStyleFactory::keys().filter("Fusion",Qt::CaseInsensitive);
    if ( !styles.isEmpty() ) QApplication::setStyle(styles.first());
#endif
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8")); // 设置编码为UTF-8
#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
    QTextCodec::setCodecForTr(QTextCodec::codecForLocale()); // Tr字符串编码
    QTextCodec::setCodecForCStrings(QTextCodec::codecForLocale()); // 字符串编码
#endif
    MainWindow w;
    w.show();
    return a.exec();
}
