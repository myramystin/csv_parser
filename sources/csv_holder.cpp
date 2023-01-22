#include "csv_holder.h"
#include "exception.h"
#include "graph.h"
#include "arithmetic.h"

#include <fstream>
#include <sstream>
#include <iostream>

std::vector<int32_t> CsvHolder::Hash::hash(const std::string& s) {
    fill_x(x_pows.size(), s.size());
    std::vector<int32_t> res(s.size());
    if (res.empty()) {
        return res;
    }
    res[0] = s[0] - 'a';
    for (int i = 1; i < s.size(); ++i) {
        res[i] = (s[i] + (int64_t)res[i - 1] * x) % p;
    }
    return res;
}

int32_t CsvHolder::Hash::substring_hash(const std::vector<int32_t>& hash, size_t l, size_t r) {
    if (l == 0) {
        return hash[r];
    }
    return (hash[r] - (int64_t)x_pows[r - l + 1] * hash[l - 1]) % p;
}

void CsvHolder::Hash::fill_x(size_t l, size_t r) {
    for (; l <= r; ++l) {
        x_pows.push_back(((int64_t)x * x_pows.back()) % p);
    }
}

CsvHolder::CsvHolder(const std::string& filename) : filename_(filename), rows_(0), cols_(0) {
}

void CsvHolder::Read() {
    std::fstream file(filename_, std::ios::in);
    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            std::stringstream str(line);
            std::string word;
            size_t cols_count = 0;
            std::vector<std::string> cells;
            while (std::getline(str, word, ',')) {
                if (rows_ == 0) {
                    // parse column names;
                    if (!word.empty()) {
                        cols_to_idx_.insert({word, cols_});
                        if (cols_ == 0) {
                            rows_to_idx_.insert({word, 0});
                        }
                    }
                    cells.push_back(word);
                    ++cols_;
                } else {
                    if (cols_count == 0) {
                        // parse rows names;
                        if (!word.empty()) {
                            rows_to_idx_.insert({word, rows_});
                        }
                    }
                    cells.push_back(word);

                    ++cols_count;
                }
            }
            unparsed_data_.push_back(cells);
            ++rows_;
        }
    } else {
        throw input_exceptions::IvalidFilename();
    }
}

void CsvHolder::FillHash() {
    // fill cols
    cols_hash_.resize(cols_);
    for (const auto& [s, i] : cols_to_idx_) {
        cols_hash_[i] = hasher.hash(s);
    }
    // fill rows
    rows_hash_.resize(rows_);
    for (const auto& [s, i] : rows_to_idx_) {
        rows_hash_[i] = hasher.hash(s);
    }
}

std::pair<size_t, size_t> CsvHolder::ParseName(const std::string& s) {
    // col_name row_name

    auto s_hash = hasher.hash(s);
    for (const auto& [col, i] : cols_to_idx_) {

        if (col.size() > s.size())
            continue;
        int32_t col_hash = hasher.substring_hash(cols_hash_[i], 0, col.size() - 1);
        int32_t s_start_hash = hasher.substring_hash(s_hash, 0, col.size() - 1);

        if (col_hash == s_start_hash) {

            for (const auto& [row, j] : rows_to_idx_) {
                if (row.size() + col.size() != s.size())
                    continue;

                if (std::stoi(row) == std::stoi(s.substr(col.size()))) {

                    return {j, i};
                }
            }
        }
    }
    throw csv_exceptions::InvalidCellName();
}

bool isnum(const std::string&& s) {
    std::string::const_iterator it = s.begin();
    while (it != s.end() && std::isdigit(*it))
        ++it;
    return !s.empty() && it == s.end();
}

bool is_arithmetic_operation(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/';
}

std::vector<size_t> CsvHolder::GetQueue() {
    // vertex num = row_id*cols_ + col_id
    Graph g(rows_ * cols_, true);
    for (size_t row_id = 1; row_id < rows_; ++row_id) {
        for (size_t col_id = 1; col_id < unparsed_data_[row_id].size(); ++col_id) {
            std::string& cell = unparsed_data_[row_id][col_id];
            if (!cell.empty() && cell[0] == '=') {
                if (cell.size() == 1) {
                    throw csv_exceptions::IvalidCellContents();
                }
                size_t start = 1, sz = 0;
                for (size_t i = start; i < cell.size(); ++i) {
                    if (is_arithmetic_operation(cell[i])) {
                        if (sz == 0) {
                            throw csv_exceptions::IvalidCellContents();
                        }
                        if (!isnum(cell.substr(start, sz))) {

                            auto [needed_row_id, needed_col_id] = ParseName(cell.substr(start, sz));
                            g.AddEdge(needed_row_id * cols_ + needed_col_id, row_id * cols_ + col_id);
                            dependencies_[row_id * cols_ + col_id].push_back(needed_row_id * cols_ + needed_col_id);
                        }
                        start = i + 1;
                        sz = 0;
                    } else {
                        ++sz;
                    }
                }
                if (sz == 0) {
                    throw csv_exceptions::IvalidCellContents();
                }
                if (!isnum(cell.substr(start, sz))) {

                    auto [needed_row_id, needed_col_id] = ParseName(cell.substr(start, sz));
                    g.AddEdge(needed_row_id * cols_ + needed_col_id, row_id * cols_ + col_id);
                    dependencies_[row_id * cols_ + col_id].push_back(needed_row_id * cols_ + needed_col_id);
                }
            }
        }
    }
    auto [success, queue] = g.TopSort();
    if (!success) {
        throw csv_exceptions::CurcularLinks();
    }
    return queue;
}

int32_t CsvHolder::GetCellVal(size_t dependent, const std::string&& cell_name) {
    for (auto i : dependencies_[dependent]) {
        size_t row_id = i / cols_, coll_id = i % cols_;
        if (unparsed_data_[0][coll_id] + unparsed_data_[row_id][0] == cell_name) {
            return std::stoi(parsed_data_[row_id][coll_id]);
        }
    }
    return 0;
}

int32_t CsvHolder::count_cell(const std::string&& s, size_t curr_id) {
    int32_t val = 0;
    // priority * / + -
    size_t start = 0, sz = 0;
    std::string operations = "";
    std::vector<int32_t> vals;
    for (size_t i = start; i < s.size(); ++i) {
        if (is_arithmetic_operation(s[i])) {
            if (sz == 0) {
                throw csv_exceptions::IvalidCellContents();
            }
            if (!operations.empty() && (operations.back() == '*' || operations.back() == '/')) {
                if (vals.empty()) {
                    throw csv_exceptions::IvalidCellContents();
                }
                int32_t a = vals.back();
                vals.pop_back();
                int32_t b;
                 if (isnum(s.substr(start, sz))) {
                    b = std::stoi(s.substr(start, sz));
                } else {
                    b = GetCellVal(curr_id, s.substr(start, sz));
                }
                vals.push_back(count(a, b, operations.back()));
                operations.pop_back();
            } else {
                if (!isnum(s.substr(start, sz))) {
                    vals.push_back(GetCellVal(curr_id, s.substr(start, sz)));
                } else {
                    vals.push_back(std::stoi(s.substr(start, sz)));
                }
            }
            operations += s[i];

            start = i + 1;
            sz = 0;
        } else {
            ++sz;
        }
    }
    if (sz == 0) {
        throw csv_exceptions::IvalidCellContents();
    }
    if (!operations.empty() && (operations.back() == '*' || operations.back() == '/')) {
        if (vals.empty()) {
            throw csv_exceptions::IvalidCellContents();
        }
        int32_t a = vals.back();
        vals.pop_back();
        int32_t b;
        if (isnum(s.substr(start, sz))) {
            b = std::stoi(s.substr(start, sz));
        } else {
            b = GetCellVal(curr_id, s.substr(start, sz));
        }
        vals.push_back(count(a, b, operations.back()));
        operations.pop_back();
    } else {
        if (!isnum(s.substr(start, sz))) {
            vals.push_back(GetCellVal(curr_id, s.substr(start, sz)));
        } else {
            vals.push_back(std::stoi(s.substr(start, sz)));
        }
    }

    if (vals.empty())
        return 0;
    size_t idx = 1;
    val = vals[0];
    for (; idx < vals.size(); ++idx) {
        val = count(val, vals[idx], operations[idx - 1]);
    }
    return val;
}

void CsvHolder::Process() {
    Read();
    parsed_data_.resize(rows_, std::vector<std::string>(cols_, ""));
    dependencies_.reserve(rows_ * cols_);
    FillHash();
    auto queue = GetQueue();
    for (auto idx : queue) {
        size_t row_id = idx / cols_, col_id = idx % cols_;
        if (!unparsed_data_[row_id][col_id].empty() && unparsed_data_[row_id][col_id][0] != '=') {
            parsed_data_[row_id][col_id] = unparsed_data_[row_id][col_id];
        } else {
            parsed_data_[row_id][col_id] = std::to_string(count_cell(unparsed_data_[row_id][col_id].substr(1), idx));
        }
    }
}

void CsvHolder::Print() {

    for (size_t i = 0; i < rows_; ++i) {
        for (size_t j = 0; j < unparsed_data_[i].size(); ++j) {
            std::cout << parsed_data_[i][j];
            if(j + 1 < unparsed_data_[i].size()) {
                std::cout << ",\t";
            }
        }
        std::cout << "\n";
    }
}