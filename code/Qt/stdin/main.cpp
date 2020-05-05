#include <QCoreApplication>
#include <QFile>
#include <QDebug>
#include <stdio.h>

void start()
{
    QFile qstdin;
    qstdin.open(stdin, QIODevice::ReadOnly);
    qDebug() << "# please input your command!";
    while (true) {
        //trimmed() 去掉字符串首尾的空格; simplified()不仅去掉首尾的空格，中间连续的空格也用一个空格替换
        QString line = QString::fromLocal8Bit(qstdin.readLine()).trimmed();
        if (line.isEmpty()) {
            continue;
        }
        else if (line == "quit") {
            break;
        }
        else {
            qDebug() << line;
        }
    }

    exit(0);
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    start();

    return a.exec();
}

