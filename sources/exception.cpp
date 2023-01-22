#include "exception.h"

const char *input_exceptions::EmptyInput::what() const noexcept {
    return "no filename";
}

const char *input_exceptions::IvalidFilename::what() const noexcept {
    return "invalid filename (expected .csv)";
}

const char *arithmetic_exceptions::IntegerOverflow::what() const noexcept {
    return "arithmetic operation caused int32 overflow";
}

const char *arithmetic_exceptions::ZeroDivision::what() const noexcept {
    return "arithmetic operation with zero division";
}

const char *csv_exceptions::InvalidCellName::what() const noexcept {
    return "file contains invalid cell name";
}

const char *csv_exceptions::IvalidCellContents::what() const noexcept {
    return "cell content is invalid";
}

const char *csv_exceptions::CurcularLinks::what() const noexcept {
    return "file contains circular links";
}