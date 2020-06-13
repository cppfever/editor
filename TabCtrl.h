#pragma once

#include <QtMath>
#include <QtCore>
#include <QPainter>
#include <QIcon>
#include <QWidget>
#include <QCommonStyle>
#include <QApplication>


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


class TabPage : public QWidget
{    
public:

    TabPage(QWidget* parent, const QString text) : QWidget(parent), m_text(text)
    {
        QCommonStyle style;
        m_tabicon = style.standardIcon(QStyle::SP_MessageBoxQuestion);
        m_closeicon = style.standardIcon(QStyle::SP_TitleBarCloseButton);
        m_iconsize = QApplication::style()->pixelMetric(QStyle::PM_TabBarIconSize);

    }

protected:

    void CalcTabSize()
    {
        //Edge size
        if(m_active) m_currentedge = 0;
        else m_currentedge = m_edge;

        //Text size
        QFontMetrics fm = this->fontMetrics();
        m_textsize.setWidth(fm.horizontalAdvance(m_text));
        m_textsize.setHeight(fm.height());

        int leftpadding = m_cornerradius + m_headoffset + m_borderwidth;
        int rightpadding = m_borderwidth + m_tailoffset + m_cornerradius;

        //Tab size
        m_tabsize.setWidth(leftpadding + m_leftpadding
                           + 2 * m_iconsize + m_textsize.width()
                           + m_rightpadding + m_tailoffset);

        int maxsize = m_iconsize;
        if(m_textsize.height() > maxsize)
            maxsize = m_textsize.height();

        m_tabsize.setHeight(m_borderwidth + m_toppadding + maxsize + m_bottompadding);

        //m_iconrect.setRect()
    }

    void DrawIcon()
    {

    }

    void DrawText()
    {

    }

    void DrawCloseButton()
    {

    }

    void resizeEvent(QResizeEvent *event) override
    {
        MakeTabPage();

        if(m_active)
            m_path = m_polygon.GetPath(m_cornerradius);
        else
            m_path = m_polygon.GetTabPath(m_cornerradius);

    }

    void paintEvent(QPaintEvent *event) override
    {
        QPainter p(this);
        p.setRenderHint(QPainter::Antialiasing);
        p.fillPath(m_path,QBrush(Qt::blue));
        p.strokePath(m_path, QPen(QBrush(Qt::white), m_borderwidth));
    }

private:

    enum class TabHorIndex
    {

    };

    enum class TabVertIndex
    {

    };

    RoundedPolygon m_polygon;
    QPainterPath m_path;

    QIcon m_tabicon;
    QString m_text;
    QIcon m_closeicon;
    int m_active = true;

    int m_iconsize = 0;
    QSize m_textsize = QSize(0,0);
    QSize m_tabsize = QSize(0,0);

    int m_position = 0;
    int m_borderwidth = 1;
    int m_edge = 2;
    int m_currentedge = 0;
    int m_headoffset = 5;
    int m_tailoffset = -5;
    int m_cornerradius = 5;
    int m_leftpadding = 2;
    int m_toppadding = 2;
    int m_rightpadding = 2;
    int m_bottompadding = 2;    

    QRect m_iconrect = QRect(0, 0, 0, 0);
    QRect m_textrect = QRect(0, 0, 0, 0);
    QRect m_closerect = QRect(0, 0, 0, 0);

    void MakeTabPage()
    {
        CalcTabSize();

        int leftbottomx = m_position + m_cornerradius;
        int leftbottomy = m_currentedge + m_tabsize.height();
        int lefttopx = leftbottomx + m_headoffset;
        int lefttopy = m_currentedge;
        int rightbottomx = leftbottomx + m_tabsize.width();
        int rightbottomy = leftbottomy;
        int righttopx = rightbottomx + m_tailoffset;
        int righttopy = lefttopy;

        m_polygon.clear();
        m_polygon << QPoint(leftbottomx, leftbottomy)
                  << QPoint(lefttopx, lefttopy)
                  << QPoint(righttopx, righttopy)
                  << QPoint(rightbottomx, rightbottomy);

        if(m_active)
        {
            righttopx = width()-1;
            righttopy = m_currentedge + m_tabsize.height();
            rightbottomx = width()-1;
            rightbottomy = height()-1;
            leftbottomx = 1;
            leftbottomy = height()-1;
            lefttopx = 1;
            lefttopy = righttopy;

            m_polygon << QPoint(righttopx, righttopy)
                      << QPoint(rightbottomx, rightbottomy)
                      << QPoint(leftbottomx, leftbottomy)
                      << QPoint(lefttopx, lefttopy);
        }
    }
};

}//namespace Editor
