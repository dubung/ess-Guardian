/********************************************************************************
** Form generated from reading UI file 'tab3alert.ui'
**
** Created by: Qt User Interface Compiler version 6.8.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TAB3ALERT_H
#define UI_TAB3ALERT_H

#include <QtCore/QDate>
#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDateTimeEdit>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Tab3Alert
{
public:
    QVBoxLayout *verticalLayout_4;
    QVBoxLayout *verticalLayout_3;
    QHBoxLayout *horizontalLayout_2;
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *verticalLayout;
    QDateTimeEdit *pDateTimeFrom;
    QComboBox *pCBEventType;
    QVBoxLayout *verticalLayout_2;
    QDateTimeEdit *pDateTimeTo;
    QComboBox *pCBLevel;
    QSpacerItem *horizontalSpacer;
    QPushButton *pPBSearchDB;
    QVBoxLayout *pAlertlogLayout;
    QTableWidget *pAlertTable;
    QHBoxLayout *horizontalLayout_3;
    QSpacerItem *horizontalSpacer_2;
    QLabel *pLSearchCount;

    void setupUi(QWidget *Tab3Alert)
    {
        if (Tab3Alert->objectName().isEmpty())
            Tab3Alert->setObjectName("Tab3Alert");
        Tab3Alert->resize(666, 513);
        verticalLayout_4 = new QVBoxLayout(Tab3Alert);
        verticalLayout_4->setObjectName("verticalLayout_4");
        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setObjectName("verticalLayout_3");
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName("verticalLayout");
        pDateTimeFrom = new QDateTimeEdit(Tab3Alert);
        pDateTimeFrom->setObjectName("pDateTimeFrom");
        pDateTimeFrom->setMinimumDate(QDate(2025, 12, 1));
        pDateTimeFrom->setDisplayFormat(QString::fromUtf8("yyyy-MM-dd hh:mm"));

        verticalLayout->addWidget(pDateTimeFrom);

        pCBEventType = new QComboBox(Tab3Alert);
        pCBEventType->addItem(QString());
        pCBEventType->addItem(QString());
        pCBEventType->addItem(QString());
        pCBEventType->setObjectName("pCBEventType");

        verticalLayout->addWidget(pCBEventType);


        horizontalLayout->addLayout(verticalLayout);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName("verticalLayout_2");
        pDateTimeTo = new QDateTimeEdit(Tab3Alert);
        pDateTimeTo->setObjectName("pDateTimeTo");
        pDateTimeTo->setMinimumDate(QDate(2025, 12, 1));
        pDateTimeTo->setDisplayFormat(QString::fromUtf8("yyyy-MM-dd hh:mm"));

        verticalLayout_2->addWidget(pDateTimeTo);

        pCBLevel = new QComboBox(Tab3Alert);
        pCBLevel->addItem(QString());
        pCBLevel->addItem(QString());
        pCBLevel->addItem(QString());
        pCBLevel->setObjectName("pCBLevel");

        verticalLayout_2->addWidget(pCBLevel);


        horizontalLayout->addLayout(verticalLayout_2);


        horizontalLayout_2->addLayout(horizontalLayout);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

        pPBSearchDB = new QPushButton(Tab3Alert);
        pPBSearchDB->setObjectName("pPBSearchDB");

        horizontalLayout_2->addWidget(pPBSearchDB);

        horizontalLayout_2->setStretch(0, 7);
        horizontalLayout_2->setStretch(2, 1);

        verticalLayout_3->addLayout(horizontalLayout_2);

        pAlertlogLayout = new QVBoxLayout();
        pAlertlogLayout->setObjectName("pAlertlogLayout");
        pAlertTable = new QTableWidget(Tab3Alert);
        if (pAlertTable->columnCount() < 7)
            pAlertTable->setColumnCount(7);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        pAlertTable->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        pAlertTable->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        pAlertTable->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        pAlertTable->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        pAlertTable->setHorizontalHeaderItem(4, __qtablewidgetitem4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        pAlertTable->setHorizontalHeaderItem(5, __qtablewidgetitem5);
        QTableWidgetItem *__qtablewidgetitem6 = new QTableWidgetItem();
        pAlertTable->setHorizontalHeaderItem(6, __qtablewidgetitem6);
        pAlertTable->setObjectName("pAlertTable");

        pAlertlogLayout->addWidget(pAlertTable);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName("horizontalLayout_3");
        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_2);

        pLSearchCount = new QLabel(Tab3Alert);
        pLSearchCount->setObjectName("pLSearchCount");

        horizontalLayout_3->addWidget(pLSearchCount);

        horizontalLayout_3->setStretch(0, 7);
        horizontalLayout_3->setStretch(1, 2);

        pAlertlogLayout->addLayout(horizontalLayout_3);


        verticalLayout_3->addLayout(pAlertlogLayout);

        verticalLayout_3->setStretch(0, 1);
        verticalLayout_3->setStretch(1, 7);

        verticalLayout_4->addLayout(verticalLayout_3);


        retranslateUi(Tab3Alert);

        QMetaObject::connectSlotsByName(Tab3Alert);
    } // setupUi

    void retranslateUi(QWidget *Tab3Alert)
    {
        Tab3Alert->setWindowTitle(QCoreApplication::translate("Tab3Alert", "Form", nullptr));
        pCBEventType->setItemText(0, QCoreApplication::translate("Tab3Alert", "all", nullptr));
        pCBEventType->setItemText(1, QCoreApplication::translate("Tab3Alert", "gas", nullptr));
        pCBEventType->setItemText(2, QCoreApplication::translate("Tab3Alert", "thermal", nullptr));

        pCBLevel->setItemText(0, QCoreApplication::translate("Tab3Alert", "all", nullptr));
        pCBLevel->setItemText(1, QCoreApplication::translate("Tab3Alert", "warning", nullptr));
        pCBLevel->setItemText(2, QCoreApplication::translate("Tab3Alert", "critical", nullptr));

        pPBSearchDB->setText(QCoreApplication::translate("Tab3Alert", "\354\241\260\355\232\214", nullptr));
        QTableWidgetItem *___qtablewidgetitem = pAlertTable->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QCoreApplication::translate("Tab3Alert", "id", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = pAlertTable->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QCoreApplication::translate("Tab3Alert", "Time", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = pAlertTable->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QCoreApplication::translate("Tab3Alert", "Type", nullptr));
        QTableWidgetItem *___qtablewidgetitem3 = pAlertTable->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QCoreApplication::translate("Tab3Alert", "Level", nullptr));
        QTableWidgetItem *___qtablewidgetitem4 = pAlertTable->horizontalHeaderItem(4);
        ___qtablewidgetitem4->setText(QCoreApplication::translate("Tab3Alert", "Value", nullptr));
        QTableWidgetItem *___qtablewidgetitem5 = pAlertTable->horizontalHeaderItem(5);
        ___qtablewidgetitem5->setText(QCoreApplication::translate("Tab3Alert", "Location", nullptr));
        QTableWidgetItem *___qtablewidgetitem6 = pAlertTable->horizontalHeaderItem(6);
        ___qtablewidgetitem6->setText(QCoreApplication::translate("Tab3Alert", "Message", nullptr));
        pLSearchCount->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class Tab3Alert: public Ui_Tab3Alert {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TAB3ALERT_H
