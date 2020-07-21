#include "mainwindow.h"
#include <QApplication>
#include <cstdlib>
struct AAA
{
    AAA(int _x):x(_x){}
    int x = 1;
    bool operator==(const AAA &aaa) const
    {
        return x == aaa.x;
    }

};
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    srand(time(NULL)); 
    MainWindow w;
    w.show();
    return a.exec();
}
