#include <iostream>
#include "../Headers/KeyVal.hpp"

/**
 * @class KeyVal
 * @brief Implementation of the KeyVal class for encapsulating key-value pairs.
 *
 * Instances of the KeyVal class can be used to represent key-value pairs, where
 * the keys are assumed to support standard comparison operations. The class provides
 * overloaded comparison operators for sorting and ordering KeyVal objects based on their keys.
 * @param key The key of the key-value pair.
 * @param value The associated value of the key-value pair.
 *
 **/
            
KeyVal::KeyVal(double key, double value) : key(key), value(value) {

};

// Comparison operators
bool KeyVal::operator<(const KeyVal& other) const {
    return key < other.key;
};

bool KeyVal::operator>(const KeyVal& other) const {
    return key > other.key;
};

bool KeyVal::operator<=(const KeyVal& other) const {
    return key <= other.key;
};

bool KeyVal::operator>=(const KeyVal& other) const {
    return key >= other.key;
};

bool KeyVal::operator!=(const KeyVal& other) const {
    return key != other.key;
};

bool KeyVal::operator==(const KeyVal& other) const {
    return key == other.key;
};

int KeyVal::getKey(){
    return key;
};