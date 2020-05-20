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

#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QEvent>
#include <QBoxLayout>

class ContentWidget;
class AutohideWidget;
class FloatBall;
class ToolBar;
class QHBoxLayout;
class QVBoxLayout;

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

    void initConnections();

public slots:
    void onHorizontalShow();
    void onVerticalShow();

protected:
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    //void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    bool event(QEvent *e) Q_DECL_OVERRIDE;

private:
    ContentWidget *m_contentWidget = nullptr;
    AutohideWidget *m_floatingWidget = nullptr;
    ToolBar *m_toolBar = nullptr;
    FloatBall *m_floatingBall = nullptr;
    QBoxLayout *m_boxLayout = nullptr;
    QBoxLayout *m_mainLayout = nullptr;
//    QHBoxLayout *m_hLayout;
//    QVBoxLayout *vMainLayout;
};

#endif // WIDGET_H
