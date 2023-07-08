#include "rdb.h"
#include<iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <list>
#include <tuple>
#include <iterator>
#include <algorithm>
#include <typeinfo>

using namespace std;

bool IntegerAttribute ::operator==(const Attr &right){
    return data == static_cast<const IntegerAttribute &>(right).data;
}
bool IntegerAttribute ::operator!=(const Attr &right) {
    return data != static_cast<const IntegerAttribute &>(right).data; 
}
bool IntegerAttribute ::operator<(const Attr &right) {
    return data < static_cast<const IntegerAttribute &>(right).data;
}
bool IntegerAttribute ::operator<=(const Attr &right) {
    return data <= static_cast<const IntegerAttribute &>(right).data;
}
bool IntegerAttribute ::operator>(const Attr &right) {
    return data > static_cast<const IntegerAttribute &>(right).data;
}
bool IntegerAttribute ::operator>=(const Attr &right) {
    return data >= static_cast<const IntegerAttribute &>(right).data;
}
int IntegerAttribute ::getData() { return data; }
void IntegerAttribute ::setData(int a) { data = a; }

bool StringAttribute ::operator==(const Attr &right){
    return data == static_cast<const StringAttribute &>(right).data;
}
bool StringAttribute ::operator!=(const Attr &right) {
    return data != static_cast<const StringAttribute &>(right).data; 
}
bool StringAttribute ::operator<(const Attr &right) {
    return data < static_cast<const StringAttribute &>(right).data;
}
bool StringAttribute ::operator<=(const Attr &right) {
    return data <= static_cast<const StringAttribute &>(right).data;
}
bool StringAttribute ::operator>(const Attr &right) {
    return data > static_cast<const StringAttribute &>(right).data;
}
bool StringAttribute ::operator>=(const Attr &right) {
    return data >= static_cast<const StringAttribute &>(right).data;
}
string StringAttribute ::getData() { return data; }
void StringAttribute ::setData(string a) { data = a; }

bool DoubleAttribute ::operator==(const Attr &right){
    return data == static_cast<const DoubleAttribute &>(right).data;
}
bool DoubleAttribute ::operator!=(const Attr &right) {
    return data != static_cast<const DoubleAttribute &>(right).data; 
}
bool DoubleAttribute ::operator<(const Attr &right) {
    return data < static_cast<const DoubleAttribute &>(right).data;
}
bool DoubleAttribute ::operator<=(const Attr &right) {
    return data <= static_cast<const DoubleAttribute &>(right).data;
}
bool DoubleAttribute ::operator>(const Attr &right) {
    return data > static_cast<const DoubleAttribute &>(right).data;
}
bool DoubleAttribute ::operator>=(const Attr &right) {
    return data >= static_cast<const DoubleAttribute &>(right).data;
}
double DoubleAttribute ::getData() { return data; }
void DoubleAttribute ::setData(double a) { data = a; }
