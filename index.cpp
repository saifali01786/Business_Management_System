#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <iomanip>

using namespace std;

class user
{
private:
    string email, password;

public:
    static int userCount;

    user()
    {
        userCount++;
    }

    virtual void displayDetails() = 0;

    bool existingUser(string &Email)
    {
        ifstream file("user.txt");
        string em;
        while (file >> em)
        {
            if (em == Email)
            {
                return true;
            }
        }
        return false;
    }
    void signup()
    {
        cin.ignore();
        cout << "Email: " << endl;
        getline(cin, email);

        if (existingUser(email))
        {
            cout << "Email already exists. Try logging in.\n";
            return;
        }

        cout << "Password: " << endl;
        getline(cin, password);

        ofstream file("user.txt", ios::app);
        file << email << " " << password << endl;

        cout << "You have signup successfully" << endl;
    }

    bool login()
    {
        cin.ignore();
        cout << "Email: " << endl;
        getline(cin, email);

        cout << "Password: " << endl;
        getline(cin, password);

        ifstream file("user.txt");
        string em, pass;
        while (file >> em >> pass)
        {
            if (em == email && pass == password)
            {
                cout << "Login successful" << endl;
                return true;
            }
            else if (em != email)
            {
                cout << "Wrong Email. try again" << endl;
                return false;
            }
            else if (pass != password)
            {
                cout << "Wrong Password. try again" << endl;
                return false;
            }
        }
        return false;
    }
};

int user::userCount = 0;

class Business : public user
{
private:
    string companyName, ownerName;
    int capital;
    int capitalThreshold;

public:
    void setData()
    {
        cout << "Enter the Name of the Company: " << endl;
        getline(cin, companyName);
        cout << "Enter the Name of owner of the Company: " << endl;
        getline(cin, ownerName);
        cout << "Enter the capital of the company: " << endl;
        cin >> capital;
        cout << "Enter the capital threshold of the company: " << endl;
        cin >> capitalThreshold;
    }

    void checkCapitalThreshold()
    {
        if (capital >= capitalThreshold)
        {
            cout << "Alert: Your capital has reached the threshold (" << capitalThreshold << " PKR)." << endl;
        }
    }

    string getCompanyName() const
    {
        return companyName;
    }

    string getOwnerName()
    {
        return ownerName;
    }

    int getCapital()
    {
        return capital;
    }
    int getCapitalThreshold()
    {
        return capitalThreshold;
    }

    void displayDetails() override
    {
        cout << "Name of the Company: " << companyName << endl;
        cout << "Name of owner of the Company: " << ownerName << endl;
        cout << "Capital of the company: " << capital << endl;
    }
    friend void compareInitialAndCurrentCapital(const Business &b, double totalProfitAfterTax);
};

void compareInitialAndCurrentCapital(const Business &b, double totalProfitAfterTax)
{

    double currentCapital = b.capital + totalProfitAfterTax;
    cout << "\n--- Capital Comparison ---" << endl;
    cout << "Initial Capital: " << b.capital << " PKR" << endl;
    cout << "Current Capital Initial + Profit After Tax: " << currentCapital << " PKR" << endl;
    if (currentCapital > b.capital)
    {
        cout << "Capital has increased by: " << (currentCapital - b.capital) << " PKR" << endl;
    }
    else if (currentCapital < b.capital)
    {
        cout << "Capital has decreased by: " << (b.capital - currentCapital) << " PKR" << endl;
    }
    else
    {
        cout << "Capital remains unchanged." << endl;
    }
}

class Shares
{
protected:
    int valuation;
    int numberOfShares;
    int pricePerShare;

public:
    void setData()
    {
        cout << "Enter the valuation of company: ";
        cin >> valuation;

        cout << "Enter the total number of shares: ";
        cin >> numberOfShares;

        while (numberOfShares == 0)
        {
            cout << "Number of shares cannot be zero. Enter again: ";
            cin >> numberOfShares;
        }

        pricePerShare = valuation / numberOfShares;
        cout << "Price of one share: " << pricePerShare << endl;
        cin.ignore();
    }

    int getValuation() const { return valuation; }
    int getNumberOfShares() const { return numberOfShares; }
    int getPricePerShare() const { return pricePerShare; }
};

class Transactions
{
private:
    struct record
    {
        string description;
        int amount;
        int months;
    };
    char choice;
    vector<record> Income;
    vector<record> Expense;
    int totalMonthsToRecord;

public:
    Transactions()
    {
        totalMonthsToRecord = 0;
    }

    void setMonths()
    {
        cout << "Enter the number of months for which you want to enter the records:" << endl;
        cin >> totalMonthsToRecord;
        cin.ignore();
    }

    int getMonths() const
    {
        return totalMonthsToRecord;
    }
    void addIncome()
    {
        if (totalMonthsToRecord == 0)
        {
            cout << "Please set the number of months first." << endl;
            return;
        }

        for (int i = 1; i <= totalMonthsToRecord; i++)
        {
            cout << "Enter records for month: " << i << endl
                 << endl;
            do
            {
                record newRecord;
                newRecord.months = i;
                cout << "Enter the source of income: " << endl;
                getline(cin, newRecord.description);
                cout << "Enter the amount of income: " << endl;
                cin >> newRecord.amount;
                Income.push_back(newRecord);
                cout << "Do you want to add income form other sources (Y/N)?: " << endl;
                cin >> choice;
                cin.ignore();

            } while (choice == 'Y' || choice == 'y');
        }
    }
    void displayAllIncome()
    {
        if (Income.empty())
        {
            cout << "No income records available." << endl;
            return;
        }

        for (int month = 1; month <= getMonths(); month++)
        {
            cout << "\nIncome for Month " << month << ":" << endl;
            int monthTotal = 0;
            bool found = false;

            for (auto &rec : Income)
            {
                if (rec.months == month)
                {
                    cout << "- " << rec.description << ": " << rec.amount << " PKR" << endl;
                    monthTotal += rec.amount;
                    found = true;
                }
            }

            if (found)
                cout << "Total income for month " << month << ": " << monthTotal << " PKR\n";
            else
                cout << "No income recorded for this month.\n";
        }
    }

    void displayIncomeForMonth(int month)
    {
        if (month < 1 || month > getMonths())
        {
            cout << "Invalid month! Please enter a month between 1 and " << getMonths() << endl;
            return;
        }

        cout << "\nIncome for Month " << month << ":" << endl;
        int monthTotal = 0;
        bool found = false;

        for (auto &rec : Income)
        {
            if (rec.months == month)
            {
                cout << "- " << rec.description << ": " << rec.amount << " PKR" << endl;
                monthTotal += rec.amount;
                found = true;
            }
        }

        if (found)
            cout << "Total income for month " << month << ": " << monthTotal << " PKR\n";
        else
            cout << "No income recorded for this month.\n";
    }

    void calculateTotalIncomePerMonth()
    {
        int totalMonths = getMonths();

        if (Income.empty())
        {
            cout << "No income records available." << endl;
            return;
        }

        for (int month = 1; month <= totalMonths; month++)
        {
            int totalIncome = 0;

            for (auto &rec : Income)
            {
                if (rec.months == month)
                {
                    totalIncome += rec.amount;
                }
            }

            cout << "Total income for month " << month << ": " << totalIncome << " PKR" << endl;
        }
    }

    void addExpense()
    {
        if (totalMonthsToRecord == 0)
        {
            cout << "Please set the number of months first." << endl;
            return;
        }
        for (int i = 1; i <= totalMonthsToRecord; i++)
        {
            cout << "Enter records for month: " << i << endl
                 << endl;
            do
            {
                record newRecord;
                newRecord.months = i;
                cout << "Enter the source of expense: " << endl;
                getline(cin, newRecord.description);

                cout << "Enter the amount spent: " << endl;
                cin >> newRecord.amount;
                Expense.push_back(newRecord);

                cout << "Do you want to add any other expense (Y/N)?:" << endl;
                cin >> choice;

                cin.ignore();
            } while (choice == 'y' || choice == 'Y');
        }
    }
    void displayAllExpenses()
    {
        if (Expense.empty())
        {
            cout << "No expense records available." << endl;
            return;
        }

        for (int month = 1; month <= getMonths(); month++)
        {
            cout << "Expense for Month " << month << ":" << endl;
            int monthTotal = 0;
            bool found = false;

            for (auto &rec : Expense)
            {
                if (rec.months == month)
                {
                    cout << "- " << rec.description << ": " << rec.amount << " PKR" << endl;
                    monthTotal += rec.amount;
                    found = true;
                }
            }

            if (found)
                cout << "Total expenses for month " << month << ": " << monthTotal << " PKR\n";
            else
                cout << "No expenses recorded for this month.\n";
        }
    }

    void displayExpenseForMonth(int month)
    {
        if (month < 1 || month > getMonths())
        {
            cout << "Invalid month! Please enter a month between 1 and " << getMonths() << endl;
            return;
        }

        cout << "\nExpense for Month " << month << ":" << endl;
        int monthTotal = 0;
        bool found = false;

        for (auto &rec : Expense)
        {
            if (rec.months == month)
            {
                cout << "- " << rec.description << ": " << rec.amount << " PKR" << endl;
                monthTotal += rec.amount;
                found = true;
            }
        }

        if (found)
            cout << "Total Expense for month " << month << ": " << monthTotal << " PKR\n";
        else
            cout << "No Expense recorded for this month.\n";
    }

    void calculateTotalExpensePerMonth()
    {
        int totalMonths = getMonths();

        if (Expense.empty())
        {
            cout << "No income records available." << endl;
            return;
        }

        for (int month = 1; month <= totalMonths; month++)
        {
            int totalExpense = 0;

            for (auto &rec : Expense)
            {
                if (rec.months == month)
                {
                    totalExpense += rec.amount;
                }
            }

            cout << "Total expense for month " << month << ": " << totalExpense << " PKR" << endl;
        }
    }

    double totalIncome() const
    {
        double sum = 0;
        for (const auto &rec : Income)
            sum += rec.amount;
        return sum;
    }

    double totalExpense() const
    {
        double sum = 0;
        for (const auto &rec : Expense)
            sum += rec.amount;
        return sum;
    }

    const vector<record> &getIncome() const
    {
        return Income;
    }

    const vector<record> &getExpense() const
    {
        return Expense;
    }

    void generateMonthlyReport(int month, const string &companyName)
    {
        if (totalMonthsToRecord == 0)
        {
            throw string("Error: No records entered yet. Please set months and add transactions first.");
        }
        if (month < 1 || month > totalMonthsToRecord)
        {
            throw 201;
        }

        cout << "\n--- Monthly Financial Report for " << companyName << " (Month " << month << ") ---" << endl;
        cout << "----------------------------------------------------" << endl;

        displayIncomeForMonth(month);
        cout << endl;
        displayExpenseForMonth(month);

        int totalIncomeMonth = 0;
        for (const auto &rec : Income)
        {
            if (rec.months == month)
                totalIncomeMonth += rec.amount;
        }

        int totalExpenseMonth = 0;
        for (const auto &rec : Expense)
        {
            if (rec.months == month)
                totalExpenseMonth += rec.amount;
        }

        cout << "\nSummary for Month " << month << ":" << endl;
        cout << "  Total Income: " << totalIncomeMonth << " PKR" << endl;                          // This line now has access to totalIncomeMonth
        cout << "  Total Expense: " << totalExpenseMonth << " PKR" << endl;                        // This line now has access to totalExpenseMonth
        cout << "  Net Profit/Loss: " << (totalIncomeMonth - totalExpenseMonth) << " PKR" << endl; // And for net profit
        cout << "----------------------------------------------------" << endl;
    }

    void saveMonthlyReportToFile(const string &filename, int month, const string &companyName)
    {
        if (totalMonthsToRecord == 0)
        {
            throw string("Error: No records entered yet. Cannot save report.");
        }
        if (month < 1 || month > totalMonthsToRecord)
        {
            throw 202;
        }

        ofstream file(filename);
        if (!file)
        {
            throw string("Error: Could not open file " + filename + " for writing monthly report.");
        }

        file << "--- Monthly Financial Report for " << companyName << " (Month " << month << ") ---" << endl;
        file << "----------------------------------------------------" << endl;

        file << "\nIncome for Month " << month << ":" << endl;

        int monthTotalIncome = 0;
        bool foundIncome = false;
        for (const auto &rec : Income)
        {
            if (rec.months == month)
            {
                file << "- " << rec.description << ": " << rec.amount << " PKR" << endl;
                monthTotalIncome += rec.amount;
                foundIncome = true;
            }
        }
        if (foundIncome)
            file << "Total income for month " << month << ": " << monthTotalIncome << " PKR\n";
        else
            file << "No income recorded for this month.\n";

        file << "\nExpense for Month " << month << ":" << endl;
        int monthTotalExpense = 0;
        bool foundExpense = false;
        for (const auto &rec : Expense)
        {
            if (rec.months == month)
            {
                file << "- " << rec.description << ": " << rec.amount << " PKR" << endl;
                monthTotalExpense += rec.amount;
                foundExpense = true;
            }
        }
        if (foundExpense)
            file << "Total Expense for month " << month << ": " << monthTotalExpense << " PKR\n";
        else
            file << "No Expense recorded for this month.\n";

        int monthlyProfitLoss = monthTotalIncome - monthTotalExpense; // Use the newly declared variables
        file << "\nSummary for Month " << month << ":" << endl;
        file << "  Total Income: " << monthTotalIncome << " PKR" << endl;   // This line now has access
        file << "  Total Expense: " << monthTotalExpense << " PKR" << endl; // This line now has access
        file << "  Net Profit/Loss: " << monthlyProfitLoss << " PKR" << endl;
        file << "----------------------------------------------------" << endl;

        file.close();
        cout << "Monthly report for Month " << month << " saved to " << filename << endl;
    }
};

class ShareHolders : public Shares
{
protected:
    struct ShareHolderInfo
    {
        string name;
        string nationalID;
        int numberOfShares;
        float percentage;
        int totalShareValue;
    };

public:
    vector<ShareHolderInfo> holders;
    void addShareHolders()
    {
        int totalCompanyShares = getNumberOfShares();
        int pricePerShare = getPricePerShare();
        int runningTotalShares = 0;

        ShareHolderInfo owner;
        cout << "\n--- Enter OWNER's Shares Detail ---" << endl;
        cout << "Enter OWNER's name: ";
        getline(cin, owner.name);

        cout << "Enter OWNER's national ID: ";
        getline(cin, owner.nationalID);

        do
        {
            cout << "Enter number of shares owned by OWNER: ";
            cin >> owner.numberOfShares;
            if (owner.numberOfShares > totalCompanyShares)
            {
                cout << "Error: Shares exceed total available company shares (" << totalCompanyShares << "). Please enter again.\n";
            }
        } while (owner.numberOfShares > totalCompanyShares);

        cin.ignore();
        runningTotalShares += owner.numberOfShares;

        owner.percentage = (float)owner.numberOfShares / totalCompanyShares * 100;
        owner.totalShareValue = owner.numberOfShares * pricePerShare;
        holders.push_back(owner);

        char choice;
        cout << "\n--- Add Other Shareholders ---" << endl;
        do
        {
            ShareHolderInfo sh;
            cout << "\nEnter shareholder's name: ";
            getline(cin, sh.name);

            cout << "Enter shareholder's national ID: ";
            getline(cin, sh.nationalID);

            do
            {
                cout << "Enter number of shares owned: ";
                cin >> sh.numberOfShares;

                if (runningTotalShares + sh.numberOfShares > totalCompanyShares)
                {
                    cout << "Error: Adding this shareholder exceeds total available company shares ("
                         << totalCompanyShares << ").\nRemaining shares: "
                         << totalCompanyShares - runningTotalShares << "\nPlease enter a valid number.\n";
                }
            } while (runningTotalShares + sh.numberOfShares > totalCompanyShares);

            cin.ignore();

            runningTotalShares += sh.numberOfShares;
            sh.percentage = (float)sh.numberOfShares / totalCompanyShares * 100;
            sh.totalShareValue = sh.numberOfShares * pricePerShare;
            holders.push_back(sh);

            if (runningTotalShares == totalCompanyShares)
            {
                cout << "All company shares have been allocated.\n";
                break;
            }

            cout << "Do you want to add another shareholder? (Y/N): ";
            cin >> choice;
            cin.ignore();
        } while ((choice == 'Y' || choice == 'y') && runningTotalShares < totalCompanyShares);

        if (runningTotalShares < totalCompanyShares)
        {
            cout << "Note: " << (totalCompanyShares - runningTotalShares) << " shares remain unallocated.\n";
        }
    }

    int totalShares()
    {
        int total = 0;
        for (auto &s : holders)
        {
            total += s.numberOfShares;
        }
        return total;
    }

    vector<ShareHolderInfo> getAllHolders()
    {
        return holders;
    }

    void displayShareHolders()
    {
        cout << "\n--- Company Share Information ---" << endl;
        cout << "Total valuation of the company: " << getValuation() << endl;
        cout << "Total number of shares of the company: " << getNumberOfShares() << endl;
        cout << "Price of each share of the company: " << getPricePerShare() << endl;

        cout << "\n--- Shareholders Details ---" << endl;
        cout << left << setw(20) << "Name"
             << setw(20) << "National ID"
             << setw(18) << "Shares Owned"
             << setw(18) << "% Ownership"
             << setw(20) << "Share Value (PKR)" << endl;

        cout << string(96, '-') << endl;

        for (const auto &s : holders)
        {
            cout << left << setw(20) << s.name
                 << setw(20) << s.nationalID
                 << setw(18) << s.numberOfShares
                 << setw(17) << fixed << setprecision(2) << s.percentage
                 << setw(20) << s.totalShareValue << endl;
        }
    }
};

class ProfitCalculator : public ShareHolders
{
private:
    double totalProfitBeforeTax;
    double totalProfitAfterTax;
    double taxRate;
    double taxAmount;
    double profitPerShareAfterTax;
    bool profitCalculated;

public:
    ProfitCalculator(double tax = 0.15)
        : taxRate(tax), totalProfitBeforeTax(0), totalProfitAfterTax(0.0), profitCalculated(false) {}

    void calculateProfit(Transactions &t)
    {
        totalProfitBeforeTax = t.totalIncome() - t.totalExpense();
        taxAmount = totalProfitBeforeTax * taxRate;
        totalProfitAfterTax = totalProfitBeforeTax - taxAmount;
        profitCalculated = true;

        cout << "\n--- Profit Report for all months---\n";
        cout << "Total Profit (Before Tax): " << totalProfitBeforeTax << " PKR" << endl;
        cout << "Tax Deducted (@ " << taxRate * 100 << "%): " << taxAmount << " PKR" << endl;
        cout << "Total Profit (After Tax): " << totalProfitAfterTax << " PKR" << endl;
    }

    vector<double> calculateProfitPerShareholder(ShareHolders &sh)
    {
        std::vector<double> distribution;
        int totalShares = sh.totalShares();

        if (totalShares == 0)
        {
            cout << "No shareholders found. Cannot calculate profit per shareholder." << endl;
            return vector<double>{};
        }

        profitPerShareAfterTax = totalProfitAfterTax / totalShares;
        cout << "Profit Per Share (After Tax): " << profitPerShareAfterTax << " PKR" << endl;

        for (const auto &s : holders)
        {
            double individualProfit = s.numberOfShares * profitPerShareAfterTax;
            distribution.push_back(individualProfit);
            cout << "Shareholder: " << s.name << " | Profit: " << individualProfit << " PKR" << endl;
        }

        return distribution;
    }

    void displayProfitPerShareholder(ShareHolders &sh)
    {
        int totalShares = sh.totalShares();
        if (totalShares == 0)
        {
            cout << "No shareholders available to distribute profit." << endl;
            return;
        }

        vector<ShareHolderInfo> holders = sh.getAllHolders();

        cout << "\n--- Profit Distribution to Shareholders ---" << endl;
        for (const auto &holder : holders)
        {
            double individualProfit = holder.numberOfShares * profitPerShareAfterTax;
            cout << "Shareholder: " << holder.name << "" << endl;
            cout << "Shares Owned: " << holder.numberOfShares << "" << endl;
            cout << "Profit Received: " << individualProfit << " PKR" << endl;
            cout << "------------------------------" << endl;
        }
    }

    void displayMonthlyProfitAfterTax(const Transactions &t)
    {
        int totalMonths = t.getMonths();

        cout << "\n--- Monthly Profit After Tax ---" << endl;

        for (int month = 1; month <= totalMonths; ++month)
        {
            int totalIncome = 0, totalExpense = 0;

            for (const auto &rec : t.getIncome())
            {
                if (rec.months == month)
                    totalIncome += rec.amount;
            }

            for (const auto &rec : t.getExpense())
            {
                if (rec.months == month)
                    totalExpense += rec.amount;
            }

            int profit = totalIncome - totalExpense;
            double tax = profit > 0 ? profit * taxRate : 0.0;
            double finalProfit = profit - tax;

            cout << "\nMonth " << month << ":" << endl;
            cout << "  Total Income: " << totalIncome << " PKR" << endl;
            cout << "  Total Expense: " << totalExpense << " PKR" << endl;
            cout << "  Profit Before Tax: " << profit << " PKR" << endl;

            if (profit > 0)
            {
                cout << "  Tax (15%): " << tax << " PKR" << endl;
                cout << "  Profit After Tax: " << finalProfit << " PKR" << endl;
            }
            else
            {
                cout << "  No Tax Applied (Loss/Zero Profit)." << endl;
                cout << "  Net Loss: " << -profit << " PKR" << endl;
            }
        }
    }

    double getProfitBeforeTax()
    {
        return totalProfitBeforeTax;
    }
    double getProfitAfterTax() { return totalProfitBeforeTax; }
    double getTaxRate() { return taxRate; }
    double getTaxAmount() { return taxAmount; }
};

class Report : virtual public ProfitCalculator
{
public:
    void generateReport(Business &business, Transactions &transactions, ShareHolders &shareholders, ProfitCalculator &profitCalc, int capitalThreshold)
    {
        cout << "\n====== Monthly Business Report ======\n<<" << endl;

        cout << "Company Details:" << endl;
        business.displayDetails();
        cout << endl;

        if (business.getCapital() >= capitalThreshold)
        {
            cout << "ALERT: Your capital has reached the threshold of " << capitalThreshold << " PKR.\n"
                 << endl;
        }

        cout << "Financial Summary:" << endl;
        cout << "Total Income: " << transactions.totalIncome() << " PKR" << endl;
        cout << "Total Expense: " << transactions.totalExpense() << " PKR" << endl;
        cout << "Total Profit (Before Tax): " << profitCalc.getProfitBeforeTax() << " PKR" << endl;
        cout << "Tax Amount (" << profitCalc.getTaxRate() * 100 << "%): " << profitCalc.getTaxAmount() << " PKR" << endl;
        cout << "Total Profit (After Tax): " << profitCalc.getProfitAfterTax() << " PKR\n"
             << endl;

        cout << "Shareholder Profit Distribution:" << endl;
        shareholders.displayShareHolders();
        profitCalc.displayProfitPerShareholder(shareholders);

        cout << "====================================" << endl;
    }

    void saveReportToFile(const string &filename, Business &business, Transactions &transactions, ShareHolders &shareholders, ProfitCalculator &profitCalc, int capitalThreshold)
    {
        ofstream file(filename);
        if (!file)
        {
            cerr << "Error opening file for writing report." << endl;
            return;
        }

        file << "====== Monthly Business Report ======" << endl;

        file << "Company Details: " << endl;
        file << "Name: " << business.getCompanyName() << endl;
        file << "Owner: " << business.getOwnerName() << endl;
        file << "Capital: " << business.getCapital() << endl
             << endl;

        if (business.getCapital() >= capitalThreshold)
        {
            file << "ALERT: Your capital has reached the threshold of " << capitalThreshold << " PKR.\n\n";
        }

        file << "Financial Summary:" << endl;
        file << "Total Income: " << transactions.totalIncome() << " PKR" << endl;
        file << "Total Expense: " << transactions.totalExpense() << " PKR" << endl;
        file << "Total Profit (Before Tax): " << profitCalc.getProfitBeforeTax() << " PKR" << endl;
        file << "Tax Amount (" << profitCalc.getTaxRate() * 100 << "%): " << profitCalc.getTaxAmount() << " PKR" << endl;
        file << "Total Profit (After Tax): " << profitCalc.getProfitAfterTax() << " PKR"
             << endl;

        file << "Shareholder Profit Distribution: " << endl;
        vector<ShareHolders::ShareHolderInfo> holders = shareholders.getAllHolders();
        vector<double> profits = profitCalc.calculateProfitPerShareholder(shareholders);

        for (size_t i = 0; i < holders.size(); i++)
        {
            file << "Name: " << holders[i].name << ", Shares: " << holders[i].numberOfShares
                 << ", Profit: " << profits[i] << " PKR" << endl;
        }

        file << "====================================" << endl;

        file.close();
        cout << "Report saved to " << filename << endl;
    }
};

int main()
{
    Business b;
    Transactions t;
    Shares s;
    ShareHolders sh;
    ProfitCalculator pc;
    Report r;

    int choice;
    bool isLoggedIn = false;
    int cth = b.getCapitalThreshold();
    int totalShares = s.getNumberOfShares();
    int priceOfEachShare = s.getPricePerShare();
    int monthToSave;
    int monthToReport;
    string filename;

    do
    {
        cout << "\n--- Welcome to Business Management System ---" << endl;
        cout << "1. Sign Up" << endl;
        cout << "2. Login" << endl;
        cout << "3. Exit" << endl
             << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
            b.signup();
            break;
        case 2:
            isLoggedIn = b.login();
            break;
        case 3:
            cout << "Exiting..." << endl;
            return 0;
        default:
            cout << "Invalid choice. Try again." << endl;
        }
    } while (!isLoggedIn);

    do
    {
        cout << "\n--- Dashboard ---" << endl;
        cout << "1. Register Business Info" << endl;
        cout << "2. Enter the shares details" << endl;
        cout << "3. Add Income" << endl;
        cout << "4. Add Expense" << endl;
        cout << "5. View Total Income & Expenses" << endl;
        cout << "6. Display Shareholders Details" << endl;
        cout << "7. Calculate & Display Profit" << endl;
        cout << "8. Print Report for one month" << endl;
        cout << "9. Save Report of one month to a file" << endl;
        cout << "10. Print Total Report" << endl;
        cout << "11. Save Total Report to a data file" << endl;
        cout << "0. Exit" << endl
             << endl;
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore();
        cout << endl;
        try
        {
            switch (choice)
            {
            case 1:
                b.setData();
                break;
            case 2:
                sh.setData();
                sh.addShareHolders();
                break;
            case 3:
                t.setMonths();
                t.addIncome();
                break;
            case 4:
                t.addExpense();
                break;
            case 5:
                cout << "Total Income: " << t.totalIncome() << " PKR" << endl;
                cout << "Total Expense: " << t.totalExpense() << " PKR" << endl;
                break;
            case 6:
                sh.displayShareHolders();
                break;
            case 7:
                pc.holders = sh.getAllHolders();
                pc.calculateProfit(t);
                pc.displayMonthlyProfitAfterTax(t);
                pc.calculateProfitPerShareholder(sh);
                pc.displayProfitPerShareholder(sh);
                compareInitialAndCurrentCapital(b, pc.getProfitAfterTax());
                break;
            case 8:
                cout << "Enter the month number for the report: ";
                cin >> monthToReport;
                cin.ignore();
                t.generateMonthlyReport(monthToReport, b.getCompanyName());
                break;
            case 9:
                cout << "Enter the month number for the report to save: ";
                cin >> monthToSave;
                cin.ignore();
                cout << "Enter filename : ";
                getline(cin, filename);
                t.saveMonthlyReportToFile(filename, monthToSave, b.getCompanyName());
                break;
            case 10:
                r.generateReport(b, t, sh, pc, cth);
                break;
            case 11:
                r.saveReportToFile("data.txt", b, t, sh, pc, cth);
                break;
            case 0:
                cout << "Thank you for using the system. Goodbye!" << endl;
                break;
            default:
                cout << "Invalid choice. Try again." << endl;
            }
        }
        catch (int errorCode)
        {
            if (errorCode == 101)
            {
                cerr << "Error: Invalid Capital Threshold. Please enter a positive value." << endl;
            }
            else if (errorCode == 201)
            {
                cerr << "Error: Invalid Month for Report. Month must be within recorded range." << endl;
            }
            else if (errorCode == 202)
            {
                cerr << "Error: Invalid Month for Saving Report. Month must be within recorded range." << endl;
            }
            else
            {
                cerr << "An unknown integer error occurred. Error Code: " << errorCode << endl;
            }
        }
        catch (string errorMessage)
        {
            cerr << errorMessage << endl;
        }
        catch (...)
        {
            cerr << "An unexpected and unknown error occurred." << endl;
        }
    } while (choice != 0);

    return 0;
}
