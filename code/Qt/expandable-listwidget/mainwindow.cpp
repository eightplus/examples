#include "mainwindow.h"
#include "listcontent.h"

#include <QApplication>
#include <QResizeEvent>
#include <QStackedWidget>
#include <QLayout>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_stackedWidget(new QStackedWidget(this))
{
    this->setWindowTitle(tr("Expandable List Demo"));
    this->setMinimumSize(550, 600);

    QWidget *w = new QWidget(this);
    this->setCentralWidget(w);

    QVBoxLayout *layout = new QVBoxLayout(w);
    layout->addWidget(m_stackedWidget, 0, Qt::AlignHCenter);

    setContent(new ListContent());
}

MainWindow::~MainWindow()
{

}

void MainWindow::setContent(QWidget *widget)
{
    if (m_stackedWidget->count() > 0)
        m_stackedWidget->removeWidget(content());

    widget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_stackedWidget->addWidget(widget);
}

QWidget *MainWindow::content() const
{
    return m_stackedWidget->count() > 0 ? m_stackedWidget->widget(0) : 0;
}

void MainWindow::resizeEvent(QResizeEvent *e)
{
    const QMargins &content_margins = layout()->contentsMargins();

    m_stackedWidget->setFixedWidth(e->size().width() - content_margins.left() - content_margins.right());
}
