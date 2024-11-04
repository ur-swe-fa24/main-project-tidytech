#ifndef SIMULATION_FLOOR_HPP
#define SIMULATION_FLOOR_HPP

#include <string>
#include <vector>
#include "types/types.hpp"

using namespace types;

class Floor {
    public:
        static int floorCreationCount; // For id

        Floor(int id, FloorRoomType room, FloorType floortype, FloorSize size, FloorInteraction interaction_level, bool restriction, int clean_level);
        ~Floor() {};

        bool operator==(const Floor& other) const {return id_ == other.id_;} // Overriding Floor comparison

        void set_room(const FloorRoomType room) {room_ = room;};
        void set_floortype(const FloorType floortype) {floortype_ = floortype;};
        void set_size(const FloorSize size) {size_ = size;};
        void set_interaction(const FloorInteraction interaction) {interaction_ = interaction;};
        void set_restriction(const bool restriction) {restricted_ = restriction;};
        void set_getting_clean(const bool getting_clean) {getting_clean_ = getting_clean;};

        int get_id() const {return id_;};
        int get_clean_level() const {return clean_level_;};
        bool get_getting_clean() const {return getting_clean_;};
        std::string to_string() const;

        void dirty();
        void clean(const RobotSize robot_size);
        bool is_clean() const {return clean_level_==100;};
        

    private:
        int id_;
        FloorRoomType room_;
        FloorType floortype_;
        FloorSize size_;
        FloorInteraction interaction_;
        int clean_level_;
        bool restricted_;
        bool getting_clean_;

};

#endif