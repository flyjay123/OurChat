#include "ImagePreview.h"

#include <QPainter>
#include <QPaintEvent>

ImagePreview::ImagePreview(const QPixmap& pixmap, QWidget* parent)
        : QWidget(parent), m_pixmap(pixmap), m_scale(1.0), m_minScale(0.1), m_maxScale(10.0), m_mousePressed(false) {
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowTitle(tr("Image Preview"));
    if (!m_pixmap.isNull()) {
        setMinimumSize(m_pixmap.size());
        m_imagePos = QPointF(width() / 2.0 - m_pixmap.width() / 2.0, height() / 2.0 - m_pixmap.height() / 2.0);
    }
}

void ImagePreview::setPixmap(const QPixmap& pixmap) {
    m_pixmap = pixmap;
    centerImage();
    update();
}

void ImagePreview::paintEvent(QPaintEvent* event) {
    QPainter painter(this);

    if (!m_pixmap.isNull()) {
        painter.scale(m_scale, m_scale);
        painter.drawPixmap(m_imagePos, m_pixmap);
    }
}

void ImagePreview::wheelEvent(QWheelEvent *event) {
    qreal oldScale = m_scale;

    if (event->angleDelta().y() > 0) {
        m_scale *= 1.1;
    } else {
        m_scale /= 1.1;
    }

    m_scale = qBound(0.1, m_scale, 10.0);

    if (m_scale <= 1.0) {
        m_imagePos.setX((width() - m_pixmap.width() * m_scale) / 2);
        m_imagePos.setY((height() - m_pixmap.height() * m_scale) / 2);
    } else {
        QPoint cursorPos = event->position().toPoint();
        QPointF oldCursorPosScaled = QPointF(cursorPos) / oldScale;
        QPointF newCursorPosScaled = QPointF(cursorPos) / m_scale;

        QPointF offset = (newCursorPosScaled - oldCursorPosScaled) * m_scale;
        m_imagePos += offset;
    }

    adjustImagePosition();
    update();
}





void ImagePreview::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        m_mousePressed = true;
        m_lastMousePos = event->pos();
    }
}

void ImagePreview::mouseReleaseEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        m_mousePressed = false;
    }
}


void ImagePreview::mouseMoveEvent(QMouseEvent* event) {
    if (m_mousePressed && m_scale > m_minScale) {
        QPointF delta = (event->pos() - m_lastMousePos) / m_scale;
        m_imagePos += delta;
        m_lastMousePos = event->pos();

        adjustImagePosition();
        update();
    }
}

void ImagePreview::resizeEvent(QResizeEvent* event) {
    centerImage();
    QWidget::resizeEvent(event);
}

void ImagePreview::centerImage() {
    QSize containerSize = size();
    QSize imageSize = m_pixmap.size();

    m_pixmapOffset.setX((containerSize.width() - imageSize.width() * m_scale) / (2 * m_scale));
    m_pixmapOffset.setY((containerSize.height() - imageSize.height() * m_scale) / (2 * m_scale));
}

void ImagePreview::adjustImagePosition() {
    qreal scaledWidth = m_pixmap.width() * m_scale;
    qreal scaledHeight = m_pixmap.height() * m_scale;

    if (scaledWidth <= width()) {
        m_imagePos.setX((width() - scaledWidth) / 2);
    } else {
        qreal leftBound = 0.0;
        qreal rightBound = width() - scaledWidth;
        m_imagePos.setX(qBound(rightBound, m_imagePos.x(), leftBound));
    }

    if (scaledHeight <= height()) {
        m_imagePos.setY((height() - scaledHeight) / 2);
    } else {
        qreal topBound = 0.0;
        qreal bottomBound = height() - scaledHeight;
        m_imagePos.setY(qBound(bottomBound, m_imagePos.y(), topBound));
    }

}



