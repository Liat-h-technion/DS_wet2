#include "Team.h"

int Team::getSize() const {
    return players_tree.getSize();
}

int Team::get_strength() const {
    if (median_player == Player()) {
        return 0;
    }
    return median_player.first * this->getSize();
}

void Team::add_player(int strength) {
    int player_id = this->getSize() + 1;
    Player player = Player(strength, player_id);
    players_stack.push(player);
    players_tree.insert(player, nullptr);
    update_median_in_insert(player);
}

void Team::update_median_in_insert(Player new_player) {
    if (this->getSize() == 1) {
        median_player = new_player;
    }
    else if (this->getSize() % 2 == 0 && new_player > median_player) {
        median_player = players_tree.getNextKey(median_player, Player());
    }
    else if (this->getSize() % 2 != 0 && new_player < median_player) {
        median_player = players_tree.getPrevKey(median_player, Player());
    }
}

void Team::update_median_in_erase(Player removed_player) {
    if (this->getSize() == 0) {
        median_player = Player();
    }
    else if (this->getSize() % 2 == 0 && removed_player <= median_player) {
        median_player = players_tree.getNextKey(median_player, Player());
    }
    else if (this->getSize() % 2 != 0 && removed_player >= median_player) {
        median_player = players_tree.getPrevKey(median_player, Player());
    }
}

void Team::remove_newest_player() {
    if (this->getSize() == 0) {
        return;
    }
    Player removed_player = players_stack.pop();
    players_tree.erase(removed_player);
    update_median_in_erase(removed_player);
}

void Team::unite_teams(Team &team2) {
    if (team2.getSize() == 0) {
        return;
    }

    // Move all players from team2 to this team's stack (with new ids)
    Stack tmp_stack;
    while (!team2.players_stack.isEmpty()) {
        Player player = team2.players_stack.pop();
        player.second += this->getSize(); // increase the player ids in team2 by the current team's size
        tmp_stack.push(player);
    }
    while (!tmp_stack.isEmpty()) {
        players_stack.push(tmp_stack.pop());
    }

    if (this->getSize() == 0) {
        this->players_tree = team2.players_tree;
        this->median_player = team2.median_player;
        team2.players_tree = AVLTree<Player, std::nullptr_t>();
        team2.median_player = Player();
        return;
    }

    // Unite the players trees:
    // Create an array from each tree:
    int size1 = this->getSize();
    int size2 = team2.getSize();
    Player* array1 = new Player[size1];
    Player* array2 = new Player[size2];
    players_tree.insertKeysInorderToArray(array1);
    team2.players_tree.insertKeysInorderToArray(array2);
    for (int i=0; i<size2; i++) {
        array2[i].second += size1; // increase the player ids in team2 by the current team's size
    }
    // Merge the arrays into sorted array:
    Player* merged_array = new Player[size1+size2];
    int actual_size = mergeSortedKeys(merged_array, array1, size1, array2, size2);
    // Fill a new tree from the merged array:
    AVLTree<Player, std::nullptr_t> new_tree;
    new_tree.nearlyCompleteTree(actual_size, Player());
    new_tree.insertKeysInorderToArray(merged_array);
    this->players_tree = new_tree;

    // Update the median: Find the new median in the merged array according to the size
    int median_idx = actual_size / 2;
    if (actual_size % 2 == 0) {
        median_idx += 1;
    }
    this->median_player = merged_array[median_idx];

    // de-allocate temp arrays
    delete[] array1;
    delete[] array2;
    delete[] merged_array;
}

int Team::getId() const {
    return team_id;
}

Pair Team::get_pair_key() const {
    return Pair(get_strength(), team_id);
}

int Team::get_previous_wins() const {
    return previous_wins;
}

void Team::set_previous_wins(int wins) {
    previous_wins = wins;
}

bool Team::isEmpty() const {
    if(this->getSize() == 0){
        return true;
    }
    return false;
}
