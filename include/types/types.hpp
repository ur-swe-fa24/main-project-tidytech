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
}


#endif