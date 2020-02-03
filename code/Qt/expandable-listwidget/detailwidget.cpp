#include "detailwidget.h"

#include <QScrollArea>
#include <QHBoxLayout>
#include <QLabel>
#include <QTimer>
#include <QEvent>
#include <QDebug>

DetailWidget::DetailWidget(QWidget *parent)
    : QWidget(parent)
    , m_scrollArea(new QScrollArea)
    , m_layout(new QVBoxLayout)
    , m_timer(new QTimer(this))
{
    m_layout->setSpacing(10);
    m_layout->setContentsMargins(0, 0, 0, 0);

    m_scrollWidget = new QFrame();
    m_scrollWidget->setLayout(m_layout);
    m_scrollWidget->installEventFilter(this);

    m_scrollArea->setWidget(m_scrollWidget);
    m_scrollArea->setWidgetResizable(true);
    m_scrollArea->setFocusPolicy(Qt::NoFocus);
    m_scrollArea->setFrameStyle(QFrame::NoFrame);
    m_scrollArea->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Expanding);
    m_scrollArea->setContentsMargins(0, 0, 0, 0);
    m_scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    QVBoxLayout *main_layout = new QVBoxLayout();
    main_layout->setContentsMargins(0, 0, 0, 0);
    main_layout->setSpacing(0);
    main_layout->addWidget(m_scrollArea, Qt::AlignHCenter);
    this->setLayout(main_layout);
    this->setContentsMargins(0, 0, 0, 0);
    QSizePolicy policy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    policy.setVerticalStretch(100);
    this->setSizePolicy(policy);

    m_timer->setSingleShot(true);
    m_timer->setInterval(15);
    connect(m_timer, &QTimer::timeout, this, &DetailWidget::updateHeight, Qt::QueuedConnection);
}

DetailWidget::~DetailWidget()
{
    QLayoutItem *child;
    while ((child = m_layout->takeAt(0)) != 0) {
      QLabel *item = qobject_cast<QLabel*>(child->widget());
      if (item)
          item->deleteLater();
      delete child;
    }

    /*for (QLayoutItem *item = m_layout->takeAt(0); item != nullptr; item = m_layout->takeAt(0)) {
        delete item->widget();
        delete item;
        item = nullptr;
    }*/
}

void DetailWidget::loadData(const QStringList &data)
{
    foreach (QString name, data) {
        QLabel *label = new QLabel;
        label->setText(name);
        m_layout->addWidget(label);
        m_timer->start();
    }
}

void DetailWidget::updateHeight()
{
    Q_ASSERT(sender() == m_timer);

    this->setFixedHeight(m_layout->sizeHint().height());
}

bool DetailWidget::eventFilter(QObject *, QEvent *event)
{
    switch (event->type())
    {
    case QEvent::Resize:
        m_timer->start();
        break;
    default:;
    }

    return false;
}
