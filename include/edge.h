#ifndef EDGE_H
#define EDGE_H

#include "vertex.h"


/**
 * order graph label type
 */
enum LabelOp{
        LABEL_LT=0,
        LABEL_LE
};


class Edge
{
public:
        Edge(Vertex v1, LabelOp lb ,Vertex v2);
        bool operator < (const Edge& edge) const;
        bool operator == (const Edge vertex) const;
        Vertex getSource();
        Vertex getDest() ;
        LabelOp getLabel() ;

        friend std::ostream& operator << (std::ostream& os, Edge& edge) ;

private:
        Vertex source;
        Vertex dest;
        LabelOp label;
};

// override the operator ==
bool operator == (const std::set<Edge>& s1, const std::set<Edge>& s2);

#endif /* EDGE_H */
