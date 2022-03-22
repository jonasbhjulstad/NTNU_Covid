/*
 *    This file is part of CasADi.
 *
 *    CasADi -- A symbolic framework for dynamic optimization.
 *    Copyright (C) 2010-2014 Joel Andersson, Joris Gillis, Moritz Diehl,
 *                            K.U. Leuven. All rights reserved.
 *    Copyright (C) 2011-2014 Greg Horn
 *
 *    CasADi is free software; you can redistribute it and/or
 *    modify it under the terms of the GNU Lesser General Public
 *    License as published by the Free Software Foundation; either
 *    version 3 of the License, or (at your option) any later version.
 *
 *    CasADi is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *    Lesser General Public License for more details.
 *
 *    You should have received a copy of the GNU Lesser General Public
 *    License along with CasADi; if not, write to the Free Software
 *    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */
#define _USE_MATH_DEFINES
#include <math.h>
#include "./include/Erdos_Renyi.hpp"
#include "./include/File_Utils.hpp"
#include <casadi/casadi.hpp>
#include <string>
#include <random>
#include <iostream>
#include <limits>

using namespace casadi;
using namespace std;

/**
Solve the Rosenbrock problem, formulated as the NLP:
minimize     x^2 + 100*z^2
subject to   z+(1-x)^2-y == 0
Joel Andersson, 2015-2016
*/

int main()
{

    const uint32_t N_nodes = 4;

    auto G = generate_Erdos_Renyi(N_nodes, .3);

    std::vector<Node> nodes(N_nodes);

    std::random_device rnd;
    std::mt19937 rng(rnd());
    double yMin, yMax, xMin, xMax;
    yMin = 0.0;
    yMax = 10.0;
    xMin = 0.0;
    xMax = 10.0;

    std::uniform_real_distribution<double> dist_x(xMin, xMax);
    std::uniform_real_distribution<double> dist_y(yMin, yMax);
    for (uint32_t i = 0; i < nodes.size(); i++)
    {
        nodes[i].idx = i;
        nodes[i].position = {dist_x(rng), dist_y(rng)};
    }

    const double radius = 10.0;

    for (uint32_t i = 0; i < nodes.size(); i++)
    {
        nodes[i].position = {sin(2*i*M_PI/nodes.size())*radius, cos(2*i*M_PI/nodes.size())*radius};
    }

    const std::string adjPath = "/home/deb/Documents/Spring_Layout/data/ER_adjlist.csv";
    write_adjacency_list(G, adjPath);

    const std::string nodePath = "/home/deb/Documents/Spring_Layout/data/ER_node_properties.csv";
    write_node_properties(nodes, nodePath);

    MX obj = 0;
    uint32_t N_edges = 0;
    for (const auto &nodeCon : G)
    {
        N_edges += nodeCon.connections.size();
    }

    MX P = MX::sym("P", N_nodes, 2);
    std::vector<double> P0(2 * N_nodes);
    for (int i = 0; i < nodes.size(); i++)
    {
        P0[2 * i] = nodes[i].position[0];
        P0[2 * i + 1] = nodes[i].position[1];
    }
    // for (uint32_t i = 0; i < G.size(); i++)
    // {
    //     for (uint32_t j = 0; j < G[i].connections.size(); j++)
    //     {
    //         double weight = G[i].connections[i].weight;
    //         obj += MX::norm_2(MX::abs(P(i, Slice()) - P(G[i].connections[j].target_idx, Slice())) - 1.0);
    //     }
    // }
    obj =  MX::sum(MX::norm_2(P));

    std::vector<double> weights(N_edges);
    uint32_t i = 0;
    for (const auto &nodeCon : G)
    {
        for (const auto &connection : nodeCon.connections)
        {
            weights[i] = connection.weight;
            i++;
        }
    }

    MX Edges = MX::sym("edge", N_edges);
    MX Weights = weights;

    // MX obj = MX::dot(Weights, Edges - 1.0);
    // // Create the NLP
    // MX g = MX::vertcat({P(Slice(), 0) - xMin, 
    // P(Slice(), 1) - yMin,
    // xMax - P(Slice(), 0),
    // yMax - P(Slice(), 1)});
    std::vector<MX> g;
    for (int i = 0; i < (P.rows() - 1); i++)
    {
        for (int j = 2; j < P.rows(); i++)
            g.push_back(MX::norm(P[])
    }

    // std::cout << obj.rows() << std::endl;
    MXDict nlp = {{"x", MX::reshape(P, N_nodes * 2, 1)}, {"f", obj}};//, {"g",  g}};

    // Allocate an NLP solver and buffers
    Dict opts = {{"ipopt.tol", 1e-6},
    {"ipopt.file_print_level", 8}, 
    {"ipopt.output_file", "log.txt"}};
    // {"ipopt.linear_solver", "mumps"}};

    Function solver = nlpsol("solver", "ipopt", nlp, opts);

    DMDict arg;
    // MX P0M = P0;
    arg["x0"] = P0;
    arg["ubg"] = 10.0;
    arg["lbg"] = 0.0;
    // arg["ubg"] = 0;
    auto result = solver(arg);
    std::cout << result["x"];

    return 0;
}