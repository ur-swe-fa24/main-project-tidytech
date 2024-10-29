#include "simulation/floor.hpp"
#include "spdlog/spdlog.h"

Floor::Floor(std::string id, std::string room, std::string floortype, std::string size, std::string interaction_level, bool restriction) {
    id_ = id;
    set_room(room);
    set_floortype(floortype);
    set_size(size);
    set_interaction(interaction_level);
    restricted_ = restriction;
    clean_level_ = 100;
    getting_clean_ = false;
}

// Set room with string
void Floor::set_room(std::string room) {
    if (room == "hallway") {
        room_ = Floor::RoomType::Hallway;
    } else if (room == "room") {
        room_ = Floor::RoomType::Room;
    } else if (room == "elevator") {
        room_ = Floor::RoomType::Elevator;
    } else {
        // Invalid room
        spdlog::error("{} is an invalid Floor room", room);
        return;
    }
}

// Set floortype with string
void Floor::set_floortype(std::string floortype) {
    if (floortype == "wood") {
        floortype_ = Floor::FloorType::Wood;
    } else if (floortype == "tile") {
        floortype_ = Floor::FloorType::Tile;
    } else if (floortype == "carpet") {
        floortype_ = Floor::FloorType::Carpet;
    } else {
        // Invalid floortype
        spdlog::error("{} is an invalid Floor floortype", floortype);
        return;
    }
}

// Set size with string
void Floor::set_size(std::string size) {
    if (size == "small") {
        size_ = Floor::Size::Small;
    } else if (size == "medium") {
        size_ = Floor::Size::Medium;
    } else if (size == "large") {
        size_ = Floor::Size::Large;
    } else {
        // Invalid Size
        spdlog::error("{} is an invalid Floor size", size);
        return;
    }
}

// Set interaction with string
void Floor::set_size(std::string interaction) {
    if (interaction == "low") {
        interaction_ = Floor::Interaction::Low;
    } else if (interaction == "medium") {
        interaction_ = Floor::Interaction::Medium;
    } else if (interaction == "large") {
        interaction_ = Floor::Interaction::High;
    } else {
        // Invalid Size
        spdlog::error("{} is an invalid Floor interaction", interaction);
        return;
    }
}

// Floor getting dirty for simulation
void Floor::get_dirty() {
    if (!getting_clean_) {
        switch (interaction_) {
        case Interaction::Low:
            clean_level_ = std::max(0, clean_level_-2);
            break;
        case Interaction::Medium:
            clean_level_ = std::max(0, clean_level_-4);
            break;
        case Interaction::High:
            clean_level_ = std::max(0, clean_level_-8);
            break;
        }
    } else {
        spdlog::info("Floor {} is getting clean.", id_);
    }
    
}

// Floor getting cleaned by different size robot
void Floor::get_clean(std::string robot_size) {
    getting_clean_ = true;
    if (robot_size == "small") {
        switch (size_) {
            case Size::Small:
                clean_level_ = std::min(100, clean_level_+10);
                break;
            case Size::Medium:
                clean_level_ = std::min(100, clean_level_+7);
                break;
            case Size::Large:
                clean_level_ = std::min(100, clean_level_+4);
                break;
        }
    } else if (robot_size == "medium") {
        switch (size_) {
            case Size::Small:
                clean_level_ = std::min(100, clean_level_+20);
                break;
            case Size::Medium:
                clean_level_ = std::min(100, clean_level_+14);
                break;
            case Size::Large:
                clean_level_ = std::min(100, clean_level_+8);
                break;
        }

    } else if (robot_size == "large") {
        switch (size_) {
            case Size::Small:
                clean_level_ = std::min(100, clean_level_+30);
                break;
            case Size::Medium:
                clean_level_ = std::min(100, clean_level_+21);
                break;
            case Size::Large:
                clean_level_ = std::min(100, clean_level_+12);
                break;
        }

    } else {
        spdlog::error("Invalid robot size to clean floor.");
        getting_clean_ = false;
        return;
    }
}