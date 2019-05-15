#include "test_qpagedproxymodel_source_10x10.h"

#include <QSignalSpy>
#include <math.h>

Q_DECLARE_METATYPE(QModelIndex)

Test_QPagedProxyModel_Source_10x10::Test_QPagedProxyModel_Source_10x10() :
    source(10, 10) {
    for (int row = 0; row < source.rowCount(); ++row) {
        for (int column = 0; column < source.columnCount(); ++column) {
            source.setData(source.index(row, column), expectedSourceData(row, column));
        }
    }

    proxy.setSourceModel(&source);
}


int Test_QPagedProxyModel_Source_10x10::calcPageCount(int pageSize) {
    return ceil((proxy.orientation() == Qt::Horizontal ? source.columnCount() : source.rowCount()) / (float)pageSize);
}


TEST_P (Test_QPagedProxyModel_Source_10x10, RowCount) {
    EXPECT_EQ (GetParam() == Qt::Horizontal ? source.rowCount() : proxy.pageSize(), proxy.rowCount());
}


TEST_P (Test_QPagedProxyModel_Source_10x10, ColumnCount) {
    EXPECT_EQ (GetParam() == Qt::Horizontal ? proxy.pageSize() : proxy.columnCount(), proxy.columnCount());
}


TEST_P (Test_QPagedProxyModel_Source_10x10, PageCount) {
    EXPECT_EQ(4, proxy.pageCount());
}


TEST_P (Test_QPagedProxyModel_Source_10x10, SetCurrentPage) {
    for (int i = 0; i < proxy.pageCount(); ++i) {
        proxy.setCurrentPage(i);
        EXPECT_EQ (i, proxy.currentPage());
    }
}


TEST_P (Test_QPagedProxyModel_Source_10x10, SetNewPageSize_PageCountChange) {
    int pageSize = proxy.pageSize() * 2;
    QSignalSpy spy(&proxy, SIGNAL(pageCountChanged(int)));
    proxy.setPageSize(pageSize);
    int newPageCount = calcPageCount(pageSize);
    EXPECT_EQ (newPageCount, proxy.pageCount());
    EXPECT_EQ (1, spy.size());
    EXPECT_EQ (newPageCount, spy.at(0).at(0));
}


TEST_P (Test_QPagedProxyModel_Source_10x10, SetNewPageSize_LayoutChanged) {
    QSignalSpy spy(&proxy, SIGNAL(layoutChanged()));
    proxy.setPageSize(proxy.pageSize() * 2);
    EXPECT_EQ (1, spy.size());
}


TEST_P (Test_QPagedProxyModel_Source_10x10, Data) {
    for (int i = 0; i < proxy.pageCount(); ++i) {
        proxy.setCurrentPage(i);
        if (proxy.orientation() == Qt::Horizontal)
            EXPECT_EQ (proxy.index(0,0).data(), source.index(0, i * proxy.pageSize()).data());
        else
            EXPECT_EQ (proxy.index(0,0).data(), source.index(i * proxy.pageSize(), 0).data());
    }
}


TEST_P (Test_QPagedProxyModel_Source_10x10, MapFromSource_ZeroIndex) {
    EXPECT_EQ (proxy.index(0,0), proxy.mapFromSource(source.index(0, 0)));
}


TEST_P (Test_QPagedProxyModel_Source_10x10, MapFromSource_LastIndexOnPage) {
    if (proxy.orientation() == Qt::Horizontal)
        EXPECT_EQ (proxy.index(proxy.rowCount() - 1, proxy.columnCount() - 1),
                   proxy.mapFromSource(source.index(source.rowCount() - 1, proxy.pageSize() - 1)));
    else
        EXPECT_EQ (proxy.index(proxy.rowCount() - 1, proxy.columnCount() - 1),
                   proxy.mapFromSource(source.index(proxy.pageSize() - 1, source.columnCount() - 1)));
}


TEST_P (Test_QPagedProxyModel_Source_10x10, MapFromSource_IndexNotInCurrentPage) {
    EXPECT_EQ (QModelIndex(), proxy.mapFromSource(source.index(source.rowCount() - 1, source.columnCount() - 1)));
}


TEST_P (Test_QPagedProxyModel_Source_10x10, MapFromSource_IndexInCurrentPage) {
    proxy.setPageSize(3);
    proxy.setCurrentPage(1);
    EXPECT_EQ (proxy.index(0,0), proxy.mapFromSource(proxy.orientation() == Qt::Horizontal ? source.index(0, 3) : source.index(3,0)));
}


TEST_P (Test_QPagedProxyModel_Source_10x10, RemoveHalfSourceRows) {
    QSignalSpy pageCountSpy(&proxy, SIGNAL(pageCountChanged(int)));
    QSignalSpy layoutSpy(&proxy, SIGNAL(layoutChanged()));
    source.removeRows(source.rowCount() / 2, source.rowCount() / 2);
    if (proxy.orientation() == Qt::Horizontal) {
        EXPECT_EQ (source.rowCount(), proxy.rowCount());
        EXPECT_EQ (0, pageCountSpy.size());
        EXPECT_EQ (1, layoutSpy.size());
    }
    else {
        ASSERT_EQ (1, pageCountSpy.size());
        EXPECT_EQ (calcPageCount(proxy.pageSize()), proxy.pageCount());
        EXPECT_EQ (0, layoutSpy.size());
    }
}


TEST_P (Test_QPagedProxyModel_Source_10x10, RemoveHalfSourceColumns) {
    QSignalSpy pageCountSpy(&proxy, SIGNAL(pageCountChanged(int)));
    QSignalSpy layoutSpy(&proxy, SIGNAL(layoutChanged()));
    source.removeColumns(source.columnCount() / 2, source.columnCount() / 2);
    if (proxy.orientation() == Qt::Horizontal) {
        ASSERT_EQ (1, pageCountSpy.size());
        EXPECT_EQ (calcPageCount(proxy.pageSize()), proxy.pageCount());
        EXPECT_EQ (0, layoutSpy.size());
    }
    else {
        EXPECT_EQ (source.columnCount(), proxy.columnCount());
        EXPECT_EQ (0, pageCountSpy.size());
        EXPECT_EQ (1, layoutSpy.size());
    }

}


TEST_P (Test_QPagedProxyModel_Source_10x10, InsertTwiceSourceRows) {
    QSignalSpy pageCountSpy(&proxy, SIGNAL(pageCountChanged(int)));
    QSignalSpy layoutSpy(&proxy, SIGNAL(layoutChanged()));
    source.insertRows(0, source.rowCount());
    if (proxy.orientation() == Qt::Horizontal) {
        EXPECT_EQ (source.rowCount(), proxy.rowCount());
        EXPECT_EQ (0, pageCountSpy.size());
        EXPECT_EQ (1, layoutSpy.size());
    }
    else {
        ASSERT_EQ (1, pageCountSpy.size());
        EXPECT_EQ (calcPageCount(proxy.pageSize()), proxy.pageCount());
        EXPECT_EQ (0, layoutSpy.size());
    }
    EXPECT_EQ (QVariant(), proxy.index(0, 0).data());
}


TEST_P (Test_QPagedProxyModel_Source_10x10, InsertTwiceSourceColumns) {
    QSignalSpy pageCountSpy(&proxy, SIGNAL(pageCountChanged(int)));
    QSignalSpy layoutSpy(&proxy, SIGNAL(layoutChanged()));
    source.insertColumns(0, source.columnCount());
    if (proxy.orientation() == Qt::Horizontal) {
        ASSERT_EQ (1, pageCountSpy.size());
        EXPECT_EQ (calcPageCount(proxy.pageSize()), proxy.pageCount());
        EXPECT_EQ (0, layoutSpy.size());
    }
    else {
        EXPECT_EQ (source.columnCount(), proxy.columnCount());
        EXPECT_EQ (0, pageCountSpy.size());
        EXPECT_EQ (1, layoutSpy.size());
    }
    EXPECT_EQ (QVariant(), proxy.index(0, 0).data());
}


TEST_P (Test_QPagedProxyModel_Source_10x10, PageCountChangeOnOrientationSwitch) {
    if (proxy.orientation() == Qt::Horizontal)
        source.removeRows(0, source.rowCount() / 2);
    else
        source.removeColumns(0, source.columnCount() / 2);

    QSignalSpy pageCountSpy(&proxy, SIGNAL(pageCountChanged(int)));
    proxy.setOrientation(proxy.orientation() == Qt::Horizontal ? Qt::Vertical : Qt::Horizontal);
    ASSERT_EQ (1, pageCountSpy.size());
}


TEST_P (Test_QPagedProxyModel_Source_10x10, DataChangedEmitting) {
    qRegisterMetaType<QModelIndex>();
    QSignalSpy spy(&proxy, SIGNAL(dataChanged(QModelIndex,QModelIndex)));
    source.setData(source.index(0, 0), "new test data");
    ASSERT_EQ (1, spy.size());
}


TEST_P (Test_QPagedProxyModel_Source_10x10, DisconnectSignalsFromOldModel) {
    qRegisterMetaType<QModelIndex>();
    QSignalSpy spy(&proxy, SIGNAL(dataChanged(QModelIndex,QModelIndex)));
    source.setData(source.index(0, 0), "new test data");
    EXPECT_EQ (1, spy.size());
    QStandardItemModel newModel(1, 1);
    proxy.setSourceModel(&newModel);
    source.setData(source.index(0, 0), "another test data");
    EXPECT_EQ (1, spy.size());
}


INSTANTIATE_TEST_CASE_P (AllOrientations, Test_QPagedProxyModel_Source_10x10, testing::Values(Qt::Horizontal, Qt::Vertical));
