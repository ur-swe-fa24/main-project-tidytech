#ifndef TYPES_HPP
#define TYPES_HPP

namespace types {
    // Simulation Enum Classes //
    // Robot enum classes
    enum class RobotSize {Small, Medium, Large};
    enum class RobotType {Scrubber, Vaccum, Shampoo};
    enum class RobotStatus {Available, Cleaning, Charging, Unavailable}; // You can task a robot if it is Available or Cleaning; but you cannot task it if it is unavailable

    enum class FloorSize {Small, Medium, Large};
    enum class FloorType {Wood, Tile, Carpet};
    enum class FloorRoomType {Hallway, Room, Elevator};
    enum class FloorInteraction {Low, Medium, High};

    // String format for every enum class
    std::string to_string(RobotSize size) {
        switch(size) {
            case RobotSize::Large:
                return "Large";
            case RobotSize::Medium:
                return "Medium";
            case RobotSize::Small:
                return "Small";
        }
    }

    std::string to_string(RobotType type) {
        switch(type) {
            case RobotType::Scrubber:
                return "Scrubber";
            case RobotType::Vaccum:
                return "Vaccum";
            case RobotType::Shampoo:
                return "Shampoo";
        }
    }

    std::string to_string(RobotStatus status) {
        switch(status) {
            case RobotStatus::Available:
                return "Available";
            case RobotStatus::Cleaning:
                return "Cleaning";
            case RobotStatus::Charging:
                return "Charging";
            case RobotStatus::Unavailable:
                return "Unavailable";
        }
    }

    std::string to_string(FloorSize size) {
        switch(size) {
            case FloorSize::Large:
                return "Large";
            case FloorSize::Medium:
                return "Medium";
            case FloorSize::Small:
                return "Small";
        }
    }

    std::string to_string(FloorType type) {
        switch(type) {
            case FloorType::Wood:
                return "Wood";
            case FloorType::Tile:
                return "Tile";
            case FloorType::Carpet:
                return "Carpet";
        }
    }

    std::string to_string(FloorRoomType roomType) {
        switch(roomType) {
            case FloorRoomType::Hallway:
                return "Hallway";
            case FloorRoomType::Room:
                return "Room";
            case FloorRoomType::Elevator:
                return "Elevator";
        }
    }

    std::string to_string(FloorInteraction interaction) {
        switch(interaction) {
            case FloorInteraction::Low:
                return "Low";
            case FloorInteraction::Medium:
                return "Medium";
            case FloorInteraction::High:
                return "High";
        }
    }


}


#endif