#ifndef OBJECTMODEL_H
#define OBJECTMODEL_H

#include <QAbstractTableModel>
#include <QList>

template <class T>
class ObjectModel : public QAbstractTableModel
{
public:
    ObjectModel(const QList<T> &dataSource=QList<T>(), QObject *parent = nullptr) : QAbstractTableModel(parent)
    {
        setDataSource(dataSource);
    }
    int rowCount(const QModelIndex &parent) const
    {
        Q_UNUSED(parent);
        return m_dataSource.count();
    }
    int columnCount(const QModelIndex &parent) const
    {
        Q_UNUSED(parent);
        T temp;
        return temp.reflection()->properties().count();
    }
    QVariant data(const QModelIndex &index, int role) const
    {
        switch(role)
        {
            case Qt::DisplayRole:
                return ((T&)m_dataSource[index.row()]).reflection()->properties()[index.column()]->value();
            case Qt::ToolTipRole:
                return ((T&)m_dataSource[index.row()]).reflection()->properties()[index.column()]->tags()["display"];
            default:
                return QVariant();
        }
    }
    QVariant headerData(int section, Qt::Orientation orientation, int role) const
    {
        if(role == Qt::DisplayRole && orientation == Qt::Horizontal)
        {
            T temp;
            return temp.reflection()->properties()[section]->tags()["display"];
        }
        else
        {
            return QVariant();
        }
    }
    void setDataSource(const QList<T> &dataSource)
    {
        beginResetModel();
        m_dataSource = dataSource;
        endResetModel();
    }
    const QList<T> &dataSource() const
    {
        return m_dataSource;
    }

private:
    QList<T> m_dataSource;
};

#endif // OBJECTMODEL_H
