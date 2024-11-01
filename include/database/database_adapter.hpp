#ifndef DATABASE_DATABASE_ADAPTER_HPP
#define DATABASE_DATABASE_ADAPTER_HPP
#pragma once
#include <bsoncxx/builder/basic/document.hpp>
#include <bsoncxx/json.hpp>
using namespace std;

class DatabaseAdapter {
    public: 
        virtual ~DatabaseAdapter() = default;
        virtual void connect(const std::string& db_name, const std::string& collection_name) = 0;
        virtual void insertDocument(const bsoncxx::document::view_or_value& document) = 0;
        virtual bsoncxx::document::value findDocument(const bsoncxx::document::view_or_value& filter) = 0;
};

#endif


