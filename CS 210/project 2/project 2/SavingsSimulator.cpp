#include <iostream>
#include <string>
#include <conio.h>

#include "Account.h"

//cleans string and casts to double
double cleanedDouble(std::string str) {
    std::string convertedStr = "";
    bool hasPeriod = false;
    for(int i = 0; i < str.length(); i++){
        // if char is number or is '.'
        if (int(str[i]) > 47 && int(str[i]) < 58){
            convertedStr += str[i];
        };
        if (str[i] == '.') {
            if (!hasPeriod){
                hasPeriod = true;
                convertedStr += str[i];
            }
            else{
                throw std::runtime_error("Too many '.'s");
            };
        };
    }
    if (convertedStr == ""){
        throw std::runtime_error("No number entered");
    }
    return stod(convertedStr);
}

void mainLoop(){
// temporary variables to help init account instances 
    double balance;
    double deposit;
    double interest;
    double years;
    std::string tempStr;
    bool validInput = false;


    // get user input for account details
    while(!validInput){

        try{
            std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;
            std::cout << "~~~~~~~~~~ Data Input ~~~~~~~~~~" << std::endl;
            std::cout << "Initial Investment Amount: ";
            std::getline(std::cin, tempStr);
            balance = cleanedDouble(tempStr);
            std::cout << "Monthly Deposit: ";
            std::getline(std::cin, tempStr);
            deposit = cleanedDouble(tempStr);           
            std::cout << "Annual Interest: ";
            std::getline(std::cin, tempStr);
            interest = cleanedDouble(tempStr);   
            std::cout << "Number of Years: ";
            std::getline(std::cin, tempStr);
            years = cleanedDouble(tempStr);   
            // if it got this far then the input is likely ok
            validInput = true;
        }

        catch(std::runtime_error excpt){
            std::cout << std::endl;
            std::cout << std::endl;
            std::cout << "Invalid Input" << std::endl;
            std::cout << excpt.what() << std::endl;
            std::cout << "Press Enter to try again";
            std::cout << std::endl;
            std::cout << std::endl;
            std::cin.ignore();
        }
    };

    // init accounts with user data
    Account accountWithDeposit(balance, deposit, interest, years);
    Account accountWithoutDeposit(balance, 0, interest, years);

    // wait for user input
    std::cout << "Press any key to continue . . . " << std::endl;
    getch();

    std::cout << std::endl;
    std::cout << "---------------------------------------------------------" << std::endl;
    std::cout << "       Balance and Interest Without Monthly Deposits     " << std::endl;
    std::cout << "---------------------------------------------------------" << std::endl;
    std::cout << "year        Year End Balance     Year End Earned Interest" << std::endl;
    accountWithoutDeposit.printBalanceAndInterestByYear();

    std::cout << std::endl;
    std::cout << "---------------------------------------------------------" << std::endl;
    std::cout << "        Balance and Interest With Monthly Deposits       " << std::endl;
    std::cout << "---------------------------------------------------------" << std::endl;
    std::cout << "year        Year End Balance     Year End Earned Interest" << std::endl;
    accountWithDeposit.printBalanceAndInterestByYear();

    std::cout << std::endl;
    std::cout << std::endl;
}

int main(){
    std::string input;
    while(input != "q"){
        mainLoop();
        std::cout << " To exit program, please enter 'q' " << std::endl;
        std::cout << " To continue, please enter anything!" << std::endl;
        std::cout << std::endl;
        std::cout << std::endl;
        std::getline(std::cin, input);
    }

    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << "Thanks for using the Savings Simulator 4000!";
    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;


    return 0;
}