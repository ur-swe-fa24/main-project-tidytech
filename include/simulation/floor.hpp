#ifndef SIMULATION_FLOOR_HPP
#define SIMULATION_FLOOR_HPP

#include <string>
#include <vector>

class Floor {
    public:
        enum class Size {Small, Medium, Large};
        enum class FloorType {Wood, Tile, Carpet};
        enum class RoomType {Hallway, Room, Elevator};
        enum class Interaction {Low, Medium, High};

        Floor(std::string id, std::string room, std::string floortype, std::string size, std::string interaction_level, bool restriction);
        ~Floor() {};

        void set_room(std::string room);
        void set_floortype(std::string floortype);
        void set_size(std::string size);
        void set_interaction(std::string interaction);
        void set_restriction(bool restriction) {restricted_ = restriction;};
        void set_getting_clean(bool getting_clean) {getting_clean_ = getting_clean;};

        std::string get_id() {return id_;};
        int get_clean_level() {return clean_level_;};

        void get_dirty();
        void get_clean(std::string robot_size);
        bool is_clean() {return clean_level_==100;};
        

    private:
        std::string id_;
        Floor::RoomType room_;
        Floor::FloorType floortype_;
        Floor::Size size_;
        Floor::Interaction interaction_;
        int clean_level_;
        bool restricted_;
        bool getting_clean_;

};

#endif