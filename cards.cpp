//
// Created by vahna on 14.04.2021.
//



#include "cards.h"

PatienceSolver::CardsSequence::CardsSequence(int card_num) : top(card_num), bot(card_num) {}

std::string PatienceSolver::CardsSequence::ToString() const {
    if (top == bot) {
        return std::to_string(top);
    }
    return std::to_string(top) + "-" + std::to_string(bot);
}

std::vector<std::pair<int, int>> PatienceSolver::Board::BestMoves() const {
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

std::string PatienceSolver::Board::Configuration() const {
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

bool PatienceSolver::Board::MoveCardsSequence(int down, int top) {
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


