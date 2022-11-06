#include "itemset.hpp"

std::vector<std::unique_ptr <Itemset>>* candidate_generation( std::vector<std::unique_ptr<Itemset>>& frequent_itemsets )
{
    std::vector<std::unique_ptr <Itemset>>& candidate_itemsets = *( new std::vector<std::unique_ptr<Itemset>>() );
    for ( int i = 0;i < int(frequent_itemsets.size()) - 1;i++ )
    {
        for ( int j = i + 1;j < frequent_itemsets.size();j++ )
        {
            if ( areJoinable( *frequent_itemsets[i], *frequent_itemsets[j] ) )
            {
                std::unique_ptr<Itemset> candidate = join( *frequent_itemsets[i], *frequent_itemsets[j] );

                bool is_candidate = true;

                for ( auto it = candidate->getBegin();it != candidate->getEnd();it++ )
                {
                    std::unique_ptr<Itemset> subset( new Itemset( candidate->getSize() - 1 ) );
                    for ( std::vector<int>::iterator iter1 = subset->getBegin(), iter2 = candidate->getBegin(); iter1 != subset->getEnd(); iter2++ )
                    {
                        if ( ( *iter2 ) != ( *it ) )
                        {
                            *iter1 = *iter2;
                            iter1++;
                        }
                    }
                    subset->setLastElem( candidate->getLastElem() );
                    int val = *it;

                    if ( find( frequent_itemsets.begin(), frequent_itemsets.end(), std::move( subset ) ) == frequent_itemsets.end() )
                    {
                        is_candidate = false;
                        break;
                    }
                }
                if ( is_candidate )
                {
                    candidate_itemsets.push_back( std::move( candidate ) );
                }
            }
        }
    }

    return &candidate_itemsets;
}
