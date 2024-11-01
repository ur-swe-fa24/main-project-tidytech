#ifndef DATABASE_DB_MANAGER_HPP
#define DATABASE_DB_MANAGER_HPP
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>

class DBManager {
    public:


    private:
        mongocxx::instance mongo_instance_;
        mongocxx::database database_;
        mongocxx::client client_;

};




#endif
