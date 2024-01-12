#include <iostream>
#include <iomanip>
#include <vector>
#include <thread>
#include <algorithm>
#include <fstream>
#include <stdexcept>
#include <sstream>
#include <map>
#include <unistd.h>
#include "Bootstrap.h"
#include "plotGnuplot.h"

#include "Menu.h"

using namespace std;
namespace fre{
    
    void print_Options(){
        cout << endl;
        cout << "1 - Enter N to retrieve 2N+1 days of historical price data for all stocks." << endl;
        cout << "2 - Pull information for one stock from one group." << endl;
        cout << "3 - Show AAR, AAR-STD, CAAR and CAAR-STD for one group." << endl;
        cout << "4 - Show the gnuplot graph with CAAR for all 3 groups." << endl;
        cout << "5 - Exit the program." << endl;
    }
    
    void Option1(vector<Ticker>& missTickers, vector<Ticker>& meetTickers, vector<Ticker>& bestTickers, vector<Ticker>& missTickersFullData, vector<Ticker>& meetTickersFullData, vector<Ticker>& bestTickersFullData){
        int N;
        bool input = true;
        while(input){
            cout << "Enter value for N between 60 and 90: ";
            cin >> N;

            if (N < 60 || N > 90) {
                cerr << "Error: N must be between 60 and 90." << endl;
            }
            else{
                input = false;
            }
        }
        
        map<string, double> historical_benchmark_prices;
        FetchBenchmarkPrices("IWV", "2022-01-01", "2023-12-14", historical_benchmark_prices);
        cout << "Benchmark prices get." << endl;
        // Use bestTickers for bestRandomTicker
        std::thread missThread(FetchPrices, std::ref(missTickers), std::ref(missTickersFullData), std::ref(historical_benchmark_prices), N);
        std::thread meetThread(FetchPrices, std::ref(meetTickers), std::ref(meetTickersFullData), std::ref(historical_benchmark_prices), N);
        std::thread bestThread(FetchPrices, std::ref(bestTickers), std::ref(bestTickersFullData), std::ref(historical_benchmark_prices), N);

        // Join threads
        missThread.join();
        meetThread.join();
        bestThread.join();
        cout << "Fetching data for all stocks." << endl;
    }
    
    void Option2(const vector<Ticker>& missTickers, const vector<Ticker>& meetTickers, const vector<Ticker>& bestTickers) {
        string symbol;
        Ticker choice;
        bool found = false;
        
        cout << "Enter ticker symbol for detailed information: ";
        cin >> symbol;
        cout << endl;
    
        // Create a lambda function to search for the ticker
        auto searchTicker = [&](const vector<Ticker>& tickers) -> bool {
            for (const auto& ticker : tickers) {
                if (ticker.GetSymbol() == symbol) {
                    choice = ticker;
                    return true; // Ticker found
                }
            }
            return false; // Ticker not found
        };
    
        // Search in all three vectors
        if (searchTicker(missTickers) || searchTicker(meetTickers) || searchTicker(bestTickers)) {
            found = true;
        }
    
        if (found) {
            cout << "Symbol: " << choice.GetSymbol() << endl;
            cout << "Earning Announcement Date: " << choice.GetDate() << endl;
            cout << "Period Ending: " << choice.GetEndPeriod() << endl;
            cout << "Estimated: " << choice.GetEstimate() << endl;
            cout << "Reported Earnings: " << choice.GetReported() << endl;
            cout << "Surprise: " << choice.GetSurprise() << endl;
            cout << "Surprise %: " << choice.GetSurprisePercent() << endl;
            cout << "Earnings Group: " << choice.GetGroup() << endl;
            cout << endl;
            cout << "Daily Adj. Close Prices: " << endl;
            Vector prices = choice.GetAdjClosePrices();
            cout << prices << endl;
            cout << endl;
            cout << "Cumulative Returns: " << endl;
            Vector cumReturns = choice.GetCumReturns();
            cout << cumReturns << endl;
            cout << endl;
            cout << "Abnormal Returns: " << endl;
            Vector abnormReturns = choice.GetAbnormReturns();
            cout << abnormReturns << endl;
            cout << endl;
        } else {
            cout << "Ticker symbol is not available, please enter another" << endl;
        }
    }
    
    void Option3(vector<Ticker>& missTickers, vector<Ticker>& meetTickers, vector<Ticker>& bestTickers,
                vector<Vector>& AAR_avg, vector<Vector>& CAAR_avg,
                vector<Vector>& AAR_std, vector<Vector>& CAAR_std)
    {
        // vector<Vector> AAR_avg(3), CAAR_avg(3), AAR_std(3), CAAR_std(3);
        Bootstrap(missTickers, meetTickers, bestTickers, AAR_avg, CAAR_avg, AAR_std, CAAR_std);
        
        // result
        for (int j = 0; j<3; j++){
            cout << "Group " << j <<endl;
            cout << "AAR_avg" <<endl;
            cout << AAR_avg[j] << endl;
            cout << "CAAR_avg" <<endl;
            cout << CAAR_avg[j] << endl;
            cout << "AAR_std" <<endl;
            cout << AAR_std[j] << endl;
            cout << "CAAR_std" <<endl;
            cout << CAAR_std[j] << endl;
        }
    }
    
    void Option4(const vector<Vector>& final_CAAR)
    {

        // Split final_CAAR into three separate vectors
        const Vector& beat = final_CAAR[0];
        const Vector& meet = final_CAAR[1];
        const Vector& miss = final_CAAR[2];
        
        plotPreparation(beat, meet, miss);
    }

    bool CheckData(bool getData){
        if(!getData){
            cout << "Choose Option 1 to extract data" << endl;
            usleep(1000000);
            return false;
        }
        return true;
    }
    
    void MenuCommands(){
        char command = '0';
        bool getData = false;
        map<string, Ticker> miss;
        map<string, Ticker> meet;
        map<string, Ticker> best;
        vector<Ticker> tickers;
        const char* cIWB3000SymbolFile = "Russell3000EarningsAnnouncements.csv";
        populateSymbolVector(tickers, cIWB3000SymbolFile, miss, meet, best);
        // Create vectors to store Tickers
        vector<Ticker> missTickers;
        vector<Ticker> meetTickers;
        vector<Ticker> bestTickers;
        
        vector<Vector> AAR_avg(3), CAAR_avg(3), AAR_std(3), CAAR_std(3);
    
        // Extract Tickers from each map
        for (const auto& item : miss) {
            missTickers.push_back(item.second);
        }
        for (const auto& item : meet) {
            meetTickers.push_back(item.second);
        }
        for (const auto& item : best) {
            bestTickers.push_back(item.second);
        }
        
        vector<Ticker> missTickersFullData;
        vector<Ticker> meetTickersFullData;
        vector<Ticker> bestTickersFullData;
        while (command != '5'){
            cout << "Enter command from 1 to 5 and press enter: ";
            print_Options();
            cin >> command;
            switch(command){
                case '1':
                    Option1(missTickers, meetTickers, bestTickers, missTickersFullData, meetTickersFullData, bestTickersFullData);
                    getData = true;
                    usleep(1000000);
                    break;
                case '2':
                    if(!CheckData(getData)){
                        break;
                    }
                    Option2(missTickersFullData, meetTickersFullData, bestTickersFullData);
                    usleep(1000000);
                    break;
                case '3':
                    if(!CheckData(getData)){
                        break;
                    }
                    Option3(missTickersFullData, meetTickersFullData, bestTickersFullData, 
                            AAR_avg, CAAR_avg, AAR_std, CAAR_std);
                    usleep(1000000);
                    break;
                case '4':
                    if(!CheckData(getData)){
                        break;
                    }
                    Option4(CAAR_avg);
                    usleep(1000000);
                    break;
                case '5':
                    break;
                default:
                    cout << endl << "Please enter the right number from 1 to 5!" << endl; 
                    usleep(1000000);
                    break;
            }
        }
    }
}
