#pragma once
#include <exception>

namespace input_exceptions {
class EmptyInput : public std::exception {
    virtual const char* what() const noexcept override;
};

class IvalidFilename : public std::exception {
    virtual const char* what() const noexcept override;
};
}  // namespace input_exceptions

namespace arithmetic_exceptions {
class IntegerOverflow : public std::exception {
    virtual const char* what() const noexcept override;
};

class ZeroDivision : public std::exception {
    virtual const char* what() const noexcept override;
};
}  // namespace arithmetic_exceptions

namespace csv_exceptions {
class InvalidCellName : public std::exception {
    virtual const char* what() const noexcept override;
};

class IvalidCellContents : public std::exception {
    virtual const char* what() const noexcept override;
};

class CurcularLinks : public std::exception {
    virtual const char* what() const noexcept override;
};
}  // namespace csv_exceptions
