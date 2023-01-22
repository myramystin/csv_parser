#include "sources/exception.h"
#include "sources/logger.h"
#include "sources/csv_holder.h"
#include <iostream>

#include <string>

int main(int argc, char *argv[]) {
    try {
        if (argc == 1) {
            throw input_exceptions::EmptyInput();
        }
        CsvHolder holder(argv[1]);
        holder.Process();
        holder.Print();

    } catch (std::exception &exception) {

        Logger::Instance().LogError(exception.what());
        std::string info_text = "Program usage: \n\t- insert filename.csv";

        Logger::Instance().LogInfo(info_text);
    }
    return 0;
}