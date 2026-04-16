#include <vector>

using State = std::size_t;

struct Transition {
    enum class Label { Char, Epsilon, AnyChar } label;
    char c;
    State destination;
};  

class NFA {
    public: 
        State fresh();
        void addCharTransition(State from, char c, State to);
        void addEpsilonTransition(State from, char c, State to);
        void addDotTransition

    private:
        std::vector<std::vector<Transition>> transitions;
        State start;
        State accept;
};
