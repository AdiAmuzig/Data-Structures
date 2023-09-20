//
// Created by adi.amuzig on 09/06/2020.
//

#ifndef CODE_TRIPLEKEY_H
#define CODE_TRIPLEKEY_H

class TripleKey{
private:
    int firstKey;
    int secondKey;
    int thirdKey;

public:
    TripleKey(int firstKey , int secondKey , int thirdKey );//constractor
    TripleKey();
//    ~TripleKey();//distractor
    TripleKey(const TripleKey& key);//copy constractor
    bool operator<(const TripleKey &key2) const;//compare. chekes if a key is larger then another given key
    int getFirstKey() const;
    int getSecondKey() const;
    int getThirdKey() const;
};



#endif //CODE_TRIPLEKEY_H
