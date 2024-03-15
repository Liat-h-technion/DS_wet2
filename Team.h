#ifndef DS_WET2_TEAM_H
#define DS_WET2_TEAM_H

#include "Pair.h"
#include "Stack.h"
#include "AVLTree.h"

typedef Pair Player;

class Team {
private:
    Stack players_stack;
    AVLTree<Player,nullptr_t> players_tree;
    Player median_player;

    void update_median_in_insert(Player new_player);
    void update_median_in_erase(Player new_player);

public:
    Team() = default;
    ~Team() = default;
    int getSize();
    int get_strength();
    void add_player(int strength);
    void remove_newest_player();
    void unite_teams(Team& other_team);
};


#endif //DS_WET2_TEAM_H
