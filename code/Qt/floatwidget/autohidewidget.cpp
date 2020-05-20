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

#include "autohidewidget.h"

#include <QApplication>
#include <QTimer>
#include <QEvent>
#include <QVBoxLayout>
#include <QMouseEvent>
#include <QDebug>
#include <QPropertyAnimation>
#include <QLabel>
#include <QStyleOption>

#define DEFAULT_WIDTH 70

AutohideWidget::AutohideWidget(QWidget * parent, Qt::WindowFlags f)
    : QWidget(parent, Qt::SubWindow)
    , turned_on(false)
    , auto_hide(false)
    , internal_widget(new QWidget)
    , timer(0)
    , m_animation(0)
    , m_gatherAnimation(0)
{
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setAutoFillBackground(true);
    this->setAttribute(Qt::WA_StyledBackground, true);
    this->setAttribute(Qt::WA_NoMousePropagation);
    this->setStyleSheet("background-color: rgb(0,0,0)");//rgba(0,0,0,20)
    this->setFixedWidth(DEFAULT_WIDTH);

    parent->installEventFilter(this);
    installFilter(parent);
//    qApp->installEventFilter(this);

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(checkUnderMouse()));
    timer->setInterval(3000);

    QVBoxLayout *m_vlayout = new QVBoxLayout(internal_widget);
    m_vlayout->setContentsMargins(0, 0, 0, 0);
    m_vlayout->setSpacing(50);

    m_closeBtn = new QPushButton();
    m_closeBtn->setText("X");
    m_closeBtn->setToolTip("关闭");
    m_closeBtn->setFixedSize(70,100);
    m_closeBtn->setFocusPolicy(Qt::NoFocus);

    m_vlayout->addStretch();
    m_vlayout->addWidget(m_closeBtn);
    m_vlayout->addStretch();

    QVBoxLayout *layout = new QVBoxLayout;
    layout->setSpacing(0);
    layout->setMargin(0);
    layout->addWidget(internal_widget);
    this->setLayout(layout);

    connect(m_closeBtn, SIGNAL(clicked(bool)), this, SIGNAL(requestClose()));
}

AutohideWidget::~AutohideWidget()
{
    if (m_animation)
        delete m_animation;

    if (m_gatherAnimation)
        delete m_gatherAnimation;

    if (timer) {
        disconnect(timer, SIGNAL(timeout()), this, SLOT(checkUnderMouse()));
        if(timer->isActive()) {
            timer->stop();
        }
        delete timer;
        timer = nullptr;
    }
}

void AutohideWidget::installFilter(QObject *o)
{
    QObjectList children = o->children();
    for (int n=0; n < children.count(); n++) {
        if (children[n]->isWidgetType()) {
            QWidget *w = static_cast<QWidget *>(children[n]);
            w->setMouseTracking(true);
            w->installEventFilter(this);
            installFilter(children[n]);
        }
    }
}

void AutohideWidget::activate()
{
    turned_on = true;
    timer->start();
}

void AutohideWidget::deactivate()
{
    turned_on = false;
    timer->stop();
    this->showGatherAnimated();
}

void AutohideWidget::showWidget()
{
    resizeAndMove();
    QWidget::show();
    showAnimated();
    if (timer->isActive()) timer->start();
}

void AutohideWidget::setAutoHide(bool b)
{
    auto_hide = b;
}

void AutohideWidget::checkUnderMouse()
{
    if (auto_hide) {
        if ((isVisible()) && (!underMouse())) {
            this->showGatherAnimated();
        }
    }
}

void AutohideWidget::resizeAndMove()
{
    QWidget *widget = parentWidget();
    int w = width();
    int h = widget->height();
    resize(w, h);
}

bool AutohideWidget::eventFilter(QObject * obj, QEvent * event)
{
    if (turned_on) {
        if (event->type() == QEvent::MouseMove) {
            if (!isVisible()) {
                showWidget();
            }
        }
    }

    return QWidget::eventFilter(obj, event);
}

void AutohideWidget::showAnimated()
{
    if (!m_animation) {
        m_animation = new QPropertyAnimation(this, "pos");
    }

    if (m_animation->state() == QPropertyAnimation::Running)
        return;

    QPoint initial_position = QPoint(parentWidget()->size().width(), 0);
    QPoint final_position = QPoint(parentWidget()->size().width() - this->width(), 0);
    move(initial_position);
    QWidget::show();

    m_animation->setDuration(300);
    m_animation->setEndValue(final_position);
    m_animation->setStartValue(initial_position);
    m_animation->start();
}

void AutohideWidget::showGatherAnimated()
{
    if (!m_gatherAnimation) {
        m_gatherAnimation = new QPropertyAnimation(this, "pos");
        connect(m_gatherAnimation, SIGNAL(finished()), this, SLOT(gatherAniFinished()));
    }

    if (m_gatherAnimation->state() == QPropertyAnimation::Running)
        return;

    QPoint initial_position = QPoint(parentWidget()->size().width() - this->width(), 0);
    QPoint final_position = QPoint(parentWidget()->size().width(), 0);
    move(initial_position);

    m_gatherAnimation->setDuration(300);
    m_gatherAnimation->setStartValue(initial_position);
    m_gatherAnimation->setEndValue(final_position);
    m_gatherAnimation->start();

    QTimer::singleShot(m_gatherAnimation->duration(), this, [=] {
        setAttribute(Qt::WA_TransparentForMouseEvents, false);
        QWidget::hide();
    });
}

void AutohideWidget::gatherAniFinished()
{
    QWidget::hide();
}
