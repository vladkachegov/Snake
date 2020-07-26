#include "sqladapter.h"

SqlAdapter::SqlAdapter(QObject *parent) : QObject(parent)
{

}

void SqlAdapter::prepare_entities(const ZoneMap &map, const std::vector<std::shared_ptr<Snake> > &snakes)
{
    this->map = map;
    this->snakes = snakes;
}

void SqlAdapter::run()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(QDir::currentPath() + "//SnakeDb.db");
    auto connection_name = db.connectionName();
    db.open();
    Q_FOREACH(auto node,map.get_obstacles())
    {
        QSqlQuery sql_query;
        sql_query.prepare(insert_map_query);
        sql_query.bindValue(0,save_id);
        sql_query.bindValue(1,node.x);
        sql_query.bindValue(2,node.y);
        sql_query.exec();
    }

    Q_FOREACH(auto snake,snakes)
    {
        int snake_id = snake->get_id();
        QSqlQuery sql_query;
        for (auto node : snake->get_snake_pos())
        {
            sql_query.prepare(insert_snake_query);
            sql_query.bindValue(0,snake_id);
            sql_query.bindValue(1,save_id);
            sql_query.bindValue(2,node.x);
            sql_query.bindValue(3,node.y);
            sql_query.exec();
        }
    }
    db.close();
    ++save_id;
    snakes.clear();
    emit finished();
}
