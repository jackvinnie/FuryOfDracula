// HunterView.c ... HunterView ADT implementation

#include <stdlib.h>
#include <assert.h>
#include "Globals.h"
#include "Game.h"
#include "GameView.h"
#include "HunterView.h"
#include "Map.h" 
     
struct hunterView {
    //REPLACE THIS WITH YOUR OWN IMPLEMENTATION
    GameView g;
}*Hunterview;
     

// Creates a new HunterView to summarise the current state of the game
HunterView newHunterView(char *pastPlays, PlayerMessage messages[])
{
    //REPLACE THIS WITH YOUR OWN IMPLEMENTATION
    HunterView hunterView = malloc(sizeof(struct hunterView));
    hunterView->g = malloc(sizeof(GameView));
    hunterView->g = newGameView(pastPlays,messages);
    return hunterView;
}
     
     
// Frees all memory previously allocated for the HunterView toBeDeleted
void disposeHunterView(HunterView toBeDeleted)
{
    //COMPLETE THIS IMPLEMENTATION
    disposeGameView(toBeDeleted->g);
    free( toBeDeleted );
}


//// Functions to return simple information about the current state of the game

// Get the current round
Round giveMeTheRound(HunterView currentView)
{
    //REPLACE THIS WITH YOUR OWN IMPLEMENTATION
    Round n = getRound(currentView->g);
    return n;
}

// Get the id of current player
PlayerID whoAmI(HunterView currentView)
{
    //REPLACE THIS WITH YOUR OWN IMPLEMENTATION
    PlayerID id = getCurrentPlayer(currentView -> g);
    return id;
}

// Get the current score
int giveMeTheScore(HunterView currentView)
{
    //REPLACE THIS WITH YOUR OWN IMPLEMENTATION
    int scores = getScore(currentView->g);
    return scores;
}

// Get the current health points for a given player
int howHealthyIs(HunterView currentView, PlayerID player)
{
    //REPLACE THIS WITH YOUR OWN IMPLEMENTATION
    int health_points = getHealth(currentView->g,player);
    return health_points;
}

// Get the current location id of a given player
LocationID whereIs(HunterView currentView, PlayerID player)
{
    //REPLACE THIS WITH YOUR OWN IMPLEMENTATION
    LocationID i = getLocation(currentView->g,player);
    return i;
}

//// Functions that return information about the history of the game

// Fills the trail array with the location ids of the last 6 turns
void giveMeTheTrail(HunterView currentView, PlayerID player,
                            LocationID trail[TRAIL_SIZE])
{
    //REPLACE THIS WITH YOUR OWN IMPLEMENTATION
    getHistory(currentView->g,player,trail);
    //maybe put in checks that if the hunter is asking for 
    //dracula's trail then it only gives the basic info
    //not revealing all of his locations
}

//// Functions that query the map to find information about connectivity

// What are my possible next moves (locations)
LocationID *whereCanIgo(HunterView currentView, int *numLocations,
                        int road, int rail, int sea)
{
    //REPLACE THIS WITH YOUR OWN IMPLEMENTATION
    PlayerID id = whoAmI(currentView);
    LocationID *w=whereCanTheyGo(currentView,numLocations,id,road,FALSE,sea);
    //the way this function works is that road, rail, sea ints 
    //are a combination of TRUEs and FALSEs and this function accomadates for all
    //sample code could be
    //if(road==TRUE && rail==TRUE && sea==TRUE){
    //   w=whereCanTheyGo(currentView,numLocations,id,TRUE,TRUE,TRUE);
    //}
    return w;
}

// What are the specified player's next possible moves
LocationID *whereCanTheyGo(HunterView currentView, int *numLocations,
                           PlayerID player, int road, int rail, int sea)
{
    //REPLACE THIS WITH YOUR OWN IMPLEMENTATION
    LocationID from = whereIs(currentView,player);
    Round round = giveMeTheRound(currentView);
    int *w = connectedLocations(currentView->g,numLocations,from,player, round,road,rail,sea);
    return w;
}
