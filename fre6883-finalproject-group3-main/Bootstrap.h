#pragma once

#include "RandomDraw.h"
#include "ModelCalculation.h"
#include "Ticker.h"
#include "Matrix.h"

namespace fre {
    void Bootstrap(vector<Ticker>& miss_tickers, vector<Ticker>& meet_tickers, vector<Ticker>& best_tickers,
                   vector<Vector>& AAR_avg, vector<Vector>& CAAR_avg,
                   vector<Vector>& AAR_std, vector<Vector>& CAAR_std);
}
