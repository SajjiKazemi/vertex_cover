#ifndef MYGRAPH_H
#define MYGRAPH_H

#include <map>
#include <iostream>
#include <vector>
#include "BFStree.h"

class MyGraph
{
private:
    int num_vertices;

public:
    std::map<int, std::pair<int, int>> edges;
    std::vector<BFStree> unvisited_trees;
    std::vector<int> visited_nodes;
    std::vector<int> unvisited_nodes;
    std::vector<int> shortest_path;
    bool triedToSetEdges = false;
    MyGraph();
    MyGraph(int num_vertices);
    void setNoVertices(int num_vertices);
    int getSize();
    void setEdges(std::map<int, std::pair<int,int>> edges);
    std::vector<int> getConnections(int node);
    bool isConnected(int start, int end);
    void createRootTree(int start);
    bool checkTree(BFStree tree, int end);
    std::vector<int> findShortestPath(int start, int end);
    std::vector<int> getShortestPath();
    void getUnvisitedTrees();
    void resetVisitedNodes();
    void resetEverything();
    void setTriedToSetEdges();
    bool checkNodeInEdges(int node);
};


#endif
