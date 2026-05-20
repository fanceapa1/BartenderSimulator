#include "Leaderboard.h"

#include <algorithm>
#include <cstddef>
#include <fstream>
#include <ostream>
#include <sstream>
#include <utility>

template <typename T>
void Leaderboard<T>::loadFromFile(const std::string& filename) {
    entries.clear();

    std::ifstream in(filename);

    if(!in) {
        return;
    }

    std::string line;

    while(std::getline(in, line)) {
        std::istringstream lineInput(line);
        T score {};
        std::string name;

        if(!(lineInput >> score)) {
            continue;
        }

        std::getline(lineInput >> std::ws, name);

        if(name.empty()) {
            continue;
        }

        entries.push_back({name, score});
    }
}

template <typename T>
void Leaderboard<T>::addEntry(std::string name, T score) {
    entries.push_back({std::move(name), score});
}

template <typename T>
void Leaderboard<T>::sortByScoreDescending() {
    std::ranges::sort(entries, [](const Entry& first, const Entry& second) {
        return first.score > second.score;
    });
}

template <typename T>
void Leaderboard<T>::saveToFile(const std::string& filename) const {
    std::ofstream out(filename);

    for(const Entry& entry : entries) {
        out << entry.score << "\t" << entry.name << "\n";
    }
}

template <typename T>
void Leaderboard<T>::print(std::ostream& out) const {
    out << "Leaderboard:\n";

    for(std::size_t index = 0; index < entries.size(); ++index) {
        out << index + 1 << ". " << entries[index].name << " - " << entries[index].score << "$\n";
    }
}

template class Leaderboard<double>;
