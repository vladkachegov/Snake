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
//    for (int ii = 0;ii<10;++ii)
//    {
//        qDebug()<< rand() % 2;
//    }

//    std::vector<AAA> d = {AAA(1),AAA(2),AAA(3),AAA(49)};
//    auto n = AAA(49);
//    auto it = std::find(d.begin(),d.end(),n);
//    bool ss = it == d.end();

//    std::vector<Node> d = {Node(1,1),Node(2,1),Node(3,12),Node(49,24)};
//    auto n = Node(49,24);
//    auto it = std::find(d.begin(),d.end(),n);
//    bool ss = it == d.end();

    MainWindow w;
    w.show();
    return a.exec();
}
