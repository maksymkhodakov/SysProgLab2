#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <string>

/**
SysProgLab2 Khodakov Maksym TTP-32:
Finite Automaton:
States: {0, 1, 2, 3}
Start state: 0
Final states: {3}
Transitions:
0 --a--> 1
1 --a--> 2
1 --b--> 3
2 --a--> 3
2 --b--> 2
3 --a--> 3
3 --b--> 3
 */

struct Automaton {
    int size;
    int startState;
    std::vector<int> finalStates;
    std::unordered_map<int, std::unordered_map<char, int>> transitions;
};

Automaton readAutomatonFromFile(const std::string &filename) {
    Automaton automaton;
    std::ifstream file(filename);

    if (!file) {
        std::cerr << "Failed to open file." << std::endl;
        exit(1);
    }

    file >> automaton.size;
    file >> automaton.startState;

    int numOfFinalStates;
    file >> numOfFinalStates;
    automaton.finalStates.resize(numOfFinalStates);
    for (int i = 0; i < numOfFinalStates; ++i) {
        file >> automaton.finalStates[i];
    }

    int from, to;
    char transitionChar;
    while (file >> from >> transitionChar >> to) {
        automaton.transitions[from][transitionChar] = to;
    }

    file.close();
    return automaton;
}

bool isWordAccepted(const Automaton &automaton, const std::string &word) {
    int currentState = automaton.startState;
    for (char ch : word) {
        if (automaton.transitions.at(currentState).find(ch) == automaton.transitions.at(currentState).end()) {
            return false;
        }
        currentState = automaton.transitions.at(currentState).at(ch);
    }
    for (int state : automaton.finalStates) {
        if (currentState == state) return true;
    }
    return false;
}

bool checkForWords(const Automaton &automaton, const std::string &w1, const std::string &w2) {
    for (int len = 1; len <= 10; ++len) {
        for (int i = 0; i < (1 << len); ++i) {
            std::string w0;
            for (int j = 0; j < len; ++j) {
                if (i & (1 << j)) w0 += 'b';
                else w0 += 'a';
            }
            if (isWordAccepted(automaton, w1 + w0 + w2)) return true;
        }
    }
    return false;
}

/**
Test cases:
Positive:
w1 = "a", w2 = "a"
Reason: The word "abba" exists, where w0 = "bb".

w1 = "a", w2 = "b"
Reason: The word "aab" exists, where w0 = "a".

Negative:
w1 = "b", w2 = "a"
Reason: No word starts with 'b' and ends with 'a' that's accepted by the automaton.

w1 = "aa", w2 = "bb"
Reason: No word starts with "aa" and ends with "bb" that's accepted by the automaton.
 */
int main() {
    Automaton automaton = readAutomatonFromFile("../automation.txt");
    std::string w1, w2;

    std::cout << "Enter w1: ";
    std::cin >> w1;
    std::cout << "Enter w2: ";
    std::cin >> w2;

    if (checkForWords(automaton, w1, w2)) {
        std::cout << "The automaton admits words of the form w = w1 w0 w2." << std::endl;
    } else {
        std::cout << "The automaton does not admit words of the form w = w1 w0 w2." << std::endl;
    }

    return 0;
}
