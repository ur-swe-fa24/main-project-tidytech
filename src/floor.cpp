#include "simulation/floor.hpp"
#include "spdlog/spdlog.h"

int Floor::floorCreationCount = 1;

Floor::Floor(FloorRoomType room, FloorType floortype, FloorSize size, FloorInteraction interaction_level, bool restriction) {
    id_ = Floor::floorCreationCount++;
    room_ = room;
    floortype_ = floortype;
    size_ = size;
    interaction_ = interaction_level;
    restricted_ = restriction;
    clean_level_ = 100;
    getting_clean_ = false;
}

// Floor getting dirty for simulation
void Floor::get_dirty() {
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
void Floor::get_clean(const RobotSize robot_size) {
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