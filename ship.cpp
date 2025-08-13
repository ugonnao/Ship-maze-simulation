//Project Identifier: 950181F63D0A883F183EC0A5CC67B19928FE896A
#include <algorithm>
#include <cassert>
#include <deque>
#include <functional>
#include <iostream>
#include <math.h>
#include <numeric>
#include <stack>
#include <string>
#include <utility>
#include <vector>
#include "getopt.h"
using namespace std;

class Ship{
    private:
    struct Discover{
        char type = '.';
        bool discovered = false; 
        char prev = 'k';
    };//discover

    struct Tile{
        size_t level; 
        size_t row; 
        size_t col; 
    };
    //3D vector of something
    vector<vector<vector<Discover>>> map; 
    Tile starting_tile; 
    Tile hanger_tile; 
    
    //make local variable in search function deque<size_t> SC; 
    bool stack_mode = 0; 
    char input_mode; 
    uint32_t level, size; 
    string output_format = "M";

    void Error_Checking(char &symbol, size_t &floor, size_t &row, size_t &col){
        if(symbol != '#' && symbol != 'H' && symbol != 'S' && symbol != '.' && symbol != 'E'){
            cerr << "Invalid map character\n";
             exit(1); 
        }if(static_cast<uint32_t>(floor) >= level){
            cerr << "Invalid map level\n";
            exit(1);
        }if(static_cast<uint32_t>(row) >= size || static_cast<uint32_t>(col) >= size){
            cerr << "Invalid map row or column\n"; 
            exit(1);
        }
    }//Error Checking

//Done
    void map_mode(){
        string line; 
        size_t row = 0; 
        size_t lvl = 0; 
        u_int32_t col = 0;
        while(getline(cin, line)){
            if(line[0] != '/' && !line.empty()){
                for(size_t i = 0; i < size; i++){
                    Error_Checking(line[i], lvl, row, i);
                    map[lvl][row][i].type = line[i];
                    if(map[lvl][row][i].type == 'S'){
                        starting_tile.level = lvl;
                        starting_tile.row = row; 
                        starting_tile.col = i; 
                    }
                    if(map[lvl][row][i].type == 'H'){
                        hanger_tile.level = lvl;
                        hanger_tile.row = row; 
                        hanger_tile.col = i;
                    }
                }
                row++;
                col++; 
                if(col == size){
                    lvl++;
                    row = 0; 
                    col = 0; 
                }
            }
          else{continue;}
        }//while
    }//map_mode
//Done
    void coord_list_mode(){
        size_t coord_level, row, col;
        char character = 'a';
        char junk = 'b';
        string line; 
        while(cin >> character){
            if(character != '/'){
                cin >> coord_level >> junk >> row >> junk >> col >> junk >> character >> junk;
                Error_Checking(character, coord_level, row, col);
                map[coord_level][row][col].type = character; 
                //Error checking
                if(map[coord_level][row][col].type == 'S'){
                    starting_tile.level = coord_level; 
                    starting_tile.row = row; 
                    starting_tile.col = col; 
                }
                if(map[coord_level][row][col].type == 'H'){
                    hanger_tile.level = coord_level; 
                    hanger_tile.row = row; 
                    hanger_tile.col = col; 
                }
            }//if
            else{getline(cin,line);}
        }//while
    }//coord_list_mode
    //Done
    void validTileNorth(deque<Tile> &container, Tile &curr){
        Tile nextTile;
        nextTile.level = curr.level;
        nextTile.row = curr.row-1; 
        nextTile.col = curr.col; 
        map[nextTile.level][nextTile.row][nextTile.col].discovered = true;
        map[nextTile.level][nextTile.row][nextTile.col].prev = 's';
        container.push_back(nextTile);
    }//valid Tile North
    //Done
    void validTileEast(deque<Tile> &container, Tile &curr){
        Tile nextTile;
        nextTile.level = curr.level;
        nextTile.row = curr.row; 
        nextTile.col = curr.col+1; 
        map[nextTile.level][nextTile.row][nextTile.col].discovered = true;
        map[nextTile.level][nextTile.row][nextTile.col].prev = 'w';
        container.push_back(nextTile);
    }//validTile East
    //Done
    void validTileSouth(deque<Tile> &container, Tile &curr){
        Tile nextTile;
        nextTile.level = curr.level;
        nextTile.row = curr.row+1; 
        nextTile.col = curr.col; 
        map[nextTile.level][nextTile.row][nextTile.col].discovered = true;
        map[nextTile.level][nextTile.row][nextTile.col].prev = 'n';
        container.push_back(nextTile); 
    }//validTileSOuth
    //Done
    void validTileWest(deque<Tile> &container, Tile &curr){
        Tile nextTile;
        nextTile.level = curr.level;
        nextTile.row = curr.row; 
        nextTile.col = curr.col-1; 
        map[nextTile.level][nextTile.row][nextTile.col].discovered = true;
        map[nextTile.level][nextTile.row][nextTile.col].prev = 'e';
        container.push_back(nextTile);  
    }//validTileWest

    void directions(Tile &current, vector<char> &backtrace){
        if(map[current.level][current.row][current.col].prev == 's'){
            backtrace.push_back('n');
            current.row = current.row + 1; 
        }
        else if(map[current.level][current.row][current.col].prev == 'n'){
            backtrace.push_back('s');
            current.row = current.row -1; 
        }
        else if(map[current.level][current.row][current.col].prev == 'e'){
            backtrace.push_back('w');
            current.col = current.col+1; 
        }
        else if(map[current.level][current.row][current.col].prev == 'w'){
            backtrace.push_back('e');
            current.col = current.col-1; 
        }
        else{
            backtrace.push_back(static_cast<char>(current.level + '0'));
            current.level = static_cast<size_t>(map[current.level][current.row][current.col].prev - '0');}
    }

    //3D vector 
    //Search Container, most likely different data types
    //when reading in, internal data structure is going to be the same
    //Discovered, when something is put into the search container(deque)
    //Investigate, when something is taken out the SC (deque)

    public:
    // Command Line

    void get_options(int argc, char** argv){
        int option_index = 0; 
        int option = 0; 
        opterr = false; 

        struct option longOpts[] = {
            {"help", no_argument, nullptr, 'h'},
            {"stack", no_argument, nullptr, 's'},
            {"queue", no_argument, nullptr, 'q'},
            {"output", required_argument, nullptr, 'o'},
            {nullptr, 0, nullptr, '\0'}
        };//struct option
        bool seen_mode = false; 
         while((option = getopt_long(argc, argv, "hsqo:", longOpts, &option_index)) != -1){
            switch(option){
                case 'h':
                    cout << "helpful message\n";
                    exit(0);
                case 's':
                    stack_mode = true; 
                    if(seen_mode){
                        cerr << "Conflicting or duplicate stack and queue specified\n";
                        exit(1); 
                    }
                    seen_mode = true; 
                    break; 
                case 'q':
                    stack_mode = false;
                    if(seen_mode){
                        cerr << "Conflicting or duplicate stack and queue specified\n";
                        exit(1); 
                    }  
                    seen_mode = true; 
                    break; 
                case 'o':
                    output_format = *optarg;
                    break;
            }//switch
        }//while
        if(!seen_mode){
            cerr << "Must specify one of stack or queue\n";
            exit(1);
        }
    }//get_options

    void read_input(){
        cin >> input_mode >> level >> size;
        map.resize(level, vector<vector<Discover>>(size, vector<Discover>(size)));
            if(input_mode ==  'L'){
                coord_list_mode();
            }
            else if(input_mode == 'M'){
                map_mode(); 
            }
    }//read_input
    bool search_algorithm(){
        deque<Tile> SC;
        Tile currTile = starting_tile;
        SC.push_back(currTile);
        map[currTile.level][currTile.row][currTile.col].discovered = true; 
        while(!SC.empty()){
            if(stack_mode){
                currTile = SC.back();
                SC.pop_back();
            }
            else{
                currTile = SC.front();
                SC.pop_front();
            }
            //North
            if(!(currTile.row == 0 ||
             map[currTile.level][currTile.row-1][currTile.col].type == '#' ||
             map[currTile.level][currTile.row-1][currTile.col].discovered)){
                validTileNorth(SC, currTile);
                if(map[SC.back().level][SC.back().row][SC.back().col].type == 'H'){
                    return true;
                } 
            }
            //East
            if(!(currTile.col == size-1 || 
            map[currTile.level][currTile.row][currTile.col+1].type == '#' || 
            map[currTile.level][currTile.row][currTile.col+1].discovered)){
                validTileEast(SC, currTile);
                if(map[SC.back().level][SC.back().row][SC.back().col].type == 'H'){
                    return true;
                } 
            }//South
            if(!(currTile.row == size-1 ||
            map[currTile.level][currTile.row +1][currTile.col].type == '#' ||
            map[currTile.level][currTile.row +1][currTile.col].discovered)){
                validTileSouth(SC, currTile);
                if(map[SC.back().level][SC.back().row][SC.back().col].type == 'H'){
                    return true;
                } 
            }//West
            if(!(currTile.col == 0 ||
             map[currTile.level][currTile.row][currTile.col-1].type == '#' ||
             map[currTile.level][currTile.row][currTile.col-1].discovered)){
                validTileWest(SC, currTile);
                if(map[SC.back().level][SC.back().row][SC.back().col].type == 'H'){
                    return true;
                } 
            }
            if(map[currTile.level][currTile.row][currTile.col].type == 'E'){
                for(size_t i = 0; i < level; i++){
                    if(map[i][currTile.row][currTile.col].type == 'E' && !map[i][currTile.row][currTile.col].discovered){
                        Tile elevatorTile;
                        elevatorTile.level = i; 
                        elevatorTile.row = currTile.row; 
                        elevatorTile.col = currTile.col; 
                        map[i][currTile.row][currTile.col].discovered = true;
                        SC.push_back(elevatorTile);
                        map[elevatorTile.level][elevatorTile.row][elevatorTile.col].prev =  static_cast<char>(currTile.level + '0');
                    }
                }//for
            }//elevator if
        }//while
        return false; 
    }//search_algorithm

    void print_output(){
        Tile currBack = hanger_tile; 
        vector<char> backtrace; 
        while(map[currBack.level][currBack.row][currBack.col].type != 'S'){
            directions(currBack, backtrace); //backtracing
        }//while
        if(output_format == "M"){
            cout << "Start in level " << starting_tile.level << ", row " << starting_tile.row << ", column " << starting_tile.col << "\n";
            Tile curr = starting_tile; 
            size_t j = backtrace.size();

            for(size_t i = 0; i < j; i++){
                map[curr.level][curr.row][curr.col].type = backtrace.back();
                if(backtrace.back() == 's'){
                    curr.row = curr.row + 1; 
                }
                else if(backtrace.back() == 'n'){
                    curr.row = curr.row -1; 
                }
                else if(backtrace.back() == 'e'){
                    curr.col = curr.col+1; 
                }
                else if(backtrace.back() == 'w'){
                    curr.col = curr.col-1; 
                }
                else{curr.level = static_cast<size_t>(backtrace.back() - '0');}
                backtrace.pop_back();
            }//for

            for(size_t i = 0; i < level; i++){
                cout << "//level " << i<< "\n";
                for(size_t j = 0; j < size; j++){
                    for(size_t k = 0; k < size; k++){
                        cout << map[i][j][k].type;
                    }//for
                    cout << "\n";  
                }//for    
            }//for
        }//if M
        else{
            cout << "//path taken\n";
            Tile curr = starting_tile; 
            size_t j = backtrace.size();
            for (size_t i = 0; i < j; i++){
                cout << "(" << curr.level << "," << curr.row << "," << curr.col << "," << backtrace.back() << ")\n";
                if(backtrace.back() == 's'){
                    curr.row = curr.row + 1; 
                }
                else if(backtrace.back() == 'n'){
                    curr.row = curr.row -1; 
                }
                else if(backtrace.back() == 'e'){
                    curr.col = curr.col+1; 
                }
                else if(backtrace.back() == 'w'){
                    curr.col = curr.col-1; 
                }
                else{curr.level = static_cast<size_t>(backtrace.back() - '0');}
                backtrace.pop_back();
            }//for
        }//else L
    }//print_output

    void noSolution(){
        if(output_format == "M"){
            cout << "Start in level " << starting_tile.level << ", row " << starting_tile.row << ", column " << starting_tile.col << "\n";
            for(size_t i = 0; i < level; i++){
                cout << "//level " << i<< "\n";
                for (size_t j = 0; j < size; j++){
                    for (size_t k = 0; k < size; k++){
                        cout << map[i][j][k].type; 
                    }
                    cout << "\n";
                }
            }
        }// if M
        else{
            cout << "//path taken\n";
        }
    }//no solution
};//class Ship

/*
1. Data Structures
2. Read Input (pick mode)
3.Searching: Solution or Not?
4./Output if no solution (Pick mode)
5.Update searching: BackTracing
6.Output if solution
7. correct
8. Other input/output
*/

int main(int argc, char** argv){
    std::ios::sync_with_stdio(false);
    Ship SpaceStation; 
    SpaceStation.get_options(argc, argv);
    SpaceStation.read_input(); 
    if(SpaceStation.search_algorithm()){
        SpaceStation.print_output(); 
    }
    else{
        SpaceStation.noSolution();
    }
    return 0; 
}//main