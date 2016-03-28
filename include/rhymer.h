/*
	(C) Copyright Thierry Seegers 2015. Distributed under the following license:
 
	Boost Software License - Version 1.0 - August 17th, 2003
 
	Permission is hereby granted, free of charge, to any person or organization
	obtaining a copy of the software and accompanying documentation covered by
	this license (the "Software") to use, reproduce, display, distribute,
	execute, and transmit the Software, and to prepare derivative works of the
	Software, and to permit third-parties to whom the Software is furnished to
	do so, all subject to the following:
 
	The copyright notices in the Software and this entire statement, including
	the above license grant, this restriction and the following disclaimer,
	must be included in all copies of the Software, in whole or in part, and
	all derivative works of the Software, unless such copies or derivative
	works are solely in the form of machine-executable object code generated by
	a source language processor.
 
	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
	SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
	FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
	ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
	DEALINGS IN THE SOFTWARE.
 */

#pragma once

#include <algorithm>
#include <array>
#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

namespace std
{
    istream& operator>>(istream&, array<char, 2>&);
    
    ostream& operator<<(ostream&, array<char, 2> const&);
}

class rhymer
{
public:
    using phoneme = std::array<char, 2>;
    using phonemes = std::vector<phoneme>;
    
    rhymer(std::ifstream&& phoneme_dictionary, std::ifstream&& phonemes_description)
    {
        // Build trie to map phonemes to words.
        std::string line, word;
        std::stringstream ss;
        phonemes pronunciation;
        
        while(getline(phoneme_dictionary, line))
        {
            ss.clear();
            ss.str(line);
            
            ss >> word;
            transform(word.begin(), word.end(), word.begin(), ::tolower);
            pronunciation.assign(std::istream_iterator<phoneme>(ss), std::istream_iterator<phoneme>());

            dictionary_[word] = pronunciation;

            reverse(pronunciation.begin(), pronunciation.end());
            lookup_.insert(pronunciation)->words_.push_back(word);
        }

        // Save which phonemes represent vowels.
        phoneme p;
        std::string description;
        
        while(phonemes_description)
        {
            phonemes_description >> p;
            phonemes_description >> description;
            
            if(description == "vowel")
            {
                vowels_.push_back(p);
            }
        }
    }
    
    std::vector<std::string> rhymes(std::string const& word) const
    {
        std::vector<std::string> matches;
        
        auto const i = dictionary_.find(word);
        if(i != dictionary_.end())
        {
            phonemes const pronunciation = i->second;
            
            std::function<void (trie const&, phonemes::const_reverse_iterator, phonemes::const_reverse_iterator)> traverse;
            
            traverse = [&](trie const& t, phonemes::const_reverse_iterator b, phonemes::const_reverse_iterator e)
            {
                // Recursive function to copy all words from a trie, starting with its children.
                std::function<void (trie const&)> f;
                f = [&](trie const& s)
                {
                    for(auto const& p : s.children_)
                    {
                        f(p.second);
                    }
                    
                    matches.insert(matches.end(), s.words_.begin(), s.words_.end());
                };
                
                // Append all words from the given trie's children, skipping over the given phoneme.
                auto const m = (b == e) ? t.children_.end() : t.children_.find(*b);

                for(auto const& p : t.children_)
                {
                    if(p.first != m->first)
                    {
                        f(p.second);
                    }
                }
                
                // Append all words from the given trie.
                auto const w = t.words_;
                matches.insert(matches.end(), w.begin(), w.end());
                
                // Recursively call traverse with the given phoneme from the given trie's children.
                if(m != t.children_.end())
                {
                    traverse(m->second, std::next(b), e);
                }
            };
            
            // Get a reverse iterator one past last vowel sound.
            auto const v = std::next(std::find_first_of(pronunciation.crbegin(), pronunciation.crend(), vowels_.cbegin(), vowels_.cend()));
            
            // Navigate the trie from the last phoneme of the pronunciation to that vowel.
            trie const* t = &lookup_;
            for_each(pronunciation.crbegin(), v, [&](phoneme const& p)
                     {
                         t = &t->children_.find(p)->second;
                     });
            
            // Find all words within that trie.
            traverse(*t, v, pronunciation.crend());
        }
        
        return matches;
    }
    
    phonemes pronunciation(std::string const& word) const
    {
        return dictionary_.at(word);
    }
    
private:
    struct trie
    {
    public:
        trie* insert(phoneme const& p)
        {
            return &children_[p];
        }
        
        trie* insert(phonemes const& p)
        {
            trie *t = this;
            
            for(phoneme const& s : p)
            {
                t = t->insert(s);
            };
            
            return t;
        }
        
        std::map<phoneme, trie> children_;
        std::vector<std::string> words_;
    } lookup_;
    
    phonemes vowels_;
    std::map<std::string, phonemes> dictionary_;
};


namespace std
{
    istream& operator>>(istream& i, rhymer::phoneme& p)
    {
        i >> p[0];
        
        if(isalpha(i.peek()))
        {
            i >> p[1];
        }
        else
        {
            p[1] = '\0';
        }
        
        return i;
    }
    
    ostream& operator<<(ostream& o, rhymer::phoneme const& p)
    {
        return o << p[0] << p[1];
    }
}
