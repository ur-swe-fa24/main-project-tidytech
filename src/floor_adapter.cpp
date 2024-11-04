#include "database/floor_adapter.hpp"
#include <bsoncxx/builder/basic/array.hpp>
#include <bsoncxx/builder/basic/document.hpp>
#include <bsoncxx/json.hpp>
#include <mongocxx/stdx.hpp>

using bsoncxx::builder::basic::make_document;
using bsoncxx::builder::basic::kvp;

void FloorAdapter::insertFloor(const std::string& id, const std::string& room, const std::string& floortype,
                               const std::string& size, const std::string& interaction, const std::string& restricted,
                               const std::string& clean_level) {
    auto query_doc = bsoncxx::builder::basic::make_document(
        bsoncxx::builder::basic::kvp("_id", id)
    );
    auto existing_doc = collection_.find_one(query_doc.view());
    if (!existing_doc) {
        auto floor_doc = make_document(
            kvp("_id", id),
            kvp("room", room),
            kvp("floorType", floortype),
            kvp("size", size),
            kvp("interactionLevel", interaction),
            kvp("restricted", restricted),
            kvp("cleanLevel", clean_level));

    // insert the doc into the collection
        collection_.insert_one(floor_doc.view());
    } else {
        throw std::invalid_argument("the floor has been added to database already");
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
    auto update_doc = make_document(kvp("$set", make_document(kvp("cleanLevel", newCleanLevel))));

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
