#include "node.h"


namespace goap {
    int Node::last_id_ = 0;

    Node::Node(const WorldState state, int g, int h, int parent_id, const Action* action) :
            ws_(state), g_(g), h_(h), parent_id_(parent_id), action_(action) {
        id_ = ++last_id_;
    }

    bool operator<(const Node& lhs, const Node& rhs) {
        return lhs.f() < rhs.f();
    }
}
