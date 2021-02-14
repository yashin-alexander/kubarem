#pragma once

#include "node.h"
#include "action.h"
#include "world_state.h"

#include <vector>


namespace goap {
    class Planner {
    private:
        const int START_NODE_COST = 0;
        const int START_NODE_PARENT_ID = 0;
        const char * START_NODE_NAME = "Start node";

        std::vector<Node> open_;
        std::vector<Node> closed_;
        int (*heuristicFunctionPointer)(const WorldState &node, const WorldState &goal);

        bool isClosed(const WorldState& ws) const;

        std::vector<goap::Node>::iterator getIfOpen(const WorldState& ws);

        Node& fromOpenToClose();

        void emplaceOpen(Node n);

    public:
        Planner(int (*heuristicFunction)(const WorldState &node, const WorldState &goal));

        void logOpenList() const;
        void logClosedList() const;

        std::vector<Action> plan(const WorldState& start, const WorldState& goal, const std::vector<Action>& actions);
    };
}