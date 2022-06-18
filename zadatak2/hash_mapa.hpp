#pragma once
#include <list>
#include <functional>
#include <bitset>
#include <vector>
#include <iostream>

template<typename key, typename T>
class hash_map{
public:
    class iterator;

    hash_map();
    hash_map(size_t);
    hash_map(const hash_map<key,T>&);
    hash_map(hash_map<key,T>&&);
    // hash_map<key,T>& operator=(const hash_map<key,T>&);
    template<typename U>
    hash_map<key,T>& operator=(U&&);
    ~hash_map<key,T>();

    T& operator[](const key&);
    bool erase(const key&);
    hash_map<key, T>& emplace(const key&, const T&);
    size_t size(){return size_;}

    iterator find(const key&);
    iterator end();
    iterator begin();

    void print(){
        for(size_t i = 0; i < elements.size()-1; ++i){
            std::cout << "List" << i << " : ";
            for(auto& k : elements.at(i)) std::cout << k.second << ' ';
            std::cout << std::endl;
        }
    }

    void rehash();
private:
    friend class hash_map<key,T>::iterator;

    size_t size_;
    size_t maxBucketSize;
    size_t capacity;
    std::vector<std::list<std::pair<key,T>>> elements;

    size_t hash(const key& kljuc);
};

// konstruktori

// zbog jednostavnosti sam ostavio ovaj pristup hashiranja
// svjestan sam da nije dobar, ali radi dobro za stringove
template<typename key, typename T>
size_t hash_map<key,T>::hash(const key& kljuc){
    std::hash<key> hash;
    return (size_t) (hash(kljuc) % (capacity)) ;
}
template<typename key, typename T>
void hash_map<key,T>::rehash(){
    maxBucketSize = 0;
    hash_map<key,T> novaLista{capacity*2+1};
    for(auto it = begin(); it!=end();++it)
        novaLista.emplace(it->first, it->second);
    elements=std::move(novaLista.elements);
}

template<typename key, typename T>
hash_map<key,T>::hash_map() : size_{0}, capacity{10}, maxBucketSize{0} { elements.resize(capacity+1); }

template<typename key, typename T>
hash_map<key,T>::hash_map(size_t c) : size_{0}, capacity{c}, maxBucketSize{0} { elements.resize(capacity+1); }

template<typename key, typename T>
hash_map<key,T>::hash_map(const hash_map<key,T>& other) 
: size_{other.size_}, maxBucketSize{other.maxBucketSize}, capacity{other.capacity}, elements{other.elements} {}

template<typename key, typename T>
hash_map<key,T>::hash_map(hash_map<key,T>&& other)
: size_{other.size_}, maxBucketSize{other.maxBucketSize}, capacity{other.capacity}, elements{std::move(other.elements)} {}

template<typename key, typename T>
template<typename U>
hash_map<key,T>& hash_map<key,T>::operator=(U&& other){
    size_ = other.size_;
    maxBucketSize = other.maxBucketSize;
    capacity = other.capacity;
    elements = std::forward<U>(other.elements);
}
template<typename key, typename T>
hash_map<key,T>::~hash_map<key,T>(){}

// metodi

template<typename key, typename T>
hash_map<key,T>& hash_map<key, T>::emplace(const key& kljuc, const T& element){
    auto& lista = elements.at( hash(kljuc) );
    lista.push_back({kljuc,element});
    if(lista.size()>maxBucketSize) maxBucketSize = lista.size();
    ++size_;
    if(maxBucketSize > 7) rehash();
    return *this;
}
template<typename key, typename T>
bool hash_map<key,T>::erase(const key& kljuc){
    auto& lista = elements.at( hash(kljuc) );
    auto it = lista.begin();
    while(it->first!=kljuc && it != lista.end()) ++it;
    if(it == lista.end()) return false;
    lista.erase(it);
    --size_;
    return true;
}
template<typename key, typename T>
T& hash_map<key,T>::operator[](const key& kljuc){
    auto& lista = elements.at( hash(kljuc) );
    for(auto it = lista.begin();it != lista.end(); ++it )
        if(it->first == kljuc) return it->second;
    emplace(kljuc,{});
    return (--lista.end())->second;
}
template<typename key, typename T>
typename hash_map<key,T>::iterator hash_map<key,T>::begin(){
    auto it = elements.begin();
    while( !it->size() ) ++it;
    return {it, it->begin(),*this};
}
template<typename key, typename T>
typename hash_map<key,T>::iterator hash_map<key,T>::end(){
    auto it = --elements.end();
    return {it, it->begin() ,*this};
}
template<typename key, typename T>
typename hash_map<key,T>::iterator hash_map<key,T>::find(const key& kljuc){
    auto lista = elements.begin() + hash(kljuc); 
    for(auto it = lista->begin();it != lista->end(); ++it )
        if(it->first == kljuc) return {lista, it, *this};
    return end();
}
// iterator :

template <typename key, typename T>
class hash_map<key, T>::iterator : public std::iterator<std::bidirectional_iterator_tag, std::pair<key,T>> {
using vektor_it = typename std::vector<std::list<std::pair<key,T>>>::iterator;
using list_it = typename std::list<std::pair<key,T>>::iterator;
    
private:
    vektor_it v_it;
    list_it it;
    hash_map<key,T>& mapa;
public:

    iterator(vektor_it v, list_it l, hash_map<key, T>& m) : v_it{v}, it{l}, mapa{m} {}

    bool operator==(iterator const& other) const { return it == other.it; }
    bool operator!=(iterator const& other) const { return it != other.it; }

    iterator operator++(){
        auto limiter = mapa.elements.end();
        --limiter;
        if(++it==v_it->end()){
            razmak :
            ++v_it;
            it=v_it->begin();
            if( v_it -> size() == 0 && v_it != limiter ) goto razmak;
        }
        return {v_it, it, mapa };
    }
    iterator operator--(){
        auto limiter = mapa.elements.begin();
        --limiter;
        if(it==v_it->begin()){
            razmak :
            --v_it;
            it=v_it->end();
            --it;
            if( v_it -> size() == 0 && v_it != limiter ) goto razmak;
        } else --it;
        return {v_it, it, mapa };
    }
    iterator operator++(int){
        iterator temp{v_it, it, mapa};
        this -> operator++();
        return temp;
    }
    iterator operator--(int){
        iterator temp{v_it, it, mapa};
        this -> operator--();
        return temp;
    }

    T& operator*() { return it->second; }
    std::pair<key,T>* operator->() const { return &(*it); }
  
};