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

#include "toolbar.h"

#include <QEvent>
#include <QDebug>

ToolBar::ToolBar(QWidget * parent)
    : QWidget(parent)
    , m_layout(new QBoxLayout(QBoxLayout::LeftToRight))
{
    this->setFixedHeight(36);
    this->setAttribute(Qt::WA_StyledBackground, true);
    this->setStyleSheet("background-color: rgb(0,0,0)");//rgba(0,0,0,20)

    m_enableFloatBtn = new QPushButton/*SystemButton*/(this);
    m_enableFloatBtn->setText("允许");
    m_enableFloatBtn->setToolTip("允许显示右侧浮动工具栏");
    m_disableFloatBtn = new /*SystemButton*/QPushButton(this);
    m_disableFloatBtn->setText("禁止");
    m_disableFloatBtn->setToolTip("禁止显示右侧浮动工具栏");

    m_enableFloatBtn->setFixedSize(56,36);
    m_disableFloatBtn->setFixedSize(56,36);

    m_enableFloatBtn->setFocusPolicy(Qt::NoFocus);
    m_disableFloatBtn->setFocusPolicy(Qt::NoFocus);

    m_layout->setContentsMargins(0, 0, 0, 0);
    m_layout->setSpacing(50);
    m_layout->addStretch();
    m_layout->addWidget(m_enableFloatBtn);
    m_layout->addWidget(m_disableFloatBtn);
    m_layout->addStretch();
    this->setLayout(m_layout);

    connect(m_enableFloatBtn, SIGNAL(clicked()), this, SIGNAL(requestEnable()));
    connect(m_disableFloatBtn, SIGNAL(clicked()), this, SIGNAL(requestDisable()));
}

ToolBar::~ToolBar()
{
    QLayoutItem *child;
    while ((child = m_layout->takeAt(0)) != 0) {
        if (child->widget())
            child->widget()->deleteLater();
        delete child;
    }
}

void ToolBar::setFlowDirection(Qt::Orientations direction)
{
    if (direction == Qt::Vertical) {
        m_layout->setDirection(QBoxLayout::TopToBottom);
    }
    else {
        m_layout->setDirection(QBoxLayout::LeftToRight);
    }
}
