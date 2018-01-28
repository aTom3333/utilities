#ifndef UTILITIES_DIJKSTRA_HPP
#define UTILITIES_DIJKSTRA_HPP

#include <set>
#include <map>
#include <functional>
#include <bitset>
#include <list>
#include <algorithm>


template<typename Node, typename Distance>
class Dijkstra_solver;

template<typename Node, typename Distance>
class Dijkstra_data
{
    friend class Dijkstra_solver<Node, Distance>;

    public:
        Dijkstra_data& set(std::set<Node> q);
        Dijkstra_data& length_function(std::function<Distance(Node, Node)> l);
        Dijkstra_data& far_value(Distance f);
        Dijkstra_data& near_value(Distance n);
        Dijkstra_data& start_node(Node A);
        Dijkstra_data& combinaison_op(std::function<Distance(Distance, Distance)> op);
        Dijkstra_data& shorter_than_op(std::function<bool(Distance, Distance)> op);

    protected:
        bool is_valid() const;
        std::bitset<7> hasData = 0x60;

        // Data for Djikstra algorithm
        std::set<Node> Q;
        std::function<Distance(Node, Node)> length;
        Distance far;
        Distance near;
        Node start;
        std::function<Distance(Distance, Distance)> combinaison = std::plus<Distance>();
        std::function<bool(Distance, Distance)> shorter = std::less<Distance>();
};


template<typename Node, typename Distance>
class Dijkstra_solver
{
    public:
        Dijkstra_solver(const Dijkstra_data<Node, Distance>& d) : data{d}, Q{data.Q} {};
        Dijkstra_solver(Dijkstra_data<Node, Distance>&& d) : data{std::move(d)}, Q{data.Q} {};
        void solve();
        Distance d(Node n);
        std::list<Node> path_to(Node n);

    protected:
        bool is_solved = false;
        std::map<Node, Distance> dist;
        const Dijkstra_data<Node, Distance> data;
        std::map<Node, Node> previous;
        std::set<Node> Q;
};


template<typename Node, typename Distance>
Dijkstra_data<Node, Distance>& Dijkstra_data<Node, Distance>::set(std::set<Node> q)
{
    Q = q;
    hasData.set(0);

    return *this;
}

template<typename Node, typename Distance>
Dijkstra_data<Node, Distance>& Dijkstra_data<Node, Distance>::length_function(std::function<Distance(Node, Node)> l)
{
    length = l;
    hasData.set(1);

    return *this;
}

template<typename Node, typename Distance>
Dijkstra_data<Node, Distance>& Dijkstra_data<Node, Distance>::far_value(Distance f)
{
    far = f;
    hasData.set(2);

    return *this;
}

template<typename Node, typename Distance>
Dijkstra_data<Node, Distance>& Dijkstra_data<Node, Distance>::near_value(Distance n)
{
    near = n;
    hasData.set(3);

    return *this;
}

template<typename Node, typename Distance>
Dijkstra_data<Node, Distance>& Dijkstra_data<Node, Distance>::start_node(Node A)
{
    start = A;
    hasData.set(4);

    return *this;
}

template<typename Node, typename Distance>
Dijkstra_data<Node, Distance>& Dijkstra_data<Node, Distance>::combinaison_op(std::function<Distance(Distance, Distance)> op)
{
    combinaison = op;
    hasData.set(5);

    return *this;
}

template<typename Node, typename Distance>
Dijkstra_data<Node, Distance>& Dijkstra_data<Node, Distance>::shorter_than_op(std::function<bool(Distance, Distance)> op)
{
    shorter = op;
    hasData.set(6);

    return *this;
}

template<typename Node, typename Distance>
bool Dijkstra_data<Node, Distance>::is_valid() const
{
    return hasData.all() && Q.find(start) != Q.end();
}



template<typename Node, typename Distance>
void Dijkstra_solver<Node, Distance>::solve()
{
    if(is_solved || !data.is_valid())
        return;

    for(Node n : data.Q)
    {
        dist[n] = data.far;
    }
    dist[data.start] = data.near;

    while(!Q.empty())
    {
        Node n1 = *std::min_element(Q.begin(), Q.end(), [this](Node a, Node b)
                                    { return data.shorter(dist[a], dist[b]); });
        Q.erase(n1);

        for(Node n2 : data.Q)
        {
            Distance alt = data.combinaison(dist[n1], data.length(n1, n2));
            if(data.shorter(alt, dist[n2]))
            {
                dist[n2] = alt;
                previous[n2] = n1;
            }
        }
    }

    is_solved = true;
}

template<typename Node, typename Distance>
Distance Dijkstra_solver<Node, Distance>::d(Node n)
{
    return (dist.find(n) != dist.end() ? dist[n] : data.far);
}

template<typename Node, typename Distance>
std::list<Node> Dijkstra_solver<Node, Distance>::path_to(Node n)
{
    if(previous.find(n) == previous.end())
        return std::list<Node>();

    std::list<Node> path;
    while(n != data.start)
    {
        path.push_front(n);
        n = previous[n];
    }

    return path;
}

#endif //UTILITIES_DIJKSTRA_HPP
