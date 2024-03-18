#ifndef DS_WET2_TEAM_H
#define DS_WET2_TEAM_H

#include "Pair.h"
#include "Stack.h"
#include "AVLTree.h"

typedef Pair Player;

class Team {
private:
    int team_id;
    Stack players_stack;
    AVLTree<Player,std::nullptr_t> players_tree;
    Player median_player;
    int previous_wins;

    void update_median_in_insert(Player new_player);
    void update_median_in_erase(Player new_player);

public:
    explicit Team(int team_id) : team_id(team_id), previous_wins(0) {};
    ~Team() = default;
    int getSize() const;
    int getId() const;
    int get_strength() const;
    void add_player(int strength);
    void remove_newest_player();
    void unite_teams(Team& other_team);
    Pair get_pair_key() const;
    int get_previous_wins() const;
    void set_previous_wins(int wins);
};


#endif //DS_WET2_TEAM_H
