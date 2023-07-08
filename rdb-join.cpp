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

void Relation::getCommonAttrs(Relation *R1, Relation *R2, list<string> &joinattr)
    {
        for (int i = 0; i < R1->nattr; i++)
        {
            for (int j = 0; j < R2->nattr; j++)
            {
                if (R1->attrnames[i] == R2->attrnames[j])
                {
                    joinattr.push_back(R1->attrnames[i]);
                    break;
                }
            }
        }
    }
    Relation* Relation::naturaljoin(Relation *R1, Relation *R2, list<string> &joinattr)
    {
        vector<int> R;
        vector<int> data_types;
        for (int i = 0; i < R1->nattr; ++i)
        {
            for (int j = 0; j < R2->nattr; ++j)
            {
                if (R1->attrnames[i] == R2->attrnames[j])
                {
                    R.push_back(i);
                    data_types.push_back(R1->attrinds[i]);
                    break;
                }
            }
        }

        int count = 0;
        Relation *T = new Relation(R2->nattr);
        T->attrnames = R2->attrnames;
        T->attrinds = R2->attrinds;
        T->recs = R2->recs;
        for (auto const &it : joinattr)
        { // Renaming R2 according to joinattr
            string st = to_string(count);
            T = difference(T, it, st);
            count++;
        }

        Relation *P = cartesianproduct(R1, T);
        P->printRel();
        list<string> temp;
        for (auto const &i : joinattr)
        {
            DNFformula f;
            list<list<tuple<string, char, Attr *>>> oopsie;
            count = 0;
            vector<Attr *> uniqueval;          // Converting column to vector
            temp.push_back(i);                 // Temporary target attribute for projection
            Relation *Q = new Relation(temp.size());
            Q = P->projection(P, temp); // unique vales of the ith attribute
            for (auto const &j : Q->recs)
            {
                vector<Attr *> v = j->getAttr();
                uniqueval.push_back(v[0]);
            }
            for (int k = 0; k < uniqueval.size(); ++k)
            {
                list<tuple<string, char, Attr *>> inner;
                tuple<string, char, Attr *> t1 = make_tuple(i, '=', uniqueval[k]);
                string s = to_string(count);
                tuple<string, char, Attr *> t2 = make_tuple(s, '=', uniqueval[k]);
                inner.push_back(t1);
                inner.push_back(t2);
                oopsie.push_back(inner);
            }
            f.ops = oopsie;
            P = union_op(P, &f);
            temp.clear();
            count++;
        }
        P->removeDupRecs();
        vector<string> intstring; // vector of attribute names after rename
        for (int k = 0; k < joinattr.size(); ++k)
        {
            string s = to_string(k);
            intstring.push_back(s);
        }
        list<string> finalattr;
        for (int k = 0; k < P->nattr; ++k)
        {
            int c = 0;
            for (auto it : intstring)
            {
                if (it == P->attrnames[k])
                {
                    c++;
                    break;
                }
            }
            if (!c)
                finalattr.push_back(P->attrnames[k]);
        }
        P = projection(P, finalattr);
        return P;
    }