#pragma once

#include <QAbstractListModel>

class Parameter;

class ParameterModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit ParameterModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    // Editable:
    bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole) override;

    // Add data:
    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;

    // Remove data:
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;


    enum ParameterRoles{
        ParameterRole   = Qt::UserRole + 100,
        ValueRole       = Qt::UserRole + 200
    };

private:
    struct PV{
        PV()
        {
            parameter = nullptr;
            value = qInf();
        }
        Parameter *parameter;
        double value;
    };
    QList<PV> m_data;

};
