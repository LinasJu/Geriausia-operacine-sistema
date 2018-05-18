#ifndef VIRTUALGUI_H
#define VIRTUALGUI_H

#include <QWidget>
#include <QTableWidget>

namespace Ui {
class virtualGUI;
}

class virtualGUI : public QWidget
{
    Q_OBJECT

public:
    explicit virtualGUI(QWidget *parent = 0);
    ~virtualGUI();
    friend class MainWindow;
    void setCellColour(int row,int column,const QBrush &brush);
    void addToTable(int row,int column,uint32_t value);
    void setPID(QString);
    void setPC(QString);
    void setSP(QString);
    void setCX(QString);

private:
    void initTable();
    void addToolTip(uint8_t row, uint8_t column, uint32_t item);
    void headerToolTip(uint8_t row);
    std::string intToHexStr(int a,int pos);
    Ui::virtualGUI *ui;
};

#endif // VIRTUALGUI_H
