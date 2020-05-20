/*
 * Authors: lixiang@kylinos.cn
 */

#include "restartwidget.h"

#include <QVBoxLayout>
#include <QHBoxLayout>

RestartWidget::RestartWidget(QString appName, QWidget * parent)
    : QWidget(parent)
    , m_msgLabel(new QLabel(this))
    , m_closeBtn(new QPushButton(this))
    //, m_restartBtn(new QPushButton(this))
    , m_appName(appName)
{
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_msgLabel->setAlignment(Qt::AlignCenter);
    m_msgLabel->adjustSize();

    QHBoxLayout *hlayout = new QHBoxLayout;
    hlayout->setSpacing(50);
    hlayout->setMargin(0);
    hlayout->addStretch();
    hlayout->addWidget(m_closeBtn);
    //hlayout->addWidget(m_restartBtn);
    hlayout->addStretch();

    QVBoxLayout *layout = new QVBoxLayout;
    layout->setSpacing(50);
    layout->setMargin(0);
    layout->addStretch();
    layout->addWidget(m_msgLabel);
    layout->addLayout(hlayout);
    layout->addStretch();
    this->setLayout(layout);

    //m_msgLabel->setText(tr("Android internal exception detected, do you need to restart the application?"));
    m_msgLabel->setText(QString(tr("检测到 %1 已退出，请关闭窗口后重启 %1 !")).arg(m_appName));
    m_closeBtn->setText(tr("关闭窗口"));
    //m_restartBtn->setText(tr("Restart App"));

    connect(m_closeBtn, &QPushButton::clicked, this, &RestartWidget::requestCloseWindow);
    //connect(m_restartBtn, &QPushButton::clicked, this, &RestartWidget::requestRestartApp);
}

RestartWidget::~RestartWidget()
{

}

