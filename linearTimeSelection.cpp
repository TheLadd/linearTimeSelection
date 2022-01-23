#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

template <class T>
T rankByOrder(vector<T> a, unsigned int k) {
    // Assuming that k is not zero-indexed
    cerr << "Call to rankByOrder()\n";
    if (a.size() < k) {    // No pls.
        cerr << "Something went wrong in rankByOrder, ";
        cerr << "looking for the " << k << "-th smallest element in a vector of size " << a.size() << endl;
        exit(EXIT_FAILURE);
    }
    sort(a.begin(), a.end());
    return a.at(k-1);
}

template <class T>
int getIndex(const vector<T> &a, T query) {
    // Returns the 0-index of 'query', -1 if not present
    for (size_t i = 0; i < a.size(); i++)
        if (a.at(i) == query)
            return i; 
    return -1;
}

template <class T>
pair<vector<T>, vector<T>> partition(vector<T> &a, T pivot) { 
    unsigned int index = getIndex(a, pivot);
    vector<T> L, R;
    for (size_t i = 0; i < a.size(); i++) {
        if (i == index) continue;
        if (a.at(i) <= pivot) // Should this be <?
            L.push_back(a.at(i));
        else
            R.push_back(a.at(i));
    }
    L.push_back(pivot);

    auto retval = pair<vector<T>, vector<T>>(L, R);
    return retval; 
} 

template <class T>
T linearTimeSelection(vector<T> a, unsigned int k) {
    //cerr << "New recursion looking for " << k << "-th smallest element\n";
    if (a.size() <= 5)  
        return rankByOrder(a, k);

    // 1. Seperate into n/5 groups (and 1 more if n%5 != 0) 
    // and find medians of those groups
    vector<T> medians;
    int numberOfGroups = a.size()/5; if (a.size() % 5) numberOfGroups++;   // To get into groups of 5 or fewer
    for (size_t i = 0; i < numberOfGroups; i++) {
        auto begin = a.begin()+(5*i);
        vector<T> five(begin, begin+5); //TODO this might only work if |a| = 5

        cerr << "Group "  << i << ":\n";
        for (T x : five)
            cerr << x << "\t";
        cerr << "\n";

        // Store the median of this group of 5
        medians.push_back(rankByOrder(five, (five.size()+1)/2));
    }

   cerr << "Medians:\n";
    for (T x : medians)
        cerr << x << endl;
    cerr << endl;

    T median = linearTimeSelection(medians, (medians.size()+1)/2);

    // 2. Parition around the median of medians
    //cerr << "Parition elemnt aka median: " << median << "\n";
    pair<vector<T>, vector<T>> partitioned_set = partition(a, median);
    vector<T> L = partitioned_set.first;
    vector<T> R = partitioned_set.second;
    if (L.size() == k) {
        cerr << "k was found at position " << L.size()-1 << "(0-indexed)\n";
        return L.at(L.size()-1);
    }
    if (L.size() > k) {
        cerr << "Left side of " << median << "chosen\n";
        cerr << "K becomes " << L.size() << endl;
        cerr << "L: ";
        for (T x : L)
            cerr << x << ",\t";
        cerr << endl;
        return linearTimeSelection(L, k);
    }
    else {
        cerr << "Right side of " << median << "chosen\n";
        cerr << "K becomes " << k << " - " << L.size() << endl;
        cerr << "R: ";
        for (T x : R)
            cerr << x << ",\t";
        cerr << endl;
        return linearTimeSelection(R, k-L.size());
    }
}

int main () { 
    // Seems to work with both these sets
    vector<int> set = {10, 8, 1, 5, 4, 2, 3, 6, 9, 7};    
    vector<int> set2 = {12, 3, 45, 7, 8, 92, 10000, 0, 4, 22};
    unsigned int k = 0;
    cout << "Which order statistic would like out of an array of size " << set2.size() << "?\n";
    cin >> k;
    
    cout << k << "-th order statistic: " << linearTimeSelection(set2, k) << endl;;
    return 0; 
}
