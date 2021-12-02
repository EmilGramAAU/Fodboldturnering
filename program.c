//***************************************************
//* Programmer: Emil Gram Jensen
//* Class: Software (Gruppe 8)
//* Programming Assignment: Eksamensopgave 3
//* Date:
//***************************************************

#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

// CONSTANTS
#define MATCHES 132
#define TEAMS 12

//PROTOTYPES
void clear_console();
void get_matches_from_file();
int exits_in_array();
int add_match_to_array();
int calculate_points();
int compare();
void print_list();

typedef struct // TEAM STRUCT
{
    char *name[1];       // TEAM NAME
    int goals;           // TEAM GOALS IN SEASON
    int goals_against;   // TEAM GOALS AGAINST
    int point;           // TOTAL TEAM POINTS IN SEASON
    int goal_difference; // THE TEAM GOAL DIFFERENCE IN SEASON
} Team;

typedef struct // TEAM DETAILS IN MATCH
{
    int goals;    // GOALS TEAM SCORED IN MATCH
    char name[4]; // TEAM NAME
} MatchTeam;

typedef struct // MATCH STRUCT
{
    char weekday[5];     // THE DAY THE MATCH WAS PLAYED
    char date[10];       // THE DATE THE MATCH WAS PLAYED
    char time[10];       // THE TIME THE MATCH WAS PLAYED
    MatchTeam home_team; // HOME TEAM DETAILS IN THE MATCH
    MatchTeam away_team; // AWAY TEAM DETAILS IN THE MATCH
    int viewers;         // THE NUMBER OF VIEWERS IN MATCH
} Match;

/************************************************************
 * Function: main()					     
 * Description: Runs the program
 *************************************************************/
int main(void)
{

    // INITIALIZE STRUCT ARRAYS
    Match matches[MATCHES]; // ARRAY WITH ALL MATCHES
    Team teams[TEAMS];      // ARRAY WITH ALL TEAMS

    get_matches_from_file(matches, teams);

    qsort(teams, TEAMS, sizeof(*teams), compare); // SORT THE TEAM BY POINTS

    clear_console();
    print_list(teams);
}

/************************************************************
 * Function: print_list()					     
 * Description: Prints the list
 * Input parameters: The struct array with all teams			 
 * Returns: nothing
 *************************************************************/
void print_list(Team teams[])
{

    printf("\n\n\033[1;37m%-7s %-7s %-7s %-15s %-7s\033[0m\n\n", "TEAM", "POINT", "GOALS", "GOALS AGAINST", "GOAL DIFFERENCE");

    for (int i = 0; i < TEAMS; i++)
    {
        printf("%-7s %-7d %-7d %-15d %-7d\n", teams[i].name[0], teams[i].point, teams[i].goals, teams[i].goals_against, teams[i].goal_difference);
    }

    printf("\n\n");
}

/************************************************************
 * Function: get_matches_from_file()					     
 * Description: Gets the match history from the data-file.
 * Input parameters: The struct arrays with all teams and matches	 
 * Returns: noting
 *************************************************************/
void get_matches_from_file(Match matches[], Team teams[])
{

    FILE *fp = fopen("database/kampe-2020-2021.txt", "r");

    if (fp)
    {

        Match m;
        int i = 0, team = 0;

        while (fscanf(fp, "%s     %s %s     %s - %s     %d - %d     %d", m.weekday, m.date, m.time, m.home_team.name, m.away_team.name, &m.home_team.goals, &m.away_team.goals, &m.viewers) == 8)
        {

            team += add_match_to_array(matches, m, i, team, teams);
            i++;
        }

        fclose(fp);
    }
    else
    {
        printf("CANNOT OPEN FILE");
    }
}

/************************************************************
 * Function: add_match_to_array()					     
 * Description: Adds the match to the array over matches.
 * Description: Adds the team to the array of teams if not exits.
 * Input parameters: Matches and Teams array, Specific match and team
 * Returns: a value of either 0 or 1, which indicates whether a new team has been created
 *************************************************************/
int add_match_to_array(Match matches[], Match m, int i, int team, Team teams[])
{

    matches[i] = m;

    int exits = exits_in_array(teams, matches[i].home_team.name, team);

    if (exits == -1) // CHECKS IF TEAM ALREADY EXITS. IF NOT CREATE TEAM.
    {
        teams[team].name[0] = matches[i].home_team.name;
        teams[team].goals = matches[i].home_team.goals;
        teams[team].goals_against = matches[i].away_team.goals;
        teams[team].point = calculate_points(matches[i]);
        teams[team].goal_difference = matches[i].home_team.goals - matches[i].away_team.goals;

        return 1;
    }

    // IF TEAM ALREADY EXITS - ADD THE NEW DATA FROM MATCH.
    teams[exits].goals += m.home_team.goals;
    teams[exits].goals_against += m.away_team.goals;
    teams[exits].point += calculate_points(matches[i]);
    teams[exits].goal_difference += matches[i].home_team.goals - matches[i].away_team.goals;

    return 0;
}

/************************************************************
 * Function: calculate_points()					     
 * Description: Calculate points and adds them to the team
 * Input parameters: The current match
 * Returns: The number of points the team needs for the match.
 *************************************************************/
int calculate_points(Match m)
{

    if (m.home_team.goals > m.away_team.goals)
    {
        return 3;
    }
    else if (m.home_team.goals == m.away_team.goals)
    {
        return 1;
    }

    return 0;
}

/************************************************************
 * Function: exits_in_array()					     
 * Description: Check if the teams already exits
 * Input parameters: Array with all teams, team name and the number of teams.	 
 * Returns: Index in teams array
 *************************************************************/
int exits_in_array(Team teams[], char *name, int team)
{

    for (int i = 0; i < team; i++)
    {

        if (strcmp(teams[i].name[0], name) == 0) // COMPARE TWO STRINGS
        {
            return i; // THE INDEX IN TEAMS ARRAY
        }
    }

    return -1;
}

/************************************************************
 * Function: compare()					     
 * Description: Compare the data and sort it.
 * Input parameters: arrays		 
 * Returns: The diff between two items in array
 *************************************************************/
int compare(const void *a, const void *b)
{
    const Team *pa = a;
    const Team *pb = b;

    int diff = pb->point - pa->point; // THE DIFF BETWEEN POINTS

    if (diff == 0) // IF NO DIFF BETWEEN POINTS CHECK DIFF BETWEEN GOAL DIFFERENCE
        diff = pb->goal_difference - pa->goal_difference;

    return diff;
}

/************************************************************
 * Function: clear_console()					     
 * Description: Clears the console
 * Input parameters: none			 
 * Returns: none
 *************************************************************/
void clear_console()
{
#ifdef _WIN32
    system("cls");
#else //In any other OS
    system("clear");
#endif
}