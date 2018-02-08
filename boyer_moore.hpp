//
// Created by thomas on 06/02/18.
//

#ifndef UTILITIES_BOYER_MOORE_HPP
#define UTILITIES_BOYER_MOORE_HPP

#include <type_traits>
#include <algorithm>
#include <functional>
#include <map>
#include <vector>


template<typename Iter>
class BoyerMoore
{
    public:
        // Setup the data required
        BoyerMoore(Iter first, Iter last);

        // Apply the algorithm / search in a sequence
        template<typename Iter2>
        Iter2 operator()(Iter2 first, Iter2 last) const;
    
    private:
        template<typename Iter2>
        Iter2 next_valid_pos(Iter2 first, Iter2 last, Iter2 s_first, Iter2 s_last) const;
    
        Iter first_;
        Iter last_;
        std::map<typename Iter::value_type, size_t> first_table;
        std::vector<size_t> second_table;
};


template<typename Iter>
BoyerMoore<Iter>::BoyerMoore(Iter first, Iter last) :
    first_{first},
    last_{last},
    second_table(last_ - first_)
{
    static_assert(std::is_same_v<
                      typename  std::iterator_traits<Iter>::iterator_category,
                      std::random_access_iterator_tag>, 
                  "Only random access iterator are supported");
    
    if(first_ == last_)
        return;
    
    auto it = last_ - 2;
    size_t i = 1;
    
    while(it >= first_)
    {
        if(first_table.find(*it) == first_table.end())
            first_table.insert({*it, i});
        ++i;
        --it;
    }

    auto rfirst = std::reverse_iterator(last_);
    auto rlast = std::reverse_iterator(first_);
    
    second_table[0] = 1;
    for(auto rit = rfirst + 1; rit != rlast; ++rit)
    {
        auto pos = next_valid_pos(rfirst + 1, rlast, rfirst, rit);
        second_table[rit-rfirst] = pos - rfirst;
    }
}


template<typename Iter>
template<typename Iter2>
Iter2 BoyerMoore<Iter>::next_valid_pos(Iter2 first, Iter2 last, Iter2 s_first, Iter2 s_last) const
{
    for (; ; ++first) {
        auto it = first;
        for (auto s_it = s_first; ; ++it, ++s_it) {
            if (it == last) {
                return last - (s_it - s_first);
            }
            if (s_it == s_last) {
                if(!(*(s_it+1) == *(it+1)))
                    break;
                return first;
            }
            if (!(*it == *s_it)) {
                break;
            }
        }
    }
}


template<typename Iter>
template<typename Iter2>
Iter2 BoyerMoore<Iter>::operator()(Iter2 first, Iter2 last) const
{  
    static_assert(std::is_same_v<
                     typename  std::iterator_traits<Iter2>::iterator_category,
                     std::random_access_iterator_tag>,
                 "Only random access iterator are supported");
    
    const auto length = last_-first_;
    
    while(first < last - length)
    {
        // First element mismatch
        auto first_elem = *(first + length - 1);
        if(!(*(last_-1) == first_elem))
        {
            auto pos = first_table.find(first_elem);
            first += (pos == first_table.end() ? length : first_table.at(first_elem));
            continue;
        }
        auto s_it = last_-2;
        for(auto it = first + length - 2; ; --s_it, --it)
        {
            // A mismatch is found
            if(!(*it == *s_it))
                break;
            // A complete match is found
            if(s_it == first_)
                return first;
        }
        
        first += second_table[last_-1 - s_it];
    }
    
    return last;
}

#endif //UTILITIES_BOYER_MOORE_HPP
