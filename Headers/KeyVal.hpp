#ifndef KEYVAL_H
#define KEYVAL_H

#include "../Headers/declarations.hpp" 

class KeyVal {
    public:
        KeyVal(double key = 0.0, double value = 0.0);

        bool operator<(const KeyVal& other) const;
        bool operator>(const KeyVal& other) const;
        bool operator<=(const KeyVal& other) const;
        bool operator>=(const KeyVal& other) const;
        bool operator!=(const KeyVal& other) const;
        bool operator==(const KeyVal& other) const;
        int getKey();

    private:
        int key;
        int value;
};

#endif 