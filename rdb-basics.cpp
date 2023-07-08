#include "rdb.h"
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

    
vector<Attr *>& Record::getAttr()
{
    return attrptr;
}
void Record:: addAttr(Attr *attr)
{
    attrptr.push_back(attr);
}

int Relation::getnattr() { return nattr; }
int Relation::getnrecs() { return nrecs; }
string Relation::getAttrName(int in) { return attrnames[in]; }
bool Relation::isAttrPresent(Relation *R, string name)
{
    for (auto it : R->attrnames)
    {
        if (it == name)
        {
            return 1;
        }
    }
    return 0;
}
bool Relation::checkAllAttr(Relation *R1, Relation *R2)
{
    if (R1->nattr != R2->nattr)
    {
        return 0;
    }
    int ch = 0;
    for (auto it1 : R1->attrnames)
    {
        for (auto it2 : R2->attrnames)
        {
            if (it1 == it2)
            {
                ch++;
            }
        }
    }
    if (ch == R1->nattr)
    {
        return 1;
    }
    return 0;
}
bool Relation::checkOneAttr(Relation *R1, Relation *R2)
{
    for (auto it1 : R1->attrnames)
    {
        for (auto it2 : R2->attrnames)
        {
            if (it1 == it2)
            {
                return 0;
            }
        }
    }
    return 1;
}
void Relation::removeDupRecs()
{
    if (nrecs > 1)
    {
        for (auto it1 = recs.begin(); it1 != recs.end(); ++it1)
        {
            for (auto it2 = next(it1); it2 != recs.end();)
            {
                int c = 0;
                vector<Attr *> v1 = (*it1)->getAttr();
                vector<Attr *> v2 = (*it2)->getAttr();
                for (int i = 0; i < v1.size(); i++)
                {
                    if (*v1[i] != *v2[i])
                    {
                        c++;
                        break;
                    }
                }
                if (!c)
                {
                    delete *it2;
                    it2 = recs.erase(it2);
                }
                else
                {
                    ++it2;
                }
            }
        }
    }
    nrecs = recs.size();
}
void Relation::makeRel(Relation *R, int &ch1)
{
    cout << "Enter the attribute types and names for the relation:" << endl;
    for (int i = 0; i < R->nattr; i++)
    {
        int c;
        cout << "Enter attribute type(Enter 0 for int, 1 for string, 2 for double): ";
        cin >> c;
        if (c > 2 || c < 0)
        {
            cout << "Invalid entry" << endl;
            ch1++;
            break;
        }
        else
        {
            string s;
            cout << "Enter attribute name: ";
            cin >> s;
            if (!R->isAttrPresent(R, s))
            {
                R->attrnames.push_back(s);
                R->attrinds.push_back(c);
            }
            else
            {
                cout << "Attribute already present. Try again" << endl;
                i--;
            }
        }
    }
}
void Relation::addRecord(Relation *R)
{
    int i1 = 0;
    vector<Attr *> a;
    Record *newR = new Record(a);
    for (auto nm : R->attrinds)
    {
        cout << "Enter the data for attribute " << R->attrnames[i1] << ": ";
        if (nm == 0)
        {
            int i;
            cin >> i;
            newR->addAttr(new IntegerAttribute(i));
        }
        else if (nm == 1)
        {
            string i;
            cin >> i;
            newR->addAttr(new StringAttribute(i));
        }
        else
        {
            double i;
            cin >> i;
            newR->addAttr(new DoubleAttribute(i));
        }
        i1++;
    }
    R->push_record(newR);
    R->removeDupRecs();
    cout << "Record added successfully" << endl;
}
void Relation::printRel()
{
    for (auto attr : attrnames)
        cout << attr << " ";
    cout << endl;
    for (auto rec : recs)
    {
        for (auto attr : rec->getAttr())
        {
            if (typeid(*attr) == typeid(IntegerAttribute))
                cout << static_cast<IntegerAttribute *>(attr)->getData() << " ";
            else if (typeid(*attr) == typeid(StringAttribute))
                cout << static_cast<StringAttribute *>(attr)->getData() << " ";
            else if (typeid(*attr) == typeid(DoubleAttribute))
                cout << static_cast<DoubleAttribute *>(attr)->getData() << " ";
        }
        cout << endl;
    }
}
void Relation::printAttr()
{
    int i = 0;
    for (auto attr : attrnames)
    {
        cout << i << ". " << attr << endl;
        i++;
    }
    cout << endl;
}
void Relation::push_record(Record *rec)
{
    recs.push_back(rec);
    nrecs++;
}
Relation* Relation::difference(Relation *R1, Relation *R2)
{
    Relation *newrel = new Relation(R1->nattr);
    newrel->attrnames = R1->attrnames;
    newrel->attrinds = R1->attrinds;
    for (auto rec : R1->recs)
    {
        bool found = true;
        for (auto origrec : R2->recs)
        {
            bool match = true;
            for (int i = 0; i < R1->nattr; i++)
            {
                vector<Attr *> oa, na;
                oa = rec->getAttr();
                na = origrec->getAttr();
                Attr *origattr = oa[i];
                Attr *newattr = na[i];
                if (!(*origattr == *newattr))
                {
                    match = false;
                    break;
                }
            }
            if (match)
            {
                found = false;
                break;
            }
        }
        if (found)
        {
            newrel->push_record(rec);
        }
    }
    return newrel;
}
Relation* Relation::projection(Relation *R1, list<string> projectattrs)
{
    // create new relation with a subset of columns
    Relation *newrel = new Relation(projectattrs.size());
    vector <int> idx;
    for (auto it : projectattrs)
    {
        newrel->attrnames.push_back(it);
        auto attridx = find(R1->attrnames.begin(), R1->attrnames.end(), it) - R1->attrnames.begin();
        newrel->attrinds.push_back(R1->attrinds[attridx]);
        idx.push_back(attridx);
    }
    for (auto rec : R1->recs)
    {
        vector<Attr *> v1 = rec->getAttr();
        vector<Attr *> a;
        Record *newrec = new Record(a);
        for (auto idx1 : idx)
        {
            newrec->addAttr(v1[idx1]);
        }
        newrel->push_record(newrec);
    }
    newrel->removeDupRecs();
    return newrel;
}
Relation* Relation::union_op(Relation *R1, DNFformula *f)
{
    Relation *result = new Relation(R1->nattr);
    for (int i = 0; i < R1->nattr; i++)
    {
        result->attrnames.push_back(R1->attrnames[i]);
        result->attrinds.push_back(R1->attrinds[i]);
    }
    for (auto it : R1->recs)
    {
        vector<Attr *> v = it->getAttr();
        int ch1 = 0;
        for (auto it1 : f->ops)
        {
            int ch = 0;
            for (auto it2 : it1)
            {
                string s;
                char c;
                Attr *a;
                tie(s, c, a) = it2;
                for (int i = 0; i < R1->nattr; i++)
                {
                    if (R1->attrnames[i] == s)
                    {
                        if (c == '0' && *v[i] != *a)
                        {
                            ch++;
                            break;
                        }
                        else if (c == '1' && *v[i] == *a)
                        {
                            ch++;
                            break;
                        }
                        else if (c == '2' && *v[i] >= *a)
                        {
                            ch++;
                            break;
                        }
                        else if (c == '3' && *v[i] > *a)
                        {
                            ch++;
                            break;
                        }
                        else if (c == '4' && *v[i] <= *a)
                        {
                            ch++;
                            break;
                        }
                        else if (c == '5' && *v[i] < *a)
                        {
                            ch++;
                            break;
                        }
                    }
                }
                if (ch)
                    break; // if any of the boolean condition tuple in a list is violated.
            }
            if (!ch)
            { // if all the tuples in any of the boolean condition tuple list is satisfied by the record.
                ch1++;
                break;
            }
        }
        if (ch1)
            result->push_record(it);
    }
    result->removeDupRecs();
    return result;
}
Relation* Relation::cartesianproduct(Relation *R1, Relation *R2)
{
    Relation *result = new Relation(R1->nattr + R2->nattr);
    for (int i = 0; i < R1->nattr; i++)
    {
        result->attrnames.push_back(R1->attrnames[i]);
        result->attrinds.push_back(R1->attrinds[i]);
    }
    for (int i = 0; i < R2->nattr; i++)
    {
        result->attrnames.push_back(R2->attrnames[i]);
        result->attrinds.push_back(R2->attrinds[i]);
    }
    for (auto rec1 : R1->recs)
    {
        vector<Attr *> v = rec1->getAttr();
        for (auto rec2 : R2->recs)
        {
            vector<Attr *> v1 = rec2->getAttr();
            Record *newrec = new Record(vector<Attr *>());
            for (int i = 0; i < R1->nattr; i++)
            {
                newrec->addAttr(v[i]);
            }
            for (int i = 0; i < R2->nattr; i++)
            {
                newrec->addAttr(v1[i]);
            }
            result->push_record(newrec);
        }
    }
    result->removeDupRecs();
    return result;
}
Relation* Relation::difference(Relation *R1, string s1, string s2) // Operation to rename an attribute in schema
{
    for (int i = 0; i < R1->nattr; i++)
    {
        if (s1 == R1->attrnames[i])
        {
            R1->attrnames[i] = s2;
            break;
        }
    }
    return R1;
}
Relation* Relation::union_op(Relation *R1, Relation *R2)
{
    Relation *result = new Relation(R1->nattr);
    result->attrnames = R1->attrnames;
    result->attrinds = R1->attrinds;
    for (auto rec : R1->recs)
    {
        result->push_record(rec);
    }
    for (auto rec : R2->recs)
    {
        vector<Attr *> v = rec->getAttr();
        bool found = false;
        for (auto origrec : R1->recs)
        {
            vector<Attr *> v1 = origrec->getAttr();
            bool match = true;
            for (int i = 0; i < R1->nattr; i++)
            {
                Attr *origattr = v1[i];
                Attr *newattr = v[i];
                if (!(*origattr == *newattr))
                {
                    match = false;
                    break;
                }
            }
            if (match)
            {
                found = true;
                break;
            }
        }
        if (!found)
        {
            result->push_record(rec);
        }
    }
    return result;
}

