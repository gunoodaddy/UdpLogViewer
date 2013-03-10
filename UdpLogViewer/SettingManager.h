#pragma once

#include "Singleton.h"

#define SettingManagerPtr()		CSingleton<CSettingManager>::Instance()

class CSettingManager : public QObject
{
	Q_OBJECT

public:
	static QString KEY_TOP_WINDOW;
	static QString KEY_WORD_WRAP;
	static QString KEY_IGNORE_NEW_LINE;
	static QString KEY_TAB_PIN;
	static QString KEY_SCROLL_PIN;
	static QString KEY_WINDOW_TRANSPARENT;
	static QString KEY_LOG_HEADER_BEGIN;
	static QString KEY_LOG_HEADER_END;
	static QString KEY_LOG_HEADER_SPLITTER;
	static QString KEY_SEARCH_TEXT;
	static QString KEY_SEARCH_CASE_SENSITIVE;
	static QString KEY_SEARCH_SELECTION_FG_COLOR;
	static QString KEY_SEARCH_SELECTION_BG_COLOR;

	CSettingManager(void);
	~CSettingManager(void);

	QVariant getValue(const QString &key)
	{
		QSettings settings( iniFile, QSettings::IniFormat );
		settings.beginGroup("Setting");
		QVariant value = settings.value(key);
		settings.endGroup();
		return value;
	}

	void setValue(const QString &key, QVariant value)
	{
		QSettings settings( iniFile, QSettings::IniFormat );
		settings.beginGroup("Setting");
		settings.setValue(key, value);
		settings.endGroup();
	}

private:
	QString iniFile;
};
