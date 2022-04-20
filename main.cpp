/*******************************************************************************
Cesar Sepulveda
CPSC 474-01 Project 1 Lamport's Logical Clock
10/15/21
*******************************************************************************/
#include <iostream>
#include "algorithms.hpp"


int main(){
  int choice = -1;
  cout << "What would you like to do?" << endl;   //Displaying the menu options
  cout << "1 Calculate" << endl;
  cout << "2 Verify" << endl;
  cout << "0 Exit" << endl;
  cin >> choice;
  switch (choice){              //Switch statement to start the menu
    case 1:
      calculate();             //Calling the calculate function
      break;
    case 2:
      verify();               //Calling the verify function
      break;
    case 0:
      break;                  //Case to exitthe program
    default:
      cout << "Invalid input" << endl;      //Notifing user of invalid input
      break;
  }
  return 0;                 //Exiting the program
}
