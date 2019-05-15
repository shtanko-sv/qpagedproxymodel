#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "../src/qpagedproxymodel.h"

class QPagedProxyModel;
class QStandardItemModel;
class QTableView;
class QSpinBox;
class QComboBox;

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);

private:

    QStandardItemModel * source;
    QPagedProxyModel * proxy;

    QTableView * view;

    QSpinBox * sourceRowCount;
    QSpinBox * sourceColumnCount;

    QComboBox * orientation;
    QSpinBox * pageSize;

    QSpinBox * currentPage;
    QSpinBox * pageCount;

private slots:
    void setSourceRowCount(int count);
    void setSourceColumnCount(int count);

    void limitCurrentPage(int pageCount);
    void changeOrientation(int index);
};

#endif // WIDGET_H
