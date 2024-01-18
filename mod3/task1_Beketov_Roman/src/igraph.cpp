#include "igraph.h"
#include "traversal.h"

#include <algorithm>

bool IGraph::operator==(const IGraph& other) const {
    if (VerticesCount() != other.VerticesCount()) {
        return false;
    }

    for (size_t i = 0; i < VerticesCount(); ++i) {
        auto children_self = GetNextVertices(i);
        auto children_other = other.GetNextVertices(i);
        std::sort(children_self.begin(), children_self.end());
        std::sort(children_other.begin(), children_other.end());

        if (children_self != children_other) {
            return false;
        }

        auto parent_self = GetPrevVertices(i);
        auto parent_other = other.GetPrevVertices(i);
        std::sort(parent_self.begin(), parent_self.end());
        std::sort(parent_other.begin(), parent_other.end());

        if (parent_self != parent_other) {
            return false;
        }

    }
    return true;
}