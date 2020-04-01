#include "mainwindow.h"
#include "contentwidget.h"



extern "C" {

#include <glib.h>
#include <libmatemixer/matemixer.h>

}

#include <QApplication>
#include <QScreen>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_contentWidget(new ContentWidget(this))
{
    this->setFixedSize(500, 480);
    this->move(qApp->primaryScreen()->geometry().center() - this->rect().center());

    this->setCentralWidget(m_contentWidget);

    m_contentWidget->initPlugins();


    if (mate_mixer_init() == false)
        qDebug() << "matemixer init failed";
    else
        qDebug() << "matemixer init success";
}
