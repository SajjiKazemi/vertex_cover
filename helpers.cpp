#include "helpers.h"
#include "MyGraph.h"
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <regex>
// defined std::unique_ptr
#include <memory>
// defines Var and Lit
#include "minisat/core/SolverTypes.h"
// defines Solver
#include "minisat/core/Solver.h"


namespace line_parser{

    std::string get_command(std::string line, MyGraph &graph){

        std::regex vertice_define_pattern(R"(V (\d+))");
        std::regex edge_define_pattern(R"(E \{<\d+,\d+>(,<\d+,\d+>)*\})");
        std::regex shortest_path_pattern(R"(s (\d+) (\d+))");
        std::regex E_only_pattern(R"(E\s.*)");
        //std::regex s_only_pattern(R"(s\s.*)");
        std::string command;

        if(std::regex_match(line, vertice_define_pattern))
        {
            command = "V";
        }
        else if(std::regex_match(line, edge_define_pattern))
        {
            if (graph.getSize() == 0)
            {
                command = "Error";
                std::cout << "Error: no vertices defined" << std::endl;
            }
            else if (graph.getSize() != 0 && graph.edges.size() != 0)
            {
                command = "No Command";
            }
            else if (graph.getSize() != 0 && graph.edges.size() == 0)
            {
                if (graph.triedToSetEdges == true)
                {
                    command = "No Command";
                }
                else if (graph.triedToSetEdges == false)
                {
                    command = "E";
                }
            }
        }        
        else if(std::regex_match(line, shortest_path_pattern))
        {
            if (graph.edges.size() == 0)
            {
                command = "Error";
                std::cout << "Error: no edges defined" << std::endl;
            }
            else if (graph.edges.size() != 0)
            {
                command = "s";
            }
        }
        else if(std::regex_match(line, E_only_pattern))
        {
            if (graph.edges.size() !=0)
            {
                return "No command";        
            }
            else if (graph.edges.size() == 0)
            {
                graph.setTriedToSetEdges();
                std::cout << "Error: invalid command" << std::endl;
            }
            
        }
        return command;
    }    

    void analyze_command(std::string command, std::string line, MyGraph &graph)
    {
        std::vector<std::string> result;
        if(command == "V")
        {
            std::istringstream input(line);
            std::string prefix;
            int number;
            
            graph.resetEverything();
            input >> prefix >> number;
            graph.setNoVertices(number);
        }
        else if (command == "E")
        {
            std::map<int, std::pair<int, int>> edges;
            std::regex pattern(R"(<(\d+),(\d+)>)");
            std::sregex_iterator iter(line.begin(), line.end(), pattern);
            std::sregex_iterator end;
            int i = 0;
            while (iter != end) 
            {
                i++;
                auto match = *iter++;
                if (std::stoi(match[1]) > graph.getSize() || std::stoi(match[2]) > graph.getSize())
                {
                    std::cout << "Error: edge is not valid" << std::endl;
                    return;
                }
                edges[i] = std::make_pair(std::stoi(match[1]), std::stoi(match[2]));
            }
            graph.setEdges(edges);
        }
        else if (command == "s")
        {
            std::istringstream input(line);
            std::string prefix;
            int start;
            int end;

            input >> prefix >> start >> end;
            graph.findShortestPath(start, end);
            for(std::vector<int>::size_type i = 0; i < graph.getShortestPath().size(); i++)
            {
                if (i < graph.getShortestPath().size() - 1)
                {
                    std::cout << graph.getShortestPath()[i] << "-";
                }
                else
                {
                    std::cout << graph.getShortestPath()[i] << std::endl;
                }
            }
        }
        else if (command == "No Command" || command == "Error")
    {
        return;
    }
    }

}


namespace polytime_reduction
{   
    void apply_first_cond(Minisat::Lit** literals, size_t rows_num, size_t cols_num, 
                            MyGraph &graph, std::unique_ptr<Minisat::Solver> &solver)
    {
        for (int i=0; i<cols_num; i++)
        {
            for (int j=0; j<rows_num; j++)
            {
                for (int k=0; k<rows_num; k++)
                {
                    if (k != j)
                    {
                        solver->addClause(literals[j][i], literals[k][i]);
                    }
                }
            }
        }
    }

    void apply_second_cond(Minisat::Lit** literals, size_t rows_num, size_t cols_num, 
                            MyGraph &graph, std::unique_ptr<Minisat::Solver> &solver)
    {
        for (int m=0; m<rows_num; m++)
        {
            for (int q=0; q<cols_num; q++)
            {
                for (int p=0; p<q; p++)
                {
                    solver->addClause(~literals[m][p], ~literals[m][q]);
                }
            }
        }
    }

    void apply_third_cond(Minisat::Lit** literals, size_t rows_num, size_t cols_num, 
                            MyGraph &graph, std::unique_ptr<Minisat::Solver> &solver)
    {
        for (int m=0; m<cols_num; m++)
        {
            for (int q=0; q<rows_num; q++)
            {
                for (int p=0; p<q; p++)
                {
                    solver->addClause(~literals[p][m], ~literals[q][m]);
                }
            }
        }
    }

    void apply_fourth_cond(Minisat::Lit** literals, size_t rows_num, size_t cols_num, 
                            MyGraph &graph, std::unique_ptr<Minisat::Solver> &solver)
    {
        for (const auto& x : graph.edges)
        {
            int i = x.second.first;
            int j = x.second.second;
            for (int k=0; k<cols_num; k++)
            {
                for (int l=0; l<rows_num; l++)
                {
                    if (k != l)
                    {
                        solver->addClause(literals[i][k], literals[i][l]);
                    }
                }
            }

            for (int k=0; k<cols_num; k++)
            {
                for (int l=0; l<cols_num; l++)
                {
                    if (k != l)
                    {
                        solver->addClause(literals[j][k], literals[j][l]);
                    }
                }
            }

            for (int k=0; k<cols_num; k++)
            {
                for (int l=0; l<cols_num; l++)
                {
                    if (k != l)
                    {
                        solver->addClause(literals[i][k], literals[j][l]);
                    }
                }
            }
        }
    }
            

    void reduce_polytime(MyGraph &graph)
    {
        std::unique_ptr<Minisat::Solver> solver(new Minisat::Solver());

            std::vector<int> edges_vertices = get_edges_vertices(graph);
            int n = edges_vertices.size();
            for (int k = 1; k < n; k++)
                {
                    Minisat::Lit** literals = new Minisat::Lit*[n];
                    for (int i = 0; i < n; i++)
                    {
                        literals[i] = new Minisat::Lit[k];
                    }
                    for (int i = 0; i < n; i++)
                    {
                        for (int j = 0; j < k; j++)
                        {
                            literals[i][j] = Minisat::mkLit(solver->newVar());
                        }
                    }

                    size_t rows_num = sizeof(literals) / sizeof(literals[0]);
                    size_t cols_num = sizeof(literals[0]) / sizeof(literals[0][0]);
                    apply_first_cond(literals, rows_num, cols_num, graph, solver);
                    apply_second_cond(literals, rows_num, cols_num, graph, solver);
                    apply_third_cond(literals, rows_num, cols_num, graph, solver);
                    apply_fourth_cond(literals, rows_num, cols_num, graph, solver);
                    bool res = solver->solve();
                    if (res == true)
                        {
                            std::vector<int> vertex_cover;
                            for (int i = 0; i < n; i++)
                            {
                                for (int j = 0; j < k; j++)
                                {
                                    if (Minisat::toInt(solver->modelValue(literals[i][j])) == 1)
                                    {
                                        vertex_cover.push_back(edges_vertices[i]);
                                    }
                                }
                            }
                            for (std::vector<int>::size_type i = 0; i < vertex_cover.size(); i++)
                            {
                                if (i < vertex_cover.size() - 1)
                                {
                                    std::cout << i+1 << std::endl;
                                }
                                else
                                {
                                    std::cout << i+1 << " ";
                                }
                            }
                            return;
                        }
                }
    }


    std::vector<int> get_edges_vertices(MyGraph &graph)
        {
            std::vector<int> edges_vertices;
            for (auto const& x : graph.edges)
            {
                edges_vertices.push_back(x.second.first);
                edges_vertices.push_back(x.second.second);
            }
            std::sort(edges_vertices.begin(), edges_vertices.end());
            auto it = std::unique(edges_vertices.begin(), edges_vertices.end());
            edges_vertices.erase(it, edges_vertices.end());
            return edges_vertices;
        }
    
}