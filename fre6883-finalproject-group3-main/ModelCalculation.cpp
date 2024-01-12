#include <iostream>
#include <map>
#include <vector>
#include <string>

#include <cmath>
#include "ModelCalculation.h"
using namespace std;

namespace fre {
    
    // Calculate average daily abnormal returns (AARt) for all stocks on each day
    void ModelCalculation::SetAAR(vector<Ticker>& tickers){
        int size = tickers[0].GetAbnormReturns().size();
        AAR.assign(size, 0.0);
        M.assign(size, (double)tickers.size());
        cout << "RETURNS SIZE: " << tickers[0].GetAbnormReturns().size() << endl;
        cout << "NUMBER OF STOCKS: " << tickers.size() << endl;
        for(unsigned int i = 0; i < tickers.size(); i++){
            //cout << tickers[i].GetAbnormReturns().size()<<" ";
            if (size != tickers[i].GetAbnormReturns().size()) {
                cout << "Drop" << tickers[i].GetSymbol()<< endl;
                continue;
            }
            AAR = AAR + tickers[i].GetAbnormReturns();
            //cout << tickers[i].GetSymbol() << endl;
            //cout << AAR << endl;
        }
        AAR = AAR/M;
    }
    
    void ModelCalculation::SetCAAR(){
        //cout << "In here" << endl;
        CAAR.assign(AAR.size(), 0.0);
        cout << "SIZE: " << CAAR.size() << endl;
        CAAR[0] = AAR[0];
        for(unsigned int i = 1; i < CAAR.size(); i++){
            CAAR[i] = CAAR[i-1] + AAR[i];
            //cout << CAAR[i] << endl;
        }
    }
}