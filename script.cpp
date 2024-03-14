#include <bits/stdc++.h>
using namespace std;

struct Node
{
    int x, y;
    Node *parent;
};

struct Point
{
    int x, y;
};

class RRT
{
private:
    double max_dist;         // Step size for each expansion
    double goal_threshold;   // Threshold to check if goal is reached
    double x_range, y_range; // Range of x and y coordinates
    vector<Node *> nodes;    // Vector to store nodes
    vector<Point> obstacles; // vector to store obstacles

public:
    RRT(double m_dist, double threshold, double x_max, double y_max) : max_dist(m_dist), goal_threshold(threshold), x_range(x_max), y_range(y_max)
    {
        srand(time(0));
        Node *start = new Node{0, 0, nullptr}; // starting point (0,0)
        nodes.push_back(start);
    }

    Node *generateRandomNode()
    {
        int x = rand() % (int)x_range;
        int y = rand() % (int)y_range;
        return new Node{x, y, nullptr};
    }

    Node *findNearestNode(Node *newNode) // finding the nearest node to the newly generated node (random node) in the explorationToGoal
    {
        Node *nearest = nullptr;
        double min_dist = numeric_limits<double>::infinity();
        for (Node *node : nodes)
        {
            double dist = sqrt(pow(newNode->x - node->x, 2) + pow(newNode->y - node->y, 2));
            if (dist < min_dist)
            {
                min_dist = dist;
                nearest = node;
            }
        }
        return nearest;
    }

    void explorationTowardsGoal(vector<Point> obstacles)
    {
        // cout << "expanding tree";
        Node *random_node = generateRandomNode(); // next new node
        Node *nearest_node = findNearestNode(random_node);

        double dx = random_node->x - nearest_node->x;
        double dy = random_node->y - nearest_node->y;
        double dist = sqrt(dx * dx + dy * dy); // distance between the random node and the nearest node.
        double theta = atan2(dy, dx);
        if (dist > max_dist) // if the distance is greater than the maximum specified distance
        {
            dist = max_dist; // the new_node will be at the maximum distance
        }
        // else the new_node will be the random_node
        int x_new = static_cast<int>(nearest_node->x + dist * cos(theta));
        int y_new = static_cast<int>(nearest_node->y + dist * sin(theta));
        Node *new_node = new Node{x_new, y_new, nearest_node};
        if (!isObstacleInPath(new_node, obstacles))
        {
            // cout << "new_node"
            //      << " " << new_node->x << " " << new_node->y << endl;
            nodes.push_back(new_node); // if no obstacle...new_node is the part of the tree(path)
        }
    }

    bool isObstacleInPath(Node *node, vector<Point> obstacles)
    {
        // obstacles = {{10, 10}, {20, 20}, {50, 40}, {40, 80}};
        for (const auto &obstacle : obstacles)
        {
            double m = atan2(node->y - node->parent->y, node->x - node->parent->x); // y=mx+c
            double c = node->y - m * node->x;
            if (obstacle.y == obstacle.x * m + c)
            {
                // cout << "obstacle between the new_node and nearest node" << endl;
                return true; // obstacle is there in the path
            }
        }
        // cout << "no osbatcle found" << endl;
        return false; // no obstacle in the path
    }

    bool isGoalReached(Node *goal) // check if goal is reached
    {
        double dx = goal->x - nodes.back()->x;
        double dy = goal->y - nodes.back()->y;
        double distance_to_goal = sqrt(dx * dx + dy * dy);

        if (distance_to_goal < goal_threshold)
        {
            return true; // reached
        }
        return false; // not reached
    }

    vector<Node *> PathFromStartToGoal(Node *goal) // final path from start to goal
    {
        vector<Node *> path;
        Node *current = nodes.back();
        while (current != nullptr)
        {
            path.push_back(current);
            current = current->parent;
        }
        reverse(path.begin(), path.end());
        return path;
    }
    vector<Point> generateRandomObstacles(double x_range, double y_range, int num_obstacles)
    {
        vector<Point> obstacles;
        srand(time(0));

        for (int i = 0; i < num_obstacles; ++i)
        {
            int x = rand() % static_cast<int>(x_range);
            int y = rand() % static_cast<int>(y_range);
            obstacles.push_back({x, y});
        }

        return obstacles;
    }
};

int main()
{
    RRT planner(10, 20, 100.0, 100.0);
    Node *goal = planner.generateRandomNode(); // Example goal
    // Node *goal = new Node;
    // goal->x = 80;
    // goal->y = 60;
    // goal->parent = nullptr;

    //  Generating random obstacles
    cout << "------------------------------------" << endl;
    vector<Point> obstacles = planner.generateRandomObstacles(100, 100, 5);
    cout << "OBSTACLE POINTS :" << endl;
    for (const auto &obstacle : obstacles)
    {
        cout << "(" << obstacle.x << ", " << obstacle.y << ")" << std::endl;
    }
    cout << "--------------------------------------" << endl;
    cout << "GOAL POINT:"
         << " "
         << "(" << goal->x << ", " << goal->y << ")" << endl;
    cout << "--------------------------------------" << endl;

    while (1)
    {
        planner.explorationTowardsGoal(obstacles);
        if (planner.isGoalReached(goal))
        {
            cout << "Goal Reached!" << endl;
            cout << "--------------------------------------" << endl;

            vector<Node *> path = planner.PathFromStartToGoal(goal);
            path.push_back(goal);
            cout << "PATH:" << endl;
            for (Node *node : path)
            {
                cout << "(" << node->x << ", " << node->y << ")"
                     << "," << endl;
            }
            break;
        }
    }

    return 0;
}
