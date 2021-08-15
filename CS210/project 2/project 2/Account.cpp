#include<iostream>
#include<string>
#include<cstdio>

#include "Account.h"


Account::Account(double initalBalance, double initalDeposit, double initalInteres, int initialYears){
    balance = initalBalance;
    monthlyDeposit = initalDeposit;
    interestRate = initalInteres;
    numYears = initialYears;
}

void Account::setBalance(double newBalance){
    balance = newBalance;
}

void Account::setMonthlyDeposit(double newMonthlyDeposit){
    monthlyDeposit = newMonthlyDeposit;
}

void Account::setInterestRate(double newInterestRate){
    interestRate = newInterestRate;
}

void Account::setNumYears(double newNumYears){
    numYears = newNumYears;
}

double Account::calculateInterest() const {
    return (balance + monthlyDeposit) * ((interestRate / 100) / 12);
}

void Account::stepOneYear(){
    double interest = calculateInterest();
    balance = balance + (monthlyDeposit * 12) + interest;
    numYears--;
}

void Account::printBalanceAndInterestByYear(){
    double interest;
    int currentYear;
    int years = numYears;

    // loop through every year and print resuling calculation
    for(int i = 0; i < years; i++){
        stepOneYear();
        interest = calculateInterest();
        currentYear = i + 1;
        printf("%4*i %23.2*f %28.2*f\n", currentYear, balance, interest);

    }
}