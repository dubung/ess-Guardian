/********************************************************************************
** Form generated from reading UI file 'tab2env.ui'
**
** Created by: Qt User Interface Compiler version 6.8.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TAB2ENV_H
#define UI_TAB2ENV_H

#include <QtCore/QDate>
#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDateTimeEdit>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Tab2Env
{
public:
    QVBoxLayout *verticalLayout_3;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout;
    QDateTimeEdit *pDateTimeFrom;
    QDateTimeEdit *pDateTimeTo;
    QSpacerItem *horizontalSpacer;
    QPushButton *pPBSearchDB;
    QPushButton *pPBDeleteDB;
    QHBoxLayout *horizontalLayout_2;
    QVBoxLayout *verticalLayout;
    QTableWidget *pEnvTable;
    QVBoxLayout *pChartViewLayout;

    void setupUi(QWidget *Tab2Env)
    {
        if (Tab2Env->objectName().isEmpty())
            Tab2Env->setObjectName("Tab2Env");
        Tab2Env->resize(686, 456);
        verticalLayout_3 = new QVBoxLayout(Tab2Env);
        verticalLayout_3->setObjectName("verticalLayout_3");
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName("verticalLayout_2");
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        pDateTimeFrom = new QDateTimeEdit(Tab2Env);
        pDateTimeFrom->setObjectName("pDateTimeFrom");
        pDateTimeFrom->setDateTime(QDateTime(QDate(2025, 12, 1), QTime(0, 0, 0)));
        pDateTimeFrom->setMinimumDate(QDate(2025, 12, 1));
        pDateTimeFrom->setDisplayFormat(QString::fromUtf8("yyyy-MM-dd hh:mm"));

        horizontalLayout->addWidget(pDateTimeFrom);

        pDateTimeTo = new QDateTimeEdit(Tab2Env);
        pDateTimeTo->setObjectName("pDateTimeTo");
        pDateTimeTo->setMinimumDate(QDate(2025, 12, 1));
        pDateTimeTo->setDisplayFormat(QString::fromUtf8("yyyy-MM-dd hh:mm"));

        horizontalLayout->addWidget(pDateTimeTo);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        pPBSearchDB = new QPushButton(Tab2Env);
        pPBSearchDB->setObjectName("pPBSearchDB");

        horizontalLayout->addWidget(pPBSearchDB);

        pPBDeleteDB = new QPushButton(Tab2Env);
        pPBDeleteDB->setObjectName("pPBDeleteDB");

        horizontalLayout->addWidget(pPBDeleteDB);

        horizontalLayout->setStretch(0, 2);
        horizontalLayout->setStretch(1, 2);
        horizontalLayout->setStretch(3, 1);
        horizontalLayout->setStretch(4, 1);

        verticalLayout_2->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName("verticalLayout");
        pEnvTable = new QTableWidget(Tab2Env);
        if (pEnvTable->columnCount() < 4)
            pEnvTable->setColumnCount(4);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        pEnvTable->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        pEnvTable->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        pEnvTable->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        pEnvTable->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        pEnvTable->setObjectName("pEnvTable");

        verticalLayout->addWidget(pEnvTable);


        horizontalLayout_2->addLayout(verticalLayout);

        pChartViewLayout = new QVBoxLayout();
        pChartViewLayout->setObjectName("pChartViewLayout");

        horizontalLayout_2->addLayout(pChartViewLayout);

        horizontalLayout_2->setStretch(0, 3);
        horizontalLayout_2->setStretch(1, 4);

        verticalLayout_2->addLayout(horizontalLayout_2);


        verticalLayout_3->addLayout(verticalLayout_2);


        retranslateUi(Tab2Env);

        QMetaObject::connectSlotsByName(Tab2Env);
    } // setupUi

    void retranslateUi(QWidget *Tab2Env)
    {
        Tab2Env->setWindowTitle(QCoreApplication::translate("Tab2Env", "Form", nullptr));
        pPBSearchDB->setText(QCoreApplication::translate("Tab2Env", "\354\241\260\355\232\214", nullptr));
        pPBDeleteDB->setText(QCoreApplication::translate("Tab2Env", "\354\202\255\354\240\234", nullptr));
        QTableWidgetItem *___qtablewidgetitem = pEnvTable->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QCoreApplication::translate("Tab2Env", "id", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = pEnvTable->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QCoreApplication::translate("Tab2Env", "Time", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = pEnvTable->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QCoreApplication::translate("Tab2Env", "Temp", nullptr));
        QTableWidgetItem *___qtablewidgetitem3 = pEnvTable->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QCoreApplication::translate("Tab2Env", "Humi", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Tab2Env: public Ui_Tab2Env {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TAB2ENV_H
