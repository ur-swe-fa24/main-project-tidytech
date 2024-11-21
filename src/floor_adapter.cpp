#include "database/floor_adapter.hpp"
#include <bsoncxx/builder/basic/array.hpp>
#include <bsoncxx/builder/basic/document.hpp>
#include <bsoncxx/json.hpp>
#include <mongocxx/stdx.hpp>

using bsoncxx::builder::basic::make_document;
using bsoncxx::builder::basic::kvp;

void FloorAdapter::insertFloor(const std::string& id, const std::string& name, const std::string& roomType, const std::string& floortype,
                               const std::string& size, const std::string& interaction, const std::string& restricted,
                               const std::string& clean_level, const std::vector<int>& neighbors) {
    // find the entry that has the given id
    auto query_doc = bsoncxx::builder::basic::make_document(
        bsoncxx::builder::basic::kvp("_id", id)
    );
    auto existing_doc = collection_.find_one(query_doc.view());
    // find the entry that has the given name
    auto query_name = bsoncxx::builder::basic::make_document(
        bsoncxx::builder::basic::kvp("name", name)
    );
    auto existing_name = collection_.find_one(query_name.view());
    
    //checking if the id is already in the databse and checking if the name is an empty string
    if (existing_doc || name == "") {
        //throw error
        throw std::invalid_argument("the floor cannot be added to database because of duplicate id");
    } 
    //if the name is already in the database throw error
    else if (existing_name){
        throw std::invalid_argument("the floor cannot be added to database because of duplicate name");
    } else {
        bsoncxx::builder::basic::array neighbors_array;
        for (const auto& neighbor : neighbors) {
            neighbors_array.append(neighbor);
        };
        auto floor_doc = make_document(
            kvp("_id", id),
            kvp("name", name),
            kvp("room_type", roomType),
            kvp("floor_type", floortype),
            kvp("size", size),
            kvp("interaction_level", interaction),
            kvp("restricted", restricted),
            kvp("clean_level", clean_level),
            kvp("neighbors", neighbors_array));
        collection_.insert_one(floor_doc.view());
    }
}

std::optional<bsoncxx::document::value> FloorAdapter::findDocumentById(const std::string& floorId) {
    // build the query doc
    auto query_doc = make_document(kvp("_id", floorId));
    mongocxx::stdx::optional<bsoncxx::document::value> result = collection_.find_one(query_doc.view());

    // Check if the result contains a document
    if (result) {
        return std::optional<bsoncxx::document::value>{std::move(*result)};
    } else {
        return std::nullopt;
    }
}

//update clean level
bool FloorAdapter::updateCleanLevel(const std::string& floorId, const std::string& newCleanLevel) {
    auto query_doc = make_document(kvp("_id", floorId));
    auto update_doc = make_document(kvp("$set", make_document(kvp("clean_level", newCleanLevel))));

    auto result = collection_.update_one(query_doc.view(), update_doc.view());
    return result && result->modified_count() > 0;
}

//update floor restrictions
bool FloorAdapter::updateRestriction(const std::string& floorId, const std::string& restricted) {
    auto query_doc = make_document(kvp("_id", floorId));
    auto update_doc = make_document(kvp("$set", make_document(kvp("restricted", restricted))));

    auto result = collection_.update_one(query_doc.view(), update_doc.view());
    return result && result->modified_count() > 0;
}

//delete floor
bool FloorAdapter::deleteFloor(const std::string& floorId) {
    auto query_doc = make_document(kvp("_id", floorId));

    auto result = collection_.delete_one(query_doc.view());
    return result && result->deleted_count() > 0;
}

std::vector<bsoncxx::document::value> FloorAdapter::getAllFloors() {
    std::vector<bsoncxx::document::value> floors;
    try {
        auto cursor = collection_.find({});
        for (const auto& doc : cursor) {
            floors.emplace_back(bsoncxx::document::value(doc));
        }
    } catch (const std::exception& e) {
        std::cerr << "Error fetching all floors: " << e.what() << std::endl;
    }
    return floors;
}