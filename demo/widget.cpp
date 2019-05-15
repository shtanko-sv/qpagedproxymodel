#include "widget.h"

#include <QComboBox>
#include <QGridLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QSpinBox>
#include <QStandardItemModel>
#include <QTableView>
#include <QVBoxLayout>

#include "../src/qpagedproxymodel.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    source(new QStandardItemModel(10, 10, this)),
    proxy(new QPagedProxyModel(Qt::Horizontal, this)),
    view(new QTableView(this))
{
    for (int row = 0; row < source->rowCount(); ++row) {
        for (int column = 0; column < source->columnCount(); ++column) {
            source->setData(source->index(row, column), QString("%1, %2").arg(row).arg(column));
        }
    }


    proxy->setSourceModel(source);

    view->setModel(proxy);
#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
    view->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    view->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
#else
    view->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    view->verticalHeader()->setResizeMode(QHeaderView::Stretch);
#endif

    QGroupBox * sourceConfig = new QGroupBox(tr("Source model"), this);
    QGridLayout * sourceConfigLayout = new QGridLayout(sourceConfig);
    sourceConfigLayout->addWidget(new QLabel(tr("Row count"), sourceConfig), sourceConfigLayout->rowCount(), 0);
    sourceConfigLayout->addWidget(sourceRowCount = new QSpinBox(sourceConfig), sourceConfigLayout->rowCount() - 1, 1);
    sourceConfigLayout->addWidget(new QLabel(tr("Column count"), sourceConfig), sourceConfigLayout->rowCount(), 0);
    sourceConfigLayout->addWidget(sourceColumnCount = new QSpinBox(sourceConfig), sourceConfigLayout->rowCount() - 1, 1);

    sourceRowCount->setValue(source->rowCount());
    sourceColumnCount->setValue(source->columnCount());

    QGroupBox * proxyConfig = new QGroupBox(tr("Proxy model"), this);
    QGridLayout * proxyConfigLayout = new QGridLayout(proxyConfig);
    proxyConfigLayout->addWidget(new QLabel(tr("Orientation"), proxyConfig), proxyConfigLayout->rowCount(), 0);
    proxyConfigLayout->addWidget(orientation = new QComboBox(proxyConfig), proxyConfigLayout->rowCount() - 1, 1);
    proxyConfigLayout->addWidget(new QLabel(tr("Page size"), proxyConfig), proxyConfigLayout->rowCount(), 0);
    proxyConfigLayout->addWidget(pageSize = new QSpinBox(proxyConfig), proxyConfigLayout->rowCount() - 1, 1);

    orientation->addItem(tr("Horizontal"), Qt::Horizontal);
    orientation->addItem(tr("Vertical"), Qt::Vertical);

    pageSize->setValue(proxy->pageSize());

    QVBoxLayout * configLayout = new QVBoxLayout;
    configLayout->addWidget(sourceConfig);
    configLayout->addWidget(proxyConfig);

    QHBoxLayout * navigationLayout = new QHBoxLayout;
    navigationLayout->addWidget(new QLabel(tr("Page"), this));
    navigationLayout->addWidget(currentPage = new QSpinBox(this));
    navigationLayout->addWidget(new QLabel(tr("from"), this));
    navigationLayout->addWidget(pageCount = new QSpinBox(this));

    currentPage->setValue(proxy->currentPage());
    currentPage->setMaximum(proxy->pageCount() - 1);
    pageCount->setValue(proxy->pageCount());
    pageCount->setReadOnly(true);


    QVBoxLayout * viewLayout = new QVBoxLayout;
    viewLayout->addWidget(view);
    viewLayout->addLayout(navigationLayout);

    QHBoxLayout * hLayout = new QHBoxLayout(this);
    hLayout->addLayout(configLayout);
    hLayout->addLayout(viewLayout);


    connect(sourceRowCount, SIGNAL(valueChanged(int)), this, SLOT(setSourceRowCount(int)));
    connect(sourceColumnCount, SIGNAL(valueChanged(int)), this, SLOT(setSourceColumnCount(int)));

    connect(proxy, SIGNAL(currentPageChanged(int)), currentPage, SLOT(setValue(int)));
    connect(currentPage, SIGNAL(valueChanged(int)), proxy, SLOT(setCurrentPage(int)));

    connect(proxy, SIGNAL(pageCountChanged(int)), pageCount, SLOT(setValue(int)));
    connect(proxy, SIGNAL(pageCountChanged(int)), this, SLOT(limitCurrentPage(int)));

    connect(orientation, SIGNAL(currentIndexChanged(int)), this, SLOT(changeOrientation(int)));

    connect(pageSize, SIGNAL(valueChanged(int)), proxy, SLOT(setPageSize(int)));
    connect(proxy, SIGNAL(pageSizeChanged(int)), pageSize, SLOT(setValue(int)));

    connect(proxy, SIGNAL(layoutChanged()), view->horizontalHeader(), SLOT(resizeSections()));
    connect(proxy, SIGNAL(layoutChanged()), view->verticalHeader(), SLOT(resizeSections()));
}


void Widget::setSourceRowCount(int count) {
    source->setRowCount(count);
}


void Widget::setSourceColumnCount(int count) {
    source->setColumnCount(count);
}


void Widget::limitCurrentPage(int count) {
    currentPage->setMaximum(count - 1);
}


void Widget::changeOrientation(int index) {
    proxy->setOrientation(static_cast<Qt::Orientation>(orientation->itemData(index).toInt()));
}

