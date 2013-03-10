#pragma once

#include "Singleton.h"

#define GlobalEventPtr()		CSingleton<GlobalEvent>::Instance()

class GlobalEvent : public QObject
{
public:
	GlobalEvent() : QObject() { }
	~GlobalEvent() { }

	bool eventFilter(QObject* object,QEvent* evt)
	{
		if(evt->type() == QEvent::MouseButtonPress) 
		{
			qDebug() << "pressed";
			mousePressed_ = true;
		}
		else if(evt->type() == QEvent::MouseButtonRelease) 
		{
			qDebug() << "released";
			mousePressed_ = false;
		}
		return QObject::eventFilter(object, evt);
	}

	bool isMousePressed()
	{
		return mousePressed_;
	}

private:
	bool mousePressed_;
};


