#ifndef DATABASE_DATABASE_HPP
#define DATABASE_DATABASE_HPP

#include <unordered_map>
#include <vector>
#include <string>
using namespace std;

class Database {
    public:
        Database() {};
        
        // Create
        void add_robot(int id, const string& type, int available, int location);
        void add_floor(int id, const string& name, const string& type);
        void add_task(int id, const vector<int>& robot_assigned, const vector<int>& room_assigned, const string& status);
        
        // Read
        vector<string> get_robot(int id);
        vector<string> get_floor(int id);
        vector<string> get_task(int id);
        
        // Update 
        void update_robot(int id, const string& type, int available, int location);
        void update_floor(int id, const string& name, const string& type);
        void update_task(int id, const vector<int>& robot_assigned, const vector<int>& room_assigned, const string& status);
        
        // Delete
        void delete_robot(int id);
        void delete_floor(int id);
        void delete_task(int id);
        
    private:
        unordered_map<int, vector<string>> robots_;  
        unordered_map<int, vector<string>> floors_;  
        unordered_map<int, vector<string>> tasks_;  
};

#endif
