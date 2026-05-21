#ifndef BARTENDER_LEADERBOARD_H
#define BARTENDER_LEADERBOARD_H

#include <iosfwd>
#include <string>
#include <vector>

template <typename T>
class Leaderboard {
private:
    struct Entry {
        std::string name;
        T score;
    };

    std::vector<Entry> entries;

public:
    void loadFromFile(const std::string& filename);
    void addEntry(std::string name, T score);
    void sortByScoreDescending();
    void saveToFile(const std::string& filename) const;
    void print(std::ostream& out, const std::string& title, const std::string& suffix) const;
};

extern template class Leaderboard<double>;
extern template class Leaderboard<int>;

#endif
