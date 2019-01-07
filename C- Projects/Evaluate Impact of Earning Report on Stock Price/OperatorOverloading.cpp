//
//  OperatorOverloading.cpp
//  SimpleCurlMac
//
//  Created by apple on 2018/12/13.
//  Copyright © 2018 Song Tang. All rights reserved.
//

#include "OperatorOverloading.hpp"


vector<double> operator+(const vector<double> &vector1, const vector<double> &vector2)
{
    int size = vector1.size();
    vector<double> return_vec;
    for (int i = 0; i < size; i++) {
        return_vec.push_back(vector1[i] + vector2[i]);
    }
    return return_vec;
}


vector<double> operator/(const vector<double>& vector1, const double& a)
{
    int size = vector1.size();
    vector<double> return_vec;
    for (int i = 0; i < size; i++)
    {
        return_vec.push_back(vector1[i]/a);
    }
    return return_vec;
}

vector<double> operator*(const double& a, const vector<double>& vector1)
{
    int v_size = vector1.size();
    vector<double> return_vec;
    for (int i = 0; i < v_size; i++)
    {
        return_vec.push_back(a* vector1[i]);
    }
    return return_vec;
}


Vector operator+(const Vector& vec1, const Vector& vec2)
{
    int v_size_1 = vec1.size();
    Vector result;
    for (int i=0; i<v_size_1; i++){
        vector<double> vec = vec1[i] + vec2[i];
        result.push_back(vec);
    }
    return result;
}


Vector operator*(const double& a, const Vector& vec2)
{
    int v_size_1 = vec2.size();
    Vector result;
    for (int i=0; i<v_size_1; i++){
        vector<double> vec = a*vec2[i];
        result.push_back(vec);
    }
    return result;
}


