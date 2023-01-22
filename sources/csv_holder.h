#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <cmath>

class CsvHolder {
public:
    CsvHolder(const std::string& filename);
    void Process();
    void Print();

private:
    void Read();
    std::vector<size_t> GetQueue();
    std::pair<size_t, size_t> ParseName(const std::string& s);
    void FillHash();
    int32_t GetCellVal(size_t dependent, const std::string&& cell_name);
    int32_t count_cell(const std::string&& s, size_t cell_id);
    struct Hash {
    public:
        const int32_t p = pow(10, 9) + 7, x = 29;
        std::vector<int32_t> x_pows = {1};

        std::vector<int32_t> hash(const std::string& s);
        int32_t substring_hash(const std::vector<int32_t>& hash, size_t l, size_t r);

    private:
        void fill_x(size_t l, size_t r);
    } hasher;

private:
    size_t cols_, rows_;
    std::vector<std::vector<std::string>> unparsed_data_;
    std::vector<std::vector<std::string>> parsed_data_;
    std::unordered_map<std::string, size_t> rows_to_idx_;
    std::unordered_map<std::string, size_t> cols_to_idx_;
    std::vector<std::vector<int32_t>> rows_hash_;
    std::vector<std::vector<int32_t>> cols_hash_;
    std::vector<std::vector<size_t>> dependencies_;
    std::string filename_;
};