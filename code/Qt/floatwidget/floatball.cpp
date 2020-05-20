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

#include "floatball.h"

#include <QTimer>
#include <QPainter>
#include <QDebug>
#include <QDesktopWidget>
#include <QApplication>

FloatBall::FloatBall(QWidget *parent)
    : QFrame(parent)
    , m_layout(new QVBoxLayout(this))
    , m_opacityEffect(new QGraphicsOpacityEffect(this))
    , m_animation(new QPropertyAnimation(this, "pos"))
    , m_hideTimer(new QTimer(this))
    , m_radius(5)
    , m_background(QBrush(QColor(255,255,255,230)))
    , m_borderColor(QColor(224,224,224,130))
{

    setAutoFillBackground(true);
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::ToolTip | Qt::WindowStaysOnTopHint);
    setAttribute(Qt::WA_TranslucentBackground);
    setContentsMargins(0, 0, 0, 0);
    this->setStyleSheet("QFrame{border:none;background-color:rgba(255,255,255,0.8);}");
    this->setFixedSize(100, 100);
    m_layout->setSpacing(0);
    m_layout->setMargin(0);

    m_opacityEffect->setOpacity(1.0);
    this->setGraphicsEffect(m_opacityEffect);
    m_animation->setEasingCurve(QEasingCurve::InOutCubic);
    m_animation->setDuration(500);

    m_maxBtn = new QPushButton(this);
    m_maxBtn->setFixedSize(this->width()-10, this->height()-10);
    m_maxBtn->setStyleSheet("QPushButton{border:none;background-color:rgba(255,255,255,0.8);}");
    m_maxBtn->setText(tr("Max"));
    connect(m_maxBtn, &QPushButton::clicked, this, [=] () {
        emit this->requestFullScreen();
        this->hide();
    });

    m_normalBtn = new QPushButton(this);
    m_normalBtn->setFixedSize(this->width()-10, this->height()-10);
    m_normalBtn->setStyleSheet("QPushButton{border:none;background-color:rgba(255,255,255,0.8);}");
    m_normalBtn->setText(tr("Normal"));
    m_normalBtn->setVisible(false);
    connect(m_normalBtn, &QPushButton::clicked, this, [=] () {
        emit this->requestNormalWindow();
        this->hide();
    });

    m_layout->addWidget(m_maxBtn, 0, Qt::AlignCenter);
    m_layout->addWidget(m_normalBtn, 0, Qt::AlignCenter);

    m_hideTimer->setSingleShot(true);
    m_hideTimer->setTimerType(Qt::VeryCoarseTimer);
    connect(m_hideTimer, &QTimer::timeout, this, &FloatBall::animationHide);
}

FloatBall::~FloatBall()
{
    m_animation->deleteLater();

    if (m_hideTimer) {
        disconnect(m_hideTimer, &QTimer::timeout, this, &FloatBall::animationHide);
        if(m_hideTimer->isActive()) {
            m_hideTimer->stop();
        }
        delete m_hideTimer;
        m_hideTimer = nullptr;
    }

    QLayoutItem *child;
    while ((child = m_layout->takeAt(0)) != 0) {
        if (child->widget())
            child->widget()->deleteLater();
        delete child;
    }
}

void FloatBall::animationShow()
{
    if (m_animation->state() == QPropertyAnimation::Running)
        return;

    QWidget::show();

    m_animation->setStartValue(m_point);
    m_animation->setEndValue(QPoint(m_point.x() - this->width(), m_point.y()));
    m_animation->start();
}

void FloatBall::animationHide()
{
    if (m_animation->state() == QPropertyAnimation::Running)
        return;

    m_animation->setStartValue(QPoint(m_point.x() - this->width(), m_point.y()));
    m_animation->setEndValue(m_point);
    m_animation->start();
    QTimer::singleShot(m_animation->duration(), this, [=] {
        setAttribute(Qt::WA_TransparentForMouseEvents, false);
        QWidget::hide();
    });
}

void FloatBall::showTooltip(const QPoint &position, bool isMax)
{
    if (!isVisible()) {
        m_point = position;
        if (isMax) {
            m_maxBtn->setVisible(true);
            m_normalBtn->setVisible(false);
        }
        else {
            m_normalBtn->setVisible(true);
            m_maxBtn->setVisible(false);
        }
        this->animationShow();
    }

    m_hideTimer->stop();
    m_hideTimer->setInterval(2000);
    m_hideTimer->start();
}

void FloatBall::hideNow()
{
    if (m_hideTimer) {
        if(m_hideTimer->isActive()) {
            m_hideTimer->stop();
        }
    }
    m_animation->stop();
    QWidget::hide();
}

void FloatBall::mousePressEvent(QMouseEvent *event)
{
    QFrame::mousePressEvent(event);

    this->animationHide();
}

void FloatBall::enterEvent(QEvent *event)
{
    QWidget::enterEvent(event);

    m_hideTimer->stop();
    m_hideTimer->setInterval(300);

    setFocus();
}

void FloatBall::leaveEvent(QEvent *event)
{
    QWidget::leaveEvent(event);

    m_hideTimer->start();
    this->animationHide();
}

void FloatBall::focusOutEvent(QFocusEvent *event)
{
    QWidget::focusOutEvent(event);
    m_hideTimer->start();
}

void FloatBall::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    bool outer = false;

    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::HighQualityAntialiasing);
    int radius = this->m_radius;
    double w_pen = 1.0;
    QBrush background =  this->m_background;
    QColor border_color = this->m_borderColor;
    double margin = 2.0;
    QMarginsF m_shadowMargins = QMarginsF(margin, margin, margin, margin);

    //background
    QRectF bg_rect = QRectF(rect()).marginsRemoved(m_shadowMargins);
    QPainterPath bg_path;
    bg_path.addRoundedRect(bg_rect, radius, radius);
    painter.fillPath(bg_path, background);

    //border
    QPainterPath border_path;
    QRectF border_rect = QRectF(rect());
    int border_radius = radius;
    QMarginsF border_margin(w_pen / 2, w_pen / 2, w_pen / 2, w_pen / 2);
    if (outer) {
        border_radius += w_pen / 2;
        border_rect = border_rect.marginsAdded(border_margin).marginsRemoved(m_shadowMargins);
    } else {
        border_radius -= w_pen / 2;
        border_rect = border_rect.marginsRemoved(border_margin).marginsRemoved(m_shadowMargins);
    }
    border_path.addRoundedRect(border_rect, border_radius, border_radius);
    QPen border_pen(border_color);
    border_pen.setWidthF(w_pen);
    painter.strokePath(border_path, border_pen);
}
