#include <cassert>
#include <vector>

template <typename T>
void ReverseArray(T* start, std::size_t size) {
    
    T* end_pointer = start + size;
    
    T* temp = new T[size];
    std::size_t it = 0;
    while (size - it)
    {
        temp[it] = *(end_pointer - it - 1);
        ++it;
    }

    for (std::size_t i = 0; i < size; ++i){
        start[i] = temp[i];
    }
    
}

int main() {
    using namespace std;
    
    vector<int> v = {1, 2};
    ReverseArray(v.data(), v.size());
    assert(v == (vector<int>{2, 1}));
}