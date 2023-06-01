#include "ImagePreview.h"
#include <QWheelEvent>
#include <QScrollBar>
#include <QOpenGLWidget>

ImagePreview::ImagePreview(QPixmap pixmap, QWidget *parent)
        : QGraphicsView(parent)
{
    setMinimumSize(300,300);
    scene.addItem(&pixmapItem);
    setScene(&scene);
    setImage(pixmap);

    // create an OpenGL widget
    QOpenGLWidget *glWidget = new QOpenGLWidget();
    setViewport(glWidget);
    setViewportUpdateMode(QGraphicsView::FullViewportUpdate);

    setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    scaleFactorLabel.setParent(glWidget);
    scaleFactorLabel.move(10, 10);  // 将标签移动到窗口的左上角
    scaleFactorLabel.setFixedSize(100, 20);  // 设置标签的大小
    scaleFactorLabel.setStyleSheet("QLabel { background-color : white; }");
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
    qreal nextScaleFactor;
    if(event->angleDelta().y() > 0) {
        nextScaleFactor = scaleFactor * factor;
        if (scaleFactor < 1.0 && nextScaleFactor > 1.0) {
            scaleFactor = 1.0;
        } else {
            scaleFactor = nextScaleFactor;
        }
    } else {
        nextScaleFactor = scaleFactor / factor;
        if (scaleFactor > 1.0 && nextScaleFactor < 1.0) {
            scaleFactor = 1.0;
        } else {
            scaleFactor = nextScaleFactor;
        }
    }

    scaleFactor = qBound(0.1, scaleFactor, 10.0);
    scaleFactor = qreal(qRound(scaleFactor * 100)) / 100;  // Round to nearest hundredth

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
    if(scaleFactor > 1) {
        int dx = event->position().x() - lastDragPos.x();
        int dy = event->position().y() - lastDragPos.y();

        horizontalScrollBar()->setSliderPosition(horizontalScrollBar()->sliderPosition() - dx);
        verticalScrollBar()->setSliderPosition(verticalScrollBar()->sliderPosition() - dy);

        lastDragPos = event->position();
    }
}

void ImagePreview::updateScaleFactorLabel()
{
    scaleFactorLabel.setText(QString("Scale: %1%").arg(scaleFactor * 100));
}


