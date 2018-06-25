#include "vertex.h"

/** class Vertex  **/
Vertex::Vertex(){}

Vertex::Vertex(std::string name) {
        this->name = name;
}

std::string Vertex::getName(){
        return this->name;
}

Vertex& Vertex::operator= (const Vertex& vertex) {
        this->name = vertex.name;
        return *this;
}

bool Vertex:: operator< (const Vertex& vertex) const{
        return this->name < vertex.name;
}
bool Vertex:: operator == (const Vertex vertex) const {
        // cout<< "in Vertex: override == \n";
        return this->name == vertex.name;
}

std::ostream& operator << (std::ostream& os, Vertex& vertex) {
        os<<vertex.name;
        return os;
}

bool operator == (const std::set<Vertex>& s1, const std::set<Vertex>& s2) {
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

int find_vertex(const std::vector<Vertex>& vec, const Vertex& v) {
        for (unsigned int i=0; i<vec.size(); i++) {
                if (vec[i] == v) {
                        return i;
                }
        }
        return -1;
}
