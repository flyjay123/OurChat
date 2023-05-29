#include "ImagePreview.h"
#include <QWheelEvent>
#include <QScrollBar>

ImagePreview::ImagePreview(QPixmap pixmap, QWidget *parent)
        : QGraphicsView(parent)
{
    scene.addItem(&pixmapItem);
    setScene(&scene);
    scaleFactorLabel.setParent(this);
    scaleFactorLabel.move(10, 10);  // 将标签移动到窗口的左上角
    scaleFactorLabel.setFixedSize(100, 20);  // 设置标签的大小
    setImage(pixmap);
    setRenderHint(QPainter::Antialiasing);
    updateScaleFactorLabel();
}

void ImagePreview::setImage(const QPixmap &pixmap)
{
    pixmapItem.setPixmap(pixmap);
    scene.setSceneRect(pixmap.rect());
}

void ImagePreview::wheelEvent(QWheelEvent *event)
{
    const qreal factor = 1.15;
    if(event->angleDelta().y() > 0) {
        scaleFactor *= factor;
    } else {
        scaleFactor /= factor;
    }

    scaleFactor = qBound(0.1, scaleFactor, 10.0);

    if(scaleFactor < 1) {
        setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    } else {
        setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    }

    QTransform transform;
    transform.scale(scaleFactor, scaleFactor);
    setTransform(transform);
    updateScaleFactorLabel();
}

void ImagePreview::mousePressEvent(QMouseEvent *event)
{
    if(scaleFactor > 1) {
        lastDragPos = event->position();
        setCursor(Qt::ClosedHandCursor);
    }
}

void ImagePreview::mouseReleaseEvent(QMouseEvent *event)
{
    if(scaleFactor > 1) {
        lastDragPos = QPointF();  // Reset the last drag position
        setCursor(Qt::ArrowCursor);
    }
}

void ImagePreview::mouseMoveEvent(QMouseEvent *event)
{
    if(scaleFactor > 1 && !lastDragPos.isNull()) {
        int dx = event->position().x() - lastDragPos.x();
        int dy = event->position().y() - lastDragPos.y();

        horizontalScrollBar()->setValue(horizontalScrollBar()->value() - dx);
        verticalScrollBar()->setValue(verticalScrollBar()->value() - dy);

        lastDragPos = event->position();
    }
}


void ImagePreview::updateScaleFactorLabel()
{
    scaleFactorLabel.setText(QString("Scale: %1%").arg(scaleFactor * 100));
}


