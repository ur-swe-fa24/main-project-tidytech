#ifndef TYPES_HPP
#define TYPES_HPP

#include <string>
using namespace std;

namespace types {
    // Events
    enum class Event {FiveSecReport, FinalReport, 
                        UpdateFloorNeighbors, 
                        UpdateRobotParameters, UpdateRobotError, UpdateNumFloorsClean,
                        AlertUiRobotError, DisplayText};

    // Error
    enum class ErrorType {OutOfBattery, RandomBreak};

    // Simulation Enum Classes //
    // Robot enum classes
    enum class RobotSize {Small, Medium, Large};
    enum class RobotType {Scrubber, Vaccum, Shampoo};
    enum class RobotStatus {Available, Cleaning, Traveling, Charging, NeedEmpty, Unavailable}; // You can task a robot if it is Available or Cleaning; but you cannot task it if it is unavailable

    enum class FloorSize {Small, Medium, Large};
    enum class FloorType {Wood, Tile, Carpet};
    enum class FloorRoomType {Hallway, Room, Elevator};
    enum class FloorInteraction {Low, Moderate, High};

    // String format for every enum class
    inline string to_string(ErrorType error) {
        switch(error) {
            case ErrorType::OutOfBattery:
                return "Out of Battery";
            case ErrorType::RandomBreak:
                return "Unknown Cause";
        }
    }

    inline string to_string(RobotSize size) {
        switch(size) {
            case RobotSize::Large:
                return "Large";
            case RobotSize::Medium:
                return "Medium";
            case RobotSize::Small:
                return "Small";
        }
    }

    // convert a string to enum
    inline RobotSize to_enum_robot_size(string size) {
        if (size == "Large") {
            return RobotSize::Large;
        } else if (size == "Medium") {
            return RobotSize::Medium;
        } else if (size == "Small") {
            return RobotSize::Small;
        }
    }

    inline string to_string(RobotType type) {
        switch(type) {
            case RobotType::Scrubber:
                return "Scrubber";
            case RobotType::Vaccum:
                return "Vaccum";
            case RobotType::Shampoo:
                return "Shampoo";
        }
    }

    inline RobotType to_enum_robot_type(string type) {
        if (type == "Scrubber") {
            return RobotType::Scrubber;
        } else if (type == "Vaccum") {
            return RobotType::Vaccum;
        } else if (type == "Shampoo") {
            return RobotType::Shampoo;
        }
    }

    inline string to_string(RobotStatus status) {
        switch(status) {
            case RobotStatus::Available:
                return "Available";
            case RobotStatus::Cleaning:
                return "Cleaning";
            case RobotStatus::Traveling:
                return "Traveling";
            case RobotStatus::Charging:
                return "Charging";
            case RobotStatus::NeedEmpty:
                return "NeedEmpty";
            case RobotStatus::Unavailable:
                return "Unavailable";
        }
    }

    inline RobotStatus to_enum_robot_status(string status) {
        if (status == "Available") {
            return RobotStatus::Available;
        } else if (status == "Cleaning") {
            return RobotStatus::Cleaning;
        } else if (status == "Traveling") {
            return RobotStatus::Traveling;
        } else if (status == "Charging") {
            return RobotStatus::Charging;
        } else if (status == "NeedEmpty") {
            return RobotStatus::NeedEmpty;
        } else if (status == "Unavailable") {
            return RobotStatus::Unavailable;
        }
    }

    inline string to_string(FloorSize size) {
        switch(size) {
            case FloorSize::Large:
                return "Large";
            case FloorSize::Medium:
                return "Medium";
            case FloorSize::Small:
                return "Small";
        }
    }

    inline FloorSize to_enum_floor_size(string size) {
        if (size == "Large") {
            return FloorSize::Large;
        } else if (size == "Medium") {
            return FloorSize::Medium;
        } else if (size == "Small") {
            return FloorSize::Small;
        }
    }

    inline string to_string(FloorType type) {
        switch(type) {
            case FloorType::Wood:
                return "Wood";
            case FloorType::Tile:
                return "Tile";
            case FloorType::Carpet:
                return "Carpet";
        }
    }

    inline FloorType to_enum_floor_type(string type) {
        if (type == "Wood") {
            return FloorType::Wood;
        } else if (type == "Tile") {
            return FloorType::Tile;
        } else if (type == "Carpet") {
            return FloorType::Carpet;
        }
    }

    inline string to_string(FloorRoomType roomType) {
        switch(roomType) {
            case FloorRoomType::Hallway:
                return "Hallway";
            case FloorRoomType::Room:
                return "Room";
            case FloorRoomType::Elevator:
                return "Elevator";
        }
    }

    inline FloorRoomType to_enum_floor_room_type(string roomType) {
        if (roomType == "Hallway") {
            return FloorRoomType::Hallway;
        } else if (roomType == "Room") {
            return FloorRoomType::Room;
        } else if (roomType == "Elevator") {
            return FloorRoomType::Elevator;
        }
    }

    inline string to_string(FloorInteraction interaction) {
        switch(interaction) {
            case FloorInteraction::Low:
                return "Low";
            case FloorInteraction::Moderate:
                return "Moderate";
            case FloorInteraction::High:
                return "High";
        }
    }

    inline FloorInteraction to_enum_floor_interaction(string interaction) {
        if (interaction == "Low") {
            return FloorInteraction::Low;
        } else if (interaction == "Moderate") {
            return FloorInteraction::Moderate;
        } else if (interaction == "High") {
            return FloorInteraction::High;
        }
    }

}

#endif