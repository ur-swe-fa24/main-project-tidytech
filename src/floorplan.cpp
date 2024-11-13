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

// Access a Floor with int
Floor FloorPlan::access_floor(int floor_id) {
    for (const auto& pair : floorgraph_) {
        if (floor_id == pair.first.get_id()) {
            return pair.first;
        }
    }
    throw std::runtime_error("Floor not found");
}

// to_string for FloorPlan
std::string FloorPlan::to_string() const {
    std::string out_str = "FloorPlan: \n";
    for (const auto& pair : floorgraph_) {
        out_str += "Id: " + std::to_string(pair.first.get_id()) + ", Adjacent Floors: ";
        std::string neighbors_str = "";
        for (const Floor& neighbor : pair.second) {
            neighbors_str += std::to_string(neighbor.get_id()) + ", ";
        }
        if (neighbors_str.length() >= 2) {
        neighbors_str.erase(neighbors_str.length() - 2, 2); // Remove last two characters
        }
        out_str += neighbors_str + "\n";
    }
    return out_str;
}

// to_string for Floor
std::string FloorPlan::floor_to_string(const Floor& floor) const {
    std::string out_str = floor.to_string();
    std::string neighbors_str = "";
    std::vector<Floor> neighbors = floorgraph_.at(floor);
    for (const auto& neighbor : neighbors) {
        neighbors_str += neighbor.get_name() + ", ";
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
        update_neighbors(floor, true); // Update all other neighbors
    } else {
        // Floor already exist in the graph
        spdlog::error("Floor {} already exist in the Floorplan", floor.get_id());
    }
}

// Remove floor from the FloorPlan
void FloorPlan::remove_floor(const Floor& floor) {
    // Check if there is any robot in the floor
    auto remove = floorgraph_.find(floor);
    if (remove != floorgraph_.end()) {
        update_neighbors(floor, false); // Update all other neighbors
        floorgraph_.erase(remove);
    } else {
        // Floor does not exist in the graph
        spdlog::error("Floor {} does not exist in the Floorplan", floor.get_id());
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
        for (Floor& modify_floor : floors_to_modify) {
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

// Get the shortest path between two floors and return a queue of floor ids
std::queue<int> FloorPlan::get_path(int floor_id_start, int floor_id_goal) {
    std::queue<Floor> frontier; // Queue
    std::unordered_map<int, int> track_path;

    // Initialization
    track_path[floor_id_start] = -1; // Starting point with -1 as its parent id
    for (const auto& pair : floorgraph_) {
        if (pair.first.get_id() == floor_id_start) {
            frontier.push(pair.first); // Push the start node into the frontier
        }
    }

    while (frontier.size() > 0) {
        int curr_floor_id = frontier.front().get_id();
        if (curr_floor_id == floor_id_goal) {
            return get_shortest_path(track_path, floor_id_goal);
        } else {
            for (const auto& pair : floorgraph_) {
                if (pair.first.get_id() == curr_floor_id) {
                    for (const Floor& floor : pair.second) {
                        if (floor_in_frontier(frontier, floor)) {
                            // Push all its neighbors nodes onto the frontier
                            frontier.push(floor);
                            track_path[floor.get_id()] = curr_floor_id; // Set parent for all of these floors
                        }
                    }
                } else {
                    // Cannot find the start node
                    spdlog::error("Floor {} is not in the FloorPlan.", floor_id_start);
                }
            }
        }
        frontier.pop(); // Remove the current floor
    }
    return std::queue<int>(); // Return empty queue if path not found
}

bool FloorPlan::floor_in_frontier(std::queue<Floor> queue, const Floor& check_floor) {
    // Check if check_floor is in queue
    while (!queue.empty()) {
        if (check_floor == queue.front()) {
            return true;
        }
        queue.pop();
    }

    return false;
}


std::queue<int> FloorPlan::get_shortest_path(std::unordered_map<int,int> track_path, int floor_id_goal) {
    // Trace the path
    std::vector<int> path_backward; // stack
    int curr = floor_id_goal;
    while (curr != -1) {
        path_backward.push_back(curr);
        curr = track_path[curr];
    }

    std::queue<int> path_forward;
    while(!path_backward.empty()) {
        path_forward.push(path_backward.back());
        path_backward.pop_back();
    }

    return path_forward;
    
}
