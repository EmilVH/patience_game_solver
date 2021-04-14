//
// Created by vahna on 14.04.2021.
//

#ifndef PATIENCE_GAME_SOLVER_CARDS_H
#define PATIENCE_GAME_SOLVER_CARDS_H

#include <stack>
#include <vector>
#include <string>
#include <algorithm>
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <fstream>

namespace PatienceSolver {
    struct CardsSequence {
        int top, bot;

        explicit CardsSequence(int card_num);

        bool operator<(const CardsSequence &other) const {
            return top < other.bot;
        }

        std::string ToString() const;
    };

    const int DECKS_NUM = 8;
    const int MAX_CARD = 8;

    class Board {
    private:
        std::vector<CardsSequence> decks_[DECKS_NUM];
    public:
        bool MoveCardsSequence(int down, int top);

        std::vector<std::pair<int, int>> BestMoves() const;

        std::string Configuration() const;

        friend std::istream &operator>>(std::istream &in, Board &board);
    };

    std::istream &operator>>(std::istream &in, Board &board) {
        std::unordered_map<std::string, int> mp;
        mp["6"] = 0;
        mp["7"] = 1;
        mp["8"] = 2;
        mp["9"] = 3;
        mp["10"] = 4;
        mp["V"] = 5;
        mp["Q"] = 6;
        mp["K"] = 7;
        mp["A"] = 8;
        for (int j = 0; j <= MAX_CARD; ++j) {
            for (auto &deck : board.decks_) {
                std::string tmp;
                in >> tmp;
                deck.emplace_back(mp[tmp]);
            }
        }
        return in;
    }

    bool solute(Board board) {
        std::vector<Board> que;
        std::unordered_set<std::string> used;
        used.insert(board.Configuration());
        que.push_back(std::move(board));
        for (int l = 0; l < que.size(); ++l) {
            auto now = que[l];
            if (now.Configuration() == "||||||||") {
                return true;
            }
            auto moves = now.BestMoves();
            for (auto it: moves) {
                auto copy = now;
                bool good = copy.MoveCardsSequence(it.first, it.second);
                if (!used.count(copy.Configuration())) {
                    if (good) {
                        if (solute(copy)) {
                            return true;
                        }
                    }
                    used.insert(copy.Configuration());
                    que.push_back(copy);
                }
            }
            ++l;
        }
        return false;
    }

    bool solution(const std::string &filename) {
        std::ifstream in(filename);
        Board board;
        in >> board;
        bool res = solute(board);
        return res;
    }
}
#endif //PATIENCE_GAME_SOLVER_CARDS_H
