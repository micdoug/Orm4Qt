/*
 * Orm4Qt - An Object Relational Mapping Library for the Qt Framework
 * Copyright (c) 2014, Michael Dougras da Silva, All rights reserved.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3.0 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library.
 */

#ifndef OBJECTMODEL_H
#define OBJECTMODEL_H

#include <QAbstractTableModel>
#include <QList>

/**
 * A model that uses the Orm4Qt reflection system to retrieve the properties values
 * from a list of objects.
 */
template <class T>
class ObjectModel : public QAbstractTableModel
{
public:
    /**
     * Constructor
     * @param dataSource
     * The list of objects to be used as data source.
     * @param parent
     * The QObject parent of this instance.
     */
    ObjectModel(const QList<T> &dataSource=QList<T>(), QObject *parent = nullptr) : QAbstractTableModel(parent)
    {
        setDataSource(dataSource);
    }

    //Implementation of the QAbstractTableModel interface
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

    //Getter and setter of the data source.
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
    //The data source.
    QList<T> m_dataSource;
};

#endif // OBJECTMODEL_H
