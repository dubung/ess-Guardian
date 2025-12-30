/********************************************************************************
** Form generated from reading UI file 'tab4access.ui'
**
** Created by: Qt User Interface Compiler version 6.8.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TAB4ACCESS_H
#define UI_TAB4ACCESS_H

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

class Ui_Tab4Access
{
public:
    QVBoxLayout *verticalLayout_3;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout;
    QDateTimeEdit *pDateTimeFrom;
    QDateTimeEdit *pDateTimeTo;
    QComboBox *pCBAccessPoints;
    QSpacerItem *horizontalSpacer;
    QPushButton *pPBSearchDB;
    QVBoxLayout *verticalLayout;
    QTableWidget *pAccessTable;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer_2;
    QLabel *pLSearchCount;

    void setupUi(QWidget *Tab4Access)
    {
        if (Tab4Access->objectName().isEmpty())
            Tab4Access->setObjectName("Tab4Access");
        Tab4Access->resize(615, 439);
        verticalLayout_3 = new QVBoxLayout(Tab4Access);
        verticalLayout_3->setObjectName("verticalLayout_3");
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName("verticalLayout_2");
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        pDateTimeFrom = new QDateTimeEdit(Tab4Access);
        pDateTimeFrom->setObjectName("pDateTimeFrom");
        pDateTimeFrom->setMinimumDate(QDate(2025, 12, 1));
        pDateTimeFrom->setDisplayFormat(QString::fromUtf8("yyyy-MM-dd hh:mm"));

        horizontalLayout->addWidget(pDateTimeFrom);

        pDateTimeTo = new QDateTimeEdit(Tab4Access);
        pDateTimeTo->setObjectName("pDateTimeTo");
        pDateTimeTo->setMinimumDate(QDate(2025, 12, 1));
        pDateTimeTo->setDisplayFormat(QString::fromUtf8("yyyy-MM-dd hh:mm"));

        horizontalLayout->addWidget(pDateTimeTo);

        pCBAccessPoints = new QComboBox(Tab4Access);
        pCBAccessPoints->addItem(QString());
        pCBAccessPoints->addItem(QString());
        pCBAccessPoints->addItem(QString());
        pCBAccessPoints->addItem(QString());
        pCBAccessPoints->addItem(QString());
        pCBAccessPoints->addItem(QString());
        pCBAccessPoints->addItem(QString());
        pCBAccessPoints->addItem(QString());
        pCBAccessPoints->setObjectName("pCBAccessPoints");

        horizontalLayout->addWidget(pCBAccessPoints);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        pPBSearchDB = new QPushButton(Tab4Access);
        pPBSearchDB->setObjectName("pPBSearchDB");

        horizontalLayout->addWidget(pPBSearchDB);

        horizontalLayout->setStretch(0, 3);
        horizontalLayout->setStretch(1, 3);
        horizontalLayout->setStretch(2, 2);
        horizontalLayout->setStretch(4, 1);

        verticalLayout_2->addLayout(horizontalLayout);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName("verticalLayout");
        pAccessTable = new QTableWidget(Tab4Access);
        if (pAccessTable->columnCount() < 5)
            pAccessTable->setColumnCount(5);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        pAccessTable->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        pAccessTable->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        pAccessTable->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        pAccessTable->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        pAccessTable->setHorizontalHeaderItem(4, __qtablewidgetitem4);
        pAccessTable->setObjectName("pAccessTable");

        verticalLayout->addWidget(pAccessTable);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);

        pLSearchCount = new QLabel(Tab4Access);
        pLSearchCount->setObjectName("pLSearchCount");

        horizontalLayout_2->addWidget(pLSearchCount);

        horizontalLayout_2->setStretch(0, 7);
        horizontalLayout_2->setStretch(1, 2);

        verticalLayout->addLayout(horizontalLayout_2);


        verticalLayout_2->addLayout(verticalLayout);

        verticalLayout_2->setStretch(0, 1);
        verticalLayout_2->setStretch(1, 7);

        verticalLayout_3->addLayout(verticalLayout_2);


        retranslateUi(Tab4Access);

        QMetaObject::connectSlotsByName(Tab4Access);
    } // setupUi

    void retranslateUi(QWidget *Tab4Access)
    {
        Tab4Access->setWindowTitle(QCoreApplication::translate("Tab4Access", "Form", nullptr));
        pCBAccessPoints->setItemText(0, QCoreApplication::translate("Tab4Access", "all", nullptr));
        pCBAccessPoints->setItemText(1, QCoreApplication::translate("Tab4Access", "main", nullptr));
        pCBAccessPoints->setItemText(2, QCoreApplication::translate("Tab4Access", "ew1", nullptr));
        pCBAccessPoints->setItemText(3, QCoreApplication::translate("Tab4Access", "ew2", nullptr));
        pCBAccessPoints->setItemText(4, QCoreApplication::translate("Tab4Access", "ww1", nullptr));
        pCBAccessPoints->setItemText(5, QCoreApplication::translate("Tab4Access", "sw1", nullptr));
        pCBAccessPoints->setItemText(6, QCoreApplication::translate("Tab4Access", "sw2", nullptr));
        pCBAccessPoints->setItemText(7, QCoreApplication::translate("Tab4Access", "nw1", nullptr));

        pPBSearchDB->setText(QCoreApplication::translate("Tab4Access", "\354\241\260\355\232\214", nullptr));
        QTableWidgetItem *___qtablewidgetitem = pAccessTable->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QCoreApplication::translate("Tab4Access", "id", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = pAccessTable->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QCoreApplication::translate("Tab4Access", "Admin id", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = pAccessTable->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QCoreApplication::translate("Tab4Access", "Access point", nullptr));
        QTableWidgetItem *___qtablewidgetitem3 = pAccessTable->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QCoreApplication::translate("Tab4Access", "Result", nullptr));
        QTableWidgetItem *___qtablewidgetitem4 = pAccessTable->horizontalHeaderItem(4);
        ___qtablewidgetitem4->setText(QCoreApplication::translate("Tab4Access", "Time", nullptr));
        pLSearchCount->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class Tab4Access: public Ui_Tab4Access {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TAB4ACCESS_H
