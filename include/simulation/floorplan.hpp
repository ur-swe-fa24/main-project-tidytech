#ifndef SIMULATION_FLOORPLAN_HPP
#define SIMULATION_FLOORPLAN_HPP

#include <string>
#include <vector>
#include <unordered_map>
#include <queue>
#include "floor.hpp"
#include "types/types.hpp"

using namespace types;

class FloorPlan {
    public:
        FloorPlan() {}; // Default Constructor
        ~FloorPlan() {}; // Destructor

        std::vector<Floor> get_all_floor() const;
        Floor access_floor(int floor_id);
        std::string to_string() const; // Graph info
        std::string floor_to_string(const Floor& floor) const; // Floor info

        void add_floor(const Floor& floor, const std::vector<Floor> neighbors);
        void remove_floor(const Floor& floor);
        std::vector<Floor> get_neighbors(const Floor& floor) const;
        void update_floor_neighbors(const Floor& floor, const std::vector<Floor> neighbors);
        void update_floor(const Floor& floor);

        std::queue<int> get_path(int floor_id_one, int floor_id_two);


    private:
        std::unordered_map<Floor, std::vector<Floor>> floorgraph_;
        void update_neighbors(const Floor& floor, bool add);
        bool floor_in_frontier(std::queue<Floor> queue, const Floor& floor);
        bool floor_visited(std::vector<Floor> visited, const Floor& check_floor);
        std::queue<int> get_shortest_path(std::unordered_map<int,int> track_path, int floor_id_goal);
};

#endif