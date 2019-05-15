#ifndef TEST_QPAGEDPROXYMODEL_SOURCE_10X10_H
#define TEST_QPAGEDPROXYMODEL_SOURCE_10X10_H

#include "tests_qpagedproxymodel.h"

#include <QStandardItemModel>
#include <QStringBuilder>
struct Test_QPagedProxyModel_Source_10x10 : public Test_QPagedProxyModel {
    QStandardItemModel source;
    Test_QPagedProxyModel_Source_10x10();

    static QString expectedSourceData(int row, int column) {
        return QString("%1 - %2").arg(row).arg(column);
    }

    int calcPageCount(int pageSize);
};

#endif // TEST_QPAGEDPROXYMODEL_SOURCE_10X10_H
