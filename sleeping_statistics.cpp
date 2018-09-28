/*
Name: Aaron Barlow
Date: 1/31/2016
Description: Allow users to read in and manipulate sleeping statistics
*/

#include <iostream>
#include "functions.h"

int main() {
  SleepData sleep[MAX_NUM_SLEEP_STATISTICS];
  int num_records;
  char choice;
  bool exit = false;

  // Read a given file into an array of structures
  read_file( sleep, num_records );

  // Display a menu
  do {

    // Get user choice
    cout << "Main Menu" << endl
    << "----------------------------------------" << endl
    << "1.) Search by Name" << endl
    << "2.) See Longest Sleep Time(s)" << endl
    << "3.) See Shortest Sleep Times(s)" << endl
    << "4.) Exit Program" << endl
    << "Choice: ";
    cin >> choice;

    // Associate choice with an action
    switch(choice) {
      case '1': // Search by name
        search_by_name(sleep, num_records);
        break;

      case '2': // See longest sleep time(s)
        longest_sleep_times(sleep, num_records);
        break;

      case '3': // See shortest sleep time(s)
        shortest_sleep_times(sleep, num_records);
        break;

      case '4': // Exit program
        exit = true;
        break;

      default: // Error occured
        cout << "Please enter a valid option." << endl;
        break;
    }

  } while(!exit);

  return 0;
}


// assume every record is on a new line
// All structures, constants, definitions, and function definitions go in .h
// seperate line for each record
// show only the SHORTEST OR LONGEST sleep time
// when searching first and last name only EXACT match by IF
// TESTING: less than 10, more than 10 and exact 10 records
// antisipate all possible outcomes

