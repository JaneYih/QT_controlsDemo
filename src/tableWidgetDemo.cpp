#include "tableWidgetDemo.h"
#include <QTableWidget>
#include <QHeaderView>
#include <QTableWidgetItem>
#include <QVBoxLayout>
#include <QSizePolicy>
#include <QTextEdit>
#include <QLineEdit>
#include <QScrollBar>

TableWidgetDemo::TableWidgetDemo(QWidget *parent)
    : QWidget(parent)
{
    setWindowTitle("QTableWidget使用Demo");
    resize(QSize(500, 500));
    QVBoxLayout* layout = new QVBoxLayout(this); //界面使用垂直布局

    m_tableWidget = new QTableWidget(this);
    m_tableWidget->setSizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding);
    m_tableWidget->setColumnCount(2); //2列
    m_tableWidget->setTextElideMode(Qt::ElideNone); //不显示省略号
    m_tableWidget->setWordWrap(true); //自动换行
    m_tableWidget->setAlternatingRowColors(true); //行间隔颜色
    m_tableWidget->setSelectionBehavior(QAbstractItemView::SelectItems); //单元格可选

    //设置水平标题
    QStringList headers;
    headers << "名称" << "参数";
    m_tableWidget->setHorizontalHeaderLabels(headers);
    m_tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive); //水平标题宽度可拖动
    m_tableWidget->horizontalHeader()->setStretchLastSection(true); //水平标题最后一列填充空白部分
    //m_tableWidget->horizontalHeader()->setFrameShadow(QFrame::Raised); //设置边框阴影

    //设置垂直标题
    m_tableWidget->verticalHeader()->setStretchLastSection(true); //垂直标题最后一行填充空白部分
    m_tableWidget->verticalHeader()->hide();

    //插入行数据
    insertTableRow("key1", "value1");
    insertTableRow("key22222", "value2");
    insertTableRow("key3", "value3");
    insertTableRow("key4", "value4");
    insertTableRow("key5", "dasdasdas6633333333333333333333333333333336666666666666666666666AAA",
                   Qt::AlignRight | Qt::AlignTop, true);

    layout->addWidget(m_tableWidget);
  // this->setLayout(layout);
}

TableWidgetDemo::~TableWidgetDemo()
{

}

void TableWidgetDemo::insertTableRow(const QString& strKey, const QString& strValue, int alignment, bool bUseTextEdit)
{
    int rowIndex = m_tableWidget->rowCount();
    m_tableWidget->insertRow(rowIndex); //末端插入一行

    //第一列
    QTableWidgetItem* item = new QTableWidgetItem(strKey);
    item->setTextAlignment(alignment); //设置文本布局
    item->setFlags(Qt::ItemIsEnabled); //设置单元格属性,是否可编辑，是否可选等属性
    m_tableWidget->setItem(rowIndex, 0, item); //设置表格的单元格

    //第二列
    if (bUseTextEdit) //使用QTextEdit
    {
        QTextEdit* textEdit = new QTextEdit;
        textEdit->setFrameShape(QFrame::NoFrame); //无边框
        textEdit->setReadOnly(true);
        textEdit->setText(strValue);
        //textEdit->document()->adjustSize();
        //textEdit->setFixedHeight(textEdit->document()->size().rheight()); //读取文本的高度，然后设置为QTextEdit的固定高度
        textEdit->setAlignment(Qt::AlignVCenter); //文本垂直居中
        textEdit->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff); //隐藏滚动条
        m_tableWidget->setCellWidget(rowIndex, 1, textEdit); //设置单元格的控件为QTextEdit
        item = new QTableWidgetItem;
        item->setFlags(Qt::ItemIsEnabled); //设置单元格属性,是否可编辑，是否可选等属性
        m_tableWidget->setItem(rowIndex, 1, item);
    }
    else
    {
        item = new QTableWidgetItem(strValue);
        item->setFlags(Qt::ItemIsEnabled); //设置单元格属性,是否可编辑，是否可选等属性
        m_tableWidget->setItem(rowIndex, 1, item); //设置表格的单元格
    }

    m_tableWidget->resizeColumnToContents(0); //按内容长度调整第一列的宽度
}
