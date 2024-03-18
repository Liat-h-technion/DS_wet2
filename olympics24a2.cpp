#include "olympics24a2.h"

olympics_t::olympics_t() {}

olympics_t::~olympics_t()
{
	// TODO: Your code goes here
}


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

StatusType olympics_t::add_player(int teamId, int playerStrength)
{
	// TODO: Your code goes here
	return StatusType::SUCCESS;
}

StatusType olympics_t::remove_newest_player(int teamId)
{
	// TODO: Your code goes here
	return StatusType::SUCCESS;
}

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
    int first_score = 0, second_score = 0;
    first_score = team1->get_strength();
    second_score = team2->get_strength();
    if(first_score > second_score){
        teams_rank_tree.add_wins_in_range(team1->get_pair_key(), team1->get_pair_key(), 1);
    }
    if(first_score < second_score){
        teams_rank_tree.add_wins_in_range(team2->get_pair_key(), team2->get_pair_key(), 1);
    }
    if(first_score == second_score){
        if(teamId1 > teamId2){
            teams_rank_tree.add_wins_in_range(team1->get_pair_key(), team1->get_pair_key(), 1);
        }
        else{
            teams_rank_tree.add_wins_in_range(team2->get_pair_key(), team2->get_pair_key(), 1);
        }
    }
    return StatusType::SUCCESS;
}

output_t<int> olympics_t::num_wins_for_team(int teamId)
{
    // TODO: Your code goes here
    static int i = 0;
    return (i++==0) ? 11 : 2;
}

output_t<int> olympics_t::get_highest_ranked_team()
{
	// TODO: Your code goes here
    return 42;
}

StatusType olympics_t::unite_teams(int teamId1, int teamId2)
{
	// TODO: Your code goes here
    return StatusType::SUCCESS;
}

output_t<int> olympics_t::play_tournament(int lowPower, int highPower)
{
    // TODO: Your code goes here
    static int i = 0;
    return (i++==0) ? 11 : 2;
}
