#pragma once
#include <fstream>
#include "ispit_i_student.hpp"

void ucitajBazu(std::unordered_multimap<datum, ispit>& ispiti, std::unordered_map<std::string,student>& studenti, std::unordered_set<std::string>& predmeti){
    std::ifstream input;
    std::ofstream output;
    input.open("baza.txt");
    output.open("proba.txt");
    short which;
    while(input){
        input >> which;
        switch (which) {
        case 1 : {
            input.clear();
            auto temp = noviStudent(input, output);
            output << '\n';
            studenti.emplace(temp.index,std::move(temp));
            break;
        } case 2 : {
            input.clear();
            auto temp = noviIspit(predmeti, input, output);
            output << '\n';
            ispiti.emplace(temp.datum_,temp);
            break;
        } case 3 : {
            std::string temp;
            input.clear();
            std::getline(input, temp);
            temp.erase(0,1);
            output << "Predmet : " << temp << '\n';
            predmeti.emplace(temp);
            break;
        } 
        default: break;
        }
    }
    input.close();
    output.close();
}