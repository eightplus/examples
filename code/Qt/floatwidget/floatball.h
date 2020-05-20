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

#ifndef FLOATBALL_H
#define FLOATBALL_H

#include <QFrame>
#include <QPropertyAnimation>
#include <QGraphicsOpacityEffect>
#include <QPushButton>
#include <QVBoxLayout>

class FloatBall : public QFrame
{
    Q_OBJECT

public:
    explicit FloatBall(QWidget *parent = 0);
    ~FloatBall();

    void showTooltip(const QPoint &position, bool isMax = true);
    void hideNow();

public slots:
    void animationShow();
    void animationHide();

signals:
    void requestFullScreen();
    void requestNormalWindow();

protected:
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void enterEvent(QEvent *event) Q_DECL_OVERRIDE;
    void leaveEvent(QEvent *event) Q_DECL_OVERRIDE;
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
    void focusOutEvent(QFocusEvent *event) Q_DECL_OVERRIDE;

private:
    QTimer *m_hideTimer = nullptr;

    QGraphicsOpacityEffect *m_opacityEffect = nullptr;
    QPropertyAnimation *m_animation = nullptr;

    QPushButton *m_maxBtn = nullptr;
    QPushButton *m_normalBtn = nullptr;
    QVBoxLayout *m_layout = nullptr;
    QBrush m_background;
    QColor m_borderColor;
    int m_radius;
    QPoint m_point;
};

#endif // FLOATBALL_H
