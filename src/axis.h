#pragma once

#include <QObject>

class Parameter;

class Axis : public QObject
{
    Q_OBJECT
public:
    Axis(QObject *parent = nullptr);
    virtual ~Axis() {}

    virtual double min() const = 0;
    virtual void setMin(double min) = 0;

    virtual double max() const = 0;
    virtual void setMax(double max) = 0;

    virtual double span() const;

    virtual double level() const = 0;
    virtual void setLevel(double level) = 0;

    virtual double range() const = 0;
    virtual void setRange(double range) = 0;

    virtual QColor color() const = 0;

    virtual bool isScalable() = 0;

    virtual int maxTics() const = 0;
    virtual void setMaxTics(int maxTics) = 0;
    virtual void calcTics(double length, double offset, int minTicDistance);
    virtual QList<double> &tics() = 0;

signals:
    void axisChanged();
    void ticsChanged();
};

class ConstAxis : public Axis
{
    Q_OBJECT
public:
    ConstAxis(QObject *parent = nullptr);
    ~ConstAxis() override;

    double min() const override;
    void setMin(double min) override;

    double max() const override;
    void setMax(double max) override;

    double level() const override;
    void setLevel(double level) override;

    double range() const override;
    void setRange(double range) override;

    QColor color() const override;

    bool isScalable() override;

    int maxTics() const override;

    void setMaxTics(int maxTics) override;

    QList<double> &tics() override;

protected:
    double m_min;
    double m_max;
    double m_level;
    double m_range;

    int m_maxTics;
    QList<double> m_ticCoordinates;
};

class ParameterAxis : public ConstAxis
{
public:
    ParameterAxis(Parameter *parameter, QObject *parent = nullptr);
    ~ParameterAxis() override;

    double min() const override;
    void setMin(double min) override;

    double max() const override;
    void setMax(double max) override;

    double level() const override;
    void setLevel(double level) override;

    double range() const override;
    void setRange(double range) override;

    QColor color() const override;

    bool isScalable() override;

    Parameter *parameter();

private:
    Parameter *m_parameter;
};

