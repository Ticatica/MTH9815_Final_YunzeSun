/**
* utility.hpp
* Contain some utility functions:
*   Str2Price: convert string to price value
*   Price2Str: convert price value to string
*   IdGenerator: generate id
*   GetPV01Value: get pv01 value by cusip
* @Yunze Sun
*/
#pragma once
#ifndef utility_h
#define utility_h

#include <iostream>
#include <string>
#include <iomanip>
#include "products.hpp"

using namespace std;

// Str2Price: convert string to price value
double Str2Price(string s) {
    int len = s.size();
    s[len - 1] = (s[len - 1] == '+') ? '4' : s[len - 1];
    // fractional notation transformation
    int a = stoi(s.substr(0, len - 4));
    double b = stoi(s.substr(len - 3, 2)) / 32.;
    double c = stoi(s.substr(len - 1, 1)) / 256.;
    return a + b + c;
}

// Price2Str: convert price value to string
string Price2Str(double price) 
{
    int a = floor(price);
    double b = price - a;
    int xy = floor(b * 32);
    int z = static_cast<int>((b * 256)) % 8;
    // be careful about corner cases
    return to_string(a) + "-" + (xy < 10 ? "0" : "") + to_string(xy) + (z == 4 ? "+" : to_string(z));
}

// IdGenerator: generate id
string IdGenerator(long index, int length)
{
    std::ostringstream ss;
    ss << std::setw(length) << std::setfill('0') << index;
    return ss.str();
}

double GetPV01Value(string _cusip)
{
    double _pv01 = 0;
    if (_cusip == "91282CLY5") _pv01 = 0.01948992;
    if (_cusip == "91282CMB4") _pv01 = 0.02865304;
    if (_cusip == "91282CMA6") _pv01 = 0.04581119;
    if (_cusip == "91282CLZ2") _pv01 = 0.06127718;
    if (_cusip == "91282CLW9") _pv01 = 0.08161449;
    if (_cusip == "912810UF3") _pv01 = 0.11707914;
    if (_cusip == "912810UE6") _pv01 = 0.15013155;
    return _pv01;
}

// get bond by its cusip
Bond GetBond(string _cusip)
{
    if (_cusip == "91282CLY5") return Bond("91282CLY5", CUSIP, "US2Y", 0.04050, from_string("2026/11/30"));
    if (_cusip == "91282CMB4") return Bond("91282CMB4", CUSIP, "US3Y", 0.04090, from_string("2027/11/15"));
    if (_cusip == "91282CMA6") return Bond("91282CMA6", CUSIP, "US5Y", 0.04130, from_string("2029/11/30"));
    if (_cusip == "91282CLZ2") return Bond("91282CLZ2", CUSIP, "US7Y", 0.04138, from_string("2031/11/30"));
    if (_cusip == "91282CLW9") return Bond("91282CLW9", CUSIP, "US10Y", 0.04290, from_string("2034/11/15"));
    if (_cusip == "912810UF3") return Bond("912810UF3", CUSIP, "US20Y", 0.04622, from_string("2044/11/15"));
    if (_cusip == "912810UE6") return Bond("912810UE6", CUSIP, "US30Y", 0.04875, from_string("2054/11/15"));
    return Bond("INVALID", CUSIP, "UNKNOWN", 0.0, from_string("1900/01/01"));
}
#endif