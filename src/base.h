#ifndef BASE_H
#define BASE_H

#include <array>
#include <iostream>
#include <map>
#include <set>
#include <vector>

using namespace std;

// Representing 54 cards with 0 ~ 53
using Card = unsigned short;
constexpr Card blackJoker = 52;
constexpr Card redJoker = 53;

// Set of Card
using Group = set<Card>;
ostream &operator<<(ostream &os, const Group &group);

// Representing the rank of Card with 0 ~ 14, ignoring suits
// 3 4 5 6 7 8 9 10 J Q K A 2 blackJoker redJoker
// 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14
using Level = unsigned short;
constexpr Level maximumLevel = 15;
constexpr Level maximumChainableLevel = 12;
constexpr Level selfishThresholdLevel = 10;
constexpr Level blackJokerLevel = 13;
constexpr Level redJokerLevel = 14;
constexpr array<Level, 15> allLevels = {0, 1, 2,  3,  4,  5,  6, 7,
                                        8, 9, 10, 11, 12, 13, 14};

// Set of Level
using Combination = set<Level>;
void output(const Combination &combinations);

// Converting Card to Level
Level card_to_level(Card card);

// Converting char to Level
Level char_to_level(char c);

// Amount of Card that have certain Level
using Count = unsigned short;

// For every Level, store the amount of Card that have this Level; a subclass of
// array<Count, maximumLevel>
struct Counter : public array<Count, maximumLevel> {
  Counter();
  // Construct a Counter from a initializer_list
  Counter(const map<Level, Count> &m);
  // Construct a Counter from a Group
  Counter(const Group &group);
  // Construct a Counter from a string
  Counter(const char *s);
  // Construct a Group from a Counter, and since the Counter does not contain
  // the information of suits, a context myCards is needed
  Group get_group(const Group &myCards) const;
  friend ostream &operator<<(ostream &os, const Counter &counter);
};

// Parametric and abstract representation of several Cards played in one turn
struct Hand {
  // Level of the biggest main Card
  Level level;
  // Length of the sequence decreasing from Level level
  Level length;
  // Count of Card that has a Level level
  Count size;
  // Count of Card that has not a Level level
  Count cosize;
  // Set of Levels that are used as attached Cards
  Combination attached;
  Hand(Level _level = 0, Level _length = 1, Count _size = 1, Count _cosize = 0,
       const Combination &_attached = {});
  // Construct a Hand from a Counter
  Hand(const Counter &counter);
  // Construct a Hand from a string
  Hand(const char *s);
  // Construct a Counter from a Hand
  Counter get_counter() const;
  bool operator==(const Hand &hand) const;
  friend ostream &operator<<(ostream &os, const Hand &hand);
  operator string() const;
  bool is_valid() const;
};

// a dummy Hand that means pass
const Hand pass(-1, 0, 0, 0);
// a dummy Hand that means no suitable Hand is found by a Selector
const Hand not_found(-2, 0, 0, 0);
// Rocket
const Hand rocket(redJokerLevel, 2, 1);

// choose k Levels from a bunch of Levels
vector<Combination> combinations(const vector<Level> &universe, unsigned k);

#endif
