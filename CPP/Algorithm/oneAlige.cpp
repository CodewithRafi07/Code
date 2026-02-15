#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <memory>
#include <chrono>
#include <cmath>
#include <functional>
#include <utility>
#include <algorithm>

// Node structure to represent grid cells
struct Node {
    int x, y;           // Coordinates
    double g_cost;      // Cost from start
    double h_cost;      // Heuristic cost to end
    double f_cost;      // Total cost (g + h)
    std::shared_ptr<Node> parent;  // Parent node for path reconstruction
    
    Node(int x, int y, double g_cost, double h_cost, std::shared_ptr<Node> parent = nullptr)
        : x(x), y(y), g_cost(g_cost), h_cost(h_cost), f_cost(g_cost + h_cost), parent(parent) {}
};

// Custom comparator for the priority queue
struct NodeComparator {
    bool operator()(const std::shared_ptr<Node>& a, const std::shared_ptr<Node>& b) const {
        return a->f_cost > b->f_cost;
    }
};

// Hash function for pair<int, int> to use in unordered_map
struct pair_hash {
    template <class T1, class T2>
    std::size_t operator () (const std::pair<T1,T2> &p) const {
        auto h1 = std::hash<T1>{}(p.first);
        auto h2 = std::hash<T2>{}(p.second);
        return h1 ^ (h2 << 1);
    }
};

// A* Pathfinder class
class Pathfinder {
private:
    std::vector<std::string> grid;
    int width, height;
    std::pair<int, int> start;
    std::pair<int, int> end;
    
    // Directions for movement (up, right, down, left)
    const std::vector<std::pair<int, int>> directions = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}};
    
    // Calculate Manhattan distance heuristic
    double calculateHeuristic(int x, int y) const {
        return std::abs(x - end.first) + std::abs(y - end.second);
    }
    
public:
    Pathfinder(const std::vector<std::string>& grid) : grid(grid) {
        height = grid.size();
        width = grid[0].size();
        
        // Find start and end positions
        for (int i = 0; i < height; ++i) {
            for (int j = 0; j < width; ++j) {
                if (grid[i][j] == 'S') {
                    start = {i, j};
                } else if (grid[i][j] == 'E') {
                    end = {i, j};
                }
            }
        }
    }
    
    // A* pathfinding algorithm
    std::vector<std::pair<int, int>> findPath() {
        // Priority queue for open list (min-heap based on f-cost)
        std::priority_queue<
            std::shared_ptr<Node>, 
            std::vector<std::shared_ptr<Node>>, 
            NodeComparator
        > openList;
        
        // Closed list using unordered_map for O(1) lookups
        std::unordered_map<std::pair<int, int>, bool, pair_hash> closedList;
        
        // Keep track of nodes in open list for better performance
        std::unordered_map<std::pair<int, int>, std::shared_ptr<Node>, pair_hash> openListMap;
        
        // Create start node
        double start_h = calculateHeuristic(start.first, start.second);
        auto start_node = std::make_shared<Node>(start.first, start.second, 0, start_h);
        openList.push(start_node);
        openListMap[start] = start_node;
        
        while (!openList.empty()) {
            // Get node with lowest f-cost
            auto current = openList.top();
            openList.pop();
            
            auto current_key = std::make_pair(current->x, current->y);
            
            // Check if we've already processed this node
            if (closedList.find(current_key) != closedList.end()) {
                continue;
            }
            
            // Mark current node as visited
            closedList[current_key] = true;
            
            // Check if we've reached the end
            if (current->x == end.first && current->y == end.second) {
                // Reconstruct path
                std::vector<std::pair<int, int>> path;
                std::shared_ptr<Node> node = current;
                
                while (node != nullptr) {
                    path.emplace_back(node->x, node->y);
                    node = node->parent;
                }
                
                // Reverse path to get start->end order
                std::reverse(path.begin(), path.end());
                return path;
            }
            
            // Explore neighbors
            for (const auto& dir : directions) {
                int new_x = current->x + dir.first;
                int new_y = current->y + dir.second;
                auto new_key = std::make_pair(new_x, new_y);
                
                // Check if new position is valid
                if (new_x < 0 || new_x >= height || new_y < 0 || new_y >= width) {
                    continue;
                }
                
                // Check if new position is an obstacle or already visited
                if (grid[new_x][new_y] == 'X' || closedList.find(new_key) != closedList.end()) {
                    continue;
                }
                
                // Calculate costs for neighbor
                double new_g_cost = current->g_cost + 1;  // Each step costs 1
                double new_h_cost = calculateHeuristic(new_x, new_y);
                double new_f_cost = new_g_cost + new_h_cost;
                
                // Check if this node is already in the open list with a lower cost
                bool in_open_list = false;
                if (openListMap.find(new_key) != openListMap.end()) {
                    auto existing_node = openListMap[new_key];
                    if (new_g_cost < existing_node->g_cost) {
                        // Update the existing node
                        existing_node->g_cost = new_g_cost;
                        existing_node->f_cost = new_f_cost;
                        existing_node->parent = current;
                        in_open_list = true;
                    } else {
                        continue;  // Skip if the new path is not better
                    }
                }
                
                // Create neighbor node if not already in open list
                if (!in_open_list) {
                    auto neighbor = std::make_shared<Node>(new_x, new_y, new_g_cost, new_h_cost, current);
                    
                    // Add to open list
                    openList.push(neighbor);
                    openListMap[new_key] = neighbor;
                }
            }
        }
        
        // No path found
        return {};
    }
    
    // Print grid with path
    void printGridWithPath(const std::vector<std::pair<int, int>>& path) const {
        std::vector<std::string> result = grid;
        
        // Mark path on grid (excluding start and end)
        for (const auto& pos : path) {
            if (grid[pos.first][pos.second] != 'S' && grid[pos.first][pos.second] != 'E') {
                result[pos.first][pos.second] = '#';
            }
        }
        
        // Print the grid
        for (const auto& row : result) {
            for (char cell : row) {
                std::cout << cell << " ";
            }
            std::cout << std::endl;
        }
    }
};

int main() {
    // Hardcoded map for testing
    std::vector<std::string> test_map = {
        "S . . . .",
        ". X X X .",
        ". . . X .",
        ". X . X .",
        ". . . . E"
    };
    
    // Process the map to remove spaces
    for (auto& row : test_map) {
        row.erase(std::remove(row.begin(), row.end(), ' '), row.end());
    }
    
    // Create pathfinder
    Pathfinder pathfinder(test_map);
    
    // Benchmark the pathfinding
    auto start_time = std::chrono::high_resolution_clock::now();
    std::vector<std::pair<int, int>> path = pathfinder.findPath();
    auto end_time = std::chrono::high_resolution_clock::now();
    
    // Calculate duration in microseconds
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
    
    // Print results
    if (path.empty()) {
        std::cout << "No path found!" << std::endl;
    } else {
        std::cout << "Path found!" << std::endl;
        std::cout << "Path length: " << path.size() << " steps" << std::endl;
        std::cout << "Execution time: " << duration.count() << " microseconds" << std::endl;
        std::cout << "\nGrid with path:" << std::endl;
        pathfinder.printGridWithPath(path);
    }
    
    return 0;
}