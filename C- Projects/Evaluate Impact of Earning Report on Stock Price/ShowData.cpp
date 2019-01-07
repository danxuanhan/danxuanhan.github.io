//
//  ShowMatrix2Excel.cpp
//  SimpleCurlMac
//
//  Created by apple on 2018/12/14.
//  Copyright © 2018 Song Tang. All rights reserved.
//

#include "ShowData.hpp"


void PrintToExcel(vector<vector<vector <double>>> OutVector)
{
    ofstream outData;
    outData.open("outfile.csv", ios::app);
    for (int i = -119; i < 121; i++)
    {
        outData << "," << i;
    }
    outData << '\n';
    int beat = 0, meet = 1, miss = 2, AAR = 0, CAAR = 1;
    outData << "Beat AAR" << ",";
    for (int i = 0; i < OutVector[beat][AAR].size(); i++)
    {
        outData << OutVector[beat][AAR][i] << ",";
    }
    outData << '\n';
    outData << "Beat CAAR" << ",";
    for (int i = 0; i < OutVector[beat][CAAR].size(); i++)
    {
        outData << OutVector[beat][CAAR][i] << ",";
    }
    outData << '\n';
    outData << "Meet AAR" << ",";
    for (int i = 0; i < OutVector[meet][AAR].size(); i++)
    {
        outData << OutVector[meet][AAR][i] << ",";
    }
    outData << '\n';
    outData << "Meet CAAR" << ",";
    for (int i = 0; i < OutVector[meet][CAAR].size(); i++)
    {
        outData << OutVector[meet][CAAR][i] << ",";
    }
    outData << '\n';
    outData << "Miss AAR" << ",";
    for (int i = 0; i < OutVector[miss][AAR].size(); i++)
    {
        outData << OutVector[miss][AAR][i] << ",";
    }
    outData << '\n';
    outData << "Miss CAAR" << ",";
    for (int i = 0; i < OutVector[miss][CAAR].size(); i++)
    {
        outData << OutVector[miss][CAAR][i] << ",";
    }
}

void Print(vector<double> vec)
{
    int v_size = vec.size();
    for (int i=0; i<v_size; i++)
    {
        cout << setiosflags(ios::fixed) << setprecision(8);
        cout << vec[i] << "\n";
    }
}
