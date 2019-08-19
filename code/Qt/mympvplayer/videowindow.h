#ifndef VIDEOWINDOW_H
#define VIDEOWINDOW_H

#include <QWidget>
#include <QLabel>

class VideoWindow : public QWidget
{
    Q_OBJECT

public:
    VideoWindow(QWidget* parent = 0, Qt::WindowFlags f = 0);
    ~VideoWindow();

    QWidget * displayLayer() { return m_displayLayer; };
    void setResolution( int w, int h);
    void updateVideoWindow();

protected:
    void resizeEvent(QResizeEvent * e) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent * e) Q_DECL_OVERRIDE;
    void wheelEvent(QWheelEvent * e) Q_DECL_OVERRIDE;
    QSize sizeHint () const Q_DECL_OVERRIDE;

signals:
    void rightClicked();
    void wheelUp();
    void wheelDown();

private:
    int m_videoWidth, m_videoHeight;
    QWidget *m_displayLayer = nullptr;
    QWidget *m_cornerWidget = nullptr;
};

#endif // VIDEOWINDOW_H
