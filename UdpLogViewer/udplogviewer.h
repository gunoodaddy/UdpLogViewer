#ifndef UdpLogViewer_H
#define UdpLogViewer_H

#include <QtGui/QMainWindow>
#include "ui_UdpLogViewer.h"
#include "UdpLogManager.h"
#include "DefferedCaller.h"

class UdpLogViewer : public QMainWindow, IUdpLogManagerEvent
{
	Q_OBJECT

public:
	UdpLogViewer(QWidget *parent = 0, Qt::WFlags flags = 0);
	~UdpLogViewer();

	void addLogMessage(const QString &address, const QString &log);
	void updateWindowTitle(void);
	void loadCSS();
	void loadQSS();
	void loadCurrent();
	void saveCurrent();
	QTextEdit *getCurrentLogTextEdit();
	void doHighlightText(QString text, bool fromFirst = true, bool caseSensitive = false);

protected slots:
	void onClickedScrollPin(bool checked);
	void onTabRemoveSlot();
	void onClickedCSSReload();
	void onClickedLogClear();
	void onClickedTabAllClear();
	void onValueChangedWindowTransparent(int value);
	void onClickedTopWindow(bool checked);
	void onChangedSearchText(QString text);
	void onClickedSearchPrev();
	void onClickedSearchNext();
	void onClickedCaseSensitive(bool checked);
	void onLogSelectionChanged();
	void onClickedWordWrap(bool checked);
	void onClickedIgnoreNewLine(bool checked);

protected:
	bool eventFilter(QObject *object, QEvent *evt);
	void closeEvent( QCloseEvent *evt );

private:
	virtual void onIUdpLogManagerEvent_LogReceived( const std::string &address, int port, const std::string &buffer )
	{
		QString addr = QString::fromStdString(address) + ":" + QString::number(port);
		QString key = addr;

		QString log = QString::fromUtf8(buffer.c_str());
		if(log[0] == '|') {
			int pos = log.indexOf('|', 1);
			if(pos > 2) {
				key = log.mid(1, pos - 1);
				log = log.mid(pos + 1);
			}
		}

		//QString addr = QString::fromStdString(address);
		CDefferedCaller::singleShot( boost::bind(&UdpLogViewer::addLogMessage, this, key, log) );
	}

private:
	Ui::UdpLogViewerClass ui;

	UdpLogManager manager_;

	QMap<QString, QTextEdit *> logTextWidgetMap_;
	QString loadedCSS_;
	QString searchSelectionFgColor_;
	QString searchSelectionBgColor_;
	bool doHighlightWorking_;
};

#endif // UdpLogViewer_H
