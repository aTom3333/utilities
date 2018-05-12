#ifndef UTILITIES_DIJKSTRA_HPP
#define UTILITIES_DIJKSTRA_HPP

#include <set>
#include <map>
#include <functional>
#include <bitset>
#include <list>
#include <algorithm>


template<typename Node, typename Distance, typename ContainerT = std::set<Node>>
class Dijkstra_solver;

template<typename Node, typename Distance, typename ContainerT = std::set<Node>>
class Dijkstra_data
{
    friend class Dijkstra_solver<Node, Distance, ContainerT>;

    public:
    Dijkstra_data& set(const ContainerT& q);
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
    ContainerT Q;
    std::function<Distance(Node, Node)> length;
    Distance far;
    Distance near;
    Node start;
    std::function<Distance(Distance, Distance)> combinaison = std::plus<Distance>();
    std::function<bool(Distance, Distance)> shorter = std::less<Distance>();
};


template<typename Node, typename Distance, typename ContainerT>
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


template<typename Node, typename Distance, typename ContainerT>
Dijkstra_data<Node, Distance, ContainerT>& Dijkstra_data<Node, Distance, ContainerT>::set(const ContainerT& q)
{
    Q = q;
    hasData.set(0);

    return *this;
}

template<typename Node, typename Distance, typename ContainerT>
Dijkstra_data<Node, Distance, ContainerT>& Dijkstra_data<Node, Distance, ContainerT>::length_function(std::function<Distance(Node, Node)> l)
{
    length = l;
    hasData.set(1);

    return *this;
}

template<typename Node, typename Distance, typename ContainerT>
Dijkstra_data<Node, Distance, ContainerT>& Dijkstra_data<Node, Distance, ContainerT>::far_value(Distance f)
{
    far = f;
    hasData.set(2);

    return *this;
}

template<typename Node, typename Distance, typename ContainerT>
Dijkstra_data<Node, Distance, ContainerT>& Dijkstra_data<Node, Distance, ContainerT>::near_value(Distance n)
{
    near = n;
    hasData.set(3);

    return *this;
}

template<typename Node, typename Distance, typename ContainerT>
Dijkstra_data<Node, Distance, ContainerT>& Dijkstra_data<Node, Distance, ContainerT>::start_node(Node A)
{
    start = A;
    hasData.set(4);

    return *this;
}

template<typename Node, typename Distance, typename ContainerT>
Dijkstra_data<Node, Distance, ContainerT>& Dijkstra_data<Node, Distance, ContainerT>::combinaison_op(std::function<Distance(Distance, Distance)> op)
{
    combinaison = op;
    hasData.set(5);

    return *this;
}

template<typename Node, typename Distance, typename ContainerT>
Dijkstra_data<Node, Distance, ContainerT>& Dijkstra_data<Node, Distance, ContainerT>::shorter_than_op(std::function<bool(Distance, Distance)> op)
{
    shorter = op;
    hasData.set(6);

    return *this;
}

template<typename Node, typename Distance, typename ContainerT>
bool Dijkstra_data<Node, Distance, ContainerT>::is_valid() const
{
    return hasData.all() && Q.find(start) != Q.end();
}



template<typename Node, typename Distance, typename ContainerT>
void Dijkstra_solver<Node, Distance, ContainerT>::solve()
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
        auto it = std::min_element(Q.begin(), Q.end(), [this](Node a, Node b)
        { return data.shorter(dist[a], dist[b]); });
        Node n1 = *it;
        Q.erase(it);

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

template<typename Node, typename Distance, typename ContainerT>
Distance Dijkstra_solver<Node, Distance, ContainerT>::d(Node n)
{
    return (dist.find(n) != dist.end() ? dist[n] : data.far);
}

template<typename Node, typename Distance, typename ContainerT>
std::list<Node> Dijkstra_solver<Node, Distance, ContainerT>::path_to(Node n)
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
