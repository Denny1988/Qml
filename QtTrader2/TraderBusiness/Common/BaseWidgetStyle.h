#ifndef COMMON_BASEWIDGETSTYLE_H
#define COMMON_BASEWIDGETSTYLE_H
#include <QWidget>
#include <QPainter>
#include <QStyleOption>

namespace Common
{
	class BaseWidgetStyle :  public QWidget
	{
	protected:
		void paintEvent(QPaintEvent * event)
		{
			QStyleOption opt;
			opt.init(this);
			QPainter p(this);
			style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
		}
	};	
}

#endif

