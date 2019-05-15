#ifndef QPAGEDPROXYMODEL_H
#define QPAGEDPROXYMODEL_H

#include <QAbstractProxyModel>

class QPagedProxyModelPrivate;

class QPagedProxyModel : public QAbstractProxyModel {

    Q_OBJECT
    Q_PROPERTY (Qt::Orientation orientation READ orientation WRITE setOrientation NOTIFY orientationChanged)
    Q_PROPERTY (int pageSize READ pageSize WRITE setPageSize NOTIFY pageSizeChanged RESET resetPageSize)
    Q_PROPERTY (int currentPage READ currentPage WRITE setCurrentPage NOTIFY currentPageChanged)
    Q_PROPERTY (int pageCount READ pageCount NOTIFY pageCountChanged STORED false)
    Q_DECLARE_PRIVATE(QPagedProxyModel)

public:
    QPagedProxyModel(Qt::Orientation orientation, QObject * parent = 0);
    void setSourceModel(QAbstractItemModel *sourceModel);

    Qt::Orientation orientation() const;

    int pageSize() const;
    static int defaultPageSize();

    int currentPage() const;
    int pageCount() const;

    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex &child) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;

    QModelIndex mapToSource(const QModelIndex &proxyIndex) const;
    QModelIndex mapFromSource(const QModelIndex &sourceIndex) const;

public slots:
    void setOrientation(Qt::Orientation orient);

    void setPageSize(int size);
    void resetPageSize();

    void setCurrentPage(int page);

signals:
    void pageSizeChanged(int);
    void currentPageChanged(int);
    void pageCountChanged(int);
    void orientationChanged(Qt::Orientation);

protected:
    QPagedProxyModelPrivate * const d_ptr;

private:
    void notifyAfterSourceItemsCountChanged();
    void emitAllChanged();

    void sourceRowCountWillBeChanged();
    void sourceColumnCountWillBeChanged();

private slots:
    void sourceRowsAboutToBeRemoved(const QModelIndex& parent, int first, int last);
    void sourceRowsRemoved(const QModelIndex& parent, int first, int last);
    void sourceRowsAboutToBeInserted(const QModelIndex& parent, int first, int last);
    void sourceRowsInserted(const QModelIndex& parent, int first, int last);

    void sourceColumnsAboutToBeRemoved(const QModelIndex& parent, int first, int last);
    void sourceColumnsRemoved(const QModelIndex& parent, int first, int last);
    void sourceColumnsAboutToBeInserted(const QModelIndex& parent, int first, int last);
    void sourceColumnsInserted(const QModelIndex& parent, int first, int last);
    void sourceDataChanged(const QModelIndex& topLeft, const QModelIndex& bottomRight);
    void sourceHeaderDataChanged(Qt::Orientation orientation, int first, int last);
    void sourceLayoutAboutToBeChanged();
    void sourceLayoutChanged();

    void sourceAboutToBeReset();
    void sourceReset();
};

Q_DECLARE_METATYPE(Qt::Orientation)

#endif // QPAGEDPROXYMODEL_H
