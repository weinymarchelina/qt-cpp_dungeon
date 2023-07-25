/***********************************************************************
 * Author:  B11115010 鍾詩靈
 *          B11115014 魏美芳
 * Create Date: 2023/05/24
 * Update Date: 2023/06/01
 * Description: This C++ program performs dungeon game with kirby theme that have many features (see readme file)
***********************************************************************/

#include "square.h"
#include <QPainter>
#include <QPaintEvent>

// Intent: Construct a Square widget with an image and a parent widget
// Pre: The image URL and the parent widget must be provided
// Post: A Square widget is created with the specified image URL and parent widget
Square::Square(const QString& imageUrl, QWidget* parent)
    : QLabel(parent), imageUrl(imageUrl), hasImage(true)
{
}

// Intent: Change the image of the Square widget to the specified URL
// Pre: The URL of the new image must be provided
// Post: The image of the Square widget is changed to the specified URL
void Square::changeImg(const QString& url)
{
     // Update the image URL
    imageUrl = url;

    // Set the flag indicating that the Square has an image
    hasImage = true;

    // Trigger repaint to reflect the new image
    update();
}

// Intent: Remove the image from the Square widget
// Pre: None
// Post: The Square widget no longer displays an image
void Square::setNoImg()
{
    // Set the flag indicating that the Square has no image
    hasImage = false;

    // Trigger repaint to remove the image
    update();
}

// Intent: Set whether the image should be stretched to fit the Square widget
// Pre: The desired stretched state must be provided
// Post: The image of the Square widget will be drawn stretched or with aspect ratio preserved based on the specified state
void Square::setStretched(bool isStretched) {
    // Update the stretched state
    this->isStretched = isStretched;

    // Trigger a repaint to reflect the updated image drawing style
    update();
}

// Intent: Handle the paint event of the Square widget
// Pre: A paint event must be triggered
// Post: The Square widget is painted, displaying the image if it exists
void Square::paintEvent(QPaintEvent* event)
{
    // Call the base class paintEvent implementation
    QWidget::paintEvent(event);

    
    // Create a QPainter for painting on the widget
    QPainter painter(this);

    // Check if the Square has an image
    if (hasImage) {
        // Calculate the size and position for drawing the image
        int imageSize = qMin(width(), height());
        int imageX = (width() - imageSize) / 2;
        int imageY = (height() - imageSize) / 2;

        // Load the image pixmap
        QPixmap pixmap(imageUrl);

         // Scale the pixmap according to the desired drawing style
        QPixmap scaledPixmap = pixmap.scaled(imageSize, imageSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        QPixmap stretchedPixmap = pixmap.scaled(imageSize, imageSize, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

        // Draw the image pixmap on the Square widget based on the drawing style
        if (isStretched) {
            painter.drawPixmap(rect(), stretchedPixmap);
        }
        else {
            painter.drawPixmap(imageX, imageY, scaledPixmap);
        }

    }
}
