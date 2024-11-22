#ifndef DATABASE_FLOOR_ADAPTER_HPP
#define DATABASE_FLOOR_ADAPTER_HPP

#include <bsoncxx/builder/basic/document.hpp>
#include <bsoncxx/document/value.hpp>
#include <mongocxx/collection.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/stdx.hpp>
#include <string>
#include "simulation/floor.hpp"

class FloorAdapter {
public:
    // Constructor to initialize the collection
    FloorAdapter(mongocxx::collection collection) : collection_(std::move(collection)) {}

    // Insert a new floor document
    void insertFloor(const std::string& id, const std::string& name, const std::string& roomType, const std::string& floortype, const std::string& size,
                     const std::string& interaction, const std::string& restricted, const std::string& clean_level, const std::vector<int>& neighbors);
    

    // Find a document by floor ID
    std::optional<bsoncxx::document::value> findDocumentById(const std::string& floorId);

    bool updateCleanLevel(const std::string& floorId, const std::string& newCleanLevel);
    bool updateRestriction(const std::string& floorId, const std::string& restricted);
    bool updateNeighbors(const std::string& floorId, const std::vector<int>& neighbors);
    bool deleteFloor(const std::string& floorId);

    std::vector<bsoncxx::document::value> getAllFloors();

private:
    mongocxx::collection collection_;
};

#endif