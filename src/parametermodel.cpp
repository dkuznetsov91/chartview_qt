#include "parametermodel.h"
#include "parameter.h"

ParameterModel::ParameterModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

int ParameterModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return m_data.count();
}

QVariant ParameterModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= m_data.size())
        return QVariant();

    if (role == Qt::DisplayRole || role == Qt::EditRole)
        return m_data[index.row()].parameter->name() + " : " +
                QString::number(m_data[index.row()].value);

    return QVariant();
}

bool ParameterModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(!(role == ParameterRole || role == ValueRole)){
        return false;
    }

    if(role == ParameterRole){
        Parameter *parameter = value.value<Parameter *>();
        if (m_data.at(index.row()).parameter == parameter){
            return true;
        }

        m_data[index.row()].parameter = parameter;
        m_data[index.row()].value = qInf();

        emit dataChanged(index, index, {ParameterRole});
        return true;
    }
    if(role == ValueRole){
        double pValue = value.toDouble();

        //if (m_data.at(index.row()).value == pValue){
        //    return true;
        //}

        m_data[index.row()].value = pValue;

        emit dataChanged(index, index, {ValueRole});
        return true;
    }

    return false;
}

bool ParameterModel::insertRows(int row, int count, const QModelIndex &parent)
{
    if (count < 1 || row < 0 || row > rowCount(parent)){
        return false;
    }

    beginInsertRows(QModelIndex(), row, row + count - 1);

    for (int r = 0; r < count; ++r){
        m_data.insert(row, PV());
    }

    endInsertRows();

    return true;
}

bool ParameterModel::removeRows(int row, int count, const QModelIndex &parent)
{
    if (count <= 0 || row < 0 || (row + count) > rowCount(parent)){
        return false;
    }

    beginRemoveRows(QModelIndex(), row, row + count - 1);

    const auto it = m_data.begin() + row;
    m_data.erase(it, it + count);

    endRemoveRows();

    return true;
}
