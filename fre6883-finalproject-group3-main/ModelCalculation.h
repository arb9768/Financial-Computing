#include <iostream>
#include <map>
#include <vector>
#include <string>
#include<cmath>
#include "Ticker.h"
#include "Matrix.h"

using namespace std;

namespace fre {
    class ModelCalculation {
        private:
            Vector AAR;
            Vector CAAR;
            Vector M;
        
        public:
            // Calculate average daily abnormal returns (AARt) for all stocks on each day
            void SetAAR(vector<Ticker>& tickers);
            // Calculate cumulative average abnormal returns (CAAR) up to each day T
            void SetCAAR();
            Vector GetAAR() const {return AAR;}
            Vector GetCAAR() const {return CAAR;}
    };
}