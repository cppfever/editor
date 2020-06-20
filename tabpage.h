#pragma once

#include <QtMath>
#include <QtCore>
#include <QPainter>
#include <QIcon>
#include <QWidget>
#include <QCommonStyle>
#include <QApplication>

//#include "accessarray.h"
#include "roundedpolygon.h".h"

namespace Editor
{

class TabPage : public QWidget
{    
public:

    TabPage(QWidget* parent, const QString text) : QWidget(parent)
    {
        m_text = text;

        QCommonStyle style;
        m_icon = style.standardIcon(QStyle::SP_MessageBoxQuestion);
        m_closeicon = style.standardIcon(QStyle::SP_TitleBarCloseButton);
        m_iconsize = QApplication::style()->pixelMetric(QStyle::PM_TabBarIconSize);
    }


protected:

    void resizeEvent(QResizeEvent *event) override
    {
        MakeTabPage();

        if(m_active)
            m_path = m_polygon.GetPath(m_radius);
        else
            m_path = m_polygon.GetTabPath(m_radius);

    }

    void paintEvent(QPaintEvent *event) override
    {
        QPainter p(this);
        p.setRenderHint(QPainter::Antialiasing);
        p.fillPath(m_path,QBrush(Qt::blue));
        p.strokePath(m_path, QPen(QBrush(Qt::white), m_border));
    }

private:

    RoundedPolygon m_polygon;
    QPainterPath m_path;

    int m_position = 5;
    int m_radius = 10;
    int m_border = 2;
    int m_currentborder = 0;
    int m_offset = 5;
    int m_padding = 2;
    int m_iconsize = 0;
    int m_textwidth = 0;
    int m_textheight = 0;
    int m_vertheight = 0;

    int m_edge = 2;
    int m_currentedge = 0;

    int m_tabwidth = 0;
    int m_tabhight = 0;

    QIcon m_icon;
    QString m_text;
    QIcon m_closeicon;
    bool m_active = true;
    bool m_drawborder = true;

    QRect m_iconrect = QRect(0, 0, 0, 0);
    QRect m_textrect = QRect(0, 0, 0, 0);
    QRect m_closerect = QRect(0, 0, 0, 0);

    void MakeTabPage()
    {
        //Text size
        QFontMetrics fm = this->fontMetrics();
        m_textwidth = fm.horizontalAdvance(m_text);
        m_textheight = fm.height();
        m_vertheight = (m_iconsize > m_textheight) ? m_iconsize : m_textheight;
        m_currentedge = m_active ? 0 : m_edge;
        m_currentborder = m_drawborder ? m_border : 0;

        m_tabwidth = m_radius + m_currentborder + m_offset + m_iconsize + m_padding + m_textwidth + m_padding + m_iconsize + m_offset + m_currentborder + m_radius;
        m_tabhight = m_currentedge + m_currentborder + m_padding + m_vertheight + m_padding + m_currentborder;


        //Tab
        int leftbottomx = m_position + m_radius;
        int leftbottomy = m_currentedge + m_tabhight;
        int lefttopx = leftbottomx + m_offset;
        int lefttopy = m_currentedge;
        int rightbottomx = leftbottomx + m_tabwidth;
        int rightbottomy = leftbottomy;
        int righttopx = rightbottomx - m_offset;
        int righttopy = lefttopy;

        m_polygon.clear();
        m_polygon << QPoint(leftbottomx, leftbottomy)
                  << QPoint(lefttopx, lefttopy)
                  << QPoint(righttopx, righttopy)
                  << QPoint(rightbottomx, rightbottomy);

        m_iconrect.setRect(lefttopx + m_currentborder, m_currentedge + m_currentborder, m_iconsize, m_iconsize);
        m_textrect.setRect(m_iconrect.right() + 1, m_iconrect.top(), m_textwidth, m_textheight);
        m_closerect.setRect(m_textrect.right() + 1, m_textrect.top(), m_iconsize, m_iconsize);

        //Page
        if(m_active)
        {
            righttopx = width();
            righttopy = m_currentedge + m_tabhight;
            rightbottomx = righttopx;
            rightbottomy = height();
            leftbottomx = 0;
            leftbottomy = rightbottomy;
            lefttopx = 0;
            lefttopy = righttopy;

            m_polygon << QPoint(righttopx, righttopy)
                      << QPoint(rightbottomx, rightbottomy)
                      << QPoint(leftbottomx, leftbottomy)
                      << QPoint(lefttopx, lefttopy);
        }
    }
};

}//namespace Editor
