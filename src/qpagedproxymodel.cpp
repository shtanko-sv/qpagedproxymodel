#include "qpagedproxymodel.h"
#include "qpagedproxymodel_p.h"

#include <QtCore/qmath.h>
#include <QDebug>

QPagedProxyModel::QPagedProxyModel(Qt::Orientation orientation, QObject *parent) :
    QAbstractProxyModel(parent),
    d_ptr (new QPagedProxyModelPrivate){

    d_ptr->orientation = orientation;

    qRegisterMetaType<Qt::Orientation>();
}


void QPagedProxyModel::setSourceModel(QAbstractItemModel *model) {
    beginResetModel();

    if (QAbstractProxyModel::sourceModel()) {
        disconnect(sourceModel(), SIGNAL(dataChanged(QModelIndex,QModelIndex)), this, SLOT(sourceDataChanged(QModelIndex,QModelIndex)));
        disconnect(sourceModel(), SIGNAL(headerDataChanged(Qt::Orientation,int,int)), this, SLOT(sourceHeaderDataChanged(Qt::Orientation,int,int)));
        disconnect(sourceModel(), SIGNAL(rowsAboutToBeInserted(QModelIndex,int,int)), this, SLOT(sourceRowsAboutToBeInserted(QModelIndex,int,int)));
        disconnect(sourceModel(), SIGNAL(rowsInserted(QModelIndex,int,int)), this, SLOT(sourceRowsInserted(QModelIndex,int,int)));
        disconnect(sourceModel(), SIGNAL(columnsAboutToBeInserted(QModelIndex,int,int)), this, SLOT(sourceColumnsAboutToBeInserted(QModelIndex,int,int)));
        disconnect(sourceModel(), SIGNAL(columnsInserted(QModelIndex,int,int)), this, SLOT(sourceColumnsInserted(QModelIndex,int,int)));
        disconnect(sourceModel(), SIGNAL(rowsAboutToBeRemoved(QModelIndex,int,int)), this, SLOT(sourceRowsAboutToBeRemoved(QModelIndex,int,int)));
        disconnect(sourceModel(), SIGNAL(rowsRemoved(QModelIndex,int,int)), this, SLOT(sourceRowsRemoved(QModelIndex,int,int)));
        disconnect(sourceModel(), SIGNAL(columnsAboutToBeRemoved(QModelIndex,int,int)), this, SLOT(sourceColumnsAboutToBeRemoved(QModelIndex,int,int)));
        disconnect(sourceModel(), SIGNAL(columnsRemoved(QModelIndex,int,int)), this, SLOT(sourceColumnsRemoved(QModelIndex,int,int)));
        disconnect(sourceModel(), SIGNAL(layoutAboutToBeChanged()), this, SLOT(sourceLayoutAboutToBeChanged()));
        disconnect(sourceModel(), SIGNAL(layoutChanged()), this, SLOT(sourceLayoutChanged()));
        disconnect(sourceModel(), SIGNAL(modelAboutToBeReset()), this, SLOT(sourceAboutToBeReset()));
        disconnect(sourceModel(), SIGNAL(modelReset()), this, SLOT(sourceReset()));
    }

    QAbstractProxyModel::setSourceModel(model);

    if (model) {
        connect(sourceModel(), SIGNAL(dataChanged(QModelIndex,QModelIndex)), this, SLOT(sourceDataChanged(QModelIndex,QModelIndex)));
        connect(sourceModel(), SIGNAL(headerDataChanged(Qt::Orientation,int,int)), this, SLOT(sourceHeaderDataChanged(Qt::Orientation,int,int)));
        connect(sourceModel(), SIGNAL(rowsAboutToBeInserted(QModelIndex,int,int)), this, SLOT(sourceRowsAboutToBeInserted(QModelIndex,int,int)));
        connect(sourceModel(), SIGNAL(rowsInserted(QModelIndex,int,int)), this, SLOT(sourceRowsInserted(QModelIndex,int,int)));
        connect(sourceModel(), SIGNAL(columnsAboutToBeInserted(QModelIndex,int,int)), this, SLOT(sourceColumnsAboutToBeInserted(QModelIndex,int,int)));
        connect(sourceModel(), SIGNAL(columnsInserted(QModelIndex,int,int)), this, SLOT(sourceColumnsInserted(QModelIndex,int,int)));
        connect(sourceModel(), SIGNAL(rowsAboutToBeRemoved(QModelIndex,int,int)), this, SLOT(sourceRowsAboutToBeRemoved(QModelIndex,int,int)));
        connect(sourceModel(), SIGNAL(rowsRemoved(QModelIndex,int,int)), this, SLOT(sourceRowsRemoved(QModelIndex,int,int)));
        connect(sourceModel(), SIGNAL(columnsAboutToBeRemoved(QModelIndex,int,int)), this, SLOT(sourceColumnsAboutToBeRemoved(QModelIndex,int,int)));
        connect(sourceModel(), SIGNAL(columnsRemoved(QModelIndex,int,int)), this, SLOT(sourceColumnsRemoved(QModelIndex,int,int)));
        connect(sourceModel(), SIGNAL(layoutAboutToBeChanged()), this, SLOT(sourceLayoutAboutToBeChanged()));
        connect(sourceModel(), SIGNAL(layoutChanged()), this, SLOT(sourceLayoutChanged()));
        connect(sourceModel(), SIGNAL(modelAboutToBeReset()), this, SLOT(sourceAboutToBeReset()));
        connect(sourceModel(), SIGNAL(modelReset()), this, SLOT(sourceReset()));
    }

    endResetModel();
}


Qt::Orientation QPagedProxyModel::orientation() const {
    Q_D (const QPagedProxyModel);
    return d->orientation;
}


int QPagedProxyModel::pageSize() const {
    Q_D (const QPagedProxyModel);
    return d->pageSize;
}


/*!
 * \brief QPagedProxyModel::setPageSize устанавливает размер страницы в \a page.
 *
 * Если \a page является отрицательным значением, то размер страницы будет сброшен
 * в значени по умолчанию.
 */
void QPagedProxyModel::setPageSize(int size) {
    Q_D (QPagedProxyModel);
    if (d->pageSize == size)
        return;

    int currentPageCount = pageCount();
    emit layoutAboutToBeChanged();
    d->pageSize = size > 0 ? size : defaultPageSize();
    emit layoutChanged();
    emit pageSizeChanged(d->pageSize);
    if (currentPageCount != pageCount())
        emit pageCountChanged(pageCount());
}


int QPagedProxyModel::defaultPageSize() {
    return 3;
}


void QPagedProxyModel::resetPageSize() {
    setPageSize(defaultPageSize());
}


int QPagedProxyModel::currentPage() const {
    Q_D (const QPagedProxyModel);
    return d->currentPage;
}


void QPagedProxyModel::setCurrentPage(int page) {
    Q_D (QPagedProxyModel);
    if (d->currentPage == page || page < 0 || page >= pageCount())
        return;

    d->currentPage = page;
    emitAllChanged();
    emit currentPageChanged(page);
}


int QPagedProxyModel::pageCount() const {
    if (!sourceModel())
        return 1;

    return (orientation() == Qt::Horizontal) ?
                qMax(1, qCeil (sourceModel()->columnCount() / (float)pageSize())) :
                qMax(1, qCeil (sourceModel()->rowCount() / (float)pageSize()));
}


QModelIndex QPagedProxyModel::index(int row, int column, const QModelIndex &parent) const {
    Q_UNUSED (parent)
    return createIndex(row, column);
}


QModelIndex QPagedProxyModel::parent(const QModelIndex &child) const {
    Q_UNUSED (child)
    return QModelIndex();
}


int QPagedProxyModel::rowCount(const QModelIndex &parent) const {
    if (orientation() == Qt::Vertical)
        return pageSize();

    return sourceModel() ? sourceModel()->rowCount(mapToSource(parent)) : 0;
}


int QPagedProxyModel::columnCount(const QModelIndex &parent) const {
    if (orientation() == Qt::Horizontal)
        return pageSize();

    return sourceModel() ? sourceModel()->columnCount(mapToSource(parent)) : 0;
}


QModelIndex QPagedProxyModel::mapToSource(const QModelIndex &proxyIndex) const {
    if (!sourceModel())
        return QModelIndex();

    if (orientation() == Qt::Horizontal)
        return sourceModel()->index(proxyIndex.row(), currentPage() * pageSize() + proxyIndex.column());
    else
        return sourceModel()->index(currentPage() * pageSize() + proxyIndex.row(), proxyIndex.column());
}


QModelIndex QPagedProxyModel::mapFromSource(const QModelIndex &sourceIndex) const {
    if (!sourceModel() || sourceIndex.model() != sourceModel())
        return QModelIndex();

    if (orientation() == Qt::Horizontal)
        return sourceIndex.column() / pageSize() == currentPage() ?
                    index(sourceIndex.row(), sourceIndex.column() % pageSize()) :
                    QModelIndex();
    else
        return sourceIndex.row() / pageSize() == currentPage() ?
                    index(sourceIndex.row() % pageSize(), sourceIndex.column()) :
                    QModelIndex();
}


void QPagedProxyModel::setOrientation(Qt::Orientation orient) {
    Q_D(QPagedProxyModel);
    if (d->orientation == orient)
        return;

    int currentPageCount = pageCount();

    emit layoutAboutToBeChanged();
    d->orientation = orient;
    emit layoutChanged();
    emit orientationChanged(d->orientation);

    if (currentPageCount != pageCount())
        emit pageCountChanged(pageCount());

    setCurrentPage(0);
}


void QPagedProxyModel::notifyAfterSourceItemsCountChanged() {
    Q_D(QPagedProxyModel);
    if (d->pageCountBeforeSourceChange != pageCount())
        emit pageCountChanged(pageCount());

    /// \todo Need optimise this. This should be emited only then real need
    emitAllChanged();
}


void QPagedProxyModel::emitAllChanged() {
    emit dataChanged(index(0,0), index(rowCount() - 1, columnCount() - 1));
    emit headerDataChanged(orientation(), 0, orientation() == Qt::Horizontal ? columnCount() - 1 : rowCount());
}

void QPagedProxyModel::sourceRowCountWillBeChanged() {
    Q_D(QPagedProxyModel);
    if (orientation() == Qt::Vertical)
        d->pageCountBeforeSourceChange = pageCount();
}


void QPagedProxyModel::sourceColumnCountWillBeChanged() {
    Q_D(QPagedProxyModel);
    if (orientation() == Qt::Horizontal)
        d->pageCountBeforeSourceChange = pageCount();
}


void QPagedProxyModel::sourceRowsAboutToBeRemoved(const QModelIndex& parent, int first, int last) {
    Q_UNUSED(parent)
    Q_UNUSED(first)
    Q_UNUSED(last)

    sourceRowCountWillBeChanged();
}


void QPagedProxyModel::sourceRowsRemoved(const QModelIndex& parent, int first, int last) {
    Q_UNUSED(parent)
    Q_UNUSED(first)
    Q_UNUSED(last)

    if (orientation() == Qt::Vertical) {
        notifyAfterSourceItemsCountChanged();
        return;
    }

    emit layoutAboutToBeChanged();
    emit layoutChanged();
}


void QPagedProxyModel::sourceRowsAboutToBeInserted(const QModelIndex &parent, int first, int last) {
    Q_UNUSED(parent)
    Q_UNUSED(first)
    Q_UNUSED(last)

    sourceRowCountWillBeChanged();
}


void QPagedProxyModel::sourceRowsInserted(const QModelIndex &parent, int first, int last) {
    Q_UNUSED(parent)
    Q_UNUSED(first)
    Q_UNUSED(last)

    if (orientation() == Qt::Vertical) {
        notifyAfterSourceItemsCountChanged();
        return;
    }

    emit layoutAboutToBeChanged();
    emit layoutChanged();
}


void QPagedProxyModel::sourceColumnsAboutToBeRemoved(const QModelIndex &parent, int first, int last) {
    Q_UNUSED(parent)
    Q_UNUSED(first)
    Q_UNUSED(last)

    sourceColumnCountWillBeChanged();
}


void QPagedProxyModel::sourceColumnsRemoved(const QModelIndex &parent, int first, int last) {
    Q_UNUSED(parent)
    Q_UNUSED(first)
    Q_UNUSED(last)

    if (orientation() == Qt::Horizontal) {
        notifyAfterSourceItemsCountChanged();
        return;
    }

    emit layoutAboutToBeChanged();
    emit layoutChanged();
}


void QPagedProxyModel::sourceColumnsAboutToBeInserted(const QModelIndex &parent, int first, int last) {
    Q_UNUSED(parent)
    Q_UNUSED(first)
    Q_UNUSED(last)

    sourceColumnCountWillBeChanged();
}


void QPagedProxyModel::sourceColumnsInserted(const QModelIndex &parent, int first, int last) {
    Q_UNUSED(parent)
    Q_UNUSED(first)
    Q_UNUSED(last)

    if (orientation() == Qt::Horizontal) {
        notifyAfterSourceItemsCountChanged();
        return;
    }

    emit layoutAboutToBeChanged();
    emit layoutChanged();
}


void QPagedProxyModel::sourceDataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight) {
    emit dataChanged(mapFromSource(topLeft), mapFromSource(bottomRight));
}


void QPagedProxyModel::sourceHeaderDataChanged(Qt::Orientation orientation, int first, int last) {
    if (this->orientation() == orientation) {
        QVector<int> changes;
        changes.reserve(last - first + 1);
        if (first > last)
            qSwap(first, last);

        for (int i = first; i <= last; ++i) {
            if (first / pageSize() == currentPage())
                changes.push_back(i);
        }

        if (!changes.isEmpty())
            emit headerDataChanged(orientation, changes.first(), changes.last());
    }
    else
        emit headerDataChanged(orientation, first, last);
}


void QPagedProxyModel::sourceLayoutAboutToBeChanged() {
    emit layoutAboutToBeChanged();
}


void QPagedProxyModel::sourceLayoutChanged() {
    emit layoutChanged();
}


void QPagedProxyModel::sourceAboutToBeReset() {
    beginResetModel();
}


void QPagedProxyModel::sourceReset() {
    endResetModel();
}
