#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <array>
#include <algorithm>
#include<utility>

#include <unistd.h>

// Real board looks to be ~1000x1000 sized
#define BOARD_SIZE 1000

//test board looks to be 10x10 sized
#define BOARD_SIZE_TEST 10

#define BOARD_USED BOARD_SIZE_TEST

// example board, a 4x4 board
// 0  1  2  3
// 4  5  6  7
// 8  9  10 11
// 12 13 14 15

struct Board{
  int boardsize = BOARD_SIZE_TEST;
  std::vector<std::pair<int, int>> overlap;
  std::array<int, BOARD_USED * BOARD_USED> vent_board {};

  //functions:
  int update_position(int x, int y);
  void display_board();
  int get_overlap();
};

//return values:
// -1  -- error, see cerr
//  0  -- no problems
int Board::update_position(int x, int y){
  //checking inputs:
  if((x < 0) || (x > (BOARD_USED - 1))){
    std::cerr << "err: x value is out of bounds: "<< x << ", should be between 0 and " << BOARD_USED - 1 << std::endl;
    return -1;
  }
  
  if((y < 0) || (y > (BOARD_USED - 1))){
    std::cerr << "err: y value is out of bounds: "<< y << ", should be between 0 and " << BOARD_USED - 1 << std::endl;
    return -1;
  }
  // easiest way to determine the current board position is vent_board[y*BOARD_SIZE + x]?
  int pos = y * BOARD_USED + x;
  vent_board[pos] ++;
  // checking to see if the board position is an overlap, then check to see if 
  if(vent_board[pos] > 1){
    if(std::find(overlap.begin(), overlap.end(), std::pair<int,int>(x,y)) == overlap.end()){
      // it's not found, add it to the vector
      overlap.emplace_back(std::pair<int,int>(x,y));
    }
  }
  return 0;
}

// probably should be piped into a text file instead ofinto cout, that is on the user to take care of
void Board::display_board(){
  for(int y = 0; y < BOARD_USED; y++){
    for(int x = 0; x < BOARD_USED; x++){
      if(vent_board[(y*BOARD_USED) + x] == 0){
	std::cout << ".";
      }
      else{
	std::cout << vent_board[(y * BOARD_USED) + x];
      }
    }
    std::cout << std::endl;
  }
}

int Board::get_overlap(){
  return overlap.size();
}

// -------------------------------------------------//

void help_screen(){
  std::cerr << "board generation for AOC 2021 -- day 5:" << std::endl;
  std::cerr << "---------------------------------------" << std::endl;
  std::cerr << "  required inputs:" << std::endl;
  std::cerr << "    -f <filename>: input file" << std::endl;
  std::cerr << "    -p <1/2> : indicator to run part 1 or 2" << std::endl;
  std::cerr << "               default value is 1" << std::endl;
  std::cerr << "  optional inputs:" << std::endl;
  std::cerr << "    -v: verbose mode" << std::endl;
  std::cerr << "    -h: this help screen" << std::endl;
}

//copy-paste from day4:
int readfile(const char* argv, std::string *fileMemory, bool verbose) {
  std::ifstream file(argv, std::ios::in|std::ios::binary|std::ios::ate);
  if(file.is_open()){

    // taken from cplusplus.com, as I have not messed with binary files before:
    std::streampos size = file.tellg();
    fileMemory->resize(size, '0');
    file.seekg(0, std::ios::beg);
    file.read(fileMemory->data(), size);
    file.close();
    if(verbose){
      std::cout << "read file" << std::endl;
    }
  } 
  else{
    std::cerr << "err: file cannot be opened: " << argv[1] << std::endl;
    return -1;
  }
  return 0;
}


//has possible values of:
// [<-1,-1>]  -- error
// [<>,<>,]   -- parsed value
std::vector<std::pair<int,int>> data_to_pair(std::string input_str, int part_indicator, bool verbose){
  // setting a default variable so high that it will never be hit, as a check to make
  // sure that all values are used.
  int warning_value = 10000;
  std::array<int,4> endpoints {warning_value};
  //int dummy = 0;
  int endpoints_found = 0;
  for(char character : input_str){
    if((character - 48 < 10) && (character - 48 > -1)){
      endpoints[endpoints_found] =  ((int)character - 48);
      endpoints_found++;
    }
  }
  if(verbose){
    std::cout << "  ";
    for(int val : endpoints){
    std::cout << val;
    }
    std::cout << std::endl;
  }

  std::vector<std::pair<int,int>> retval;
  
  for(int value : endpoints){
    // consolidating both checks for value outside of board range, and value unchanged
    // from default into one test, as default warning_value is > max board size.
    if(value >= BOARD_USED){
      std::cerr << "err: value is not right. " << value << " is larger than max board size " << BOARD_USED << std::endl;
      retval.emplace_back(std::pair<int,int>(-1,-1));
      return retval;
    }
  }
  // here is the magic -- this checks turns the endpoints into a vector of pairs
  // and for part 1, ignores all none straight vents  
  if(endpoints[0] == endpoints[2]){ // horizontal lines
    int stable = endpoints[0];
    int move_start = endpoints[1];
    int move_end = endpoints[3];
    for(int itr = move_start; itr <= move_end; itr++){
      std::pair<int,int> temp(stable, itr);
      retval.emplace_back(temp);
    }
  }
  else if(endpoints[1] == endpoints[3]){ // vertical lines
    int stable = endpoints[1];
    int move_start = endpoints[0];
    int move_end = endpoints[2];
    for(int itr = move_start; itr <= move_end; itr++){
      std::pair<int,int> temp(itr, stable);
      retval.emplace_back(temp);
    }
  }
  else{
    if(verbose){ std::cout << "  passing to part 2" << std::endl;}
    
    return retval;
    if(part_indicator == 2){ //

    }
  }

  return retval;
}


int main(int argc, char* argv[]){
  std::string fileName = "TEMP";
  int p = 1;
  bool verbose = false;
  bool help = false;
  int c;
  opterr = 0;
  while((c = getopt(argc, argv, "f:p:vh")) != -1){
    switch(c){
    case 'f':
      fileName = optarg;
      break;
    case 'p':
      p = atoi(optarg);
      break;
    case 'v':
      verbose = true;
      break;
    case 'h':
      help = true;
      break;
    case '?':
      std::cout << "err: malformed user input: " << optopt << std::endl;
      break;
      return 0;
    default:
      std::cout << "err: user input error" << std::endl;
      abort();
    }
  }
  if(help){
    help_screen();
    return 0;
  }
  std::cout << fileName << " " << verbose <<" " << p<<std::endl;

  //handling value checking of required variables
  if(fileName == "TEMP"){
    std::cerr << "err: fileName not set."<< std::endl;
    return -1;
  }

  if((p != 1) && (p != 2)){
    std::cerr << "err: p-value not intialized correctly. " << p << " is not a valid value." << std::endl;
    return -1;
  }

  // parsing the input file into fileMemory, and reading from that from now on
  std::string fileMemory;
  if(readfile(fileName.c_str(), &fileMemory, verbose) == -1){
    return -1;
  }

  Board board;

  std::istringstream iss(fileMemory);
  std::string vent_line = "";
  std::string temp = "";
  //int pos;
  std::vector<std::pair<int,int>> input_array;

  while(std::getline(iss, vent_line)){
    //pos = 0;
    vent_line.push_back('F');    
    for(auto i : vent_line){
      if (i == 'F'){
	if(verbose){
	  std::cerr << temp << std::endl;
	}
	std::vector<std::pair<int,int>> vent_line = data_to_pair(temp,p,verbose);
	if(vent_line.size() != 0){
	  if(vent_line[0].first == -1){
	    return -1;
	  }
	  if(verbose){std::cout << "  size of line: "<< vent_line.size() << std::endl;}
	  for(std::pair<int,int> value : vent_line){
	    input_array.insert(input_array.end(), value);
	  }
	}
	temp = "";
	continue;
      }
      temp.push_back(i);
    }
    
  }
  for(auto vent_tile : input_array){
    board.update_position(vent_tile.first, vent_tile.second);
  }
  if(verbose){
    board.display_board();
  }
}
