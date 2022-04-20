/*******************************************************************************
Cesar Sepulveda
CPSC 474-01 Project 1 Lamport's Logical Clock
10/15/21
*******************************************************************************/
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
using namespace std;

//Simple Function to find the maximum of 2 integers.
int findMax(int a, int b){
  if(a > b)                 //Comparing intergers a and b.
    return a;               //Returning the maximum.
  else
    return b;
}

//Function to read the input matrix from a file and place it in a 2 dimensional
//vector.
vector<vector<string>> readInput(int n, int m){
  vector<vector<string>> vec;
  int i = 0;                          //Initializing variables.
  int j = 0;
  ifstream file;
  file.open("input.txt");             //Opening the input file.
  string str;

  while(file >> str){                 //While loop to read from the file one input at a time.
    vec.push_back(vector<string>());  //Adding a string vector(row) to the vector of string vectors.
    vec[i].push_back(str);            //Adding the next value of the input matrix to the correct vector.
    j++;                              //Incrementing the column.
    if(j == m){                       //Condition to find the end of the line and switch rows.
      j = 0;                          //Resetting the column variable and incrementing the row variable.
      i++;
    }
    if(i == n){                       //Making sure the vector has the correct amount of rows.
      vec.resize(n);
      break;
    }
  }

  file.close();                     //Closing the input file
  return vec;                       //Returning the vector
}

//Function to change vector of strings to vector of ints.
vector<vector<int>> changeToInt(vector<vector<string>> vec){
  vector<vector<int>> num;
  for (int i = 0; i < vec.size(); i++){         //Looping through the 2 dimensional vector.
    num.push_back(vector<int>());
    for (int j = 0; j < vec[i].size(); j++){
      num[i].push_back(stoi(vec[i][j]));
    }
  }
  return num;
}

//Simply display function to display the int matrix.
void displayInt(vector<vector<int>> vec){
  for (int i = 0; i < vec.size(); i++){         //Looping through the 2 dimensional vector.
    for (int j = 0; j < vec[i].size(); j++){
      cout << vec[i][j] << " ";                 //Displaying the contents of the vector.
    }
    cout << endl;
  }
}

//Simply display function to display the string matrix.
void displayString(vector<vector<string>> vec){
  for (int i = 0; i < vec.size(); i++){         //Looping through the 2 dimensional vector.
    for (int j = 0; j < vec[i].size(); j++){
      cout << vec[i][j] << " ";                 //Displaying the contents of the vector.
    }
    cout << endl;
  }
}

//Algorithm Calculate for Lamport's Logical Clock.
void calculate(){
  int n, m;             //Initializing variables.
  int s = 0;
  int r = 0;
  bool finished = false;
  string senders[9] = {"s1", "s2", "s3", "s4", "s5", "s6", "s7", "s8", "s9"}; //Initializing arrays to keep track of events, sends, and receives.
  string receivers[9] = {"r1", "r2", "r3", "r4", "r5", "r6", "r7", "r8", "r9"};
  vector <vector<int>> matrix;
  cout << "How many rows (N)?" << endl;   //Asking user to enter the amount of rows(n) and columns(m).
  cin >> n;
  cout << "How many columns (M)?" << endl;
  cin >> m;
  int count[n];
  for(int i = 0; i < n; i++){
    count[i] = 0;
  }
  vector<vector<string>> vec = readInput(n,m);    //Calling the readInput function.

  for(int i = 0; i < vec.size(); i++){            //For loops to iterate through the matrix
    matrix.push_back(vector<int> ());            //and fill every entry with 0.
    for(int j = 0; j < vec[i].size(); j++){
      matrix[i].push_back(0);
    }
  }
  while(finished == false){
    for (int i = 0; i < vec.size(); i++){       //For loops to iterate through the matrix.
      for (int j = 0; j < vec[i].size(); j++){
        if(vec[i][j].length() == 1 && count[i] == j){   //If statements to find the LC values of events.
          if(j == 0){                 //If statement in case event is the first item in the row.
            matrix[i][j] = 1;
            count[i]++;
          }else{
            matrix[i][j] = matrix[i][j-1]+1;     //Adding the event LC value to the matrix.
            count[i] += 1;
          }
        }
        if(vec[i][j].find("s") == 0 && count[i] == j){  //If statements to find the LC value for send values.
          if(j == 0){           //If statement in case the send value is the first item in the row.
            matrix[i][j] = 1;
            count[i] += 1;
          }else{
            matrix[i][j] = matrix[i][j-1]+1;  //Adding the send LC value to the matrix.
            count[i] += 1;
          }
        }
        if(vec[i][j].find("r") == 0 && count[i] == j){    //If statement to find receive values
          if(vec[i][j].compare(receivers[r]) == 0){       //If stament to find the correct receive value
            for(int k = 0; k < vec.size(); k++){          //For loops to find search values.
              for(int l = 0; l < vec[k].size(); l++){
                if(vec[k][l].compare(senders[s]) == 0){     //If statement to find matching send and receive.
                  if(j == 0){                         //If statement in case the receive value is the first item in the row.
                    matrix[i][j] = findMax(0,matrix[k][l]) + 1;
                    count[i] += 1;
                  }else{                      //Adding the receive LC value to the matrix.
                    matrix[i][j] = findMax(matrix[i][j-1],matrix[k][l]) + 1;
                    count[i]+= 1;
                  }
                }
              }
            }
            s++;
            r++;
          }
        }
        if(vec[i][j].length() == 4 && count[i] == j){  //If statement to handle NULL values.
          count[i] += 1;
        }
      }
    }
    finished = true;
    for(int i = 0; i < n; i++){         //Checking the conditon to exit the while loop.
      if(count[i] != vec[i].size()){
        finished = false;
      }
    }
  }

  cout << "Original Matrix: " << endl;    //Displaying the origninal matrix.
  displayString(vec);
  cout << "Updated Matrix: " << endl;
  displayInt(matrix);                   //Displaying the updated matrix.
}

//Algorithm Verify for Lamport's Logical Clock.
void verify(){
  int n, m;                            //Initializing variables.
  vector <vector<string>> matrix;
  int s = 0;
  int r = 0;
  int e = 0;
  bool found;
  string senders[9] = {"s1", "s2", "s3", "s4", "s5", "s6", "s7", "s8", "s9"}; //Initializing arrays to keep track of events, sends, and receives.
  string receivers[9] = {"r1", "r2", "r3", "r4", "r5", "r6", "r7", "r8", "r9"};
  string events[24] = {"a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l",
                    "m", "n", "o", "p", "q", "t", "u", "v", "w",  "x", "y", "z"};
  cout << "How many rows (N)?" << endl;    //Asking user to enter the amount of rows(n) and columns(m).
  cin >> n;
  cout << "How many columns (M)?" << endl;
  cin >> m;
  vector<vector<string>> vec = readInput(n,m);    //Calling the readInput function.
  vector<vector<int>> num = changeToInt(vec);     //Changeing the strings in the vector to integers.

  for(int i = 0; i < num.size(); i++){            //For loops to iterate through the matrix
    matrix.push_back(vector<string> ());            //and fill every entry with NULL.
    for(int j = 0; j < num[i].size(); j++){
      matrix[i].push_back("NULL");
    }
  }
  for(int i = 0; i < num.size(); i++){          //For loops to locate where the recieve values are in the matrix.
    for(int j = 0; j < num[i].size(); j++){
      if(j > 0 && num[i][j-1] + 1 != num[i][j] && num[i][j] != 0){ //Checking if the previous entry is 1 less than the current entry
        matrix[i][j] = receivers[r];            //Adding a recive value to to the matrix and incrmenting the recievers array.
        r++;
      }else if(j == 0 && num[i][j] != 1){      //If statement to locate when a receive message is the first in the row.
        matrix[i][j] = receivers[r];          //Adding a recive value to to the matrix and incrmenting the recievers array.
        r++;
      }
    }
  }
  for(int i = 0; i < matrix.size(); i++){     //For to locate where the send values are in the matrix.
    found = false;
    for(int j = 0; j < matrix[i].size(); j++){
      if(matrix[i][j].find("r") == 0){      //Finding the location of previous receive values.
        for(int k = 0; k < num.size(); k++){
          for(int l = 0; l < num[k].size(); l++){
            if(num[i][j] - num[k][l] == 1){   //Finding a time slot that is one less then the recieve value.
              matrix[k][l] = senders[s];      //Adding the send value to the matrix.
              s++;
              found = true;
            }
          }
        }
      }
    }
    if(found == false){               //In case a correct place for a send event is not found
      cout << "incorrect" << endl;    //the program will output "incorrect".
      return;
    }
  }
  for(int i = 0; i < matrix.size(); i++){     //For loops to fill ot the rest of the matrix with events.
    for(int j = 0; j < matrix[i].size(); j++){
      if(matrix[i][j].find("r") != 0 && matrix[i][j].find("s") != 0 && num[i][j] != 0){   //Checking that the value is not a send, receive, or null.
        matrix[i][j] = events[e];       //Adding the event to the matrix
        e++;
      }
    }
  }
  cout << "Original Matrix: " << endl;    //Displaying the original matrix.
  displayString(vec);
  cout << "Updated Matrix: " << endl;     //Displaying the updated matrix.
  displayString(matrix);
}
