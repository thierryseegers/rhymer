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
    auto const rhymes = r.rhymes(word, false);
    
    map<int, vector<string>, greater<>> matches;
    for(auto const& rhyme : rhymes)
    {
        auto const p = r.pronunciation(rhyme);
        auto const score = distance(p.rbegin(), mismatch(p.rbegin(), p.rend(), pronunciation.rbegin(), pronunciation.rend(), [](rhymer::phoneme const& a, rhymer::phoneme const& b)
                                                         {
                                                             return a[0] == b[0] && a[1] == b[1];
                                                         }).first);
        
        matches[score].push_back(rhyme);
    }
    
    cout << word << '\t';
    copy(pronunciation.begin(), pronunciation.end(), ostream_iterator<rhymer::phoneme>(cout, " "));
    cout << endl;
    
    for(auto const match : matches)
    {
        for(auto const rhyme : match.second)
        {
            cout << '[' << match.first << ']' << ' ' << rhyme << '\t';
        
            auto const p = r.pronunciation(rhyme);
            copy(p.begin(), p.end(), ostream_iterator<rhymer::phoneme>(cout, " "));

            cout << endl;
        }
    }

    cout << rhymes.size() << endl;
    
    return 0;
}