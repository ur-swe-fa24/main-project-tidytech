#include "simulation/floor.hpp"
#include "spdlog/spdlog.h"

int Floor::floorCreationCount = 1;

Floor::Floor(int id, FloorRoomType room, FloorType floortype, FloorSize size, FloorInteraction interaction_level, bool restriction, int clean_level) {
    //id_ = Floor::floorCreationCount++;
    id_ = id;
    room_ = room;
    floortype_ = floortype;
    size_ = size;
    interaction_ = interaction_level;
    restricted_ = restriction;
    // clean_level_ = 100;
    clean_level_ = clean_level;
    getting_clean_ = false;
}

std::string Floor::to_string() const {
    std::string str_restriction = "Not Restricted";
    std::string str_getting_clean = "Not At This Time";
    
    if (restricted_) {str_restriction = "Restricted";}
    if (getting_clean_) {str_getting_clean = "Right Now";}
    

    return "Floor Id: " + std::to_string(id_) +
            ", Size: " + types::to_string(size_) +
            ", RoomType: " + types::to_string(room_) +
            ", FloorType: " + types::to_string(floortype_) + "\n" +
            "Interaction Level: " + types::to_string(interaction_) +
            ", Restriction: " + str_restriction + "\n" +
            "Clean Level: " + std::to_string(clean_level_) +
            ", Gettting Clean: " + str_getting_clean + "\n";
}


// Floor getting dirty for simulation
void Floor::dirty() {
    if (!getting_clean_) {
        switch (interaction_) {
            case FloorInteraction::Low:
                clean_level_ = std::max(0, clean_level_-2);
                break;
            case FloorInteraction::Medium:
                clean_level_ = std::max(0, clean_level_-4);
                break;
            case FloorInteraction::High:
                clean_level_ = std::max(0, clean_level_-8);
                break;
        }
    } else {
        spdlog::info("Floor {} is getting clean.", id_);
    }
    
}

// Floor getting cleaned by different size robot
void Floor::clean(const RobotSize robot_size) {
    getting_clean_ = true;
    switch (robot_size) {
        case RobotSize::Small:
            switch (size_) {
                case FloorSize::Small:
                    clean_level_ = std::min(100, clean_level_+10);
                    break;
                case FloorSize::Medium:
                    clean_level_ = std::min(100, clean_level_+7);
                    break;
                case FloorSize::Large:
                    clean_level_ = std::min(100, clean_level_+4);
                    break;
            }
            break;
        case RobotSize::Medium:
            switch (size_) {
                case FloorSize::Small:
                    clean_level_ = std::min(100, clean_level_+20);
                    break;
                case FloorSize::Medium:
                    clean_level_ = std::min(100, clean_level_+14);
                    break;
                case FloorSize::Large:
                    clean_level_ = std::min(100, clean_level_+8);
                    break;
            }
            break;
        case RobotSize::Large:
            switch (size_) {
                case FloorSize::Small:
                    clean_level_ = std::min(100, clean_level_+30);
                    break;
                case FloorSize::Medium:
                    clean_level_ = std::min(100, clean_level_+21);
                    break;
                case FloorSize::Large:
                    clean_level_ = std::min(100, clean_level_+12);
                    break;
            }
            break;
    }
}