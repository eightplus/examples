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

#ifndef AUTOHIDEWIDGET_H
#define AUTOHIDEWIDGET_H

#include <QWidget>
#include <QPushButton>

class QTimer;
class QPropertyAnimation;

class AutohideWidget : public QWidget
{
    Q_OBJECT

public:
    enum Activation { Anywhere = 1, Bottom = 2 };

    AutohideWidget(QWidget * parent = 0, Qt::WindowFlags f = Qt::Window);
    ~AutohideWidget();

public slots:
    void showWidget();
    void activate();
    void deactivate();
    void setAutoHide(bool b);

protected:
    bool eventFilter(QObject * obj, QEvent * event) Q_DECL_OVERRIDE;

private slots:
    void checkUnderMouse();
    void showAnimated();
    void gatherAniFinished();
    void showGatherAnimated();

signals:
    void requestClose();

private:
    void installFilter(QObject *o);
    void resizeAndMove();

private:
    bool turned_on;
    bool auto_hide;
    Activation activation_area;
    QWidget *internal_widget;
    QTimer *timer;
    QPropertyAnimation *m_animation;
    QPropertyAnimation *m_gatherAnimation;
    QPushButton *m_closeBtn;
};

#endif // AUTOHIDEWIDGET_H
