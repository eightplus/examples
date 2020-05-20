/*
 * Copyright (C) 2013 ~ 2020 National University of Defense Technology(NUDT) & Tianjin Kylin Ltd.
 *
 * Authors:
 *  Kobe Lee    lixiang@kylinos.cn/kobe24_lixiang@126.com
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 3.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "widget.h"
#include "contentwidget.h"
#include "floatball.h"
#include "toolbar.h"
#include "autohidewidget.h"

#include <QApplication>
#include <QTimer>
#include <QHelpEvent>
#include <QStyle>
#include <QToolTip>

#define DEFAULT_SLIDER_WIDTH_OR_HEIGHT 36
#define DEFAULT_MAX_WIDTH   4096
#define DEFAULT_MAX_HEIGHT  4096
#define DEFAULT_HORIZONTAL_WIDTH 480
#define DEFAULT_HORIZONTAL_HEIGHT 800
#define DEFAULT_VERTICAL_WIDTH 800
#define DEFAULT_VERTICAL_HEIGHT 480

Widget::Widget(QWidget *parent)
  : QWidget(parent)
  , m_boxLayout(new QBoxLayout(QBoxLayout::TopToBottom))
  , m_mainLayout(new QBoxLayout(QBoxLayout::LeftToRight))
{
    this->setWindowFlags(windowFlags() | Qt::WindowMinimizeButtonHint);
    this->setMouseTracking(true);

    m_contentWidget = new ContentWidget(this);

    m_toolBar = new ToolBar(this);

    m_floatingWidget = new AutohideWidget(this);
    m_floatingWidget->setAutoHide(true);
    m_floatingWidget->hide();

    m_floatingBall = new FloatBall(this);
    m_floatingBall->setVisible(false);

    //程序启动时默认以竖直方位显示界面，即高度比宽度的尺寸大
    this->setFixedSize(DEFAULT_HORIZONTAL_WIDTH, DEFAULT_HORIZONTAL_HEIGHT + DEFAULT_SLIDER_WIDTH_OR_HEIGHT);
    m_contentWidget->setFixedSize(DEFAULT_HORIZONTAL_WIDTH, DEFAULT_HORIZONTAL_HEIGHT);
    m_toolBar->setFixedSize(DEFAULT_HORIZONTAL_WIDTH, DEFAULT_SLIDER_WIDTH_OR_HEIGHT);

    this->initConnections();

    m_boxLayout->setSpacing(0);
    m_boxLayout->setMargin(0);
    m_boxLayout->addStretch();
    m_boxLayout->addWidget(m_contentWidget);
    m_boxLayout->addWidget(m_toolBar);
    m_boxLayout->addStretch();

    m_mainLayout->setSpacing(0);
    m_mainLayout->setMargin(0);
    m_mainLayout->addStretch();
    m_mainLayout->addLayout(m_boxLayout);
    m_mainLayout->addStretch();

    this->setLayout(m_mainLayout);

//    m_hLayout = new QHBoxLayout;
//    m_hLayout->setSpacing(0);
//    m_hLayout->setMargin(0);

//    vMainLayout = new QVBoxLayout;
//    vMainLayout->setSpacing(0);
//    vMainLayout->setMargin(0);

//    QHBoxLayout *hMainLayout = new QHBoxLayout;
//    hMainLayout->setSpacing(10);
//    hMainLayout->setMargin(0);
//    vMainLayout->addStretch();
//    vMainLayout->addWidget(m_contentWidget, 0, Qt::AlignCenter);
//    vMainLayout->addWidget(m_toolBar, 0, Qt::AlignCenter);
//    vMainLayout->addStretch();

//    hMainLayout->addStretch();
//    hMainLayout->addLayout(vMainLayout);
//    hMainLayout->addLayout(m_hLayout);
//    hMainLayout->addStretch();
//    this->setLayout(hMainLayout);
}

Widget::~Widget()
{

}

void Widget::initConnections()
{
    connect(m_contentWidget, SIGNAL(requestHorizontal()), this, SLOT(onHorizontalShow()));
    connect(m_contentWidget, SIGNAL(requestVertical()), this, SLOT(onVerticalShow()));
    connect(m_floatingWidget, SIGNAL(requestClose()), this, SLOT(close()));
    connect(m_toolBar, &ToolBar::requestEnable, this, [=] () {
        m_floatingWidget->deactivate();
        //全屏时显示浮动工具栏
        QTimer::singleShot(100, m_floatingWidget, SLOT(activate()));
    });
    connect(m_toolBar, &ToolBar::requestDisable, this, [=] () {
        //隐藏浮动工具栏
        m_floatingWidget->deactivate();
    });
}

void Widget::onHorizontalShow()
{
    this->setFixedSize(DEFAULT_VERTICAL_WIDTH + DEFAULT_SLIDER_WIDTH_OR_HEIGHT, DEFAULT_VERTICAL_HEIGHT);
    m_contentWidget->setFixedSize(DEFAULT_VERTICAL_WIDTH, DEFAULT_VERTICAL_HEIGHT);
    m_toolBar->setFixedSize(DEFAULT_SLIDER_WIDTH_OR_HEIGHT, DEFAULT_VERTICAL_HEIGHT);
    if (!m_toolBar->isVisible()) {
        m_toolBar->setVisible(true);
    }

    m_mainLayout->setDirection(QBoxLayout::TopToBottom);
    m_boxLayout->setDirection(QBoxLayout::LeftToRight);
    m_toolBar->setFlowDirection(Qt::Vertical);

    /*
    //去掉所有弹簧
    for (int i = 0; i < vMainLayout->count(); ++i) {
        QLayoutItem *layoutItem = vMainLayout->itemAt(i);
        if (layoutItem->spacerItem()) {
            vMainLayout->removeItem(layoutItem);
            --i;
        }
    }
//        while (vMainLayout->count()) {
//            vMainLayout->removeWidget(vMainLayout->itemAt(0)->widget());
//            vMainLayout->itemAt(0)->widget()->setParent(nullptr);
//        }

    vMainLayout->removeWidget(m_contentWidget);
    vMainLayout->removeWidget(m_toolBar);
    m_contentWidget->setParent(nullptr);
    m_restartWidget->setParent(nullptr);
    m_toolBar->setParent(nullptr);

    m_hLayout->addStretch();
    m_hLayout->addWidget(m_contentWidget, 0, Qt::AlignCenter);
    m_hLayout->addWidget(m_toolBar, 0, Qt::AlignCenter);
    m_hLayout->addStretch();*/
}

void Widget::onVerticalShow()
{
    this->setFixedSize(DEFAULT_HORIZONTAL_WIDTH, DEFAULT_HORIZONTAL_HEIGHT + DEFAULT_SLIDER_WIDTH_OR_HEIGHT);
    m_contentWidget->setFixedSize(DEFAULT_HORIZONTAL_WIDTH, DEFAULT_HORIZONTAL_HEIGHT);
    m_toolBar->setFixedSize(DEFAULT_HORIZONTAL_WIDTH, DEFAULT_SLIDER_WIDTH_OR_HEIGHT);
    if (!m_toolBar->isVisible()) {
        m_toolBar->setVisible(true);
    }

    m_mainLayout->setDirection(QBoxLayout::LeftToRight);
    m_boxLayout->setDirection(QBoxLayout::TopToBottom);
    m_toolBar->setFlowDirection(Qt::Horizontal);

    /*
    //去掉所有弹簧
    for (int i = 0; i < m_hLayout->count(); ++i) {
        QLayoutItem *layoutItem = m_hLayout->itemAt(i);
        if (layoutItem->spacerItem()) {
            m_hLayout->removeItem(layoutItem);
            --i;
        }
    }
//        while (m_hLayout->count()) {
//            m_hLayout->removeWidget(m_hLayout->itemAt(0)->widget());
//            m_hLayout->itemAt(0)->widget()->setParent(nullptr);
//        }
    m_hLayout->removeWidget(m_contentWidget);
    m_hLayout->removeWidget(m_toolBar);
    m_contentWidget->setParent(nullptr);
    m_toolBar->setParent(nullptr);

    vMainLayout->addStretch();
    vMainLayout->addWidget(m_contentWidget, 0, Qt::AlignCenter);
    vMainLayout->addWidget(m_toolBar, 0, Qt::AlignCenter);
    vMainLayout->addStretch();
    */
}

void Widget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_floatingBall->hideNow();
    }
}

void Widget::mouseReleaseEvent(QMouseEvent *event)
{
    if (this->rect().contains(event->pos())) {
        m_floatingBall->hideNow();
    }
}

/*void Widget::mouseMoveEvent(QMouseEvent *event)
{
    QRect rect = QRect(this->width() - 100, 0, 100, 100);
    QPoint point = event->pos();//this->mapFromGlobal(QCursor::pos());
    if (rect.contains(point)) {
        QPoint globalPoint = this->pos();//this->mapToGlobal(this->pos());//主窗口相对于屏幕的坐标
        globalPoint.setX(globalPoint.x() + this->width());
        globalPoint.setY(globalPoint.y() + QApplication::style()->pixelMetric(QStyle::PM_TitleBarHeight));//去掉标题栏高度
        m_floatingBall->showTooltip(globalPoint, !isFullScreen());
    }
}*/

bool Widget::event(QEvent *e)
{
    bool result = QWidget::event(e);
    QEvent::Type type = e->type();
    switch (type) {
    case QEvent::ToolTip:
    {
        QHelpEvent *he = static_cast<QHelpEvent *>(e);
//        QToolTip::showText(he->globalPos(), "Hello", this);
        QRect rect = QRect(this->width() - 100, 0, 100, 100);
        QPoint point = he->pos();//this->mapFromGlobal(QCursor::pos());
        if (rect.contains(point)) {
            QPoint globalPoint = this->pos();//this->mapToGlobal(this->pos());//主窗口相对于屏幕的坐标
            globalPoint.setX(globalPoint.x() + this->width());
            globalPoint.setY(globalPoint.y() + QApplication::style()->pixelMetric(QStyle::PM_TitleBarHeight));//去掉标题栏高度
            m_floatingBall->showTooltip(globalPoint, !isFullScreen());
        }
        e->accept();
        return true;
    }
    break;
    case QEvent::ActivationChange:
        if (isActiveWindow()) {
            m_floatingBall->hideNow();
        }
        break;
    default:
        break;
    }

    return result;
}
