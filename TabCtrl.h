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

template<typename U, typename T, T N>
class AccessArray
{
    using ThisType = AccessArray<U, T, N>;

    static const size_t SIZE = static_cast<size_t>(N);
    U m_array[SIZE];
    bool m_mask[SIZE];

public:

    AccessArray()
    {
        Reset();
    }

    U& operator[](T index)
    {
        size_t n = static_cast<size_t>(index);
        return m_array[n];
    }

    ThisType& operator|(T v)
    {
        (*this)[v] = true;
        return *this;
    }

    ThisType& operator^(T v)
    {
        (*this)[v] = false;
        return *this;
    }

    U Sum()
    {
        U sum = 0;

        for(size_t i = 0; i < SIZE; i++)
        {
            if(m_mask[i])
                sum += m_array[i];
        }

        return sum;
    }

    void Reset(U initvalue = 0, bool initflag = false)
    {
        for(size_t i = 0; i < SIZE; i++)
        {
            m_array[i] = initvalue;
            m_mask[i] = initflag;
        }
    }
};

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

    TabPage(QWidget* parent, const QString text) : QWidget(parent)
    {
        m_text = text;
        LoadDefaults();
    }

    void LoadDefaults()
    {
        QCommonStyle style;
        m_tabicon = style.standardIcon(QStyle::SP_MessageBoxQuestion);
        m_closeicon = style.standardIcon(QStyle::SP_TitleBarCloseButton);
        m_iconsize = QApplication::style()->pixelMetric(QStyle::PM_TabBarIconSize);

        m_hor[Hor::Position] = 10;
        m_hor[Hor::HeadOffset] = 2;
        m_hor[Hor::LeftBorder] = 2;
        m_hor[Hor::IconLeftPadding] = 2;
        m_hor[Hor::IconWidth] = 2;
        m_hor[Hor::IconRightPadding] = 2;
        m_hor[Hor::TextWidth] = 2;
        m_hor[Hor::CloseLeftPadding] = 2;
        m_hor[Hor::CloseWidth] = 2;
        m_hor[Hor::CloseRightPadding] = 2;
        m_hor[Hor::RightBorder] = 2;
        m_hor[Hor::TailOffset] = 2;

        m_vert[Vert::CurrentEdge] = 2;
        m_vert[Vert::TopBorder] = 2;
        m_vert[Vert::TopPadding] = 2;
        m_vert[Vert::MaxHeight] = 2;
        m_vert[Vert::BottomPadding] = 2;
        m_vert[Vert::BottomBorder] = 2;
    }

protected:

    void CalcTabSize()
    {
        //Edge size
        if(m_active) m_vert[Vert::CurrentEdge] = 0;
        else m_vert[Vert::CurrentEdge] = m_edge;

        //Text size
        QFontMetrics fm = this->fontMetrics();
        m_hor[Hor::TextWidth] = fm.horizontalAdvance(m_text);
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

    //Tab array access horizontal value index
    enum class Hor
    {
        Position,
        LeftCornerRadius,
        HeadOffset,
        LeftBorder,
        IconLeftPadding,
        IconWidth,
        IconRightPadding,
        TextWidth,
        CloseLeftPadding,
        CloseWidth,
        CloseRightPadding,
        RightBorder,
        TailOffset,
        RightCornerRadius,
        Count
    };

    //Tab array access vertical value index
    enum class Vert
    {
        CurrentEdge,
        TopBorder,
        TopPadding,
        MaxHeight,
        BottomPadding,
        BottomBorder,
        Count
    };

    AccessArray<int, Hor, Hor::Count> m_hor;
    AccessArray<int, Vert, Vert::Count> m_vert;

    RoundedPolygon m_polygon;
    QPainterPath m_path;

    int m_edge = 2;
    int m_iconsize = 0;
    QIcon m_tabicon;
    QString m_text;
    QIcon m_closeicon;
    int m_active = true;

    QRect m_iconrect = QRect(0, 0, 0, 0);
    QRect m_textrect = QRect(0, 0, 0, 0);
    QRect m_closerect = QRect(0, 0, 0, 0);
    QRect m_tabrect = QRect(0, 0, 0, 0);

    void MakeTabPage()
    {
        CalcTabSize();

        int leftbottomx = m_hor[Hor::Position] + m_hor[Hor::LeftCornerRadius];
        int leftbottomy = m_vert[Vert::CurrentEdge] + m_tabrect.height();
        int lefttopx = leftbottomx + m_hor[Hor::HeadOffset];
        int lefttopy = m_vert[Vert::CurrentEdge];
        int rightbottomx = leftbottomx + m_tabrect.width();
        int rightbottomy = leftbottomy;
        int righttopx = rightbottomx + m_hor[Hor::TailOffset];
        int righttopy = lefttopy;

        m_polygon.clear();
        m_polygon << QPoint(leftbottomx, leftbottomy)
                  << QPoint(lefttopx, lefttopy)
                  << QPoint(righttopx, righttopy)
                  << QPoint(rightbottomx, rightbottomy);

        if(m_active)
        {
            righttopx = width()-1;
            righttopy = m_vert[Vert::CurrentEdge] + m_tabrect.height();
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
