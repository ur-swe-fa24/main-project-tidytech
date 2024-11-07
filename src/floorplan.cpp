#include "simulation/floorplan.hpp"
#include "spdlog/spdlog.h"

// Return all the floors in the FloorPlan 
std::vector<Floor> FloorPlan::get_all_floor() const {
    std::vector<Floor> floors;
    for (const auto& pair : floorgraph_) {
        floors.push_back(pair.first);
    }
    return floors;
}

// to_string for FloorPlan
std::string FloorPlan::to_string() const {
    std::string out_str = "FloorPlan: \n";
    for (const auto& pair : floorgraph_) {
        out_str += "Id: " + std::to_string(pair.first.get_id()) + ", Number of Robots: " + std::to_string(get_num_robots(pair.first)) + ", Adjacent Floors: ";
        std::string neighbors_str = "";
        for (const auto& neighbor : pair.second) {
            neighbors_str += std::to_string(neighbor.get_id()) + ", ";
        }
        if (neighbors_str.length() >= 2) {
        neighbors_str.erase(neighbors_str.length() - 2, 2); // Remove last two characters
        }
        out_str += neighbors_str = "\n";
    }
    return out_str;
}

// to_string for Floor
std::string FloorPlan::floor_to_string(const Floor& floor) const {
    std::string out_str = floor.to_string();
    std::string neighbors_str = "";
    std::vector<Floor> neighbors = floorgraph_.at(floor);
    for (const auto& neighbor : neighbors) {
        neighbors_str += std::to_string(neighbor.get_id()) + ", ";
    }

    if (neighbors_str.length() >= 2) {
        neighbors_str.erase(neighbors_str.length() - 2, 2); // Remove last two characters
    }

    return out_str + "Adjacent Floors: [" + neighbors_str + "]";

}

// Add floor to the FloorPlan
void FloorPlan::add_floor(const Floor& floor, const std::vector<Floor> neighbors) {
    if (floorgraph_.count(floor) != 1) {
        floorgraph_[floor] = neighbors;
        floor_to_robots_[floor] = std::vector<RobotSize>(); // add to floor_to_robots
        update_neighbors(floor, true); // Update all other neighbors
    } else {
        // Floor already exist in the graph
        spdlog::error("Floor {} already exist in the Floorplan", floor.get_id());
    }
}

// Remove floor from the FloorPlan
void FloorPlan::remove_floor(const Floor& floor) {
    // Check if there is any robot in the floor
    auto remove_floor_to_robot = floor_to_robots_.find(floor);
    if ((remove_floor_to_robot != floor_to_robots_.end()) && (remove_floor_to_robot->second.size() == 0)) {
        auto remove = floorgraph_.find(floor);
        if (remove != floorgraph_.end()) {
            update_neighbors(floor, false); // Update all other neighbors
            floorgraph_.erase(remove);
            floor_to_robots_.erase(remove_floor_to_robot);
        } else {
            // Floor does not exist in the graph
            spdlog::error("Floor {} does not exist in the Floorplan", floor.get_id());
        }
    } else {
        // Floor does not exist in the floor_to_robots or there are robots in the floor
        spdlog::error("Floor {} either has robots or does not exist in the Floor_to_Robot", floor.get_id());
    }
}

// Return the adjacent floors of the floor
std::vector<Floor> FloorPlan::get_neighbors(const Floor& floor) const {
    return floorgraph_.at(floor);
}

// Modify the neighbors of the Floor
void FloorPlan::update_floor_neighbors(const Floor& floor, const std::vector<Floor> neighbors) {
    auto update = floorgraph_.find(floor);
    if (update != floorgraph_.end()) {
        update_neighbors(floor, false); // Update all other neighbors
        update->second = neighbors;
    } else {
        // Floor does not exist in the graph
        spdlog::error("Floor {} does not exist in the Floorplan", floor.get_id());
    }
}

// Update the neighbors from either adding or removing floor
void FloorPlan::update_neighbors(const Floor& floor, bool add) {
    std::vector<Floor> floors_to_modify = floorgraph_[floor];
    if (add) {
        // append floor to every neighbor's vector
        for (Floor modify_floor : floors_to_modify) {
            floorgraph_[modify_floor].push_back(floor);
        }

    } else {
        // remove floor from every neighbor's vector
        for (Floor modify_floor : floors_to_modify) {
            int pos = 0;
            std::vector<Floor> modify_neighbors = floorgraph_[modify_floor];
            for (int i = 0; i < modify_neighbors.size(); pos++) {
                if (modify_neighbors[pos] == floor) {
                    break;
                }
                pos++;
            }
            if (pos != modify_neighbors.size()) {
                auto it = floorgraph_[modify_floor].begin() + pos; // iterator to that index of pos
                floorgraph_[modify_floor].erase(it);
            }
        }
    }
}

// Add robotsize to the floor_to_robots_
void FloorPlan::add_robot_to_floor(const Floor& floor, const RobotSize robot_size) {
    floor_to_robots_.at(floor).push_back(robot_size);
}

// Remove an instance of robotsize from the floor_to_robots_
void FloorPlan::remove_robot_from_floor(const Floor& floor, const RobotSize robot_size) {
    auto remove = floor_to_robots_.find(floor);
    if (remove != floor_to_robots_.end()) {
        floor_to_robots_.erase(remove);
    } else {
        // RobotSize does not exist in the Floor
        spdlog::error("RobotSize does not exist in the Floor {}", std::to_string(floor.get_id()));
    }
}