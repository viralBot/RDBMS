#include "rdb.h"
#include "rdb-basics.cpp"
#include "rdb-attr.cpp"
#include "rdb-join.cpp"
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

vector<Relation *> rels;

void printRelations()
{
    int i = 1;
    for (auto it : rels)
    {
        cout << i << "." << endl;
        it->printRel();
        i++;
        cout << endl;
    }
    cout << endl;
}

void printMenu()
{
    cout << "Choose which you action you wish to perform:-" << endl
         << "1. Create a new empty table and enter its schema(Enter 1)" << endl
         << "2. Delete an existing table with all data in it" << endl
         << "3. Add a record to a table(Enter 3)" << endl
         << "4. Print a table in a appropriate format(Enter 4)" << endl
         << "5. Create a table from existing table using various operations(Enter 5)" << endl
         << "6. Exit the console(Enter 6)" << endl;
}

int main()
{
    int ch = 0;
    while (ch != 6)
    {
        printMenu();
        cin >> ch;
        if (ch == 1)
        {
            cout << "Enter number of attributes in the relation: ";
            int n, ch1 = 0;
            cin >> n;
            Relation *newT = new Relation(n);
            newT->makeRel(newT, ch1);
            if (!ch1)
            {
                rels.push_back(newT);
            }
        }
        else if (ch == 2)
        {
            int ind;
            cout << "Select the index of the table from the following list of tables which you want to delete:" << endl;
            printRelations();
            cin >> ind;
            if (ind > rels.size() || ind <= 0)
            {
                cout << "Invalid index. Table doesn't exist" << endl;
            }
            else
            {
                rels.erase(rels.begin() + ind - 1);
            }
        }
        else if (ch == 3)
        {
            int ind;
            cout << "Select the index of the table from the following list of tables to which you want to add a record:" << endl;
            printRelations();
            cin >> ind;
            if (ind > rels.size() || ind <= 0)
            {
                cout << "Invalid index. Table doesn't exist" << endl;
            }
            else
            {
                rels[ind - 1]->addRecord(rels[ind - 1]);
            }
        }
        else if (ch == 4)
        {
            int ind;
            cout << "Enter the index of the table which you want to print:";
            cin >> ind;
            if (ind > rels.size() || ind <= 0)
            {
                cout << "Invalid index. Table doesn't exist" << endl;
            }
            else
            {
                rels[ind - 1]->printRel();
            }
        }
        else if (ch == 5)
        {
            int ind1, ind2, op, fs = 0;
            cout << "Choose which operation you want to perform(Enter 1 for union_op of 2 relations, 2 for difference of 2 relations, "
                 << "3 for cartesian product of 2 relations, 4 for natural join of 2 relations, 5 for projection of a relation, 6 for selection of a relation, 7 for renaming an attribute of a relation): ";
            cin >> op;
            if (op >= 1 && op <= 4)
            {
                cout << "Select the indices of the tables from the following list of tables on which you want to perform the operation on:" << endl;
                printRelations();
                cin >> ind1 >> ind2;
                if (ind1 > rels.size() || ind1 <= 0 || ind2 > rels.size() || ind2 <= 0)
                {
                    cout << "Invalid index. Table doesn't exist" << endl;
                }
                else
                {
                    Relation *newT;
                    if (op == 1)
                    {
                        if ((rels[ind1 - 1]->checkAllAttr(rels[ind1 - 1], rels[ind2 - 1])))
                        {
                            newT = new Relation(rels[ind1 - 1]->getnattr());
                            newT = rels[ind1 - 1]->union_op(rels[ind1 - 1], rels[ind2 - 1]);
                            cout << "Union relation:" << endl;
                            newT->printRel();
                            fs++;
                        }
                        else
                        {
                            cout << "The relations are not complatible for union_op" << endl;
                        }
                    }
                    else if (op == 2)
                    {
                        if ((rels[ind1 - 1]->checkAllAttr(rels[ind1 - 1], rels[ind2 - 1])))
                        {
                            newT = new Relation(rels[ind1 - 1]->getnattr());
                            newT = rels[ind1 - 1]->difference(rels[ind1 - 1], rels[ind2 - 1]);
                            cout << "Difference relation:" << endl;
                            newT->printRel();
                            fs++;
                        }
                        else
                        {
                            cout << "The relations are not complatible for difference" << endl;
                        }
                    }
                    else if (op == 3)
                    {
                        newT = new Relation(rels[ind1 - 1]->getnattr() + rels[ind2 - 1]->getnattr());
                        newT = rels[ind1 - 1]->cartesianproduct(rels[ind1 - 1], rels[ind2 - 1]);
                        cout << "Cartesian Product relation:" << endl;
                        newT->printRel();
                        fs++;
                    }
                    else
                    {
                        list<string> ls;
                        rels[ind1 - 1]->getCommonAttrs(rels[ind1 - 1], rels[ind2 - 1], ls);
                        newT = new Relation(rels[ind1 - 1]->getnattr() + rels[ind2 - 1]->getnattr() - ls.size());
                        newT = rels[ind1 - 1]->naturaljoin(rels[ind1 - 1], rels[ind2 - 1], ls);
                        cout << "Natural Join relation:" << endl;
                        newT->printRel();
                        fs++;
                    }
                    if (fs)
                        rels.push_back(newT);
                }
            }
            else if (op >= 5 && op <= 7)
            {
                cout << "Select the index of the table from the following list of tables which you want to perform the operation on:" << endl;
                printRelations();
                cin >> ind1;
                if (ind1 > rels.size() || ind1 <= 0)
                {
                    cout << "Invalid index. Table doesn't exist" << endl;
                }
                else
                {
                    if (op == 5)
                    {
                        int subs;
                        cout << "Enter the size of subset of columns which you want to print: ";
                        cin >> subs;
                        if (subs > rels[ind1 - 1]->getnattr() || subs < 0)
                        {
                            cout << "Invalid entry" << endl;
                        }
                        else
                        {
                            list<string> projectattrs;
                            while (subs--)
                            {
                                int attin;
                                cout << "Enter the index of the attribute which you want to include:" << endl;
                                rels[ind1 - 1]->printAttr();
                                cin >> attin;
                                if (attin >= rels[ind1 - 1]->getnattr() || attin < 0)
                                {
                                    cout << "Invalid index. Attribute doesn't exist" << endl;
                                    subs++;
                                }
                                else
                                {
                                    projectattrs.push_back(rels[ind1 - 1]->getAttrName(attin));
                                }
                            }
                            Relation *newT = new Relation(projectattrs.size());
                            newT = rels[ind1 - 1]->projection(rels[ind1 - 1], projectattrs);
                            cout << "Projection relation:" << endl;
                            newT->printRel();
                            rels.push_back(newT);
                        }
                    }
                    else if (op == 6)
                    {
                        Relation *newT = new Relation(rels[ind1 - 1]->getnattr());
                        DNFformula f;
                        list<list<tuple<string, char, Attr *>>> ols;
                        int ncl;
                        printf("Enter number of clauses whose disjuction you want to take: ");
                        cin >> ncl;
                        while (ncl--)
                        {
                            list<tuple<string, char, Attr *>> ls;
                            int ncomp;
                            printf("Enter number of comparisons whose conjunction you want to take: ");
                            cin >> ncomp;
                            while (ncomp--)
                            {
                                tuple<string, char, Attr *> tp;
                                int n;
                                string s;
                                char c;
                                Attr *at;
                                cout << "Enter attribute name: ";
                                cin >> s;
                                cout << "Enter attibute type(0 for int, 1 for string, 2 for double): ";
                                cin >> n;
                                cout << "Enter following characters for various comparisons:" << endl;
                                cout << "Enter 0 for ==" << endl;
                                cout << "Enter 1 for !=" << endl;
                                cout << "Enter 2 for <" << endl;
                                cout << "Enter 3 for <=" << endl;
                                cout << "Enter 4 for >" << endl;
                                cout << "Enter 5 for >=" << endl;
                                cin >> c;
                                if (c > '5' || c < '0')
                                {
                                    cout << "Invalid entry. Try again" << endl;
                                    ncomp++;
                                }
                                else
                                {
                                    if (n == 0)
                                    {
                                        int data;
                                        cout << "Enter the data: ";
                                        cin >> data;
                                        at = new IntegerAttribute(data);
                                    }
                                    else if (n == 1)
                                    {
                                        string data;
                                        cout << "Enter the data: ";
                                        cin >> data;
                                        at = new StringAttribute(data);
                                    }
                                    else if (n == 2)
                                    {
                                        double data;
                                        cout << "Enter the data: ";
                                        cin >> data;
                                        at = new DoubleAttribute(data);
                                    }
                                    tp = make_tuple(s, c, at);
                                    ls.push_back(tp);
                                }
                            }
                            ols.push_back(ls);
                        }
                        f.ops = ols;
                        newT = rels[ind1 - 1]->union_op(rels[ind1 - 1], &f);
                        cout << "Selection Relation:" << endl;
                        newT->printRel();
                        rels.push_back(newT);
                    }
                    else
                    {
                        int attin;
                        cout << "Enter the index of the attribute which you want to rename:" << endl;
                        rels[ind1-1]->printAttr();
                        cin >> attin;
                        if (attin >= rels[ind1 - 1]->getnattr() || attin < 0)
                        {
                            cout << "Invalid index. Attribute doesn't exist" << endl;
                        }
                        else
                        {
                            string newN;
                            cout << "Enter the new name of the attribute: ";
                            cin >> newN;
                            rels[ind1 - 1]->difference(rels[ind1 - 1], rels[ind1 - 1]->getAttrName(attin), newN);
                        }
                    }
                }
            }
            else
            {
                cout << "Invalid entry" << endl;
            }
        }
        else if (ch == 6)
        {
            break;
        }
        else
        {
            cout << "Invalid entry. Please try again" << endl;
        }
    }
}