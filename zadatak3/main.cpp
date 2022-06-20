// VAZNA NAPOMENA !!
// da bi mogli kompajlirati ovaj program potrebna vam je boost biblioteka
// koja mora biti instalirana na racunaru
//
// s pristupom u ovom zadatku sesti uslov iz zadace nece biti tacno O(1)
// ali ce biti O(1+bucket_size) sto je priblizno jednako O(1)
// 
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include "datum.hpp"
#include "ispit_i_student.hpp"
#include "database.hpp"

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
        std::cin.ignore(1000, '\n');
        std::cout << "Predmet :"; std::getline(std::cin, predmet);
        std::cout << "Datum : "; std::cin >> date;
        auto it = pronadjiIspit(ispiti, date, predmet);

        std::cout << "Indeks studenta : "; std::cin >> indeks;
        auto s_it = studenti.find(indeks);
        if(s_it == studenti.end()) throw std::logic_error{"Student nije pronadjen"};

        std::cout << "Ocjena : "; std::cin >> ocjena;
        it->second.studenti.push_back({indeks,ocjena});

        std::pair<ispit*, short> toInsert{&(it->second), ocjena};
        if(ocjena>5) s_it->second.polozeni_ispiti.emplace(date, std::move(toInsert));
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
    std::cin.ignore(1000, '\n');
    std::cout << "Predmet : "; std::getline(std::cin, predmet);
    std::cout << "Datum : "; std::cin >> date;
    auto it = pronadjiIspit(ispiti, date, predmet);
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
            std::cout << "\nDatum : " << i.first << "\nPredmet : " << i.second.first->pred << "\nOcejna :"  << i.second.second;
            prosjecna_ocjena += i.second.second;
        }
    std::cout << "\nProsjecna ocjena : " << prosjecna_ocjena/it->second.polozeni_ispiti.size() << "\n-------------------\n";
}


//main
int main(){
    std::unordered_multimap<datum, ispit> ispiti;
    std::unordered_map<std::string,student> studenti;
    std::unordered_set<std::string> predmeti;

    ispiti.rehash(50);

    ucitajBazu(ispiti, studenti, predmeti); // samo za ucitavanje malo predmeta i studenata da ne moram svaki put

    for(auto& i : predmeti) std::cout << i << ' ';
    std::cout << std::endl;

    int odabir = 0;
    std::cout << "1. Dodaj novog studenta\n2. Dodaj novi ispit\n3. Dodaj novi predmet\n4. Dodaj novi rezultat ispita\n5. Dohvati sve ispite za određeni datum\n6. Dohvati po datumu i imenu predmeta, sve studente koji su položili određeni ispit\n7. Dohvati po indeksu sve ispite koje je položio student hronološki, te prosjecnu ocjenu" << std::endl;
    while(std::cin >> odabir){
        try{
        switch (odabir){
        case 1: {
            auto temp = noviStudent();
            studenti.emplace(temp.index,std::move(temp));
            break; 
        } case 2: {
            auto temp = noviIspit(predmeti);
            ispiti.emplace(temp.datum_,temp);
            break;
        } case 3: {
            std::cout << "Ime predmeta: ";
            std::string temp; 
            std::cin.ignore(1000, '\n');
            std::getline(std::cin, temp);
            predmeti.emplace(temp);
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
        }catch(std::logic_error& a){ std::cout << "\n" <<a.what() << "\n----------------------\n"; }
    std::cout << "1. Dodaj novog studenta\n2. Dodaj novi ispit\n3. Dodaj novi predmet\n4. Dodaj novi rezultat ispita\n5. Dohvati sve ispite za određeni datum\n6. Dohvati po datumu i imenu predmeta, sve studente koji su položili određeni ispit\n7. Dohvati po indeksu sve ispite koje je položio student hronološki, te prosjecnu ocjenu" << std::endl;
    }

    return 0;
}