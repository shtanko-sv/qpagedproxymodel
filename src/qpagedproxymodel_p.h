#ifndef QPAGEDPROXYMODEL_P_H
#define QPAGEDPROXYMODEL_P_H

#include "qpagedproxymodel.h"

struct QPagedProxyModelPrivate {
    Qt::Orientation orientation;
    int pageSize;
    int currentPage;
    int pageCountBeforeSourceChange;

    QPagedProxyModelPrivate() :
        orientation(Qt::Horizontal),
        pageSize(QPagedProxyModel::defaultPageSize()),
        currentPage(0),
        pageCountBeforeSourceChange(0) {
    }
};

#endif // QPAGEDPROXYMODEL_P_H
