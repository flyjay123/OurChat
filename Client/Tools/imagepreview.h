#pragma once
#ifndef IMAGEPREVIEW_H
#define IMAGEPREVIEW_H
#include <QWidget>
#include <QPixmap>
#include <QWheelEvent>
#include <QMouseEvent>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QLabel>

class ImagePreview : public QGraphicsView
{
Q_OBJECT

public:
    explicit ImagePreview(QPixmap pixmap, QWidget *parent = nullptr);
    void setImage(const QPixmap &pixmap);
    void updateScaleFactorLabel();

protected:
    void wheelEvent(QWheelEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    QGraphicsScene scene;
    QGraphicsPixmapItem pixmapItem;
    QLabel scaleFactorLabel;
    qreal scaleFactor = 1.0;
    QPointF lastDragPos;
};

#endif // IMAGEPREVIEW_H