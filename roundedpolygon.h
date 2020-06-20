#pragma once

//#include <QtMath>
#include <QtCore>
#include <QPolygon>
#include <QPainterPath>


namespace Editor
{

class RoundedPolygon : public QPolygon
{
public:

    RoundedPolygon()
    {}

    QPainterPath GetPath(unsigned int radius)
    {
        QPainterPath path;

        if (count() < 3) {
            qWarning() << "Polygon should have at least 3 points!";
            return path;
        }

        QPointF pt1;
        QPointF pt2;
        for (int i = 0; i < count(); i++) {
            pt1 = GetLineStart(i, radius);

            if (i == 0)
                path.moveTo(pt1);
            else
                path.quadTo(at(i), pt1);

            pt2 = GetLineEnd(i, radius);
            path.lineTo(pt2);
        }

        // close the last corner
        pt1 = GetLineStart(0, radius);
        path.quadTo(at(0), pt1);

        return path;
    }

    QPainterPath GetTabPath(unsigned int radius)
    {
        QPainterPath path;

        if (count() < 3) {
            qWarning() << "Polygon should have at least 3 points!";
            return path;
        }

        QPointF pt1;
        QPointF pt2;
        for (int i = 0; i < count(); i++) {
            pt1 = GetLineStart(i, radius);

            if (i == 0)
                path.moveTo(at(0));
            else
                if (i == 3)
                    path.lineTo(at(i));
                else
                    path.quadTo(at(i), pt1);

            pt2 = GetLineEnd(i, radius);
            path.lineTo(pt2);
        }

        // close the last corner
        path.lineTo(at(0));

        return path;
    }

private:

    float GetDistance(QPoint pt1, QPoint pt2) const
    {
        float fD = (pt1.x() - pt2.x())*(pt1.x() - pt2.x()) +
                (pt1.y() - pt2.y()) * (pt1.y() - pt2.y());
        return ::sqrtf(fD);
    }

    QPointF GetLineStart(int i, unsigned int radius) const
    {
        QPointF pt;
        QPoint pt1 = at(i);
        QPoint pt2 = at((i+1) % count());
        float fRat = radius / GetDistance(pt1, pt2);
        if (fRat > 0.5f)
            fRat = 0.5f;

        pt.setX((1.0f-fRat)*pt1.x() + fRat*pt2.x());
        pt.setY((1.0f-fRat)*pt1.y() + fRat*pt2.y());
        return pt;
    }

    QPointF GetLineEnd(int i, unsigned int radius) const
    {
        QPointF pt;
        QPoint pt1 = at(i);
        QPoint pt2 = at((i+1) % count());
        float fRat = radius / GetDistance(pt1, pt2);
        if (fRat > 0.5f)
            fRat = 0.5f;
        pt.setX(fRat*pt1.x() + (1.0f - fRat)*pt2.x());
        pt.setY(fRat*pt1.y() + (1.0f - fRat)*pt2.y());
        return pt;
    }
};

}//namespace Editor
