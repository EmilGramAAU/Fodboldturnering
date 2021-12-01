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

// STRUCTS
typedef struct
{
    char *name[1];
    int goals;
    int goals_against;
    int point;
    int goal_difference;
} Team;

typedef struct
{
    int goals;
    char name[4];
} MatchTeam;

typedef struct
{
    char weekday[5];
    char date[10];
    char time[10];
    MatchTeam home_team;
    MatchTeam away_team;
    int viewers;
} Match;

int main(void)
{

    // INITIALIZE STRUCT ARRAYS
    Match matches[MATCHES];
    Team teams[TEAMS];

    clear_console();
    get_matches_from_file(matches, teams);

    qsort(teams, TEAMS, sizeof(*teams), compare);

    print_list(teams);
}

/************************************************************
 * Function: print_list()					     
 * Description: Prints the list
 * Input parameters: 			 
 * Returns: 
 *************************************************************/
void print_list(Team teams[])
{

    printf("\n\n\033[1;37m%-7s %-7s %-7s %-7s %-7s\033[0m\n\n", "TEAM", "POINT", "WINS", "LOSES", "GOAL DIFFERENCE");

    for (int i = 0; i < TEAMS; i++)
    {
        printf("%-7s %-7d %-7d %-7d %-7d\n", teams[i].name[0], teams[i].point, teams[i].goals, teams[i].goals_against, teams[i].goal_difference);
    }

    printf("\n\n");
}

/************************************************************
 * Function: get_matches_from_file()					     
 * Description: Gets the match history from the data-file.
 * Input parameters: 			 
 * Returns: 
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
}

/************************************************************
 * Function: add_match_to_array()					     
 * Description: Adds the match to the array over matches.
 * Description: Adds the team to the array of teams if not exits.
 * Input parameters: 			 
 * Returns: 
 *************************************************************/
int add_match_to_array(Match matches[], Match m, int i, int team, Team teams[])
{

    matches[i] = m;

    int exits = exits_in_array(teams, matches[i].home_team.name, team);

    if (exits == -1)
    {
        teams[team].name[0] = matches[i].home_team.name;
        teams[team].goals = matches[i].home_team.goals;
        teams[team].goals_against = matches[i].away_team.goals;
        teams[team].point = calculate_points(matches[i]);
        teams[team].goal_difference = matches[i].home_team.goals - matches[i].away_team.goals;

        return 1;
    }

    teams[exits].goals += m.home_team.goals;
    teams[exits].goals_against += m.away_team.goals;
    teams[exits].point += calculate_points(matches[i]);
    teams[exits].goal_difference += matches[i].home_team.goals - matches[i].away_team.goals;

    return 0;
}

/************************************************************
 * Function: calculate_points()					     
 * Description: Calculate points and adds them to the team
 * Input parameters: 			 
 * Returns: 
 *************************************************************/
int calculate_points(Match m)
{

    int point = 0;

    if (m.home_team.goals > m.away_team.goals)
    {
        point += 3;
    }
    else if (m.home_team.goals == m.away_team.goals)
    {
        point += 1;
    }

    return point;
}

/************************************************************
 * Function: exits_in_array()					     
 * Description: Check if the teams already exits
 * Input parameters: 			 
 * Returns: 
 *************************************************************/
int exits_in_array(Team teams[], char *name, int team)
{

    for (int i = 0; i < team; i++)
    {

        if (strcmp(teams[i].name[0], name) == 0)
        {
            return i;
        }
    }

    return -1;
}

/************************************************************
 * Function: compare()					     
 * Description: 
 * Input parameters: none			 
 * Returns: none
 *************************************************************/
int compare(const void *a, const void *b)
{
    const Team *pa = a;
    const Team *pb = b;

    int diff = pb->point - pa->point;

    if (diff == 0)
    {
        if (pb->goal_difference < pa->goal_difference)
            diff = -1;
        else if (pb->goal_difference > pa->goal_difference)
            diff = +1;
        else
            diff = 0;
    }

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