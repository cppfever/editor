#pragma once

#include <QtMath>
#include <QtCore>
#include <QPainter>
#include <QIcon>
#include <QWidget>
#include <QCommonStyle>
#include <QApplication>

#include "accessarray.h"
#include "roundedpolygon.h".h"

namespace Editor
{

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
        m_radius = 5;
        m_edge = 2;
        m_position = 10;
        m_border = 2;
        m_offset = 5;
        m_padding = 2;
        m_tabwidth = 0;
        m_tabhight = 0;

        m_hor[Hor::Position] = m_position;
        m_hor[Hor::LeftCornerRadius] = m_radius;
        m_hor[Hor::HeadOffset] = m_offset;
        m_hor[Hor::LeftBorder] = m_border;
        m_hor[Hor::IconLeftPadding] = m_padding;
        m_hor[Hor::IconWidth] = m_iconsize;
        m_hor[Hor::IconRightPadding] = m_padding;
        m_hor[Hor::TextWidth] = 0;
        m_hor[Hor::CloseLeftPadding] = m_padding;
        m_hor[Hor::CloseWidth] = m_iconsize;
        m_hor[Hor::CloseRightPadding] = m_padding;
        m_hor[Hor::RightBorder] = m_border;
        m_hor[Hor::TailOffset] = m_offset;
        m_hor[Hor::RightCornerRadius] = m_radius;

        m_vert[Vert::CurrentEdge] = m_edge;
        m_vert[Vert::TopBorder] = m_border;
        m_vert[Vert::TopPadding] = m_padding;
        m_vert[Vert::MaxHeight] = m_iconsize;
        m_vert[Vert::BottomPadding] = m_padding;
        m_vert[Vert::BottomBorder] = m_border;
    }

protected:

    void resizeEvent(QResizeEvent *event) override
    {
        MakeTabPage();

        if(!m_active)
            m_path = m_polygon.GetPath(m_radius);
        else
            m_path = m_polygon.GetTabPath(m_radius);

    }

    void paintEvent(QPaintEvent *event) override
    {
        QPainter p(this);
        p.setRenderHint(QPainter::Antialiasing);
        p.fillPath(m_path,QBrush(Qt::blue));
        //p.strokePath(m_path, QPen(QBrush(Qt::white), m_border));
    }

private:

    //Access array for horizontal value indexes
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

    //Access array for vertical value indexes
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

    int m_radius;
    int m_edge;
    int m_position;
    int m_border;
    int m_iconsize;
    int m_offset;
    int m_padding;
    int m_tabwidth;
    int m_tabhight;

    QIcon m_tabicon;
    QString m_text;
    QIcon m_closeicon;
    bool m_active = true;

    QRect m_iconrect = QRect(0, 0, 0, 0);
    QRect m_textrect = QRect(0, 0, 0, 0);
    QRect m_closerect = QRect(0, 0, 0, 0);


    void CalcTabSize()
    {
        //Position may change
        m_hor[Hor::Position] = m_position;

        //Is active tab ?
        if(m_active) m_vert[Vert::CurrentEdge] = 0;
        else m_vert[Vert::CurrentEdge] = m_edge;

        //Text size
        QFontMetrics fm = this->fontMetrics();
        m_hor[Hor::TextWidth] = fm.horizontalAdvance(m_text);
        int textheight = fm.height();
        if(m_iconsize > textheight)
            m_vert[Vert::MaxHeight] = m_iconsize;
        else
            m_vert[Vert::MaxHeight] = textheight;

        m_tabwidth = m_hor.Sum();
        m_tabhight = m_vert.Sum();
    }

    void MakeTabPage()
    {
        CalcTabSize();

        //Tab
        int leftbottomx = m_hor[Hor::Position] + m_hor[Hor::LeftCornerRadius];
        int leftbottomy = m_vert[Vert::CurrentEdge] + m_tabhight;
        int lefttopx = leftbottomx + m_hor[Hor::HeadOffset];
        int lefttopy = m_vert[Vert::CurrentEdge];
        int rightbottomx = leftbottomx + m_tabwidth;
        int rightbottomy = leftbottomy;
        int righttopx = rightbottomx - m_hor[Hor::TailOffset];
        int righttopy = lefttopy;

        m_polygon.clear();
        m_polygon << QPoint(leftbottomx, leftbottomy)
                  << QPoint(lefttopx, lefttopy)
                  << QPoint(righttopx, righttopy)
                  << QPoint(rightbottomx, rightbottomy);

        m_iconrect.setRect(lefttopx + m_border, m_edge + m_border, m_iconsize, m_iconsize);
        m_textrect.setRect(m_iconrect.right() + 1, m_iconrect.top(), m_hor[Hor::TextWidth], m_vert[Vert::MaxHeight]);
        m_closerect.setRect(0, 0, 0, 0);

        //Page
        if(m_active)
        {
            righttopx = width()-1;
            righttopy = m_vert[Vert::CurrentEdge] + m_tabhight;
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
