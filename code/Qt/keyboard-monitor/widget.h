#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QKeyEvent>

class QLineEdit;
class QLabel;

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = 0);
    ~Widget();

    bool checkStatusStatus();
    void checkStatus();

public slots:
    void onLineEditTextChanged(const QString &text);

protected:
    bool eventFilter(QObject *watched, QEvent *event) Q_DECL_OVERRIDE;
    void keyPressEvent(QKeyEvent *event) Q_DECL_OVERRIDE;
    bool event(QEvent *event) Q_DECL_OVERRIDE;

private:
    QLineEdit *m_lineEdit;
    QLabel *m_label;
};

#endif // WIDGET_H
