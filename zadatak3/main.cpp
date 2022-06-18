// VAZNA NAPOMENA !!
// da bi mogli kompajlirati ovaj program potrebna vam je boost biblioteka
// koja mora biti instalirana na racunaru
//
// s pristupom u ovom zadatku sesti uslov iz zadace nece biti tacno O(1)
// ali ce biti O(1+bucket_size) sto je priblizno jednako O(1)
// 
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

struct student;

struct ispit{
    datum datum_;
    std::string pred;
    std::list<std::pair<std::string, short int>> studenti;
    // nisam koristio student* zato sto rehash u mapama nije inplace te bi moglo doci do pogresnog pointera
    // a pretrazivanje je svakako O(1)
};

struct student{
    std::string index;
    std::string ime;
    std::string prezime;
    std::list<std::pair<ispit*, short int>> polozeni_ispiti;
    // ispit* moze izazvati gresku pa sam rehasho niz ispita na pocetku da bi izbjegao rehashing naknadno
};

// hashiranje mojih struktura :
namespace std{ 
    template<> struct hash<datum>{ size_t operator()(const datum& dat) const {
        size_t hash = 0;
        boost::hash_combine(hash, dat.godina);
        boost::hash_combine(hash, dat.mjesec);
        boost::hash_combine(hash, dat.dan);
        return hash;
    }};
    template<> struct hash<ispit>{ size_t operator()(const ispit& ispi) const {
        size_t hash = 0;
        boost::hash_combine(hash, ispi.datum_.dan+ispi.datum_.mjesec+ispi.datum_.godina);
        boost::hash_combine(hash, ispi.pred);
        return hash;
    }};
};

// funkcije za main :

student noviStudent(){
    student temp;
    std::cout << "Dodavanje novog studenta;\nIndex : "; std::cin >> temp.index;
    std::cout << "Ime : "; std::cin >> temp.ime;
    std::cout << "Prezime : "; std::cin >> temp.prezime;
    return temp;
}

ispit noviIspit(std::unordered_map<std::string, std::string>& predmeti){
    ispit temp;
    std::cout << "Dodavanje novog ispita;\nIme predmeta : "; std::getline(std::cin,temp.pred);
    auto it = predmeti.find(temp.pred);
    if(it == predmeti.end()) throw std::logic_error{"Predmet nije pronadjen!"};
    std::cout << "Datum (format : dd.mm.yyyy.) : "; std::cin >> temp.datum_;
    return temp;
}

std::unordered_multimap<datum,ispit>::iterator pronadjiIspit(std::unordered_multimap<datum, ispit>& ispiti, datum& date, std::string predmet){
    auto range = ispiti.equal_range(date);
    for(auto it = range.first; it != range.second; ++it)
        if( it->second.pred == predmet ) return it;
    throw std::logic_error{"Ispit nije pronadjen"};
}

void noviRezultatIspita(std::unordered_multimap<datum, ispit>& ispiti, std::unordered_map<std::string, student>& studenti){
    std::string indeks,predmet;
    short int ocjena;
    datum date;
    try{
        std::cout << "Datum : "; std::cin >> date;
        std::cout << "Predmet :"; std::getline(std::cin, predmet);
        auto it = pronadjiIspit(ispiti, date, predmet);
        std::cout << "Indeks studenta : "; std::cin >> indeks;
        auto s_it = studenti.find(indeks);
        if(s_it == studenti.end()) throw std::logic_error{"Student nije pronadjen"};
        std::cout << "Ocjena : "; std::cin >> ocjena;
        it->second.studenti.push_back({indeks,ocjena});
        if(ocjena>5) s_it->second.polozeni_ispiti.push_back({&(it->second), ocjena});
    }catch(std::logic_error& a){std::cout << a.what() << std::endl;}
}

void dohvatiSveIspite(std::unordered_multimap<datum, ispit>& ispiti){
    datum date;
    std::cout << "Datum (dd.mm.yyyy.) : "; std::cin >> date;
    auto k = ispiti.equal_range(date);
    std::cout << "--------------------------\n";
    for(auto it = k.first; it!=k.second;++it)std::cout << it->second.pred << '\n';
    std::cout << "--------------------------\n";
}

void studentiKojiSuPolozili(std::unordered_multimap<datum, ispit>& ispiti, std::unordered_map<std::string,student>& studenti){
    datum date;
    std::string predmet;
    std::cout << "Datum : "; std::cin >> date;
    std::cout << "Predmet : "; std::getline(std::cin, predmet);
    auto it = pronadjiIspit(ispiti, date, predmet);
    // for(auto v_it = it->second.studenti.begin(); v_it != it->second.studenti.end(); ++it){
    //     if( v_it->second > 5) std::cout << "\n-------------------\n" << studenti.find(v_it->first)->second.ime
    //     << ' ' << studenti.find(v_it->first)->second.prezime << "\nOcjena : " << v_it ->second;
    // }
    for(auto& i : it->second.studenti){
        std::cout << "\n---------------\nStudent : " << studenti.find(i.first)->second.prezime << " " << studenti.find(i.first)->second.ime  << "\nOcjena : " << i.second;
    }
    std::cout << "\n-------------------\n";
}

void polozeniIspitiSudenta(std::unordered_map<std::string,student>& studenti){
    std::string indeks;
    std::cout << "Indeks : "; std::cin >> indeks;
    auto it = studenti.find(indeks);
    double prosjecna_ocjena = 0;
        std::cout << "\n-------------------";
        for(auto& i : it->second.polozeni_ispiti){
            std::cout << "\nDatum : " << i.first->datum_ << "\nPredmet : " << i.first->pred << "\nOcejna :"  << i.second;
            prosjecna_ocjena += i.second;
        }
    std::cout << "\nProsjecna ocjena : " << prosjecna_ocjena/it->second.polozeni_ispiti.size() << "\n-------------------\n";
}

//main
int main(){
    std::unordered_multimap<datum, ispit> ispiti;
    std::unordered_map<std::string,student> studenti;
    std::unordered_map<std::string, std::string> predmeti;

    ispiti.rehash(50);

    int odabir = 0;
    std::cout << "1. Dodaj novog studenta\n2. Dodaj novi ispit\n3. Dodaj novi predmet\n4. Dodaj novi rezultat ispita\n5. Dohvati sve ispite za određeni datum\n6. Dohvati po datumu i imenu predmeta, sve studente koji su položili određeni ispit\n7. Dohvati po indeksu sve ispite koje je položio student hronološki, te prosjecnu ocjenu" << std::endl;
    while(std::cin >> odabir){
        switch (odabir){
        case 1: {
            auto temp = noviStudent();
            studenti.emplace(temp.index,std::move(temp));
            break; 
        } case 2: {
            auto temp = noviIspit(predmeti);
            ispiti.emplace(temp.datum_,temp);
        } case 3: {
            break;
            std::cout << "Ime predmeta: ";
            std::string temp; std::getline(std::cin, temp);
            predmeti.emplace(temp,temp);
            break;
        } case 4: {
            noviRezultatIspita(ispiti,studenti);
            break;
        } case 5: {
            dohvatiSveIspite(ispiti);
            break;
        } case 6: {
            studentiKojiSuPolozili(ispiti, studenti);
            break;
        } case 7:{
            polozeniIspitiSudenta(studenti);
            break;
        }default:
            std::cout << "Pogresan unos!\n";
        }
    std::cout << "1. Dodaj novog studenta\n2. Dodaj novi ispit\n3. Dodaj novi predmet\n4. Dodaj novi rezultat ispita\n5. Dohvati sve ispite za određeni datum\n6. Dohvati po datumu i imenu predmeta, sve studente koji su položili određeni ispit\n7. Dohvati po indeksu sve ispite koje je položio student hronološki, te prosjecnu ocjenu" << std::endl;
    }

    return 0;
}

/*


std::unordered_multimap<char, int> test;

test.emplace('a', 1);
test.emplace('a', 2);
test.emplace('a', 3);
test.emplace('a', 4);

auto k = test.equal_range('a');
for(auto it = k.first; it!=k.second;++it) std::cout << it->first << ' ' << it->second << '\n';



ispit prvi{{25,7,2001},"Matematika 1"};
ispit prvi1{{25,7,2001},"Fizika 1"};
ispit prvi2{{25,7,2001},"OE 1"};
ispit drugi{{23,7,2001},"OE 2"};

ispiti.emplace(prvi.datum_, prvi);
ispiti.emplace(drugi.datum_, drugi);
ispiti.emplace(prvi1.datum_, prvi1);
ispiti.emplace(prvi2.datum_, prvi2);


auto k = ispiti.equal_range({25,7,2001});
for(auto it = k.first; it!=k.second;++it) std::cout << it->first << ' ' << it->second.pred << '\n';


*/