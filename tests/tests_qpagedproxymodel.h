#ifndef TESTS_QPAGEDPROXYMODEL_H
#define TESTS_QPAGEDPROXYMODEL_H

#include <gtest/gtest.h>

#include "../src/qpagedproxymodel.h"

struct Test_QPagedProxyModel : public testing::TestWithParam<Qt::Orientation> {
    QPagedProxyModel proxy;
    Test_QPagedProxyModel();
};

#endif // TESTS_QPAGEDPROXYMODEL_H
