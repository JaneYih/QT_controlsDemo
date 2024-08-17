#include "pictureDisplayWidget.h"
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QSlider>
#include <QDebug>
#include <QScrollBar>
#include <QPushButton>
#include <QVBoxLayout>

static qreal sliderValueToPixmapScale(int value)
{
    return value*0.02;
}
static int pixmapScaleToSliderValue(qreal scale)
{
    return scale/0.02;
}

PictureDisplayWidget::PictureDisplayWidget(QWidget *parent)
    : QWidget(parent)
{
    setWindowTitle("Graphics View Framework的使用Demo");

    QVBoxLayout* verticalLayout = new QVBoxLayout(this);

    //创建场景
    m_scene = new QGraphicsScene;
    QRectF sceneRect(-200, -200, 400, 400);
    m_scene->setSceneRect(sceneRect); //设置view显示的场景框

    //添加左上角文本图元
    m_infoTextItem = m_scene->addText("", QFont("Times", 12, QFont::Bold));
    m_infoTextItem->setDefaultTextColor(Qt::yellow);
    m_infoTextItem->setPos(-200, -200); //设置文本图元位于场景框左上角，该坐标是场景坐标系的坐标
    m_infoTextItem->setZValue(5); //设置z轴值，使得图元在最上层

    //添加场景矩形边框图元，用于图片缩小时的碰撞检测
    m_framRectItem = m_scene->addRect(sceneRect, QPen(Qt::red, 1));

    //添加场景矩形边框的四条边线图元，用于图片放大时的碰撞检测
    QPen framLinePen(Qt::blue, 1);
    //int gap = 1;
    //QRectF framLineRect = sceneRect.adjusted(gap, gap, -gap, -gap);
    QRectF framLineRect(sceneRect);
    m_framLineList.append(m_scene->addLine(QLineF(framLineRect.topLeft(), framLineRect.topRight()), framLinePen));
    m_framLineList.append(m_scene->addLine(QLineF(framLineRect.topRight(), framLineRect.bottomRight()), framLinePen));
    m_framLineList.append(m_scene->addLine(QLineF(framLineRect.bottomRight(), framLineRect.bottomLeft()), framLinePen));
    m_framLineList.append(m_scene->addLine(QLineF(framLineRect.bottomLeft(), framLineRect.topLeft()), framLinePen));

    //添加图片图元
    m_pixmapItem = m_scene->addPixmap(QPixmap());

    //创建视图
    m_view = new QGraphicsView(m_scene, this);
    m_view->setMinimumSize(m_scene->sceneRect().width() + m_view->verticalScrollBar()->width(),
                           m_scene->sceneRect().height() + m_view->horizontalScrollBar()->height());
    m_view->setBackgroundBrush(QBrush(Qt::gray));
    m_view->show();

    verticalLayout->addWidget(m_view);

    //添加图片旋转角度条
    m_rotateSlider = new QSlider(Qt::Horizontal, this);
    m_rotateSlider->setTickInterval(1);
    m_rotateSlider->setMinimum(0);
    m_rotateSlider->setMaximum(360);
    m_rotateSlider->setTickPosition(QSlider::TicksBelow);
    verticalLayout->addWidget(m_rotateSlider);
    connect(m_rotateSlider, &QSlider::valueChanged, this, &PictureDisplayWidget::slot_SliderValueChanged);

    //添加图片缩放条
    m_imageSizeSlider = new QSlider(Qt::Horizontal, this);
    //m_imageSizeSlider->setTickInterval(1);
    m_imageSizeSlider->setMinimum(30);
    m_imageSizeSlider->setMaximum(70);
    m_imageSizeSlider->setValue(50);
    m_imageSizeSlider->setTickPosition(QSlider::TicksBelow);
    verticalLayout->addWidget(m_imageSizeSlider);
    connect(m_imageSizeSlider, &QSlider::valueChanged, this, &PictureDisplayWidget::slot_SliderValueChanged);

    //添加按钮
    m_btn1 = new QPushButton("1", this);
    m_btn2 = new QPushButton("2", this);
    m_btn3 = new QPushButton("3", this);
    m_btn4 = new QPushButton("4", this);
    verticalLayout->addWidget(m_btn1);
    verticalLayout->addWidget(m_btn2);
    verticalLayout->addWidget(m_btn3);
    verticalLayout->addWidget(m_btn4);
    connect(m_btn1, &QPushButton::clicked, this, &PictureDisplayWidget::slot_btnClicked);
    connect(m_btn2, &QPushButton::clicked, this, &PictureDisplayWidget::slot_btnClicked);
    connect(m_btn3, &QPushButton::clicked, this, &PictureDisplayWidget::slot_btnClicked);
    connect(m_btn4, &QPushButton::clicked, this, &PictureDisplayWidget::slot_btnClicked);

    //初始化
    setImage(":/images/1.png");
    showInfo();
    resize(700, 650);
}

PictureDisplayWidget::~PictureDisplayWidget()
{

}

void PictureDisplayWidget::slot_btnClicked(bool checked)
{
    Q_UNUSED(checked);
    QPushButton* sender = static_cast<QPushButton*>(this->sender());
    QString strImagesPath(":/images/1.png");
    if (sender == m_btn1)
    {
        strImagesPath = ":/images/1.png";
    }
    else if (sender == m_btn2)
    {
        strImagesPath = ":/images/1.jpeg";
    }
    else if (sender == m_btn3)
    {
        strImagesPath = ":/images/1.jpg";
    }
    else if (sender == m_btn4)
    {
        m_rotateSlider->setValue(0);
        strImagesPath = ":/images/OIP-C.jfif";
    }
    setImage(strImagesPath);
}

void PictureDisplayWidget::setImage(const QString& strImagesPath)
{
    //修改图片大小，使图片大小与场景边框尺寸一致
    m_pixmapItem->setPixmap(QPixmap(strImagesPath).scaled(m_scene->sceneRect().size().toSize(), Qt::KeepAspectRatio));
    //设置图元变换(旋转)的原点为图元边框的中心点
    m_pixmapItem->setTransformOriginPoint(m_pixmapItem->boundingRect().center());

    QPointF itemBoundingRectCenterPoint = m_pixmapItem->boundingRect().center();
    qDebug() << m_pixmapItem->boundingRect() << itemBoundingRectCenterPoint; //返回的是图元坐标系坐标

    //以pixmapItem边界矩形左上角为(0,0)进行移动---关键
    m_pixmapItem->setPos(0-itemBoundingRectCenterPoint.x(), 0-itemBoundingRectCenterPoint.y());
    qDebug() << "pos:" << m_pixmapItem->pos();//返回的是父项坐标系坐标
    qDebug() << "scenePos:" << m_pixmapItem->scenePos();//返回的是场景坐标系坐标
    qDebug() << "sceneBoundingRectPathPoint:" << m_pixmapItem->mapToScene(m_pixmapItem->boundingRect());//返回的是场景坐标系坐标

    //缩小图元，使其在场景边框内
    shrinkImageSizeToInsideFramRect([](){
        //qDebug() << "setImage-->shrinkImageSizeToInsideFramRect";
    });
    //放大图元，使其填满场景边框
    enlargeImageSizeToInsideFramRect();
}


//超出边界矩形时，自动缩小图片尺寸
void PictureDisplayWidget::shrinkImageSizeToInsideFramRect(const std::function<void (void)>& op)
{
    if (m_imageSizeSlider)
    {
        //不断缩小图元大小，直至其完全包含在场景边框内
        int imageSizeSliderValue = m_imageSizeSlider->value();
        while (!m_pixmapItem->collidesWithItem(m_framRectItem, Qt::ContainsItemShape)) //碰撞检测
        {
            m_imageSizeSlider->setValue(--imageSizeSliderValue);
            op();
        }
    }
}

//自动放大图片尺寸，至触碰边界线
void PictureDisplayWidget::enlargeImageSizeToInsideFramRect()
{
    // for (int i = 0; i < m_framLineList.size(); ++i) {
    //     qDebug() << "ContainsItemShape " << i << ": "<< m_pixmapItem->collidesWithItem(m_framLineList.at(i), Qt::ContainsItemShape);
    //     qDebug() << "IntersectsItemShape " << i << ": "<< m_pixmapItem->collidesWithItem(m_framLineList.at(i), Qt::IntersectsItemShape);
    //     qDebug() << "ContainsItemBoundingRect " << i << ": "<< m_pixmapItem->collidesWithItem(m_framLineList.at(i), Qt::ContainsItemBoundingRect);
    //     qDebug() << "IntersectsItemBoundingRect " << i << ": "<< m_pixmapItem->collidesWithItem(m_framLineList.at(i), Qt::IntersectsItemBoundingRect);
    //     qDebug() << "--------------------------------";
    // }

    if (m_imageSizeSlider)
    {
        //不断放大图元，直至其碰撞到任意一条边线或者达到最大缩放比
        int count = 0;
        do
        {
            bool bCollidesLine = false;
            foreach (auto framLine, m_framLineList)
            {
                if (bCollidesLine = m_pixmapItem->collidesWithItem(framLine, Qt::IntersectsItemShape)) //碰撞检测
                {
                    break;
                }
            }
            if (bCollidesLine)
            {
                break; //当图元碰撞到任意一条边线时退出
            }
            int imageSizeSliderValue = m_imageSizeSlider->value();
            m_imageSizeSlider->setValue(++imageSizeSliderValue); //不断放大图元
            if (m_imageSizeSlider->value() < imageSizeSliderValue)
            {
                break; //当图元放大无效时退出
            }
        } while (++count < m_imageSizeSlider->maximum());
    }
}

void PictureDisplayWidget::slot_SliderValueChanged(int value)
{
    QSlider* sender = static_cast<QSlider*>(this->sender());
    if (sender == m_rotateSlider)
    {
        m_pixmapItem->setRotation(value);
        //旋转并检测当超出边界矩形时，自动缩小图片尺寸
        shrinkImageSizeToInsideFramRect(std::bind(&QGraphicsItem::setRotation, m_pixmapItem, value));
        //自动放大图片尺寸，至触碰边界线
        enlargeImageSizeToInsideFramRect();
    }
    else if (sender == m_imageSizeSlider)
    {
        qreal oldScale = m_pixmapItem->scale(); //记录原缩放比
        m_pixmapItem->setScale(sliderValueToPixmapScale(value)); //设置图元缩放比
        //当碰撞检测到pixmapItem不完全在边界矩形内时，恢复原缩放比
        if (!m_pixmapItem->collidesWithItem(m_framRectItem, Qt::ContainsItemShape))
        {
            m_pixmapItem->setScale(oldScale);
            m_imageSizeSlider->setValue(pixmapScaleToSliderValue(oldScale));
        }
    }
    showInfo();
}

//左上角信息打印
void PictureDisplayWidget::showInfo()
{
    QString infoText = QString("rotate:%1\r\nimageSize:%2")
    .arg(m_rotateSlider->value())
        .arg(m_imageSizeSlider->value());
    m_infoTextItem->setPlainText(infoText);
}
