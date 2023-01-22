#pragma once

#include <vector>

class Graph {
public:
    Graph(size_t amount, bool oriented);

    void AddEdge(size_t start, size_t end);

    std::pair<bool, std::vector<size_t>> TopSort();

private:
    std::vector<int> visited_;
    std::vector<std::vector<size_t>> adj_;
    size_t ver_amount_;
    bool oriented_;

private:
    void dfs(size_t now, bool& success, std::vector<size_t>& topsort_res);
};