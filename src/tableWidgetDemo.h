/*
 * QTableWidget使用Demo
 * 支持表格宽高按内容进行调整，且支持多行输入（单元格编辑控件自定义）
*/

#pragma once
#include <QMainWindow>

class QTableWidget;
class TableWidgetDemo : public QWidget
{
    Q_OBJECT

public:
    TableWidgetDemo(QWidget *parent = nullptr);
    ~TableWidgetDemo();

private:
    void insertTableRow(const QString& key, const QString& value, int alignment = Qt::AlignRight | Qt::AlignVCenter, bool bUseTextEdit = false);

private:
    QTableWidget* m_tableWidget;
};

