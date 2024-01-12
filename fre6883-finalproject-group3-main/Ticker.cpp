#include <algorithm>  // Add this header for std::sort
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <stdio.h>
#include <cstring>
#include <cmath>
#include <string> 
#include <locale>
#include <iomanip>
#include "curl/curl.h"
#include "Ticker.h"

using namespace std;
namespace fre{

    Vector Ticker::CalcReturns(){
        Vector calc_returns;
        int size = adjClosePrices.size();
        for (int i = 0; i < size - 1; i++){
            calc_returns.push_back(log(adjClosePrices[i+1]/adjClosePrices[i]));
        }
        //cout << "CalcReturns finish" << endl;
        return calc_returns;
    }
    
    Vector Ticker::CalcCumReturns(){
        Vector calc_returns = CalcReturns();
        // cout << returns << endl;
        double cumReturn = 0.0;
        int size = calc_returns.size();
        Vector calc_cumReturns(size);
        // cout << "SIZE: " << size << endl;
        for (int i = 0; i < size; i++){
            cumReturn = cumReturn + calc_returns[i];
            calc_cumReturns[i] = cumReturn;
        }
        //cout << calc_cumReturns << endl;
        //cout << "Calc_cumreturns finish" << endl;
        return calc_cumReturns;
    }
    
    Vector Ticker::CalcAbnormReturns(Vector& benchmarkReturns){
        Vector calc_abnormReturns;
        Vector calc_returns = CalcReturns();
        calc_abnormReturns = calc_returns - benchmarkReturns;
        //cout << "ABNORMAL RETURNS" << endl;
        //cout << abnormReturns << endl;
        return calc_abnormReturns;
    }
}