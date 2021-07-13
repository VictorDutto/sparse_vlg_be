#include "strategy.hh"


//Defines strategy
//basic strategy: take the first valid vertex
//not called
long int starting_ite_point_basic(std::vector<int> got_ecc)
{
    long int res = 0;
    size_t n = got_ecc.size();
    while (res < n && got_ecc[res] == 1)
    {
        res++;
    }
    return res;
}



//Aux function, called in starting_ite_point
//check next chunk
long int starting_ite_point_aux(std::vector<int> got_ecc, std::optional<size_t> avg_pos)
{
    long int res = 0;
    while (got_ecc[res] == 1)
        res++;
    return res;
}




//Defines strategy
//density strategy
//in cases where vertices are linked according to index
//we choose a starting point according to the average position observed
//until call with opt to avoid less relevant cases
long int starting_ite_point(std::vector<int> got_ecc, std::optional<size_t> avg_pos)
{
    long int res = 0;
    if (avg_pos == std::nullopt)
    {
        return starting_ite_point_basic(got_ecc);
    }
    size_t n = got_ecc.size();
    if (avg_pos.value() * 4 > n && n - avg_pos.value() > n / 4)
    {
        res = n - avg_pos.value();
    }
    while (res < n && got_ecc[res] == 1)
    {
        res++;
    }
    if (res == n)
    {
        return starting_ite_point_aux(got_ecc, avg_pos.value());
    }
    return res;
}