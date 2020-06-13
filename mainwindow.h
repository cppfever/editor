#pragma once

#include <QMainWindow>
#include "TabCtrl.h"


namespace Editor
{

class MainWindow : public QMainWindow
{
    Q_OBJECT
    TabPage* m_tabsheet = nullptr;

public:
    MainWindow(QWidget *parent = nullptr)
    {
        m_tabsheet = new TabPage(this);
        QPalette pal(palette());
        pal.setColor(QPalette::Background, Qt::darkGray);
        m_tabsheet->setAutoFillBackground(true);
        m_tabsheet->setPalette(pal);
        m_tabsheet->show();
    }

    ~MainWindow()
    {}

protected:

    void resizeEvent(QResizeEvent *event) override
    {
        m_tabsheet->setGeometry(10, 10, width()-20, height()-20);
    }
};

}//namespace Editor
