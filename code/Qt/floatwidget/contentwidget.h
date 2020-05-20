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

#ifndef CONTENTWIDGET_H
#define CONTENTWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>

class ContentWidget : public QWidget
{
    Q_OBJECT

public:
    ContentWidget(QWidget *parent = 0);
    ~ContentWidget();

signals:
    void requestHorizontal();
    void requestVertical();

public slots:
    void handleHorizontal();
    void handleVertical();

private:
    QPushButton *m_horizontalButton;
    QPushButton *m_verticalButton;
    QVBoxLayout *m_layout;
};

#endif // CONTENTWIDGET_H
