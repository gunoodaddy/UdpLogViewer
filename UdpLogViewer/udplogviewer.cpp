#include "stdafx.h"
#include "UdpLogViewer.h"
#include "SettingManager.h"
#include "GlobalEvent.h"


UdpLogViewer::UdpLogViewer(QWidget *parent, Qt::WFlags flags)
: QMainWindow(parent, flags), doHighlightWorking_(false), requestHighlight_(false), requestHighlightForceMode_(false)
{
	UpgradeManagerPtr()->registerObserver( this );

	setWindowIcon( QIcon( QPixmap(":/UdpLogViewer/Resources/UdpLogViewer.ico") ) );

	ui.setupUi(this);

	manager_.setEvent(this);
	manager_.listen(12500);

	QShortcut *shortcut = new QShortcut(QKeySequence("Ctrl+W"), this);
	QObject::connect(shortcut, SIGNAL(activated()), this, SLOT(onTabRemoveSlot()));

	shortcut = new QShortcut(QKeySequence("Ctrl+Q"), this);
	QObject::connect(shortcut, SIGNAL(activated()), ui.logClearBtn, SLOT(click()));

	shortcut = new QShortcut(QKeySequence("Ctrl+E"), this);
	QObject::connect(shortcut, SIGNAL(activated()), ui.tabAllClearBtn, SLOT(click()));

	shortcut = new QShortcut(QKeySequence("F2"), this);
	QObject::connect(shortcut, SIGNAL(activated()), ui.scrollPinCheck, SLOT(click()));

	shortcut = new QShortcut(QKeySequence("F3"), this);
	QObject::connect(shortcut, SIGNAL(activated()), ui.prevBtn, SLOT(click()));

	shortcut = new QShortcut(QKeySequence("F4"), this);
	QObject::connect(shortcut, SIGNAL(activated()), ui.nextBtn, SLOT(click()));

	loadCSS();
	loadQSS();
	loadCurrent();

	updateWindowTitle();

	QToolButton *toolBtn = new QToolButton;
    toolBtn->setCursor(Qt::ArrowCursor);
    toolBtn->setAutoRaise(true);
    toolBtn->setIcon(QIcon(":/UdpLogViewer/Resources/remove.png"));
	QObject::connect(toolBtn, SIGNAL(clicked()), this, SLOT(onTabRemoveSlot()));
	ui.tabWidget->setCornerWidget(toolBtn, Qt::TopRightCorner);

	timerUpdate_ = new QTimer(this);
	timerUpdate_->start(200);
	connect(timerUpdate_, SIGNAL(timeout()),this, SLOT(onHighlightTimer()));
}

UdpLogViewer::~UdpLogViewer()
{
	UpgradeManagerPtr()->unregisterObserver( this );
}

void UdpLogViewer::closeEvent( QCloseEvent *evt )
{
	manager_.destroy();

	saveCurrent();

	QMainWindow::closeEvent( evt );
}

void UdpLogViewer::loadCSS()
{
#define STYLE_CSS_FILE "style.css"
	QString cssFile = qApp->applicationDirPath() + QDir::separator() + STYLE_CSS_FILE;

	QFile f(cssFile);
	if(!f.open(QIODevice::ReadOnly)) 
	{
		return;
	}

	QByteArray byteArray;
	byteArray = f.readAll();
	QString css(byteArray);
	loadedCSS_ = css;
}

void UdpLogViewer::loadQSS()
{
#define STYLE_QSS_FILE "style.css"
	QString qssFile = qApp->applicationDirPath() + QDir::separator() + STYLE_QSS_FILE;
	QFile f(qssFile);

	if(!f.open(QFile::ReadOnly))
	{
		return;
	}

	QString StyleSheet = QLatin1String(f.readAll());

	qApp->setStyleSheet(StyleSheet);
}

bool UdpLogViewer::eventFilter(QObject *object, QEvent *evt)
{
	return QMainWindow::eventFilter(object, evt);
}

QString getAuthorText()
{
	QString author;

	author.append(60 + 43);	// 103
	author.append(50 + 67);	// 117
	author.append(30 + 80);	// 110
	author.append(25 + 86);	// 111
	author.append(100 + 11); // 111
	author.append(11 + 89);	// 100
	author.append(17 + 80);	// 97
	author.append(10 + 90);	// 100
	author.append(1 + 99);	// 100
	author.append(1 + 120);	// 121
	return author;
}

void UdpLogViewer::onHighlightTimer()
{
	if(requestHighlight_) 
	{
		requestHighlight_ = false;

		if(doHighlightWorking_ == false) 
		{
			CDefferedCaller::singleShot( boost::bind(&UdpLogViewer::doHighlightText, this, ui.searchText->text(), true, ui.caseSensitiveCheck->isChecked(), requestHighlightForceMode_ ));
			if(requestHighlightForceMode_)
				requestHighlightForceMode_ = false;
		}
	}
}

void UdpLogViewer::updateWindowTitle( void )
{
	// Title
	QString newTitle = PROGRAM_TEXT;
	newTitle += " Ver ";
	newTitle += VERSION_TEXT;
	newTitle += " by ";
	newTitle += getAuthorText();
	newTitle += " Server IP = ";
	newTitle += QString::fromAscii(Util::getMyIPAddress().c_str());

	setWindowTitle( newTitle );
}

void UdpLogViewer::loadCurrent()
{
	ui.tabPinCheck->setChecked(SettingManagerPtr()->getValue(CSettingManager::KEY_TAB_PIN).toBool());
	ui.scrollPinCheck->setChecked(SettingManagerPtr()->getValue(CSettingManager::KEY_SCROLL_PIN).toBool());
	int value = SettingManagerPtr()->getValue(CSettingManager::KEY_WINDOW_TRANSPARENT).toInt();
	if(value < 20)
		value = 100;
	ui.transparentSlider->setValue(value);
	setWindowOpacity(((double)value / 100));

	bool checked = SettingManagerPtr()->getValue(CSettingManager::KEY_TOP_WINDOW).toBool();
	ui.topWindowCheck->setChecked(checked);
	onClickedTopWindow(checked);

	checked = SettingManagerPtr()->getValue(CSettingManager::KEY_WORD_WRAP).toBool();
	ui.wordWrapCheck->setChecked(checked);
	onClickedWordWrap(checked);

	ui.searchText->setText(SettingManagerPtr()->getValue(CSettingManager::KEY_SEARCH_TEXT).toString());
	ui.caseSensitiveCheck->setChecked(SettingManagerPtr()->getValue(CSettingManager::KEY_SEARCH_CASE_SENSITIVE).toBool());
	ui.ignoreNewLineCheck->setChecked(SettingManagerPtr()->getValue(CSettingManager::KEY_IGNORE_NEW_LINE).toBool());
	searchSelectionFgColor_ = SettingManagerPtr()->getValue(CSettingManager::KEY_SEARCH_SELECTION_FG_COLOR).toString();
	searchSelectionBgColor_ = SettingManagerPtr()->getValue(CSettingManager::KEY_SEARCH_SELECTION_BG_COLOR).toString();
}

void UdpLogViewer::saveCurrent()
{
	SettingManagerPtr()->setValue(CSettingManager::KEY_TAB_PIN, ui.tabPinCheck->isChecked());
	SettingManagerPtr()->setValue(CSettingManager::KEY_SCROLL_PIN, ui.scrollPinCheck->isChecked());
	SettingManagerPtr()->setValue(CSettingManager::KEY_WINDOW_TRANSPARENT, ui.transparentSlider->value());
	SettingManagerPtr()->setValue(CSettingManager::KEY_TOP_WINDOW, ui.topWindowCheck->isChecked());
	SettingManagerPtr()->setValue(CSettingManager::KEY_WORD_WRAP, ui.wordWrapCheck->isChecked());
	SettingManagerPtr()->setValue(CSettingManager::KEY_IGNORE_NEW_LINE, ui.ignoreNewLineCheck->isChecked());
	SettingManagerPtr()->setValue(CSettingManager::KEY_SEARCH_TEXT, ui.searchText->text());
	SettingManagerPtr()->setValue(CSettingManager::KEY_SEARCH_CASE_SENSITIVE, ui.caseSensitiveCheck->isChecked());
}

void UdpLogViewer::addLogMessage(const QString &key, const QString &log)
{
	QTextEdit *logTextEdit = NULL;
	QTextEditorContextMap::const_iterator i = logTextWidgetMap_.find(key);
	if(i == logTextWidgetMap_.end())
	{
		// New tab created..
		logTextEdit = new QTextEdit();
		QObject::connect(logTextEdit, SIGNAL(selectionChanged()), this, SLOT(onLogSelectionChanged()));
		logTextEdit->document()->setDefaultStyleSheet(loadedCSS_);

		boost::shared_ptr<UdpLogViewContext> ctx = boost::shared_ptr<UdpLogViewContext>(new UdpLogViewContext());
		ctx->logTextEdit = logTextEdit;
		logTextWidgetMap_.insert(key, ctx);
		ui.tabWidget->insertTab(0, logTextEdit, key);

		// word wrap
		logTextEdit->setWordWrapMode(ui.wordWrapCheck->isChecked() ? QTextOption::WrapAnywhere : QTextOption::NoWrap);

		// tab pin
		if(!ui.tabPinCheck->isChecked())
			ui.tabWidget->setCurrentIndex(0);	
	}
	else
	{
		// Already the tab exists.
		logTextEdit = i.value()->logTextEdit;
		if(!ui.tabPinCheck->isChecked())
		{
			int index = ui.tabWidget->indexOf(logTextEdit);
			if(index >= 0)
			{
				ui.tabWidget->setCurrentIndex(index);
			}
		}
	}

	int index = ui.tabWidget->indexOf(logTextEdit);
	if(index >= 0 && index != ui.tabWidget->currentIndex())
	{
		ui.tabWidget->setTabIcon(index, QIcon(":/UdpLogViewer/Resources/img_new.png"));
	}


	// [Log: 222, 87901.005, 226944, EDT]: 
	QString logMsg = log.trimmed();
	//logMsg = "13-02-15 17:35:17.604 450686C4 [api] fetchOperations";

	logMsg = logMsg.replace("<", "&lt;");
	logMsg = logMsg.replace(">", "&gt;");

	if(ui.ignoreNewLineCheck->isChecked() == false)
		logMsg = logMsg.replace("\n", "<br>");
	else {
		logMsg = logMsg.replace("\r\n", " ");
		logMsg = logMsg.replace("\n", " ");
	}

	bool headerParsed = false;
	int pos1 = -1;

	QString logHeaderBeginStr = SettingManagerPtr()->getValue(CSettingManager::KEY_LOG_HEADER_BEGIN).toString();

	if(logHeaderBeginStr.length() <= 0)
		pos1 = 0;
	else
		pos1 = logMsg.indexOf(logHeaderBeginStr, 0);
	
	if(pos1 == 0) 
	{
		QString logHeaderEndStr = SettingManagerPtr()->getValue(CSettingManager::KEY_LOG_HEADER_END).toString();

		int pos2;
		if(logHeaderEndStr.length() <= 0)
			pos2 = logMsg.length();
		else
			pos2 = logMsg.indexOf(logHeaderEndStr, 1);

		if(pos2 > 0)
		{
			if(logHeaderBeginStr.length() <= 0)
				pos1 = -1;

			QString logHeaderSplitStr = SettingManagerPtr()->getValue(CSettingManager::KEY_LOG_HEADER_SPLITTER).toString();

			QString logHeader = logMsg.mid(pos1+1, pos2-1);
			QString logContents = logMsg.mid(pos2+1);

			std::vector<std::string> resultList;
			if(Util::stringTokenizer(logHeader.toStdString(), logHeaderSplitStr.toStdString(), resultList))
			{
				QString tempLogHeader = "<html>";
				QString tagName = "header";
				tempLogHeader += "<span class=bracket>";
				tempLogHeader += logHeaderBeginStr;
				tempLogHeader += "</span>";
				for(size_t i = 0; i < resultList.size(); i++)
				{
					tempLogHeader += "<span class=" + tagName + QString::number(i+1) + ">";
					tempLogHeader += QString::fromStdString(resultList[i]);
					tempLogHeader += "</span>";
					if(i != resultList.size() - 1)
					{
						tempLogHeader += "<span class=bracket>";
						tempLogHeader += logHeaderSplitStr;
						tempLogHeader += "</span>";
					}
				}
				tempLogHeader += "<span class=bracket>";
				tempLogHeader += logHeaderEndStr;
				tempLogHeader += "</span>";
				tempLogHeader += "<span class=contents>";
				tempLogHeader += logContents;
				tempLogHeader += "</span>";

				tempLogHeader += "</html>";

				logTextEdit->append( tempLogHeader );
				headerParsed = true;
			}
		}
	}
	
	if(headerParsed == false)
		logTextEdit->append(logMsg);

	doHighlightText(ui.searchText->text(), false, ui.caseSensitiveCheck->isChecked(), true);

	if(ui.scrollPinCheck->isChecked() == false)
	{
		logTextEdit->moveCursor(QTextCursor::End);
		logTextEdit->ensureCursorVisible();
		logTextEdit->horizontalScrollBar()->setValue(0);
	}
}

boost::shared_ptr<UdpLogViewContext> UdpLogViewer::getCurrentLogTextEdit() 
{
	int index = ui.tabWidget->currentIndex();
	if(index >= 0)
	{
		QString tabText = ui.tabWidget->tabText(index);
		QTextEditorContextMap::iterator i = logTextWidgetMap_.find(tabText);
		if(i != logTextWidgetMap_.end())
		{
			return i.value();
		}
	}
	return boost::shared_ptr<UdpLogViewContext>();
}

void UdpLogViewer::onTabRemoveSlot()
{
	int index = ui.tabWidget->currentIndex();
	if(index >= 0)
	{
		QString tabText = ui.tabWidget->tabText(index);
		QTextEditorContextMap::iterator i = logTextWidgetMap_.find(tabText);
		if(i != logTextWidgetMap_.end())
		{
			logTextWidgetMap_.erase(i);
		}
	
		ui.tabWidget->removeTab(index);
	}
}

void UdpLogViewer::onClickedCSSReload()
{
	saveCurrent();
	loadCurrent();
	loadCSS();
	loadQSS();

	for(int i = 0; i < ui.tabWidget->count(); i++)
	{
		QString address = ui.tabWidget->tabText(i);
		QTextEdit *logTextEdit = (QTextEdit*)ui.tabWidget->widget(i);
		logTextEdit->document()->setDefaultStyleSheet(loadedCSS_);
		QString text = logTextEdit->toPlainText();
		QStringList list = text.split('\n');
		logTextEdit->setText("");
		for(int i = 0; i < list.size(); i++)
		{
			addLogMessage(address, list.at(i));
		}
	}
}

static bool g_orgFormatSettingFlag = false;
static QTextCharFormat g_orgFormat;

void UdpLogViewer::doHighlightText(QString text, bool fromFirst, bool caseSensitive, bool forceMode)
{
	qDebug() << "doHighlightText : forceMode=" << forceMode << "fromFirst=" << fromFirst;

	if(forceMode == false && text == lastHighlightText_) 
	{
		return;
	}

	lastHighlightText_ = text;

	if(doHighlightWorking_)
		return;

	doHighlightWorking_ = true;
	do
	{
		boost::shared_ptr<UdpLogViewContext> logTextEditCtx = getCurrentLogTextEdit();
		if(logTextEditCtx == NULL)
			break;

		QTextEdit *logTextEdit = logTextEditCtx->logTextEdit;
		if(logTextEdit == NULL)
			break;

		int orgValueVert = logTextEdit->verticalScrollBar()->value();
		int orgValueHor = logTextEdit->horizontalScrollBar()->value();
		QTextCursor orgCursor = logTextEdit->textCursor();

		QTextDocument::FindFlags flags = 0x0;
		if(caseSensitive)
			flags |=  QTextDocument::FindCaseSensitively;

		QList<QTextEdit::ExtraSelection> extraSelections;
		if(fromFirst)
		{
			logTextEdit->moveCursor(QTextCursor::Start);
		}
		else
		{
			logTextEdit->setTextCursor(logTextEditCtx->prevSearchCursor);
			extraSelections = logTextEdit->extraSelections();
			if(extraSelections.size() > 0)
				extraSelections.removeLast();
		}

		bool find = false;

		QColor fgColor = QColor(searchSelectionFgColor_);
		QColor bgColor = QColor(searchSelectionBgColor_);

		while(logTextEdit->find(text, flags))
		{
			QTextEdit::ExtraSelection extra;
			extra.format.setForeground(fgColor);
			extra.format.setBackground(bgColor);
			extra.cursor = logTextEdit->textCursor();
			extraSelections.append(extra);
			find = true;
		}

		logTextEditCtx->prevSearchCursor = logTextEdit->textCursor();
		logTextEditCtx->prevSearchCursor.setPosition(std::max(0, logTextEditCtx->prevSearchCursor.position() - 100));
		logTextEditCtx->prevSearchCursor.clearSelection();

		logTextEdit->setExtraSelections(extraSelections);

		orgCursor.clearSelection();

		logTextEdit->setTextCursor(orgCursor);
		logTextEdit->verticalScrollBar()->setValue(orgValueVert);
		logTextEdit->horizontalScrollBar()->setValue(orgValueHor);
	} while(false);

	doHighlightWorking_ = false;
}

void UdpLogViewer::onLogSelectionChanged()
{
	if(doHighlightWorking_ || GlobalEventPtr()->isMousePressed() == true)
		return;

	boost::shared_ptr<UdpLogViewContext> logTextEditCtx = getCurrentLogTextEdit();
	if(logTextEditCtx == NULL)
		return;

	QTextEdit *logTextEdit = logTextEditCtx->logTextEdit;
	if(logTextEdit != NULL)
	{
		QString s = logTextEdit->textCursor().selectedText();

		if(s.isEmpty() == false && s.length() < 50)
		{
			qDebug() << "onLogSelectionChanged : " << doHighlightWorking_ << GlobalEventPtr()->isMousePressed() << s;

			ui.searchText->setText(s);
		}
	}
}

void UdpLogViewer::onChangedSearchText(QString text)
{
	requestHighlight_ = true;
	//doHighlightText(text, true, ui.caseSensitiveCheck->isChecked());
}

void UdpLogViewer::onClickedLogClear()
{
	int index = ui.tabWidget->currentIndex();
	if(index >= 0)
	{
		QTextEdit *logTextEdit = (QTextEdit*)ui.tabWidget->widget(index);
		logTextEdit->setText("");
	}
}

void UdpLogViewer::onClickedTabAllClear()
{
	logTextWidgetMap_.clear();
	ui.tabWidget->clear();
}

void UdpLogViewer::onClickedCaseSensitive(bool checked)
{
	requestHighlight_ = true;
	//doHighlightText(ui.searchText->text(), true, checked);
}

void UdpLogViewer::onClickedWordWrap(bool checked)
{
	for(int i = 0; i < ui.tabWidget->count(); i++)
	{
		QString address = ui.tabWidget->tabText(i);
		QTextEdit *logTextEdit = (QTextEdit*)ui.tabWidget->widget(i);
		logTextEdit->setWordWrapMode(checked ? QTextOption::WrapAnywhere : QTextOption::NoWrap);
	}
}

void UdpLogViewer::onClickedIgnoreNewLine(bool checked)
{

}

void UdpLogViewer::onValueChangedWindowTransparent(int value)
{
	setWindowOpacity(((double)value / 100));
}

void UdpLogViewer::onClickedSearchPrev()
{
	boost::shared_ptr<UdpLogViewContext> logTextEditCtx = getCurrentLogTextEdit();
	if(logTextEditCtx == NULL)
		return;

	QTextEdit *logTextEdit = logTextEditCtx->logTextEdit;
	if(logTextEdit != NULL)
	{
		QTextDocument::FindFlags flags = QTextDocument::FindBackward;
		if(ui.caseSensitiveCheck->isChecked())
			flags |=  QTextDocument::FindCaseSensitively;

		bool find = logTextEdit->find(ui.searchText->text(), flags);
		if(find == false)
		{
			logTextEdit->moveCursor(QTextCursor::End);
			logTextEdit->horizontalScrollBar()->setValue(0);
			logTextEdit->find(ui.searchText->text(), flags);
		}
	}
}


void UdpLogViewer::onClickedSearchNext()
{
	boost::shared_ptr<UdpLogViewContext> logTextEditCtx = getCurrentLogTextEdit();
	if(logTextEditCtx == NULL)
		return;

	QTextEdit *logTextEdit = logTextEditCtx->logTextEdit;
	if(logTextEdit != NULL)
	{
		QTextDocument::FindFlags flags = 0x0;
		if(ui.caseSensitiveCheck->isChecked())
			flags |=  QTextDocument::FindCaseSensitively;

		bool find = logTextEdit->find(ui.searchText->text(), flags);
		if(find == false)
		{
			logTextEdit->moveCursor(QTextCursor::Start);
			logTextEdit->find(ui.searchText->text(), flags);
		}
	}
}


void UdpLogViewer::onClickedScrollPin(bool checked)
{
	if(checked == false)
	{
		boost::shared_ptr<UdpLogViewContext> logTextEditCtx = getCurrentLogTextEdit();
		if(logTextEditCtx == NULL)
			return;

		QTextEdit *logTextEdit = logTextEditCtx->logTextEdit;
		if(logTextEdit != NULL)
		{
			logTextEdit->moveCursor(QTextCursor::End);
			logTextEdit->ensureCursorVisible();
			logTextEdit->horizontalScrollBar()->setValue(0);
		}
	}
}

void UdpLogViewer::onTabCurrentChanged(int index)
{
	requestHighlight_ = true;
	requestHighlightForceMode_ = true;
	ui.tabWidget->setTabIcon(index, QIcon());
}

void UdpLogViewer::onClickedClearSearchText()
{
	ui.searchText->setText("");
}

void UdpLogViewer::onClickedTopWindow(bool checked)
{
#ifdef Q_OS_WIN
    if (checked)
    {
        SetWindowPos(this->winId(), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
    }
    else
    {
        SetWindowPos(this->winId(), HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
    }
#else
    Qt::WindowFlags flags = this->windowFlags();
    if (checked)
    {
        this->setWindowFlags(flags | Qt::CustomizeWindowHint | Qt::WindowStaysOnTopHint);
        this->show();
    }
    else
    {
        this->setWindowFlags(flags ^ (Qt::CustomizeWindowHint | Qt::WindowStaysOnTopHint));
        this->show();
    }
#endif

}