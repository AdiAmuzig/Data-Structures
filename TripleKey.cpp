//
// Created by adi.amuzig on 15/06/2020.
//

#include "TripleKey.h"

TripleKey::TripleKey(int firstKey, int secondKey, int thirdKey) :
        firstKey(firstKey),
        secondKey(secondKey),
        thirdKey(thirdKey){
}

TripleKey::TripleKey(const TripleKey &key) :
        firstKey(key.firstKey),
        secondKey(key.secondKey),
        thirdKey(key.thirdKey){
}

bool TripleKey::operator<(const TripleKey &key2) const {
    return (firstKey < key2.firstKey) ||
           (firstKey == key2.firstKey && secondKey > key2.secondKey) ||
           (firstKey == key2.firstKey && secondKey == key2.secondKey && thirdKey > key2.thirdKey);
}

int TripleKey::getFirstKey() const {
    return firstKey;
}

int TripleKey::getSecondKey() const {
    return secondKey;
}

int TripleKey::getThirdKey() const {
    return thirdKey;
}

TripleKey::TripleKey() {
    firstKey = 0;
    secondKey = 0;
    thirdKey = 0;
}
