#include "arithmetic.h"
#include "exception.h"

int32_t add(int32_t a, int32_t b) {
    int64_t res = static_cast<int64_t>(a) + b;
    if (INT32_MIN <= res && res <= INT32_MAX) {
        return res;
    }
    throw arithmetic_exceptions::IntegerOverflow();
}

int32_t mul(int32_t a, int32_t b) {
    int64_t res = static_cast<int64_t>(a) * b;
    if (INT32_MIN <= res && res <= INT32_MAX) {
        return res;
    }
    throw arithmetic_exceptions::IntegerOverflow();
}

int32_t sub(int32_t a, int32_t b) {
    int64_t res = static_cast<int64_t>(a) - b;
    if (INT32_MIN <= res && res <= INT32_MAX) {
        return res;
    }
    throw arithmetic_exceptions::IntegerOverflow();
}

int32_t divs(int32_t a, int32_t b) {
    if (b == 0) {
        throw arithmetic_exceptions::ZeroDivision();
    }
    int64_t res = static_cast<int64_t>(a) / b;
    if (INT32_MIN <= res && res <= INT32_MAX) {
        return res;
    }
    throw arithmetic_exceptions::IntegerOverflow();
}

int32_t count(int32_t a, int32_t b, char c) {
    if (c == '+')
        return add(a, b);
    if (c == '-')
        return sub(a, b);
    if (c == '*')
        return mul(a, b);
    if (c == '/')
        return divs(a, b);
    return 0;
}