#ifndef SQLADAPTER_H
#define SQLADAPTER_H

#include <QObject>
#include "snake.h"
#include "zonemap.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>
#include <QSqlError>
#include <QDir>
class SqlAdapter : public QObject
{
    Q_OBJECT
public:
    explicit SqlAdapter(QObject *parent = nullptr);
    void prepare_entities(const ZoneMap &map,const std::vector<std::shared_ptr<Snake>> &snakes);
public slots:
    void run();
signals:
    void finished();
private:
    int save_id = 1;
    ZoneMap map;
    std::vector<std::shared_ptr<Snake>> snakes;
    const QString insert_snake_query = "INSERT INTO Snakes (snake_id,map_id,pos_x,pos_y)"
                                     "VALUES (?,?,?,?);";
    const QString insert_map_query = "INSERT INTO Nodes (map_id,obstacle_x,obstacle_y)"
                                     "VALUES (?,?,?);";

};

#endif // SQLADAPTER_H
