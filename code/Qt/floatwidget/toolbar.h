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

#ifndef BOTTOMWIDGET_H
#define BOTTOMWIDGET_H

#include <QWidget>
#include <QBoxLayout>
#include <QPushButton>
class ToolBar : public QWidget
{
    Q_OBJECT

public:
    ToolBar(QWidget * parent = 0);
    ~ToolBar();

    void setFlowDirection(Qt::Orientations direction);

signals:
    void requestEnable();
    void requestDisable();

private:
    QBoxLayout *m_layout = nullptr;
    QPushButton *m_enableFloatBtn = nullptr;
    QPushButton *m_disableFloatBtn = nullptr;
};

#endif // BOTTOMWIDGET_H
