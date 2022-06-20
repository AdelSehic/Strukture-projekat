#pragma once
#include <iostream>
#include <boost/functional/hash.hpp>

struct datum{
    short int dan;
    short int mjesec;
    short int godina;
    //potrebno za sort:
    bool operator<(const datum& other) const {
        if (godina != other.godina) return godina < other.godina;
        if (mjesec != other.mjesec) return mjesec < other.mjesec;
        if (dan != other.dan) return dan < other.dan;
        return false;
    }
    bool operator>(const datum& other) const {
        if (godina != other.godina) return godina > other.godina;
        if (mjesec != other.mjesec) return mjesec > other.mjesec;
        if (dan != other.dan) return dan > other.dan;
        return false;
    }
    //potrebno za hashiranje:
    bool operator==(const datum& other) const { return (godina==other.godina && dan==other.dan && mjesec==other.mjesec); }
};
std::ostream& operator<<(std::ostream& out, const datum& dat){
    std::cout << dat.dan <<'.'<<dat.mjesec<<'.'<<dat.godina<<'.';
    return out;    
}
std::istream& operator>>(std::istream& in, datum& dat){
    scanf("%hd.%hd.%hd.", &dat.dan,&dat.mjesec,&dat.godina);
    return in;
}

namespace std{ 
    template<> struct hash<datum>{ size_t operator()(const datum& dat) const {
        size_t hash = 0;
        boost::hash_combine(hash, dat.godina);
        boost::hash_combine(hash, dat.mjesec);
        boost::hash_combine(hash, dat.dan);
        return hash;
    }};
};