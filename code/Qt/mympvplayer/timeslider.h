#ifndef TIMESLIDER_H
#define TIMESLIDER_H

#include <QSlider>

class TimeSlider : public QSlider
{
	Q_OBJECT

public:
    TimeSlider( QWidget * parent );
    ~TimeSlider();

public slots:
    virtual void setPos(int);
    void setDuration(double t) { total_time = t; };

signals:
    void posChanged(int);
    void draggingPos(int);
    void wheelUp();
    void wheelDown();

protected slots:
    void stopUpdate();
    void resumeUpdate();
    void mouseReleased();
    void valueChanged_slot(int);
    void checkDragging(int);
    void sendDelayedPos();

protected:
    void mousePressEvent ( QMouseEvent * event );
    inline int pick(const QPoint &pt) const;
    int pixelPosToRangeValue(int pos) const;
    void wheelEvent(QWheelEvent * e);

private:
    bool dont_update;
    int position;
    double total_time;
    int last_pos_to_send;
    QTimer * timer;
    QPoint cur_pos;
};

#endif // TIMESLIDER_H

