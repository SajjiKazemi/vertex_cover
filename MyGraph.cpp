#include "MyGraph.h"
#include <vector>
#include <algorithm>
#include "BFStree.h"

// Constructor definition
MyGraph::MyGraph()
{
    this->num_vertices = 0;
}

MyGraph::MyGraph(int num_vertices)
{
    this->num_vertices = num_vertices;
}

// Method definition
int MyGraph::getSize()
{
    return this->num_vertices;
}

void MyGraph::setEdges(std::map<int, std::pair<int,int>> edges)
{
    resetEverything();
    this->edges = edges;
}

void MyGraph::setNoVertices(int num_vertices)
{
    this->num_vertices = num_vertices;
    resetEverything();
}

bool MyGraph::checkNodeInEdges(int node)
{
    for (auto const& x : this->edges)
    {
        if(x.second.first == node || x.second.second == node)
        {
            return true;
        }
    }
    return false;
}

std::vector<int> MyGraph::getConnections(int node)
{
    std::vector<int> connections;
    for (auto const& x : this->edges)
    {
        if(x.second.first == node)
        {
            connections.push_back(x.second.second);
        }
        else if(x.second.second == node)
        {
            connections.push_back(x.second.first);
        }
    }
    std::sort(connections.begin(), connections.end());
    connections.erase(std::unique(connections.begin(), connections.end()), connections.end());
    return connections;
}

bool MyGraph::isConnected(int start, int end)
    {
        std::vector<int> connections = getConnections(start);    
            
        auto it = std::find(connections.begin(), connections.end(), end);
        if(it != connections.end())
            {
                return true;
            }
        else
            {
                return false;
            }
    }

void MyGraph::createRootTree(int start)
{
    BFStree root_tree = BFStree(start, this->getConnections(start));
    for (std::vector<int>::size_type i = 0; i < root_tree.getChildren().size(); i++)
    {
        root_tree.addGrandchild(BFStree(root_tree.getChildren()[i], this->getConnections(root_tree.getChildren()[i]), root_tree));            
    }
    this->unvisited_trees.push_back(root_tree);
}

bool MyGraph::checkTree(BFStree tree, int end)
{
    if(tree.isIncluded(end))
    {
        return true;
    }
    else
    {
        this->visited_nodes.push_back(tree.getRootNode());
        return false;
    }
}

void MyGraph::getUnvisitedTrees()
{   
    int controller = this->unvisited_trees.size();
    for(int i=0; i < controller; i++)
    {
        for(std::vector<int>::size_type j=0; j<this->unvisited_trees[i].getChildren().size(); j++)
        {
            if(std::find(this->visited_nodes.begin(), this->visited_nodes.end(), this->unvisited_trees[i].getChildren()[j]) == this->visited_nodes.end())
            {
                this->unvisited_trees.push_back(BFStree(this->unvisited_trees[i].getChildren()[j], this->getConnections(this->unvisited_trees[i].getChildren()[j]), this->unvisited_trees[i]));
            }
        }
    }
    this->unvisited_trees.erase(this->unvisited_trees.begin(), this->unvisited_trees.begin() + controller);

}

std::vector<int> MyGraph::findShortestPath(int start, int end)
{
    resetVisitedNodes();
    if (start == end && checkNodeInEdges(start) == true)
    {
        shortest_path.push_back(start);
        return shortest_path;
    }
    
    createRootTree(start);

    if(checkTree(this->unvisited_trees[0], end) == true)
    {
        shortest_path.push_back(start);
        shortest_path.push_back(end);
        return shortest_path;
    }
    else
    {
        bool found = false;
        while (found != true && this->unvisited_trees.size() != 0)
        {
            for (std::vector<int>::size_type i=0; i<unvisited_trees.size(); i++)
            {
                if(checkTree(unvisited_trees[i], end))
                {
                    found = true;
                    shortest_path = unvisited_trees[i].getAncestors();
                    shortest_path.push_back(unvisited_trees[i].getRootNode());
                    shortest_path.push_back(end);
                    break;
                }
            }
            getUnvisitedTrees();
        }
        if(shortest_path.empty())
        {
            std::cout << "Error: no path exists" << std::endl;
            return shortest_path;
        }
        else
        {
            return shortest_path;        
        }
    }
}

std::vector<int> MyGraph::getShortestPath()
{
    return this->shortest_path;
}

void MyGraph::resetVisitedNodes()
{
    this->unvisited_trees.clear();
    this->unvisited_nodes.clear();
    this->visited_nodes.clear();
    this->shortest_path.clear();
}

void MyGraph::resetEverything()
{
    this->edges.clear();
    this->unvisited_trees.clear();
    this->visited_nodes.clear();
    this->shortest_path.clear();
    this->triedToSetEdges = false;
}

void MyGraph::setTriedToSetEdges()
{
    this->triedToSetEdges = true;
}