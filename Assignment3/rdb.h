#pragma once
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <list>
#include <tuple>
#include <iterator>
#include <algorithm>
#include <typeinfo>

using namespace std;

class Attr
{
public:
    virtual bool operator==(const Attr &right) = 0;
    virtual bool operator!=(const Attr &right) = 0;
    virtual bool operator<(const Attr &right) = 0;
    virtual bool operator<=(const Attr &right) = 0;
    virtual bool operator>(const Attr &right) = 0;
    virtual bool operator>=(const Attr &right) = 0;
    friend class Record;
    friend class Relation;
};

// Derived classes for different attribute types
class IntegerAttribute : public Attr
{
    int data;

public:
    IntegerAttribute(int v) : data(v) {}
    bool operator==(const Attr &right) ; 
    bool operator!=(const Attr &right) ; 
    bool operator<(const Attr &right) ;
    bool operator<=(const Attr &right) ; 
    bool operator>(const Attr &right) ;
    bool operator>=(const Attr &right) ; 
    int getData();
    void setData(int a);
};

class StringAttribute : public Attr
{
    string data;

public:
    StringAttribute(string v) : data(v) {}
    bool operator==(const Attr &right) ;
    bool operator!=(const Attr &right) ;
    bool operator<(const Attr &right) ;
    bool operator<=(const Attr &right) ;
    bool operator>(const Attr &right) ;
    bool operator>=(const Attr &right) ;
    string getData();
    void setData(string a);
};

class DoubleAttribute : public Attr
{
    double data;

public:
    DoubleAttribute(double v) : data(v) {}
    bool operator==(const Attr &right) ;
    bool operator!=(const Attr &right) ;
    bool operator<(const Attr &right) ;
    bool operator<=(const Attr &right) ;
    bool operator>(const Attr &right) ;
    bool operator>=(const Attr &right) ;
    double getData();
    void setData(double a);
};

class Record
{
    vector<Attr *> attrptr;

public:
    Record(vector<Attr *> aptr) : attrptr(aptr) {}
    vector<Attr *> &getAttr();
    void addAttr(Attr *attr);
    ~Record()
    {
        for (auto ptr : attrptr)
            delete ptr;
    }
    friend class Relation;
};

typedef struct dnf
{
    list<list<tuple<string, char, Attr *>>> ops;
} DNFformula;

class Relation
{
    int nattr, nrecs;
    vector<string> attrnames; // schema
    vector<int> attrinds;     // mapping schema to indices
    list<Record *> recs;      // list of records
    public:
    Relation(int a) : nattr(a), nrecs(0)
    {
        vector<string> nm;
        vector<int> in;
        list<Record *> ls;
        attrnames = nm;
        attrinds = in;
        recs = ls;
    }
    ~Relation()
    {
        for (auto ptr : recs)
            delete ptr;
    }
    int getnattr() ;
    int getnrecs() ;
    string getAttrName(int in);
    bool isAttrPresent(Relation *R, string name);
    bool checkAllAttr(Relation *R1, Relation *R2);
    bool checkOneAttr(Relation *R1, Relation *R2);
    void removeDupRecs();
    void makeRel(Relation *R, int &ch1);
    void addRecord(Relation *R);
    void printRel();
    void printAttr();
    void push_record(Record *rec);
    Relation *difference(Relation *R1, Relation *R2);
    Relation *projection(Relation *R1, list<string> projectattrs);
    Relation *union_op(Relation *R1, DNFformula *f);
    Relation *cartesianproduct(Relation *R1, Relation *R2);
    Relation *difference(Relation *R1, string s1, string s2); // Operation to rename an attribute in schema{}
    Relation *union_op(Relation *R1, Relation *R2);
    void getCommonAttrs(Relation *R1, Relation *R2, list<string> &joinattr);
    Relation *naturaljoin(Relation *R1, Relation *R2, list<string> &joinattr);
};

extern vector<Relation *> rels;
extern void printRelations();
void printMenu();