#include <vector>
#include <cstddef>
#include <algorithm>
#include <iterator>
#include <cstring>
#include <iostream>

// VAZNO : insert metod  - insert( prioritet(tipa int), podaci - genericki tip )

template<typename T>
class heap
{
public:
    heap();
    heap(heap &&);
    heap(const heap &);
    heap &operator=(heap &&);
    heap &operator=(const heap &);
    ~heap();

    template<typename U>
    heap<T>& insert(U&&);
    T min();
    T max();
    T removeMin();
    T removeMax();

    size_t size() const { return size_; }

    void print() const {
        print_rek(0);
    }

    void print2(){
        for(size_t i = 0; i < size_; ++i) std::cout << arr[i] << ' ';
        std::cout << std::endl;
    }

private:
    // struct par{
    //     int prioritet;
    //     T data;
    //     bool operator>(const par& other){ return prioritet > other.prioritet; }
    //     par& operator=(const par& other){prioritet=other.prioritet;data=other.data;return *this;}
    // };

    T* arr;
    size_t capacity_;
    size_t size_;

    void reallocate();
    void srediNagore(size_t);
    void srediNadole(size_t i, size_t n);
    void print_rek(size_t) const;
};

template<typename T>
std::ostream& operator<<(std::ostream& out, heap<T>& hip){
    hip.print();
    return out;
}

template<typename T>
void heap<T>::print_rek(size_t index) const {
    if(index*2+1<size_) print_rek(index*2+1);
    std::cout << arr[index] << ' ';
    if(index*2+2<size_) print_rek(index*2+2);
}

template<typename T>
heap<T>::heap() : arr{new T[15]}, capacity_{15}, size_{0} {}

template<typename T>
heap<T>::heap(heap&& other) : capacity_{other.capacity_}, size_{other.size_}, arr{other.arr} {
    other.size_ = other.capacity_ = 0;
    other.arr = nullptr;
}

template<typename T>
heap<T>::heap(const heap& other) : capacity_{other.capacity_}, size_{other.size_}, arr{new T[other.capacity_]} {
    memcpy(arr, other.arr, sizeof(T) * other.size_ );
    // memcpy(arr,other.arr,(sizeof(int)+sizeof(T) * size_ ));
}

template<typename T>
heap<T>& heap<T>::operator=(const heap<T>& other){
    delete [] arr;
    capacity_ = other.capacity_;
    size_ = other.size_;
    arr = new T[capacity_];
    memcpy(arr, other.arr, sizeof(T) * other.size_ );
    return *this;
}

template<typename T>
heap<T>& heap<T>::operator=(heap<T>&& other){
    std::swap(arr, other.arr);
    capacity_ = other.capacity_;
    size_ = other.size_;
    return *this;
}


template<typename T>
heap<T>::~heap() {delete [] arr;}

template<typename T>
void heap<T>::reallocate(){
    capacity_+=capacity_*2;
    T* temp = new T[capacity_];
    memcpy(temp, arr, sizeof(T) * size_ );
    std::swap(arr, temp);
    delete [] temp;
}

template<typename T>
void heap<T>::srediNagore(size_t index){
    while(index >= 1 && arr[index] > arr[index/2]){
        std::swap(arr[index], arr[index/2]);
        index/=2;
    }
}

template<typename T>
void heap<T>::srediNadole(size_t i, size_t n){
    int largest = i;
    int l = 2 * i + 1;
    int r = 2 * i + 2;

    if (l < n && arr[l] > arr[largest]) largest = l;
    if (r < n && arr[r] > arr[largest]) largest = r;
    if (largest != i) {
        std::swap(arr[i], arr[largest]);
        srediNadole(largest, n);
    }
}

template<typename T>
template<typename U>
heap<T>& heap<T>::insert(U&& elem){
    if(size_==capacity_) reallocate();
    auto temp = size_;
    arr[size_++] = std::forward<U>(elem);
    srediNagore(temp);
    return *this;
}

template<typename T>
T heap<T>::min(){
    if(!size_) throw std::logic_error{"Empty heap"};
    T k = arr[size_/2];
    for (auto i = size_/2+1; i < size_; ++i) k = std::min(k, arr[i]);
    return k;
}

template<typename T>
T heap<T>::max(){
    if(!size_) throw std::logic_error{"Empty heap"};
    return arr[0];
}

template<typename T>
T heap<T>::removeMax(){
    if(!size_) throw std::logic_error{"Empty heap!"};
    auto t = arr[0];
    std::swap(arr[0], arr[size_-1]);
    srediNadole(0, --size_);
    return t;
}

template<typename T>
T heap<T>::removeMin(){
    for (auto i = capacity_/2-1; i < size_; ++i)
        if(arr[i]<arr[size_-1]) std::swap(arr[i],arr[size_-1]);
    auto t = arr[--size_];
    for (auto i = capacity_/2; i < size_; ++i) srediNagore(i);
    return t;
}