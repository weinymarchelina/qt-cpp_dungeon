/***********************************************************************
 * Author:  B11115010 鍾詩靈
 *          B11115014 魏美芳
 * Create Date: 2023/05/24
 * Update Date: 2023/06/01
 * Description: This C++ program performs dungeon game with kirby theme that have many features (see readme file)
***********************************************************************/

#include "menuwindow.h"

#include <QApplication>

// Intent: Start the application.
// Pre: None.
// Post: The application is executed, and the menu window is displayed.
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MenuWindow w;
    w.show();
    return a.exec();
}
