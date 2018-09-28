/*
Name: Aaron Barlow
Date: 1/31/2016
Description: Allow users to read in and manipulate sleeping statistics
*/

#pragma once
#include <iostream>
#include <fstream>
#include <iomanip>
using namespace std;

const int MAX_NUM_SLEEP_STATISTICS = 10; // Max sleeping statistics to utilize
const int MAX_FILE_LENGTH = 256; // Max file length on linux is 255

// Full date in MM-DD-YYYY format
struct Date {
  int month;
  int day;
  int year;

  string format_date();
};

// Time in 24 hour HH:MM format
struct Time {
  int hour;
  int minute;

  string format_time();
};

struct SleepData {
  string first_name;
  string last_name;
  Date date;
  Time start_time;
  Time end_time;
  Time amount;

  void set_sleep_amount();
};


/*
get_file_name: gets file name from user
Parameters:
  filename: an array of characters
Returns: whether or not an error occured opening the file
*/
bool get_file_name( char filename[] ) {
  bool file_errors = false;
  ifstream input;

  cout << "Enter input file name: ";
  cin.getline( filename, MAX_FILE_LENGTH ); // Max file length on linux is 255

  input.open( filename );

  // When file could not be found
  if( input.fail() ) {
    cout << "Input file " << filename << " does not exist. \n";
    file_errors = true;

  // When file is empty
  } else if( input.peek() == EOF ) {
    cout << "Input file " << filename << " is empty. \n";
    file_errors = true;
  }

  // Close file
  input.close();

  return file_errors;
}

/*
read_file: reads data into an array of structures
Parameters:
  sleep: an array of structures containing first name, last name,
  date, sleep start time, sleep end time, and sleep amount
  row: the number of sleep records retrieved from the file
Returns: nothing
*/
void read_file( SleepData sleep[], int& row ) {
  char filename[MAX_FILE_LENGTH];
  bool file_errors;
  ifstream input;

  do { // Find a file that exists
    file_errors = get_file_name( filename );
  } while( file_errors );

  input.open( filename );

  // Read file data into an array of structures
  for( row = 0; row < MAX_NUM_SLEEP_STATISTICS && !input.eof(); row++ ) {

    // Get first and last name
    getline( input, sleep[row].first_name );
    getline( input, sleep[row].last_name );

    // Get date from MM-DD-YYYY format
    string month, day, year;
    getline( input, month, '-' );
    getline( input, day, '-' );
    getline( input, year );

    // Get sleep start time from HH:MM format
    string start_hour, start_minute;
    getline( input, start_hour, ':' );
    getline( input, start_minute );

    // Get sleep end time from HH:MM format
    string end_hour, end_minute;
    getline( input, end_hour, ':' );
    getline( input, end_minute  );

    // Convert strings to integers for date
    sleep[row].date.month = stoi(month);
    sleep[row].date.day = stoi(day);
    sleep[row].date.year = stoi(year);

    // Convert strings to integers for start time
    sleep[row].start_time.hour = stoi(start_hour);
    sleep[row].start_time.minute = stoi(start_minute);

    // Convert strings to integers for end time
    sleep[row].end_time.hour = stoi(end_hour);
    sleep[row].end_time.minute = stoi(end_minute);

    sleep[row].set_sleep_amount();

  }

  // Inform user of success
  cout << "File successfully loaded." << endl;

  // Then close file
  input.close();

}

/*
set_sleep_amount: sets the amount of time slept to be end time - start time
Parameters: none
Returns: nothing
*/
void SleepData::set_sleep_amount() {
  // Ensure that sleep amount is valid
  if( end_time.hour >= start_time.hour ) {
    amount.hour = end_time.hour - start_time.hour;
    amount.minute = abs(end_time.minute - start_time.minute);
  } else { // Sleep amount is invalid
    amount.hour = 0;
    amount.minute = 0;
  }
}

/*
format_time: ensures validity of time and then formats time into 24 hour HH:MM
Parameters: none
Returns: a string of the formatted time or invalid time if impossible time
*/
string Time::format_time() {
  string str_hour = to_string(hour), str_minute = to_string(minute);

  // Append a zero before times if necessary to ensure date validity
  if( hour < 10 ) {
    str_hour = "0" + str_hour;
  } else if( hour > 24 ) {
    return "Invalid time";
  }

  if( minute < 10 ) {
    str_minute = "0" + str_minute;
  } else if( minute >= 60 ) {
    return "Invalid time";
  }

  // Return 24 hour HH:MM format if valid time
  return str_hour + ":" + str_minute;
}

/*
format_date: ensures validity of date and formats date into MM-DD-YYYY
Parameters: none
Returns: a string of the formatted date or invalid date if impossible date
*/
string Date::format_date() {
  string str_month = to_string(month), str_day = to_string(day);

  if( month > 12 || month < 1 ) {
    // Date is invalid if month is more than 12 months or less than 1 month
    return "Invalid date";
  } else if( month < 10 ) {
    // Append a zero before dates if necessary to ensure date validity
    str_month = "0" + str_month;
  }

  if( day > 31 || day < 1 ) {
    // Date is invalid if day is more than 31 days or less than 1 day
    return "Invalid date";

  } else if( day < 10 ) {
    // Append a zero before dates if necessary to ensure date validity
    str_day = "0" + str_day;
  }

  // Return MM-DD-YYYY format if valid date
  return str_month + "-" + str_day + "-" + to_string(year);
}

/*
swap_sleep_data: swaps two given SleepData structures
Parameters:
  x: a SleepData structure
  y: a SleepData structure
Returns: nothing
*/
void swap_sleep_data( SleepData& x, SleepData& y ){
  // Swap two given SleepData structures
  SleepData temp;
  temp = x;
  x = y;
  y = temp;
}

/*
search_by_name: find and display a sleep record's sleep start, end, and amount
Parameters:
  sleep: an array of structures containing first name, last name,
  date, sleep start time, sleep end time, and sleep amount
  num_records: the number of sleep records retrieved
Returns: nothing
*/
void search_by_name( SleepData sleep[], int num_records ) {
  bool user_found = false;
  string first_name;
  string last_name;

  cout << "Enter a first name: ";
  cin >> first_name;

  cout << "Enter a last name: ";
  cin >> last_name;

  for( int row = 0; row < num_records && !user_found; row++ ) {
    // Select the first instance of first and last name matching given name
    if( sleep[row].first_name == first_name && sleep[row].last_name == last_name ) {
      user_found = true;

      cout << "Sleep Start    Sleep End    Sleep Amount" << endl;
      cout << "----------------------------------------" << endl;

      // Print start date, end date, and sleep amount
      cout << sleep[row].start_time.format_time();
      cout << setw(15) << right << sleep[row].end_time.format_time();
      cout << setw(13) << right << sleep[row].amount.format_time() << endl;
    }
  }

  if(!user_found) cout << "No user was found." << endl;

}

/*
display_sleep_times: display formatted sleep date and sleep amount
Parameters:
  sleep: an array of structures containing first name, last name,
  date, sleep start time, sleep end time, and sleep amount
  num_records: the number of sleep records retrieved
Returns: nothing
*/
void display_sleep_times( SleepData sleep[], int num_records ) {
  for( int row = 0; row < num_records; row++ ) {
    // Print date and sleep amount
    cout << sleep[row].date.format_date();
    cout << setw(15) << right << sleep[row].amount.format_time() << endl;
  }
}

/*
longest_sleep_times: sort the sleep records by longest sleep times
Parameters:
  sleep: an array of structures containing first name, last name,
  date, sleep start time, sleep end time, and sleep amount
  num_records: the number of sleep records retrieved
Returns: nothing
*/
void longest_sleep_times( SleepData sleep[], int num_records ) {
  int top = 0;
  int min_index, i;

  // Numerically order each structure by the longest sleep times
  for( top = 0; top < num_records; top++ ) {
    min_index = top;
    for ( i = top+1; i < num_records; i++ ) {
      // Compare the sleeper's numerical position
      if( sleep[min_index].amount.hour == sleep[i].amount.hour ) {
        if( sleep[min_index].amount.minute <= sleep[i].amount.minute ) {
          min_index = i; // Update the minimum index
        }
      } else if( sleep[min_index].amount.hour < sleep[i].amount.hour ) {
        min_index = i; // Update the minimum index
      }
    }

    // Swap the structures
    swap_sleep_data( sleep[min_index], sleep[top] );

  }

  cout << "Date                 Longest Sleep Times" << endl;
  cout << "----------------------------------------" << endl;

  // print longest sleep times
  display_sleep_times( sleep, num_records );

}

/*
shortest_sleep_times: sort the sleep records by shortest sleep times
Parameters:
  sleep: an array of structures containing first name, last name,
  date, sleep start time, sleep end time, and sleep amount
  num_records: the number of sleep records retrieved
Returns: nothing
*/
void shortest_sleep_times( SleepData sleep[], int num_records ) {
  int top = 0;
  int min_index, i;

  // Numerically order each structure by the shortest sleep times
  for( top = 0; top < num_records; top++ ) {
    min_index = top;
    for ( i = top+1; i < num_records; i++ ) {
      // Compare the sleeper's numerical position
      if( sleep[min_index].amount.hour == sleep[i].amount.hour ) {
        if( sleep[min_index].amount.minute >= sleep[i].amount.minute ) {
          min_index = i; // Update the minimum index
        }
      } else if( sleep[min_index].amount.hour > sleep[i].amount.hour ) {
        min_index = i; // Update the minimum index
      }

    }

    // Swap the structures
    swap_sleep_data( sleep[min_index], sleep[top] );

  }

  cout << "Date                Shortest Sleep Times" << endl;
  cout << "----------------------------------------" << endl;

  // print shortest sleep times
  display_sleep_times( sleep, num_records );

}
