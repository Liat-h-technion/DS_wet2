#ifndef DS_WET1_PAIR_H
#define DS_WET1_PAIR_H
#define DEFAULT (-1)

class Pair {
public:
    int first;
    int second;
    Pair(int first, int second) : first(first), second(second) {};
    Pair():first(DEFAULT), second(DEFAULT){};
    bool operator>(const Pair& other) const{
        return (first > other.first || (first == other.first && second < other.second));
    }
    bool operator<(const Pair& other) const{
        return (first < other.first || (first == other.first && second > other.second));
    }
    bool operator==(const Pair& other) const{
        return(first == other.first && second == other.second);
    }
    Pair& operator=(const Pair& other) {
        if (this != &other) {
            first = other.first;
            second = other.second;
        }
        return *this;
    }
    bool operator<=(const Pair& other) const{
        return (*this < other || *this == other);
    }
    bool operator>=(const Pair& other) const{
        return (*this > other || *this == other);
    }
};





#endif //DS_WET1_PAIR_H
