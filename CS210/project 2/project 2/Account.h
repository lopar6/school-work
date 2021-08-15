class Account {
    private: 
        double balance;
        double monthlyDeposit;
        double interestRate;
        int numYears;

        void stepOneYear();
        double calculateInterest() const;

    public:
        Account(double initalBalance, double initalDeposit, double initalInteres, int initialYears);
        double getBalance(){return balance;};
        void setBalance(double newBalance);
        double getMonthlyDeposit(){return monthlyDeposit;};
        void setMonthlyDeposit(double newMonthlyDeposit);
        double getInterestRate(){return interestRate;};
        void setInterestRate(double newInterestRate);
        int getNumYears(){return numYears;};
        void setNumYears(double newNumYears);

        void printBalanceAndInterestByYear();

};