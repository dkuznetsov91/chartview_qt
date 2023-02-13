#pragma once

#include <QColor>

enum class LineStyle {
    Dot     = 1,    //!< Точки
    Line    = 2,    //!< Линии
    Marker  = 4,    //!< Маркировка
    DotLine = 8     //!< Точки соединенные линиями
};


enum class ParamType {
  Signal    = 1,    //!< Разовая команда
  Analog    = 2     //!< Аналоговый параметр
};

class ParameterAttributes
{
public:
    ParameterAttributes();

    double min() const;
    void setMin(double min);

    double max() const;
    void setMax(double max);

    double level() const;
    void setLevel(double level);

    double range() const;
    void setRange(double range);

    LineStyle lineStyle() const;
    void setLineStyle(LineStyle style);

    ParamType type() const;
    void setType(ParamType type);

    QColor color() const;
    void setColor(const QColor &color);

    QString name() const;
    void setName(const QString &name);

    bool isBaseLine() const;
    void setBaseLine(bool baseLine);

private:
    double      m_min;          //!< Минимум
    double      m_max;          //!< Максимум
    double      m_level;        //!< Уровень вывода минимального значения в процетнах от размера поля графика
    double      m_range;        //!< Размах вывода параметра в процетнах от размера поля графика

    LineStyle   m_lineStyle;    //!< Тип линии
    ParamType   m_type;         //!< Тип параметра
    QColor      m_color;        //!< Цвет

    QString     m_name;         //!< Краткое имя
    bool        m_baseLine;     //!< Базовая линия
};
