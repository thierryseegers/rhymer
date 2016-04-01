#include "rhymer.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>

using namespace std;

int main()
{
    rhymer r(ifstream("cmudict-0.7b"), ifstream("cmudict-0.7b.phones"));
    
    string word;
    cin >> word;
    
    auto const pronunciation = r.pronunciation(word);
    auto const rhymes = r.rhymes(word, true);
    
    for(auto const& rhyme : rhymes)
    {
        cout << rhyme << '\t';
        
        auto const p = r.pronunciation(rhyme);
        copy(p.begin(), p.end(), ostream_iterator<rhymer::phoneme>(cout, " "));
        
        auto const score = distance(p.rbegin(), mismatch(p.rbegin(), p.rend(), pronunciation.rbegin(), pronunciation.rend()).first);
        cout << '[' << score << ']' << endl;
    }
    
    cout << rhymes.size() << endl;
    
    return 0;
}