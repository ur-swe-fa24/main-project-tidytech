#include "database/db_manager.hpp"

mongocxx::database DBManager::getDatabase() const {
    return database_;
}
