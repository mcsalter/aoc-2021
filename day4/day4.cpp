#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>

#define boardSize 25

struct Board{
  int boardName;
  std::pair<int,bool> board[boardSize];
  int val_of_unselected;
  int last_number;
  bool victory;
  // functions:
  Board(int input[boardSize], int name);
  int update_board(int val);
  int get_output();
};


Board::Board(int input[boardSize], int name){
  val_of_unselected = 0;
  for(int itr = 0; itr < boardSize; itr++){
      board[itr] = std::pair<int,bool>(input[itr], false);
      val_of_unselected += input[itr];
    }
  victory = false;
  boardName = name;
}

int Board::update_board(int val){
  // checks to see if the token is on the board, then will update the values as needed
  last_number = val;
  bool found = false;
  for(std::pair<int, bool> &board_piece : this->board){
    if((board_piece.first == val) && (board_piece.second != true) && (victory == false)){
      board_piece.second = true;
      val_of_unselected -= board_piece.first;
      found = true;
      break;
    }
  }

  // checks to see if there is a victory condition met:

  for(int row = 0; row < boardSize; row += 5){
    if((board[row].second == true) && (board[row + 1].second == true) && (board[row + 2].second == true) && (board[row + 3].second == true) && (board[row + 4].second == true)){
      victory = true;
    }
  }

  for(int col = 0; col < boardSize; col ++){
    if((board[col].second == true) && (board[col + 5].second == true) && (board[col + 10].second == true) && (board[col + 15].second == true) && (board[col + 20].second == true)){
      victory = true;
    }
  }

  if(found){
    if(victory){
      return 1;
    }else{
      return 0;
    }
  }
  else{
    return -1;
  }
}

int Board::get_output(){
  return val_of_unselected * last_number;
}


int readfile(char* argv, std::string *fileMemory) {
  std::ifstream file(argv, std::ios::in|std::ios::binary|std::ios::ate);
  if(file.is_open()){

    // taken from cplusplus.com, as I have not messed with binary files before:
    std::streampos size = file.tellg();
    fileMemory->resize(size, '0');
    file.seekg(0, std::ios::beg);
    file.read(fileMemory->data(), size);
    file.close();
    // if(verbose){
    // 	std::cout << "read file" << std::endl;
    //      }
  } 
  else{
    std::cerr << "err: file cannot be opened: " << argv[1] << std::endl;
    return -1;
    }
  return 0;
}

int main(int argc, char *argv[]){
  bool verbose;
  if (argc < 2){
    std::cerr << "err: no file detected" << std::endl;
    return -1;
  }
  if(argc >= 3){
    std::string arg_three = argv[2];
    if(arg_three == "-v"){
      verbose = true;
    }
    
  }

  std::string fileMemory;

  int readval = readfile(argv[1], &fileMemory);
  if(readval == -1){
    return -1;
  }

  std::vector<int> bingoNum;
  std::istringstream iss(fileMemory);
  std::string bingoStr = "";
  std::string temp = "";
  
  std::getline(iss, bingoStr);
  bingoStr.push_back('F');
  for(char i : bingoStr){
    if((i == ',') || (i == 'F')){
      bingoNum.push_back(stoi(temp));
      temp = "";
      continue;
    }else{
      temp.push_back(i);
    }
  }

  if(verbose){
    std::cout << "bingo string parsed: ";
    for (auto i : bingoNum){
      std::cout << i << " ";
    }
    std::cout << std::endl;
  }
  temp = "";

  int itr = 1;
  int pos = 0;
  std::vector<Board*> BoardVec;
  while(std::getline(iss, bingoStr)){
    pos = 0;
    bingoStr.push_back('F');
    int inputarr[boardSize]= {};
    for(auto i : bingoStr){
      if ((i == ' ') || (i == 'F')){
	inputarr[pos] = stoi(temp);
	pos ++;
	temp = "";
	continue;
      }
      temp.push_back(i);
    }
    Board *board = new Board(inputarr, itr);
    BoardVec.push_back(board);
    if(verbose){
      std::cout << "board number: "<< BoardVec.back()->boardName << " board Values: ";
      for(auto val : BoardVec.back()->board){
	std::cout << val.first << " ";
      }std::cout << std::endl;
    }
    itr ++;
    continue;
  }

  for(int bingoVal : bingoNum){
    for(Board *board : BoardVec){
      int value = board->update_board(bingoVal);
      if(verbose){
	std::cout << "checking board: "<< board->boardName << " with value: "<< bingoVal << " result is: " << board->val_of_unselected << std::endl;
	for(auto state : board->board){ 
	  std::cout << "(" << state.first << " " << state.second << ") ";
	}std::cout << std::endl;
      }
      
      if(value == 1){
	int retval = board->get_output();
	std::cout << "board number: " << board->boardName << " wins with end value: " << retval << std::endl;
	
      }
    }
  }
  
  return 0;
}
