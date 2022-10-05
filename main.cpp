//
//  main.cpp
//  Hangman
//
//  Created by Alessia on 9/27/22.
//
// MAKE GAME REPEAT LOOP
// MAKE WELCOME MENU
// MAKE BY LETTER AND BY WORD OPTIONS

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <random>
#include <ctime>

// GAME WIDTH SETTINGS ================================================================
int GAME_WIDTH = 45;

// Game width cannot be less than the length of the word or available_letters
void CheckGameWidth(std::string word) {
  if (GAME_WIDTH < 25 || GAME_WIDTH < word.length()) {
    if (GAME_WIDTH > word.length())
      GAME_WIDTH = 25;
    else if (GAME_WIDTH > 25)
      GAME_WIDTH = word.length();
  }
}
//=====================================================================================

// WORD GENERATION ====================================================================
std::string RandomWordGenerator(std::string file_path) {
  std::ifstream word_bank(file_path);
  std::vector<std::string> word_list;
  std::string word;
  
  if (word_bank.is_open()) {
    while(std::getline(word_bank, word))
      word_list.push_back(word);
    
    srand(time(0));
    int random_line = rand() % word_list.size();
    word = word_list.at(random_line);
  }
  return word;
}
//=====================================================================================

// GAME SCREEN OUTPUT FUNCTIONS =======================================================
void PrintSeperator() {
  for (int top = 0; top < (GAME_WIDTH); ++top) {
        if (top == 0)
          std::cout << "+";
        if (top == (GAME_WIDTH - 1))
          std::cout << "+\n";
        else
          std::cout << "-";
  }
}

void CenterPrint(std::string msg, bool border=false) {
  if (border == true)
    PrintSeperator();
  
  bool left = true;
  for (int i = msg.length(); i < GAME_WIDTH; i++) {
    if (left)
      msg = " " + msg;
    else
      msg = msg + " ";
    left = !left;
  }
  std::cout << msg << std::endl;
  
  if (border == true)
    PrintSeperator();
}

void DrawHangMan(int tries=0) {
  CenterPrint("|");
    
  if (tries >= 1) //>=
    CenterPrint("|");
  else std::cout << '\n';
  
  if (tries >= 2) // >=
    CenterPrint("O");
  else std::cout << '\n';
  
  if (tries == 3)
    CenterPrint("/  ");
  if (tries == 4)
    CenterPrint("/| ");
  if (tries >= 5)
    CenterPrint("/|\\");
  else std::cout << '\n';
  
  if (tries >= 6)
    CenterPrint("|");
  else std::cout << '\n';
  
  if (tries == 7)
    CenterPrint("/  ");
  if (tries >= 8)
    CenterPrint("/ \\");
  else std::cout << '\n';
    
  CenterPrint("+----------+");
  CenterPrint("|          |");
}

void PrintAvailableLetters(std::string taken="") {
  CenterPrint("Available letters", true);
  
  std::string letters;
  for (char i = 'A'; i <= 'M'; i++) {
    if (taken.find(i) == std::string::npos)
      letters = letters + i + " ";
    else
      letters += "_ ";
  }
  CenterPrint(letters);
  
  letters = "";
  for (char j = 'N'; j <= 'Z'; j++) {
    if (taken.find(j) == std::string::npos)
      letters = letters + j + " ";
    else
      letters += "_ ";
  }
  CenterPrint(letters);
}
//=====================================================================================

// GAME STATE FUNCTIONS ===============================================================
std::string GuessState(std::string word, std::string guessed) {
  std::string guess_state;

  for (int i = 0; i < word.length(); i++) {
    if (guessed.find(word[i]) == std::string::npos)
      guess_state += "_";
    else
      guess_state = guess_state + word[i] + "";
  }
  return guess_state;
}

bool Win(std::string word, std::string guess) {
  if (word == guess)
    return true;
  else
    return false;
}
//=====================================================================================

int main() {
  // Must pass a file_path to RandomWordGenerator()
  std::string word = RandomWordGenerator("");
  transform(word.begin(), word.end(), word.begin(), toupper);
  std::string guess_state = GuessState(word, ""); std::string current_try = ""; std::string letters_used = "";
  
  int tries = 0;
  do {
    // Print Game
    CenterPrint("HANG MAN", true);
    DrawHangMan(tries);
    PrintAvailableLetters(letters_used);
    if (tries == 1)
      CenterPrint("Guess the word!", true);
    else
      CenterPrint("Take another guess", true);
    CenterPrint(guess_state);
    CenterPrint("word length (" + std::to_string(word.length()) + ")");
    
    // Make a guess
    bool invalid_guess;
    do {
      try {
        std::cout << "\n>\t"; std::cin >> current_try;
        
        if (current_try.length() != word.length() && current_try.length() != 1) {
          invalid_guess = true;
          throw(word.length());
        }
        else {
          invalid_guess = false;
          transform(current_try.begin(), current_try.end(), current_try.begin(), toupper);
        }
      } catch(...) {
        std::cout << "\nGuess must be " << word.length() << " characters long. Guess again.";
      }
    } while (invalid_guess == true);
    
    // Update try count, letters used, and guess state
    ++tries;
    letters_used += current_try;
    std::unique(letters_used.begin(), letters_used.end());
    guess_state = GuessState(word, letters_used);
    
    // Check for win
    if(Win(word, guess_state) == true) {
      CenterPrint("YOU WIN!");
      CenterPrint("The word is: " + word);
      // Game won! End program.
      return 0;
    }
  } while (tries < 8);

  // Game lost. End program.
  CenterPrint("YOU LOSE!", true);
  DrawHangMan(tries);
  CenterPrint("The word was: " + word, true);
  std::cout << std::endl;
  return 0;
}
