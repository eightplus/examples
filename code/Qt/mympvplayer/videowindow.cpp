#include "videowindow.h"
#include "global.h"
#include "utils.h"

#include <QApplication>
#include <QDebug>
#include <QWheelEvent>
#include <QMouseEvent>

//4:3的屏幕显示比例
VideoWindow::VideoWindow(QWidget* parent, Qt::WindowFlags f)
    : QWidget(parent, f)
    , m_videoWidth(0)
    , m_videoHeight(0)
{
    this->setMouseTracking(true);
    this->setMinimumSize(QSize(0,0));
    this->setAutoFillBackground(true);
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    this->setFocusPolicy(Qt::StrongFocus);

    Utils::setBackgroundColor(this, QColor(0,0,0));

    m_displayLayer = new QWidget(this);
    m_displayLayer->setObjectName("m_displayLayer");
    m_displayLayer->setAutoFillBackground(true);

    Utils::setBackgroundColor(m_displayLayer, QColor("121212"));
}

VideoWindow::~VideoWindow()
{

}

void VideoWindow::setResolution( int w, int h)
{
    m_videoWidth = w;
    m_videoHeight = h;

    updateVideoWindow();
}

void VideoWindow::resizeEvent( QResizeEvent *e)
{
     updateVideoWindow();
}

void VideoWindow::updateVideoWindow()
{
    int w_width = size().width();
    int w_height = size().height();

    int w = w_width;
    int h = w_height;
    int x = 0;
    int y = 0;

    double aspect = ((double) 4/3);

    int pos1_w = w_width;
    int pos1_h = w_width / aspect + 0.5;

    int pos2_h = w_height;
    int pos2_w = w_height * aspect + 0.5;

    if (pos1_h <= w_height) {
        w = pos1_w;
        h = pos1_h;
        y = (w_height - h) /2;
    }
    else {
        w = pos2_w;
        h = pos2_h;
        x = (w_width - w) /2;
    }

    m_displayLayer->move(x,y);
    m_displayLayer->resize(w, h);
}

void VideoWindow::mouseReleaseEvent(QMouseEvent * e)
{
    if (e->button() == Qt::LeftButton) {
        e->accept();
    }
    else if (e->button() == Qt::RightButton) {
        e->accept();
        emit rightClicked();
    }
    else {
        e->ignore();
    }
}

void VideoWindow::wheelEvent(QWheelEvent *e)
{
    e->accept();

    if (e->orientation() == Qt::Vertical) {
        if (e->delta() >= 0)
            emit wheelUp();
        else
            emit wheelDown();
    } else {
        qDebug("VideoWindow::wheelEvent: horizontal event received, doing nothing");
    }
}

QSize VideoWindow::sizeHint() const
{
    return QSize(m_videoWidth, m_videoHeight);
}
