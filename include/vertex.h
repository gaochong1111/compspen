#ifndef VERTEX_H
#define VERTEX_H

#include <string>
#include <iostream>
#include <set>
#include <vector>

class Vertex
{
public:
        Vertex();
        Vertex(std::string name);

        std::string getName();

        Vertex& operator=(const Vertex& vertex);
        bool operator < (const Vertex& vertex) const;
        bool operator == (const Vertex vertex) const;
        friend std::ostream& operator << (std::ostream& os, Vertex& vertex);
private:
        std::string name;
};

// override the operator ==
bool operator == (const std::set<Vertex>& s1, const std::set<Vertex>& s2);
// find the position of v in vec
int find_vertex(const std::vector<Vertex>& vec, const Vertex& v);

#endif /* VERTEX_H */
