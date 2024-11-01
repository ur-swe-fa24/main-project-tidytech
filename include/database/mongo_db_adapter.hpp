#ifndef DATABASE_MONGO_DB_ADAPTER_HPP
#define DATABASE_MONGO_DB_ADAPTER_HPP
#pragma once
#include "DatabaseAdapter.hpp"
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/stdx.hpp>
#include <mongocxx/uri.hpp>
using namespace std;

class MongoDBAdapter : DatabaseAdapter {
    public:

    private:
        mongocxx::instance mongo_instance_;
        mongocxx::database database_;
        mongocxx::client client_;
    


};



#endif