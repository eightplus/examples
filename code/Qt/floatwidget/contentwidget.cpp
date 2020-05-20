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

#include <QDebug>
#include <QApplication>

#include "contentwidget.h"

ContentWidget::ContentWidget(QWidget *parent)
    : QWidget(parent)
{
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    this->setFocusPolicy(Qt::StrongFocus);

    m_layout = new QVBoxLayout();
    m_layout->setContentsMargins(0, 0, 0, 0);
    this->setLayout(m_layout);

    m_horizontalButton = new QPushButton;
    m_horizontalButton->setText("切换到水平显示界面");
    connect(m_horizontalButton, SIGNAL(clicked()), this, SLOT(handleHorizontal()));

    m_verticalButton = new QPushButton;
    m_verticalButton->setText("切换到垂直显示界面");
    connect(m_verticalButton, SIGNAL(clicked()), this, SLOT(handleVertical()));

    m_layout->addWidget(m_horizontalButton);
}

ContentWidget::~ContentWidget()
{
    QLayoutItem *child;
    while ((child = m_layout->takeAt(0)) != 0) {
        if (child->widget())
            child->widget()->deleteLater();
        delete child;
    }
}

void ContentWidget::handleHorizontal()
{
    m_layout->removeWidget(m_horizontalButton);
    m_horizontalButton->setParent(NULL);
    m_layout->addWidget(m_verticalButton);
    emit requestHorizontal();
}

void ContentWidget::handleVertical()
{
    m_layout->removeWidget(m_verticalButton);
    m_verticalButton->setParent(NULL);
    m_layout->addWidget(m_horizontalButton);
    emit requestVertical();
}
