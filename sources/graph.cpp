#include "graph.h"

Graph::Graph(size_t amount, bool oriented = false) : ver_amount_(amount), oriented_(oriented) {
    adj_.resize(amount);
    visited_.resize(amount);
    std::fill(visited_.begin(), visited_.end(), 0);
}

void Graph::AddEdge(size_t start, size_t end) {
    adj_[start].push_back(end);
    if (!oriented_) {
        adj_[start].push_back(end);
    }
}

void Graph::dfs(size_t now, bool& success, std::vector<size_t>& topsort_res) {
    visited_[now] = 1;
    for (size_t neighbor : adj_[now]) {
        if (visited_[neighbor] == 1) {
            success = false;
        }
        if (visited_[neighbor] == 0) {
            dfs(neighbor, success, topsort_res);
        }
    }
    topsort_res.push_back(now);
    visited_[now] = 2;
}

std::pair<bool, std::vector<size_t>> Graph::TopSort() {
    bool success = true;
    std::vector<size_t> result;
    for (size_t i = 1; i < ver_amount_; ++i) {
        if (!visited_[i]) {
            dfs(i, success, result);
        }
    }
    std::reverse(result.begin(), result.end());

    return {success, result};
}