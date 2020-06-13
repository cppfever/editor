#pragma once

#include <QWidget>
#include <QtMath>
#include <QtCore>
#include <QPainter>


namespace Editor
{
class RoundedPolygon : public QPolygon
{
public:

    RoundedPolygon()
    {
        SetRadius(10);
    }

    void SetRadius(unsigned int iRadius)
    {
        m_radius = iRadius;
    }

    const QPainterPath& GetPath()
    {
        m_path.clear();

        if (count() < 3) {
         qWarning() << "Polygon should have at least 3 points!";
         return m_path;
        }

        QPointF pt1;
        QPointF pt2;
        for (int i = 0; i < count(); i++) {
         pt1 = GetLineStart(i);

         if (i == 0)
             m_path.moveTo(pt1);
         else
             m_path.quadTo(at(i), pt1);

         pt2 = GetLineEnd(i);
         m_path.lineTo(pt2);
        }

        // close the last corner
        pt1 = GetLineStart(0);
        m_path.quadTo(at(0), pt1);

        return m_path;
    }

private:

    float GetDistance(QPoint pt1, QPoint pt2) const
    {
        float fD = (pt1.x() - pt2.x())*(pt1.x() - pt2.x()) +
             (pt1.y() - pt2.y()) * (pt1.y() - pt2.y());
        return ::sqrtf(fD);
    }

    QPointF GetLineStart(int i) const
    {
        QPointF pt;
        QPoint pt1 = at(i);
        QPoint pt2 = at((i+1) % count());
        float fRat = m_radius / GetDistance(pt1, pt2);
        if (fRat > 0.5f)
         fRat = 0.5f;

        pt.setX((1.0f-fRat)*pt1.x() + fRat*pt2.x());
        pt.setY((1.0f-fRat)*pt1.y() + fRat*pt2.y());
        return pt;
    }

    QPointF GetLineEnd(int i) const
    {
        QPointF pt;
        QPoint pt1 = at(i);
        QPoint pt2 = at((i+1) % count());
        float fRat = m_radius / GetDistance(pt1, pt2);
        if (fRat > 0.5f)
         fRat = 0.5f;
        pt.setX(fRat*pt1.x() + (1.0f - fRat)*pt2.x());
        pt.setY(fRat*pt1.y() + (1.0f - fRat)*pt2.y());
        return pt;
    }

private:

    QPainterPath m_path;
    unsigned int m_radius;
};


class TabPage : public QWidget
{
public:

    TabPage(QWidget* parent) : QWidget(parent)
    {
        m_polygon.SetRadius(5);
    }

protected:

    void resizeEvent(QResizeEvent *event) override
    {
        m_polygon.clear();
        m_polygon << QPoint(125, 25) << QPoint(130, 1)<< QPoint(225, 1)<< QPoint(230, 25)
         << QPoint(width()-1, 25)<< QPoint(width()-1, height()-1)<< QPoint(1, height()-1)<< QPoint(1, 25);

        m_path = m_polygon.GetPath();
    }

    void paintEvent(QPaintEvent *event) override
    {
        QPainter p(this);
        p.setRenderHint(QPainter::Antialiasing);
        p.fillPath(m_path,QBrush(Qt::blue));
        p.strokePath(m_path, QPen(QBrush(Qt::white), 3));
    }

private:

    RoundedPolygon m_polygon;
    QPainterPath m_path;
};

}//namespace Editor
