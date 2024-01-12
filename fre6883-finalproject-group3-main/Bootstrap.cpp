#include "Bootstrap.h"

namespace fre {

    void Bootstrap(vector<Ticker>& miss_tickers, vector<Ticker>& meet_tickers, vector<Ticker>& best_tickers,
                   vector<Vector>& AAR_avg, vector<Vector>& CAAR_avg,
                   vector<Vector>& AAR_std, vector<Vector>& CAAR_std)
    {
        // 40 times sampling
        vector<vector<Vector>> AAR(40), CAAR(40);
        for (int a = 0; a<40; a++){
            AAR[a].resize(3);
            CAAR[a].resize(3);
        }
        // record result for each sample, each shape = 40 samples * 3 groups * Time length
        
        // ModelCalculation objects
        vector<ModelCalculation> calc_best(40), calc_meet(40), calc_miss(40);
        
        vector<int> draw_result; // a vector of integers representing the index of selected stocks
        RandomDraw rd;
        rd.setn(30);
        for (unsigned int n = 0; n < 40; n++){ // n-th sample
            // deal with exceed stocks
            cout << n << "th loop start"<<endl;
            
            rd.setN(best_tickers.size());
            draw_result = rd.getResult(); 
            
            vector<Ticker> chosen_best(30);
            
            for (unsigned int i = 0; i < draw_result.size(); i++){
                cout << best_tickers[draw_result[i]].GetSymbol() << " ";
                chosen_best[i] = best_tickers[draw_result[i]];
            }
            cout << endl;
            
            calc_best[n].SetAAR(chosen_best);
            calc_best[n].SetCAAR();
            AAR[n][0] = calc_best[n].GetAAR();
            CAAR[n][0] = calc_best[n].GetCAAR();
            
            cout<<"finish best"<<endl;
            
            //cout<< "In main" << endl;
            
            // deal with meet stocks
            rd.setN(meet_tickers.size());
            draw_result = rd.getResult();
            vector<Ticker> chosen_meet(30);
            
            for (unsigned int i = 0; i < draw_result.size(); i++){
                cout << meet_tickers[draw_result[i]].GetSymbol() << " ";
                chosen_meet[i] = meet_tickers[draw_result[i]];
                // other caluculation for exceed stocks
            }
            cout << endl;
            
            calc_meet[n].SetAAR(chosen_meet);
            calc_meet[n].SetCAAR();
            AAR[n][1] = calc_meet[n].GetAAR();
            CAAR[n][1] = calc_meet[n].GetCAAR();
            
            cout<< "finish meet" <<endl;
            
            // deal with miss stocks
            rd.setN(miss_tickers.size());
            draw_result = rd.getResult();
            
            cout<<"Miss N = " << miss_tickers.size()<<endl;
            
            vector<Ticker> chosen_miss(30);
            for (unsigned int i = 0; i < draw_result.size(); i++){
                cout << miss_tickers[draw_result[i]].GetSymbol() << " ";
                chosen_miss[i] = miss_tickers[draw_result[i]];
                // other caluculation for exceed stocks
            }
            cout << endl;
            
            calc_miss[n].SetAAR(chosen_miss);
            calc_miss[n].SetCAAR();
            AAR[n][2] = calc_miss[n].GetAAR();
            CAAR[n][2] = calc_miss[n].GetCAAR();

            cout<< "finish miss" << endl;
        }

    // calculate average AAR/CAAR and standard deviation
        cout<< "start calculation"<<endl;
        // vector<Vector> AAR_avg(3), CAAR_avg(3), AAR_std(3), CAAR_std(3);
        
        for (int i = 0; i<40; i++){
            for (int j = 0; j < 3; j++){
                if (i == 0){
                    AAR_avg[j] = AAR[i][j];
                    CAAR_avg[j] = CAAR[i][j];
                    AAR_std[j] = AAR[i][j]*AAR[i][j];
                    CAAR_std[j] = CAAR[i][j]*CAAR[i][j];
                }
                else{
                    AAR_avg[j] = AAR_avg[j] + AAR[i][j]; //size!!!
                    CAAR_avg[j] = CAAR_avg[j] + CAAR[i][j];
                    AAR_std[j] = AAR_std[j] + AAR[i][j]*AAR[i][j];
                    CAAR_std[j] = CAAR_avg[j] + CAAR[i][j]*CAAR[i][j];
                }
            }
        }
        for (int j = 0; j < 3; j++){
            AAR_avg[j] = AAR_avg[j]/40.0;
            CAAR_avg[j] = CAAR_avg[j]/40.0;
            AAR_std[j] = AAR_std[j]/40.0;
            AAR_std[j] = AAR_std[j] - AAR_avg[j]*AAR_avg[j];
            CAAR_std[j] = CAAR_std[j]/40.0;
            CAAR_std[j] = CAAR_std[j] - CAAR_avg[j]*CAAR_avg[j];
        }
    }
}