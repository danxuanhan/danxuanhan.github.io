//
//  OperatorOverloading.hpp
//  SimpleCurlMac
//
//  Created by apple on 2018/12/13.
//  Copyright © 2018 Song Tang. All rights reserved.
//

#ifndef OperatorOverloading_hpp
#define OperatorOverloading_hpp

#include <stdio.h>
#include <iostream>
#include <vector>
using namespace std;

typedef vector<vector<double>> Vector;
typedef vector<Vector> matrix;

vector<double> operator+(const vector<double> &vector1, const vector<double> &vector2);

vector<double> operator/(const vector<double>& vector1, const double& a);

vector<double> operator*(const double& a, const vector<double>& vector1);


Vector operator+(const Vector& vec1, const Vector& vec2);

Vector operator*(const double& a, const Vector& vec2);
#endif /* OperatorOverloading_hpp */
