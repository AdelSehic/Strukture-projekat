#pragma once
#include <iostream>
#include <map>
#include <boost/functional/hash.hpp>
#include "datum.hpp"

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
    std::map<datum, std::pair<ispit*, short int>> polozeni_ispiti;
    // ispit* moze izazvati gresku pa sam rehasho niz ispita na pocetku da bi izbjegao rehashing naknadno
};

// hashiranje mojih struktura :
namespace std{ 
    template<> struct hash<ispit>{ size_t operator()(const ispit& ispi) const {
        size_t hash = 0;
        boost::hash_combine(hash, ispi.datum_.dan+ispi.datum_.mjesec+ispi.datum_.godina);
        boost::hash_combine(hash, ispi.pred);
        return hash;
    }};
};

student noviStudent(std::istream& in = std::cin, std::ostream& out = std::cout){
    student temp;
    out << "Dodavanje novog studenta;\nIndex : "; in >> temp.index; out << temp.index << '\n';
    out << "Ime : "; in >> temp.ime; out << temp.ime << '\n';
    out << "Prezime : "; in >> temp.prezime; out << temp.prezime << '\n';
    return temp;
}

ispit noviIspit(std::unordered_set<std::string>& predmeti, std::istream& in = std::cin, std::ostream& out = std::cout){
    std::string temp;
    datum date;
    out << "Dodavanje novog ispita;\nIme predmeta : "; 
    in.ignore(1000, '\n');
    std::getline(in, temp);
    auto it = predmeti.find(temp);
    if(it == predmeti.end()) throw std::logic_error{"Predmet nije pronadjen!"};
    out << "Datum (format : dd.mm.yyyy.) : "; in >> date;
    return {date,temp};
}