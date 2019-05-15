#include "tests_qpagedproxymodel.h"

#include <QStandardItemModel>
#include <QSignalSpy>


Test_QPagedProxyModel::Test_QPagedProxyModel() :
    proxy(GetParam()) {
}


TEST_P (Test_QPagedProxyModel, Orientation) {
    EXPECT_EQ (GetParam(), proxy.orientation());
}


TEST_P (Test_QPagedProxyModel, RowCountWithoutSource) {
    EXPECT_EQ (GetParam() == Qt::Horizontal ? 0 : proxy.pageSize(), proxy.rowCount());
}


TEST_P (Test_QPagedProxyModel, ColumnCountWithoutSource) {
    EXPECT_EQ (GetParam() == Qt::Horizontal ? proxy.pageSize() : 0, proxy.columnCount());
}


TEST_P (Test_QPagedProxyModel, PageSize) {
    EXPECT_EQ (QPagedProxyModel::defaultPageSize(), proxy.pageSize());
    int newPageSize = 5;
    proxy.setPageSize(newPageSize);
    EXPECT_EQ (newPageSize, proxy.pageSize());
}


TEST_P (Test_QPagedProxyModel, PageSizeChangedSignal) {
    int pageSize = proxy.pageSize() + 1;
    QSignalSpy spy(&proxy, SIGNAL(pageSizeChanged(int)));
    proxy.setPageSize(pageSize);
    proxy.setPageSize(pageSize);
    EXPECT_EQ (1, spy.size());
    EXPECT_EQ (pageSize, spy.at(0).at(0).toInt());
}


TEST_P (Test_QPagedProxyModel, SetNegativePageSize) {
    int pageSize = proxy.pageSize() + 1;
    proxy.setPageSize(pageSize);
    EXPECT_EQ (pageSize, proxy.pageSize());
    proxy.setPageSize(-5);
    EXPECT_EQ (proxy.defaultPageSize(), proxy.pageSize());
}


TEST_P (Test_QPagedProxyModel, ResetPageSize) {
    int pageSize = proxy.pageSize() + 1;
    proxy.setPageSize(pageSize);
    ASSERT_EQ (pageSize, proxy.pageSize());
    proxy.resetPageSize();
    EXPECT_EQ (proxy.defaultPageSize(), proxy.pageSize());
}


TEST_P (Test_QPagedProxyModel, CurrentPage) {
    EXPECT_EQ (0, proxy.currentPage());
}


TEST_P (Test_QPagedProxyModel, PageCount) {
    EXPECT_EQ (1, proxy.pageCount());
}


TEST_P (Test_QPagedProxyModel, Parent) {
    EXPECT_EQ (QModelIndex(), proxy.parent(proxy.index(0,0)));
}


TEST_P (Test_QPagedProxyModel, MappingToSource_WithoutSource) {
    EXPECT_EQ (QModelIndex(), proxy.mapToSource(proxy.index(0, 0)));
}


TEST_P (Test_QPagedProxyModel, MappingFromSource_WithoutSource) {
    QStandardItemModel model;
    EXPECT_EQ (QModelIndex(), proxy.mapFromSource(model.index(0, 0)));
}


TEST_P (Test_QPagedProxyModel, ChangeOrientation) {
    Qt::Orientation newOrientation = proxy.orientation() == Qt::Horizontal ? Qt::Vertical : Qt::Horizontal;
    QSignalSpy spy(&proxy, SIGNAL(orientationChanged(Qt::Orientation)));
    QSignalSpy layoutSpy(&proxy, SIGNAL(layoutChanged()));
    proxy.setOrientation(newOrientation);
    EXPECT_EQ (newOrientation, proxy.orientation());
    ASSERT_EQ (1, spy.size());
    EXPECT_EQ (newOrientation, spy.at(0).at(0).value<Qt::Orientation>());
    EXPECT_LT (0, layoutSpy.size());
}


TEST_P (Test_QPagedProxyModel, SetSameOrientation_OrientatiobChangedNotEmited) {
    QSignalSpy spy(&proxy, SIGNAL(orientationChanged(Qt::Orientation)));
    proxy.setOrientation(proxy.orientation());
    ASSERT_EQ (0, spy.size());
}

TEST (Test_QPagedProxyModel, SetEmptySource) {
    QPagedProxyModel proxy(Qt::Horizontal);
    proxy.setSourceModel(0);
}


INSTANTIATE_TEST_CASE_P (AllOrientations, Test_QPagedProxyModel, testing::Values(Qt::Horizontal, Qt::Vertical));
