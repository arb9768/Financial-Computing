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
#include "Ticker.h"
#include "HistoricalPriceFetcher.h"
#include "Matrix.h"

using namespace std;
namespace fre{
    void PrintOptions();
    void Option1(vector<Ticker>& missTickers, vector<Ticker>& meetTickers, vector<Ticker>& bestTickers, vector<Ticker>& missTickersFullData, vector<Ticker>& meetTickersFullData, vector<Ticker>& bestTickersFullData);
    void Option2(const vector<Ticker>& missTickers, const vector<Ticker>& meetTickers, const vector<Ticker>& bestTickers);
    void Option3(vector<Ticker>& missTickers, vector<Ticker>& meetTickers, vector<Ticker>& bestTickers,
                vector<Vector>& AAR_avg, vector<Vector>& CAAR_avg,
                vector<Vector>& AAR_std, vector<Vector>& CAAR_std);
    void Option4(const vector<Vector>& final_CAAR);
    bool CheckData(bool getData);
    void MenuCommands();
}