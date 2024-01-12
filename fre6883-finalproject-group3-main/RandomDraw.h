#pragma once
#include <iostream>
#include <vector>
using namespace std;

class RandomDraw{
    private:
        int N; //total numbers in the sample
        int n; //t=numbers to draw
        vector<int> result;
    public:
        
        //constructors
        RandomDraw():N(3000),n(30){
            srand(999);
            result.resize(30);
        }
        RandomDraw(int N_, int n_):N(N_),n(n_){
            srand(999);
            result.resize(n_);
        }
        void setN(int N_){
            N = N_;
        }
        void setn(int n_){
            n = n_;
            result.resize(n_);
        }
        // get random draw result
        vector<int> getResult(){
            int chosen[N+1];
            for (int a = 0; a<N+1; a++){
                chosen[a] = 0;
            }
            int i = 0;
            while (i < n){
                int c = (int)rand() % N;
                if (chosen[c] == 0){
                    chosen[c] = 1;
                    result[i] = c;
                    i++;
                }
            }
            return result;
        }
};