#include <iostream>
#include <unordered_map>
#include <vector>
#include <functional>
#include <algorithm>
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

bool find_elem(std::unordered_multimap<int,char>& k, int a, char b){
    auto range = k.equal_range(a);
    for(auto it = range.first; it != range.second; ++it){
        if( it->second == b ){
            std::cout << it->first << " " << it->second << std::endl;
            return true;
        }
    }
    return false;
}

int main(int argc, const char** argv) {

    // datum k;

    // std::cin >> k;
    // std::cout << k;

    std::unordered_multimap<int, char> k;
    k.emplace(1,'c');
    k.emplace(1,'b');
    k.emplace(1,'a');
    k.emplace(2,'d');

    std::cout << (find_elem(k,1,'c') ? "pronadjen" : "nije pronadjen") << std::endl;
    std::cout << (find_elem(k,1,'f') ? "pronadjen" : "nije pronadjen") << std::endl;

    return 0;
}