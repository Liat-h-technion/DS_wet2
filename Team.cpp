#include "Team.h"

int Team::getSize() {
    return players_tree.getSize();
}

int Team::get_strength() {
    return median_player.first;
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
        median_player = players_tree.getNextKey(median_player);
    }
    else if (this->getSize() % 2 != 0 && new_player < median_player) {
        median_player = players_tree.getPrevKey(median_player);
    }
}

void Team::update_median_in_erase(Player removed_player) {
    if (this->getSize() == 0) {
        median_player = Player();
    }
    else if (this->getSize() % 2 == 0 && removed_player <= median_player) {
        median_player = players_tree.getNextKey(median_player);
    }
    else if (this->getSize() % 2 != 0 && removed_player >= median_player) {
        median_player = players_tree.getPrevKey(median_player);
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
    AVLTree<Player, nullptr_t> new_tree;
    new_tree.nearlyCompleteTree(actual_size, Player());
    new_tree.insertKeysInorderToArray(merged_array);
    this->players_tree = new_tree;

    // Update the median

    // de-allocate temp arrays
    delete[] array1;
    delete[] array2;
    delete[] merged_array;
}
