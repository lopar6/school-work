/* 
Logan Parker
SNHU CS220
Project 1
05/22/2021
*/

#include <iostream>
#include <string>
#include <stdlib.h>
#include <thread>
#include <chrono>
#include <conio.h>

using namespace std;
using namespace chrono; //seconds(), now()

class Clock{

    protected:
        int hours;
        int minutes;
        int seconds;

    public:
   
        Clock() {

            // init time to actual time on OS
            time_t now = time(0);
            int _timeLocation;
            // convert now to string form
            char* dateTime = ctime(&now);
            string dateTimeString = string(dateTime);
            _timeLocation = dateTimeString.find(":");
            _timeLocation -= 2;

            // set time values based on location in string
            hours = stoi(dateTimeString.substr(_timeLocation, _timeLocation + 2));
            minutes = stoi(dateTimeString.substr(_timeLocation + 3, _timeLocation + 5));
            seconds = stoi(dateTimeString.substr(_timeLocation + 6, _timeLocation + 8));
        };

        void IncrementSeconds() { 
            if (seconds < 59){
                seconds ++;
            }
            else{
                seconds = 0;
                IncrementMinutes();
            }
        };

        void IncrementMinutes(){
            if (minutes < 59){
                minutes ++;
            }
            else{
                minutes = 0;
                IncrementHours();
            }
        }

        void IncrementHours(){
            if (hours < 24){
                hours ++;
            }
            else{
                hours = 1;
            }
        }

        void DisplayTime24() { 
            


            cout << "┏----------┓" << endl;
            printf("| %2.i:%2.i:%2i |\n", hours, minutes, seconds);
            cout << "┗----------┛" << endl;
        }

        void DisplayTime12() { 
            
            int _hours = hours;
            string amPm = "am";
            if (hours > 12) {
                _hours = hours - 12;
                amPm = "pm";
            }

            cout << "┏-------------┓" << endl;
            printf("| %2.i:%2.i:%2i ", _hours, minutes, seconds);
            cout << amPm << " |" << endl;
            cout << "┗-------------┛" << endl;
        }
};

void clearScreen(){
    cout << string( 30, '\n' ); 
}

void displayMenu(){
    cout << "┏----------------------------┓" << endl;
    cout << "| - To add an hour enter \'h\' |" << endl;
    cout << "| - To add a minute enter \'m\'|" << endl;
    cout << "| - To add a second enter \'s\'|" << endl;
    cout << "| - To exit enter  \'e\'       |" << endl;
    cout << "┗----------------------------┛" << endl;
    cout << endl;
}

// check and exicute based on  menu selection
bool canAndDoRouteSelectedOption(string selection, Clock& clock){
    if (selection == ("h")) {
        clock.IncrementHours();
        return true;
    }
    else if (selection == ("m")){
        clock.IncrementMinutes();
        return true;
    }
    else if (selection == ("s")) {
        clock.IncrementSeconds();
        return true;
    }
    else if (selection == ("e")) {
        return true;
    }
    else{
        cout << "Please enter a valid input"<< endl;
        return false;
    }
    cout << endl;
}

// display the clocks every second
void runClockDisplayLoop(Clock& clock, string& input){
    using namespace this_thread; // sleep_for, sleep_until
    
    // main loop for clock display
    while(true){
        while(input == ""){
            clearScreen();
            clock.DisplayTime24();
            clock.DisplayTime12();
            cout << endl;
            clock.IncrementSeconds();
            sleep_for(seconds(1));
        }

        // continue tracking time even if not displaying
        clock.IncrementSeconds();
        sleep_for(seconds(1));
    }
}

// auto lookForInput = [] (string& input){ input = getch(); };

int main(){


    string input;
    string selection;
    bool isValidSelection = false;
    Clock clock; 

    // create new thread for clock display
    // with another thread, getch() can now run simultaniously
    thread clockLoopThread(runClockDisplayLoop, ref(clock), ref(input));

    do {
        isValidSelection = false;

        // tells the clock thread to display the clock again
        input = "";
        // tells the clock thread to stop when key is struck
        input = getch();

        while (!isValidSelection){
            displayMenu();
            cin >> selection;
            isValidSelection = canAndDoRouteSelectedOption(selection, ref(clock));
        }

    } while (selection != "e");

    cout << "thanks!" << endl;
}

