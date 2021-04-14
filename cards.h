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


struct CardsSequence {
    int top, bot;

    explicit CardsSequence(int card_num) : top(card_num), bot(card_num) {}

    bool operator<(const CardsSequence &other) const {
        return top < other.bot;
    }

    std::string ToString() const {
        if (top == bot) {
            return std::to_string(top);
        }
        return std::to_string(top) + "-" + std::to_string(bot);
    }
};

const int DECKS_NUM = 8;
const int MAX_CARD = 8;

class Board {
private:
    std::vector<CardsSequence> decks_[DECKS_NUM];
public:
    bool MoveCardsSequence(int down, int top) {
        if (decks_[down].empty()) {
            return false;
        }
        if (decks_[top].empty()) {
            CardsSequence tmp(decks_[down].back());
            decks_[down].pop_back();
            decks_[top].push_back(tmp);
            return false;
        } else {
            if (decks_[down].back() < decks_[top].back()) {
                CardsSequence tmp(decks_[down].back());
                decks_[down].pop_back();
                if (decks_[top].back().bot == tmp.top + 1) {
                    decks_[top].back().bot = tmp.bot;
                    if (decks_[top].back().bot == 0 && decks_[top].back().top == MAX_CARD) {
                        decks_[top].pop_back();
                        return true;
                    }
                } else {
                    decks_[top].push_back(tmp);
                }
                return false;
            } else {
                return false;
            }
        }
    }

    std::vector<std::pair<int, int>> BestMoves() const {
        std::vector<std::pair<int, int>> ans;
        for (size_t i = 0; i < DECKS_NUM; ++i) {
            for (size_t j = 0; j < DECKS_NUM; ++j) {
                if (i == j) continue;
                if (decks_[i].empty()) continue;
                if (decks_[j].empty()) {
                    ans.emplace_back(i, j);
                } else if (decks_[i].back() < decks_[j].back() &&
                           decks_[i].back().top + 1 == decks_[j].back().bot) {
                    ans.emplace_back(i, j);
                }
            }
        }
        return ans;
    }

    std::string Configuration() const {
        std::vector<std::string> decks;
        for (const auto &deck : decks_) {
            std::string s;
            for (size_t j = 0; j < deck.size(); ++j) {
                s += deck[j].ToString();
            }
            decks.push_back(s);
        }
        std::sort(decks.begin(), decks.end());
        std::string ans;
        for (const auto &deck : decks) {
            ans += deck + "|";
        }
        return ans;
    }

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
            //std::cerr << tmp;
            deck.emplace_back(mp[tmp]);
        }
    }
    return in;
}


bool solute(Board board){
    std::vector<Board> que;
    std::unordered_set<std::string> used;
    used.insert(board.Configuration());
    que.push_back(std::move(board));
    for (int l = 0; l < que.size(); ++l) {
        if (l % 10000 == 9999)
            std::cerr << l << std::endl;
        auto now = que[l];
        if (now.Configuration() == "||||||||") {
            return true;
        }
        auto moves = now.BestMoves();
        for (auto it: moves) {
            auto copy = now;
            bool good = copy.MoveCardsSequence(it.first, it.second);
            if (!used.count(copy.Configuration())) {
                if (good){
                    if (solute(copy)){
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

bool solution() {
    std::ifstream in("test.txt");
    Board board;
    in >> board;
    bool res = solute(board);
    return res;
}

#endif //PATIENCE_GAME_SOLVER_CARDS_H
