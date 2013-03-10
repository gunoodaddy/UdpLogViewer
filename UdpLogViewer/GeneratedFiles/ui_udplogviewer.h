/********************************************************************************
** Form generated from reading UI file 'udplogviewer.ui'
**
** Created: Sun Mar 10 18:52:57 2013
**      by: Qt User Interface Compiler version 4.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_UDPLOGVIEWER_H
#define UI_UDPLOGVIEWER_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLineEdit>
#include <QtGui/QMainWindow>
#include <QtGui/QPushButton>
#include <QtGui/QSlider>
#include <QtGui/QSpacerItem>
#include <QtGui/QTabWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_UdpLogViewerClass
{
public:
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout_2;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QCheckBox *topWindowCheck;
    QCheckBox *tabPinCheck;
    QCheckBox *scrollPinCheck;
    QPushButton *tabAllClearBtn;
    QPushButton *logClearBtn;
    QPushButton *cssReloadBtn;
    QSlider *transparentSlider;
    QHBoxLayout *horizontalLayout_3;
    QCheckBox *wordWrapCheck;
    QCheckBox *ignoreNewLineCheck;
    QSpacerItem *horizontalSpacer;
    QHBoxLayout *horizontalLayout_2;
    QLineEdit *searchText;
    QPushButton *prevBtn;
    QPushButton *nextBtn;
    QCheckBox *caseSensitiveCheck;
    QTabWidget *tabWidget;

    void setupUi(QMainWindow *UdpLogViewerClass)
    {
        if (UdpLogViewerClass->objectName().isEmpty())
            UdpLogViewerClass->setObjectName(QString::fromUtf8("UdpLogViewerClass"));
        UdpLogViewerClass->resize(666, 555);
        centralWidget = new QWidget(UdpLogViewerClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        verticalLayout_2 = new QVBoxLayout(centralWidget);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        topWindowCheck = new QCheckBox(centralWidget);
        topWindowCheck->setObjectName(QString::fromUtf8("topWindowCheck"));

        horizontalLayout->addWidget(topWindowCheck);

        tabPinCheck = new QCheckBox(centralWidget);
        tabPinCheck->setObjectName(QString::fromUtf8("tabPinCheck"));

        horizontalLayout->addWidget(tabPinCheck);

        scrollPinCheck = new QCheckBox(centralWidget);
        scrollPinCheck->setObjectName(QString::fromUtf8("scrollPinCheck"));

        horizontalLayout->addWidget(scrollPinCheck);

        tabAllClearBtn = new QPushButton(centralWidget);
        tabAllClearBtn->setObjectName(QString::fromUtf8("tabAllClearBtn"));
        tabAllClearBtn->setMinimumSize(QSize(90, 40));

        horizontalLayout->addWidget(tabAllClearBtn);

        logClearBtn = new QPushButton(centralWidget);
        logClearBtn->setObjectName(QString::fromUtf8("logClearBtn"));
        logClearBtn->setMinimumSize(QSize(80, 40));

        horizontalLayout->addWidget(logClearBtn);

        cssReloadBtn = new QPushButton(centralWidget);
        cssReloadBtn->setObjectName(QString::fromUtf8("cssReloadBtn"));
        cssReloadBtn->setMinimumSize(QSize(80, 40));

        horizontalLayout->addWidget(cssReloadBtn);

        transparentSlider = new QSlider(centralWidget);
        transparentSlider->setObjectName(QString::fromUtf8("transparentSlider"));
        transparentSlider->setMinimum(20);
        transparentSlider->setMaximum(100);
        transparentSlider->setOrientation(Qt::Horizontal);

        horizontalLayout->addWidget(transparentSlider);


        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        wordWrapCheck = new QCheckBox(centralWidget);
        wordWrapCheck->setObjectName(QString::fromUtf8("wordWrapCheck"));

        horizontalLayout_3->addWidget(wordWrapCheck);

        ignoreNewLineCheck = new QCheckBox(centralWidget);
        ignoreNewLineCheck->setObjectName(QString::fromUtf8("ignoreNewLineCheck"));

        horizontalLayout_3->addWidget(ignoreNewLineCheck);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer);


        verticalLayout->addLayout(horizontalLayout_3);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        searchText = new QLineEdit(centralWidget);
        searchText->setObjectName(QString::fromUtf8("searchText"));

        horizontalLayout_2->addWidget(searchText);

        prevBtn = new QPushButton(centralWidget);
        prevBtn->setObjectName(QString::fromUtf8("prevBtn"));

        horizontalLayout_2->addWidget(prevBtn);

        nextBtn = new QPushButton(centralWidget);
        nextBtn->setObjectName(QString::fromUtf8("nextBtn"));

        horizontalLayout_2->addWidget(nextBtn);

        caseSensitiveCheck = new QCheckBox(centralWidget);
        caseSensitiveCheck->setObjectName(QString::fromUtf8("caseSensitiveCheck"));

        horizontalLayout_2->addWidget(caseSensitiveCheck);


        verticalLayout->addLayout(horizontalLayout_2);

        tabWidget = new QTabWidget(centralWidget);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));

        verticalLayout->addWidget(tabWidget);


        verticalLayout_2->addLayout(verticalLayout);

        UdpLogViewerClass->setCentralWidget(centralWidget);

        retranslateUi(UdpLogViewerClass);
        QObject::connect(logClearBtn, SIGNAL(clicked()), UdpLogViewerClass, SLOT(onClickedLogClear()));
        QObject::connect(tabAllClearBtn, SIGNAL(clicked()), UdpLogViewerClass, SLOT(onClickedTabAllClear()));
        QObject::connect(cssReloadBtn, SIGNAL(clicked()), UdpLogViewerClass, SLOT(onClickedCSSReload()));
        QObject::connect(transparentSlider, SIGNAL(valueChanged(int)), UdpLogViewerClass, SLOT(onValueChangedWindowTransparent(int)));
        QObject::connect(scrollPinCheck, SIGNAL(clicked(bool)), UdpLogViewerClass, SLOT(onClickedScrollPin(bool)));
        QObject::connect(topWindowCheck, SIGNAL(clicked(bool)), UdpLogViewerClass, SLOT(onClickedTopWindow(bool)));
        QObject::connect(searchText, SIGNAL(textEdited(QString)), UdpLogViewerClass, SLOT(onChangedSearchText(QString)));
        QObject::connect(searchText, SIGNAL(textChanged(QString)), UdpLogViewerClass, SLOT(onChangedSearchText(QString)));
        QObject::connect(caseSensitiveCheck, SIGNAL(clicked(bool)), UdpLogViewerClass, SLOT(onClickedCaseSensitive(bool)));
        QObject::connect(prevBtn, SIGNAL(clicked()), UdpLogViewerClass, SLOT(onClickedSearchPrev()));
        QObject::connect(searchText, SIGNAL(returnPressed()), UdpLogViewerClass, SLOT(onClickedSearchNext()));
        QObject::connect(nextBtn, SIGNAL(clicked()), UdpLogViewerClass, SLOT(onClickedSearchNext()));
        QObject::connect(wordWrapCheck, SIGNAL(clicked(bool)), UdpLogViewerClass, SLOT(onClickedWordWrap(bool)));
        QObject::connect(ignoreNewLineCheck, SIGNAL(clicked(bool)), UdpLogViewerClass, SLOT(onClickedIgnoreNewLine(bool)));

        tabWidget->setCurrentIndex(-1);


        QMetaObject::connectSlotsByName(UdpLogViewerClass);
    } // setupUi

    void retranslateUi(QMainWindow *UdpLogViewerClass)
    {
        UdpLogViewerClass->setWindowTitle(QApplication::translate("UdpLogViewerClass", "Udp Log Server by gunoodaddy", 0, QApplication::UnicodeUTF8));
        topWindowCheck->setText(QApplication::translate("UdpLogViewerClass", "Top Window", 0, QApplication::UnicodeUTF8));
        tabPinCheck->setText(QApplication::translate("UdpLogViewerClass", "Tab Pin", 0, QApplication::UnicodeUTF8));
        scrollPinCheck->setText(QApplication::translate("UdpLogViewerClass", "Scroll Pin (F2)", 0, QApplication::UnicodeUTF8));
        tabAllClearBtn->setText(QApplication::translate("UdpLogViewerClass", "Tab All Crear", 0, QApplication::UnicodeUTF8));
        logClearBtn->setText(QApplication::translate("UdpLogViewerClass", "Log Clear", 0, QApplication::UnicodeUTF8));
        cssReloadBtn->setText(QApplication::translate("UdpLogViewerClass", "CSS Reload", 0, QApplication::UnicodeUTF8));
        wordWrapCheck->setText(QApplication::translate("UdpLogViewerClass", "Word Wrap", 0, QApplication::UnicodeUTF8));
        ignoreNewLineCheck->setText(QApplication::translate("UdpLogViewerClass", "Ignore New Line", 0, QApplication::UnicodeUTF8));
        prevBtn->setText(QApplication::translate("UdpLogViewerClass", "Prev (F3)", 0, QApplication::UnicodeUTF8));
        nextBtn->setText(QApplication::translate("UdpLogViewerClass", "Next (F4)", 0, QApplication::UnicodeUTF8));
        caseSensitiveCheck->setText(QApplication::translate("UdpLogViewerClass", "Case Sensitive", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class UdpLogViewerClass: public Ui_UdpLogViewerClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_UDPLOGVIEWER_H
