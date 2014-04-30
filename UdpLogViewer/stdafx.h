#pragma once

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/asio/deadline_timer.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>

#include <QtGui>
#include "Util.h"

#define VERSION_TEXT	"1.4.10"

#define AUTHOR_TEXT		"gunoodaddy"
#define PROGRAM_TEXT	"UDP Log Viewer"
#define PROGRAM_SEMAPHORE_KEY L"gunoodaddy-0813-udplogviewer"