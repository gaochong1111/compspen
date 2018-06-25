#include "edge.h"


/** class Edge **/

Edge::Edge(Vertex v1, LabelOp lb ,Vertex v2) {
        this->source = v1;
        this->label = lb;
        this->dest = v2;
}

bool Edge::operator < (const Edge& edge) const {
        if (this->source < edge.source ||
            (edge.source == this->source && this->label < edge.label) ||
            (edge.source == this->source && this->label == edge.label && this->dest < edge.dest)) {
                return true;
        }

        return false;
}

bool Edge::operator == (const Edge edge) const {
        return this->label== edge.label && this->source == edge.source && this->dest == edge.dest;
}

Vertex Edge::getSource() {
        return this->source;
}

Vertex Edge::getDest() {
        return this->dest;
}

LabelOp Edge::getLabel() {
        return this->label;
}

std::ostream& operator << (std::ostream& os, Edge& edge) {
        os << edge.source << "--" << edge.label <<"-->" << edge.dest;
        return os;
}

/** global function  **/
bool operator == (const std::set<Edge>& s1, const std::set<Edge>& s2) {
        if(s1.size() == s2.size()) {
                for (auto edge : s1) {
                        if (s2.find(edge) == s2.end()) {
                                return false;
                        }
                }
                return true;
        }
        return false;
}
