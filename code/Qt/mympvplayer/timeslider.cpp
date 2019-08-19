#include "timeslider.h"

#include <QWheelEvent>
#include <QTimer>
#include <QDebug>
#include <QApplication>
#include <QMouseEvent>
#include <QStyle>
#include <QStyleOption>

TimeSlider::TimeSlider( QWidget * parent ) : QSlider(parent)
	, dont_update(false)
	, position(0)
	, total_time(0)
{
    setOrientation(Qt::Horizontal);
	setMinimum(0);
    setMaximum(1000);

	setFocusPolicy( Qt::NoFocus );
	setSizePolicy( QSizePolicy::Expanding , QSizePolicy::Fixed );

	connect( this, SIGNAL( sliderPressed() ), this, SLOT( stopUpdate() ) );
	connect( this, SIGNAL( sliderReleased() ), this, SLOT( resumeUpdate() ) );
	connect( this, SIGNAL( sliderReleased() ), this, SLOT( mouseReleased() ) );
    connect( this, SIGNAL( valueChanged(int) ), this, SLOT( valueChanged_slot(int) ) );
    connect( this, SIGNAL(draggingPos(int) ), this, SLOT(checkDragging(int)) );
    cur_pos = QPoint(0,0);
    last_pos_to_send = -1;
    timer = new QTimer(this);
//    timer->setInterval(100);
    connect( timer, SIGNAL(timeout()), this, SLOT(sendDelayedPos()) );
    timer->start(200);
}

TimeSlider::~TimeSlider()
{
}

inline int TimeSlider::pick(const QPoint &pt) const
{
    return orientation() == Qt::Horizontal ? pt.x() : pt.y();
}
\

void TimeSlider::stopUpdate() {
	dont_update = true;
}

void TimeSlider::resumeUpdate()
{
	dont_update = false;
}

void TimeSlider::mouseReleased()
{
	emit posChanged( value() );
}

void TimeSlider::valueChanged_slot(int v)
{
    bool dragging = dont_update;
    if (!dragging) {
        if (v != position) {
            emit posChanged(v);
        }
    }
}

void TimeSlider::checkDragging(int v)
{
    last_pos_to_send = v;
}

void TimeSlider::sendDelayedPos()
{
    if (last_pos_to_send != -1) {
        last_pos_to_send = -1;
    }
}

void TimeSlider::setPos(int v)
{
    if (v != position) {
        if (!dont_update) {
            position = v;
            setValue(v);
        }
    }
}

void TimeSlider::wheelEvent(QWheelEvent * e)
{
	e->accept();

	if (e->orientation() == Qt::Vertical) {
		if (e->delta() >= 0)
			emit wheelUp();
		else
			emit wheelDown();
	} else {
		qDebug("Timeslider::wheelEvent: horizontal event received, doing nothing");
	}
}

int TimeSlider::pixelPosToRangeValue(int pos) const
{
    QStyleOptionSlider opt;
    initStyleOption(&opt);
    QRect gr = style()->subControlRect(QStyle::CC_Slider, &opt, QStyle::SC_SliderGroove, this);
    QRect sr = style()->subControlRect(QStyle::CC_Slider, &opt, QStyle::SC_SliderHandle, this);
    int sliderMin, sliderMax, sliderLength;

    if (orientation() == Qt::Horizontal) {
        sliderLength = sr.width();
        sliderMin = gr.x();
        sliderMax = gr.right() - sliderLength + 1;
    } else {
        sliderLength = sr.height();
        sliderMin = gr.y();
        sliderMax = gr.bottom() - sliderLength + 1;
    }
    return QStyle::sliderValueFromPosition(minimum(), maximum(), pos - sliderMin,
                                           sliderMax - sliderMin, opt.upsideDown);
}

void TimeSlider::mousePressEvent( QMouseEvent * e )
{
    if (e->button() == Qt::LeftButton) {
        QStyleOptionSlider opt;
        initStyleOption(&opt);
        const QRect sliderRect = style()->subControlRect(QStyle::CC_Slider, &opt, QStyle::SC_SliderHandle, this);
        const QPoint center = sliderRect.center() - sliderRect.topLeft();

        if (!sliderRect.contains(e->pos())) {
            e->accept();

            setSliderPosition(pixelPosToRangeValue(pick(e->pos() - center)));
            triggerAction(SliderMove);
            setRepeatAction(SliderNoAction);
        } else {
            QSlider::mousePressEvent(e);
        }
    } else {
        QSlider::mousePressEvent(e);
    }
}
