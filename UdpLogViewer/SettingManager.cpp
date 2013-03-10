#include "StdAfx.h"
#include "SettingManager.h"

#define DEFAULT_FILE_NAME	"setting.ini"

QString CSettingManager::KEY_WORD_WRAP = "wordWrap";
QString CSettingManager::KEY_IGNORE_NEW_LINE = "ignoreNewLine";
QString CSettingManager::KEY_LOG_HEADER_BEGIN = "logHeaderBegin";
QString CSettingManager::KEY_LOG_HEADER_END = "logHeaderEnd";
QString CSettingManager::KEY_LOG_HEADER_SPLITTER = "logHeaderSplitter";
QString CSettingManager::KEY_TOP_WINDOW = "topWindow";
QString CSettingManager::KEY_TAB_PIN = "tabPin";
QString CSettingManager::KEY_SCROLL_PIN = "scrollPin";
QString CSettingManager::KEY_WINDOW_TRANSPARENT = "winTransparent";
QString CSettingManager::KEY_SEARCH_TEXT = "searchText";
QString CSettingManager::KEY_SEARCH_CASE_SENSITIVE = "searchCaseSensitive";
QString CSettingManager::KEY_SEARCH_SELECTION_FG_COLOR = "searchSelectionFgColor";
QString CSettingManager::KEY_SEARCH_SELECTION_BG_COLOR = "searchSelectionBgColor";

CSettingManager::CSettingManager(void)
{
	iniFile = qApp->applicationDirPath() + QDir::separator() + DEFAULT_FILE_NAME;
}

CSettingManager::~CSettingManager(void)
{
}
