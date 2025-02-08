#include <iostream>
#include <vector>
#include <set>
#include <queue>

using namespace std;



//node definition
struct node{
    vector<vector<int>> state;
    node* parentState;
    int cost;
    int heauristic;
    //since uniform cost all moves cost the same. This will be initialized to 0 for all
    //constructor 
    node(const vector<vector<int>>& s, node* p, int c, int h) : state(s), parentState(p), cost(c), heauristic(h) {};
};


//comparison function to use in priority queue
struct compareCost{
    bool operator()(const node* a, const node* b)const{
        return a->cost > b->cost;
    }
};

//Goal state vector
vector<vector<int>> Goal_State = {{1,2,3},
                                  {4,5,6},
                                  {7,8,0}};


vector<vector<int>> Default_Start = {{1,2, 3},
                                    {5, 0, 6},
                                    {4, 7, 8}};

vector<pair<int, int>> MOVES {{0, -1}, {0, 1}, {-1,0}, {1, 0}};
//Moves list 
/*
    represented as (row, column) modifiers
    these will be added to current 0s position and checked for bounds
    {0,-1}  moves tile left
    {0, 1}  moves tile right
    {-1, 0} Moves tile up
    {1, 0}  Moves tile down

*/


//Start variables
vector<vector<int>> inputStart(3, vector<int>(3));
int input = 0;
int searchInput = 0;

//Print puzzle 
void printState(vector<vector<int>> currState){
    for(int i = 0; i < 3; i++){
        for (int j = 0; j < 3; j++){
            cout << currState.at(i).at(j) << " ";
        }
        cout << endl;
    }
    cout << endl;
}

//print Solution
//pull each state into a vector, print vector backwards
void printTree(node* solNode){
    vector<vector<vector<int>>> solutionPath;
    while(solNode){
        solutionPath.push_back(solNode->state);
        solNode = solNode->parentState;
    }
    for(int i = solutionPath.size() - 1; i >= 0; i--){
        printState(solutionPath.at(i));
    }
}

//finds zero by itering through each element
pair<int, int> findZero(const vector<vector<int>>& currState){
    for (int row = 0; row < 3; row++){
        for (int col = 0; col < 3; col++){
            if(currState[row][col] == 0){
                return {row, col};
            }
        }
    }
    return {-1,-1}; //terminal case something wrong happened
}

//Swap Tiles. Takes in coordinates of tiles to be swapped and returns as a new state
vector<vector<int>> switchTiles (const vector<vector<int>>& currState, int rowT1, int colT1, int rowT2, int colT2){
    vector<vector<int>> newState = currState;
    swap(newState[rowT1][colT1], newState[rowT2][colT2]);
    //newState[rowT1][colT1] = newState[rowT2][colT2];
    
    return newState;
}

//adds move list operators to zero then checks for boundaries. If pass it is added to a vector of possible states
vector<vector<vector<int>>> possibleMoves(const vector<vector<int>>& currState){
    vector<vector<vector<int>>> possibleStates;
    auto [row, col] = findZero(currState);
    for (int i = 0; i < MOVES.size(); i++){
        int newRow = row + MOVES.at(i).first;
        int newCol = col + MOVES.at(i).second;
        if(newRow >= 0 && newRow <3 && newCol >=0 && newCol< 3){
            vector<vector<int>> tempState = switchTiles(currState, row, col, newRow, newCol);
            possibleStates.push_back(tempState);
            
        }
    }
    return possibleStates;
}


//Checks each element in their position
bool checkGoal(vector<vector<int>>& currState){
    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
            if(currState.at(i).at(j) != Goal_State.at(i).at(j)){
                return false;
            }
        }
    }
    return true;
}
//mispalced tiles check for how many tiles are not in their correct state
int calculateMisplacedTiles(const vector<vector<int>>& currState){
    int counter = 0;
    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
            if(currState.at(i).at(j) != Goal_State.at(i).at(j)){
                counter++;
            }
        }
    }
    return counter;
}





void uniformCostSearch(const vector<vector<int>>& startState){
    priority_queue<node*, vector<node*>, compareCost>  pq;    //create queue of nodes, stored in a vector, based on cost comparator
    node* start_node = new node(startState, nullptr, 0, 0); //cost 0 moves to get to start state
    set<vector<vector<int>>> visitedStates;
    pq.push(start_node);
    int count = 0;

    while(!pq.empty()){ //check for empty or failure
        node* currNode = pq.top();
        pq.pop();   // remove front 
        vector<vector<int>> currState = currNode->state;
        //Check win condition
        if(checkGoal(currState)){
            cout << "Puzzle Solved! " << endl;
            printTree(currNode);
            cout << "Nodes Expanded: " << count << endl;
            return;
        }
        //check if its a visited state. If so break iteration of while lopp and move to nect node
        if(visitedStates.find(currState) != visitedStates.end()){
            delete currNode;
            continue;
        }
        //if not visited add to visited set
        visitedStates.insert(currState);
        //since unvsited and not goal state, generate all next states
        vector<vector<vector<int>>> nextMoves = possibleMoves(currState);
        for(int i = 0; i < nextMoves.size();i++){
            node* newNode = new node(nextMoves.at(i), currNode, currNode->cost + 1, 0);
            pq.push(newNode);
        }
        count++;
    }
    //loop breaks when queue is empty meaning no solution
    cout << "Puzzle Unsolvable" << endl;
}


int main(){

    cout << "Welcome to the Puzzle solver" << endl;
    cout << "Select 1 for a default puzzle, or 2 to input one" << endl;

    cin >> input;

    if (input == 1){
        cout << "Here is the puzzle" << endl;
        printState(Default_Start);
        inputStart = Default_Start;

    }
    else if(input == 2){
        cout << "Input your puzzle one number at a time" << endl;

        for(int i = 0; i < 3; i++){
            for (int j = 0; j < 3; j++){
                cin >> input;
                inputStart.at(i).at(j) = input;
            }
    }
    cout << endl << "This is your puzzle" << endl;
    printState(inputStart);
    }
    else {
        cout << "invalid input. Program terminated" << endl;
        return 0;
    }

    cout << endl << "Select your method of search:" << endl <<
                    " 1 for Uniform cost search" << endl <<
                    "2 for A* with Misplaced tile" << endl <<
                    "3 for A* with Manhattan Distance" << endl;

    cin >> searchInput;

    if (searchInput == 1){
        uniformCostSearch(inputStart);
    }



    return 0;
}