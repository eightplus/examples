#include "listitem.h"
#include "detailwidget.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QPainter>
#include <QEvent>
#include <QDebug>

class Label : public QWidget
{
public:
    explicit Label(QWidget *parent = 0);

    void setTitle(const QString &text);

private:
    void resizeEvent(QResizeEvent *e) Q_DECL_OVERRIDE;
    void updateTitle();

    QLabel *titleLabel;
    QLabel *messageLabel;
    QString titleText;

    friend class ListItem;
};

Label::Label(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *main_layout = new QVBoxLayout(this);

    titleLabel = new QLabel(this);
    messageLabel = new QLabel(this);
    messageLabel->setWordWrap(true);
    messageLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    main_layout->addWidget(titleLabel);
    main_layout->addWidget(messageLabel);
}

void Label::setTitle(const QString &text)
{
    titleText = text;
    updateTitle();
}

void Label::resizeEvent(QResizeEvent *e)
{
    updateTitle();

    return QWidget::resizeEvent(e);
}

void Label::updateTitle()
{
    QFontMetrics font_metrics = titleLabel->fontMetrics();

    titleLabel->setText(font_metrics.elidedText(titleText, Qt::ElideMiddle, titleLabel->width()));
}

ListItem::ListItem(QWidget *parent)
    : QWidget(parent)
    , m_mouseHover(false)
    , m_label(new Label(this))
    , m_btn(new QPushButton(this))
    , m_titleLayout(new QHBoxLayout())
    , m_detailWidget(new DetailWidget(this))
{
    m_label->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    m_btn->setText(tr("Connect"));
    m_btn->setFocusPolicy(Qt::NoFocus);

    m_titleLayout->addWidget(m_label, 0, Qt::AlignVCenter | Qt::AlignLeft);
    m_titleLayout->addWidget(m_btn, 0, Qt::AlignVCenter | Qt::AlignRight);

    m_detailWidget->setVisible(false);

    QVBoxLayout *main_layout = new QVBoxLayout();
    main_layout->setMargin(0);
    main_layout->setSpacing(0);
    main_layout->addLayout(m_titleLayout);
    main_layout->addWidget(m_detailWidget);
    this->setLayout(main_layout);
}

void ListItem::setTitle(const QString &title)
{
    m_label->setTitle(title);
}

void ListItem::setMessage(const QString &message)
{
    m_label->messageLabel->setText(message);
}

void ListItem::setDetailContents(const QStringList &data)
{
    m_detailWidget->loadData(data);
}

QString ListItem::title() const
{
    return m_label->titleLabel->text();
}

void ListItem::addWidget(QWidget *w, int stretch, Qt::Alignment alignment)
{
    m_titleLayout->addWidget(w, stretch, alignment);
    w->installEventFilter(this);
}

void ListItem::setDetailWidgetVisible()
{
    m_detailWidget->setVisible(!m_detailWidget->isVisible());
}

bool ListItem::isDetailWidgetVisible() const
{
    return m_detailWidget->isVisible();
}

int ListItem::getDetailWidgetHeight() const
{
    return m_detailWidget->height();
}

void ListItem::enterEvent(QEvent *event)
{
    QWidget::enterEvent(event);
    this->m_mouseHover = true;
    update();
}

void ListItem::leaveEvent(QEvent *event)
{
    QWidget::leaveEvent(event);
    this->m_mouseHover = false;
    update();
}

void ListItem::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    if (m_mouseHover) {
        QPainterPath path;
        path.addRect(QRectF(this->rect().x(), this->rect().y(), this->rect().width(), this->rect().height()));
        painter.setOpacity(1.0);
        painter.fillPath(path, QColor("#f5fbff"));

        m_btn->setVisible(true);
    }
    else {
        m_btn->setVisible(false);
    }
}
