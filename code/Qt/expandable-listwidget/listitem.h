#ifndef LISTITEM_H
#define LISTITEM_H

#include <QWidget>
#include <QIcon>

QT_BEGIN_NAMESPACE
class QLabel;
class QPushButton;
class QHBoxLayout;
class DetailWidget;
QT_END_NAMESPACE

class Label;
class ListItem : public QWidget
{
    Q_OBJECT
public:
    explicit ListItem(QWidget *parent);

    void setTitle(const QString &title);
    void setMessage(const QString &message);
    void setDetailContents(const QStringList &data);

    QString title() const;

    void addWidget(QWidget *w, int stretch = 0, Qt::Alignment alignment = Qt::Alignment());
    void setDetailWidgetVisible();
    bool isDetailWidgetVisible() const;
    int getDetailWidgetHeight() const;

protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
    void enterEvent(QEvent *event) Q_DECL_OVERRIDE;
    void leaveEvent(QEvent *event) Q_DECL_OVERRIDE;

private:
    Label *m_label = nullptr;
    QPushButton *m_btn = nullptr;
    QHBoxLayout *m_titleLayout = nullptr;
    DetailWidget *m_detailWidget = nullptr;
    bool m_mouseHover;
};

#endif // LISTITEM_H
