#include "olympics24a2.h"


/* Complexity: time: O(1), space: O(1)
 */
olympics_t::olympics_t() {}


/* Complexity: time: O(n+k) worst case, space: O(1) amortized on average
 */
olympics_t::~olympics_t()
{
    teams_hash.deAllocateAllInfo();
}


/* Complexity: time: O(1) Amortized on average
 */
StatusType olympics_t::add_team(int teamId)
{
	if (teamId <= 0) {
        return StatusType::INVALID_INPUT;
    }
    if(teams_hash.find(teamId)){
        return StatusType::FAILURE;
    }
    try {
        Team *new_team = new Team(teamId);
        if (!new_team) {
            return StatusType::ALLOCATION_ERROR;
        }
        teams_hash.insert(teamId, new_team);
    }
    catch(const std::bad_alloc&) {
        return StatusType::ALLOCATION_ERROR;
    }
    return StatusType::SUCCESS;
}

/* Complexity: time: O( log n +k(in team) ) Amortized on average
 * (because remove from hash table is O(1) amortized, and remove from rank tree is O(log n)
 */
StatusType olympics_t::remove_team(int teamId)
{
    if(teamId<=0){
        return StatusType::INVALID_INPUT;
    }
    Team* removed_team = teams_hash.find(teamId);
    if(!removed_team){
        return StatusType::FAILURE;
    }
    try{
        teams_hash.erase(teamId);
        teams_rank_tree.erase(removed_team->get_pair_key());
        delete removed_team;
    }
    catch (const std::bad_alloc&) {
        return StatusType::ALLOCATION_ERROR;
    }
	return StatusType::SUCCESS;
}


/* Complexity: time: O( log n + log k ) worst case
 */
StatusType olympics_t::add_player(int teamId, int playerStrength)
{
	if (teamId <= 0 || playerStrength <= 0) {
        return StatusType::INVALID_INPUT;
    }

    Team* team = teams_hash.find(teamId);
    if (!team) {
        // Team doesn't exist
        return StatusType::FAILURE;
    }

    int wins = 0;
    if (team->getSize() > 0) {
        // Remove the team from the teams rank tree (and save the amount of wins the team has):
        wins = teams_rank_tree.get_num_wins(team->get_pair_key());
        teams_rank_tree.erase(team->get_pair_key());
    }
    else {
        // If team is empty, use the previous number of wins from the team and reset previous_wins
        wins = team->get_previous_wins();
        team->set_previous_wins(0);
    }

    // Add a player to the team:
    team->add_player(playerStrength);

    // Re-add the team to the teams rank tree (and re-add the wins)
    teams_rank_tree.insert(team->get_pair_key(), team);
    teams_rank_tree.add_wins_in_range(team->get_pair_key(), team->get_pair_key(), wins);
	return StatusType::SUCCESS;
}


/* Complexity: time: O( log n + log k ) worst case
 */
StatusType olympics_t::remove_newest_player(int teamId)
{
    if (teamId <= 0) {
        return StatusType::INVALID_INPUT;
    }

    Team* team = teams_hash.find(teamId);
    if (!team || team->getSize() == 0) {
        // Team doesn't exist or is empty
        return StatusType::FAILURE;
    }

    // Remove the team from the teams rank tree (and save the amount of wins the team has):
    int wins = teams_rank_tree.get_num_wins(team->get_pair_key());
    teams_rank_tree.erase(team->get_pair_key());

    // Remove the player:
    team->remove_newest_player();

    // If the team is not empty, re-add it to the teams rank tree (and re-add the wins)
    if (team->getSize() > 0) {
        teams_rank_tree.insert(team->get_pair_key(), team);
        teams_rank_tree.add_wins_in_range(team->get_pair_key(), team->get_pair_key(), wins);
    }
    else {
        team->set_previous_wins(wins);
    }
	return StatusType::SUCCESS;
}


/* Complexity: time: O(log n) worst case
 */
output_t<int> olympics_t::play_match(int teamId1, int teamId2)
{
    if(teamId1<=0 || teamId2<=0 || teamId1 == teamId2){
        return StatusType::INVALID_INPUT;
    }
    Team* team1 = teams_hash.find(teamId1);
    Team* team2 = teams_hash.find(teamId2);
    if(!team1 || !team2 || team1->isEmpty() || team2->isEmpty()){
        return StatusType::FAILURE;
    }
    int first_score, second_score;
    first_score = team1->get_strength();
    second_score = team2->get_strength();
    if(first_score > second_score){
        teams_rank_tree.add_wins_in_range(team1->get_pair_key(), team1->get_pair_key(), 1);
        return teamId1;
    }
    else if(first_score < second_score){
        teams_rank_tree.add_wins_in_range(team2->get_pair_key(), team2->get_pair_key(), 1);
        return teamId2;
    }
    else{  //(first_score == second_score)
        if(teamId1 < teamId2){
            teams_rank_tree.add_wins_in_range(team1->get_pair_key(), team1->get_pair_key(), 1);
            return teamId1;
        }
        else{
            teams_rank_tree.add_wins_in_range(team2->get_pair_key(), team2->get_pair_key(), 1);
            return teamId2;
        }
    }
}


/* Complexity: time: O(log n) worst case
 */
output_t<int> olympics_t::num_wins_for_team(int teamId)
{
    if (teamId <= 0) {
        return StatusType::INVALID_INPUT;
    }

    Team* team = teams_hash.find(teamId);
    if (!team) {
        // Team doesn't exist
        return StatusType::FAILURE;
    }

    if (team->getSize() == 0) {
        return team->get_previous_wins();
    }
    return teams_rank_tree.get_num_wins(team->get_pair_key());;
}


/* Complexity: time: O(1) worst case
 */
output_t<int> olympics_t::get_highest_ranked_team()
{
    if(teams_hash.isEmpty()){
        return -1;
    }
    if(teams_rank_tree.isEmpty()){
        return 0;
    }
	return teams_rank_tree.get_max_rank();
}


/* Complexity: time: O(log n + k1 + k2 ) Amortized on average (because of the remove_team)
 */
StatusType olympics_t::unite_teams(int teamId1, int teamId2)
{
    if (teamId1 <= 0 || teamId2 <= 0 || teamId1 == teamId2) {
        return StatusType::INVALID_INPUT;
    }
    Team* team1 = teams_hash.find(teamId1);
    Team* team2 = teams_hash.find(teamId2);
    if (!team1 || !team2) {
        return StatusType::FAILURE;
    }

    // Remove both teams from the teams rank tree (and save the amount of wins team1 has):
    int wins = 0;
    if (team1->getSize() > 0) {
        // Remove the team from the teams rank tree (and save the amount of wins the team has):
        wins = teams_rank_tree.get_num_wins(team1->get_pair_key());
        teams_rank_tree.erase(team1->get_pair_key());
    }
    else {
        // If team is empty, use the previous number of wins from the team and reset previous_wins
        wins = team1->get_previous_wins();
        team1->set_previous_wins(0);
    }
    teams_rank_tree.erase(team2->get_pair_key());

    // Unite the teams:
    team1->unite_teams(*team2);

    // If team1 is not empty, re-add it to the teams rank tree (and re-add the wins)
    if (team1->getSize() > 0) {
        teams_rank_tree.insert(team1->get_pair_key(), team1);
        teams_rank_tree.add_wins_in_range(team1->get_pair_key(), team1->get_pair_key(), wins);
    }
    else {
        team1->set_previous_wins(wins);
    }

    // Remove team2 from olympics
    remove_team(teamId2);

    return StatusType::SUCCESS;
}


bool power_of_two(int x) {
    if (ceil(log2(x)) == floor(log2(x))) {
        return true;
    }
    return false;
}


/* Complexity: time: O( (log i)*(log n) ) worst case
 */
output_t<int> olympics_t::play_tournament(int lowPower, int highPower)
{
    if (lowPower <= 0 || highPower <= 0 || highPower <= lowPower) {
        return StatusType::INVALID_INPUT;
    }

    // Find the lowest key in which strength >= lowPower, and the highest key in which  strength <= highPower
    Pair low_team_key = teams_rank_tree.getNextKey(Pair(lowPower-1, -1));
    Pair high_team_key = teams_rank_tree.getPrevKey(Pair(highPower, -1));

    if (low_team_key == Pair() || high_team_key == Pair()) {
        return StatusType::FAILURE;
    }

    // Get indexes of low and high teams in tournament:
    int low_index = teams_rank_tree.get_index_from_key(low_team_key);
    int high_index = teams_rank_tree.get_index_from_key(high_team_key);
    int count_teams_in_tournament = high_index - low_index + 1;
    if (low_index > high_index || low_index < 1 || high_index > teams_rank_tree.getSize() ||
        !power_of_two(count_teams_in_tournament)) {
        // Indexes must be valid, and amount of teams in tournament must be a power of 2
        return StatusType::FAILURE;
    }

    // Run for log(count_teams_in_tournament) iterations, each add a win to all teams between median and high,
    // And update low to be the median
    for (int i=1; i<=log2(count_teams_in_tournament); i++) {
        int mid = (high_index - low_index + 1) / 2 + low_index;
        Pair mid_team_key = teams_rank_tree.get_key_from_index(mid);
        teams_rank_tree.add_wins_in_range(mid_team_key, high_team_key, 1);
        low_index = mid;
    }

    return high_team_key.second; // Return the winning team id
}
