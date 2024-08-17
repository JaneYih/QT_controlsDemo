/*
 * 图片展示控件
 * Graphics View Framework的使用Demo
 * QGraphicsView、QGraphicsScene、QGraphicsItem的使用Demo
 * 支持图片的旋转与缩放，自动缩放至接触边框
*/

#pragma once
#include <QWidget>

class QGraphicsScene;
class QGraphicsView;
class QGraphicsTextItem;
class QGraphicsPixmapItem;
class QGraphicsRectItem;
class QGraphicsLineItem;
class QSlider;
class QPushButton;
class PictureDisplayWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PictureDisplayWidget(QWidget *parent = nullptr);
    ~PictureDisplayWidget();


public slots:
    void slot_SliderValueChanged(int value);
    void slot_btnClicked(bool checked = false);

private:
    void showInfo();
    void setImage(const QString& strImagesPath);
    void shrinkImageSizeToInsideFramRect(const std::function<void (void)>& op);
    void enlargeImageSizeToInsideFramRect();

private:
    QGraphicsView* m_view;
    QGraphicsScene* m_scene;
    QGraphicsTextItem* m_infoTextItem;
    QGraphicsRectItem* m_framRectItem;
    QList<QGraphicsLineItem*> m_framLineList;
    QGraphicsPixmapItem* m_pixmapItem;

    QSlider* m_rotateSlider;
    QSlider* m_imageSizeSlider;

    QPushButton* m_btn1;
    QPushButton* m_btn2;
    QPushButton* m_btn3;
    QPushButton* m_btn4;
};
