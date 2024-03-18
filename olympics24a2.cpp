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
	if (teamId <= 0 || playerStrength <= 0) {
        return StatusType::INVALID_INPUT;
    }

    Team* team = teams_hash.find(teamId);
    if (!team) {
        // Team doesn't exist
        return StatusType::FAILURE;
    }


    // Remove the team from the teams rank tree (and save the amount of wins the team has):
    int wins = teams_rank_tree.get_num_wins(team->get_pair_key());
    teams_rank_tree.erase(team->get_pair_key());

    // Add a player to the team:
    team->add_player(playerStrength);

    // Re-add the team to the teams rank tree (and re-add the wins)
    teams_rank_tree.insert(team->get_pair_key(), team);
    teams_rank_tree.add_wins_in_range(team->get_pair_key(), team->get_pair_key(), wins);
	return StatusType::SUCCESS;
}


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
	return StatusType::SUCCESS;
}


output_t<int> olympics_t::play_match(int teamId1, int teamId2)
{
    // TODO: Your code goes here
    return 2008;
}


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

    return teams_rank_tree.get_num_wins(team->get_pair_key());;
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
