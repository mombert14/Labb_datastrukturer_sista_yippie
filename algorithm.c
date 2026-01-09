#include "algorithm.h"
// queue - defines a queue as a pointer to an array of pointers to nodes
typedef pnode *queue;
// local prototypes
void init_single_source(pnode G, char s);
void relax(pnode u, pnode v, int w);
int Q_insert(queue Q, pnode u, int index);
int Q_is_empty(queue Q, int size);
pnode Q_extract_min(queue Q, int size);
bool Q_exists(queue Q, int qsize, char name);
int name_to_pos(pnode G, char c);
void list_to_matrix(pnode G, double matrix[MAXNODES][MAXNODES]);
//--------------------------------------------------------------------------
// init single source, relax and min are to be used by algorithms
//--------------------------------------------------------------------------
void init_single_source(pnode G, char s)
{
	for (pnode node = G; !is_empty(node); node = get_next(node)) {
		if (get_name(node) == s) // start node
			set_d(node, 0);
		else
			set_d(node, INFINITY);
		set_pi(node, '-');
	}
}
void relax(pnode u, pnode v, int w)
{
	if (get_d(v) > (get_d(u) + w)) {
		set_d(v, get_d(u) + w);
		set_pi(v, get_name(u));
	}
}
double min(double a, double b)
{
	return a < b ? a : b;
}
//--------------------------------------------------------------------------
// Q functions - used by other algorithms
//--------------------------------------------------------------------------
int Q_insert(queue Q, pnode u, int index)
{
	Q[index] = u;
	return 0;
}
int Q_is_empty(queue Q, int size)
{
	for (int i = 0; i < size; i++)
	{
		if(Q[i]!=NULL)return false;
	}
	
	return true;
}
pnode Q_extract_min(queue Q, int size)
{
	if (Q_is_empty(Q,size) ==false)
	{
		int min_pos = 0;
		pnode min;
		for (int i = 0; i < size; i++)
		{
			if (Q[i])
			{
				min = Q[i];
				min_pos = i;
				break;
			}
			
		}
		for (int i = 0; i < size; i++)
		{
			if (Q[i])
			{
				if (get_d(Q[i])<get_d(min))
				{
					min = Q[i];
					min_pos = i;
				}
			}
		}
		Q[min_pos] = NULL;
		return min;
		
	}
	
	return NULL;
}
bool Q_exists(queue Q, int qsize, char name)
{
	for (int i = 0; i < qsize; i++)
	{
		if (Q[i])
		{
			if (Q[i]->name == name)return true;
		}
	}
	return false;
}
//--------------------------------------------------------------------------
// Dijkstra's algorithm, single source shortest path tree generator
// a -> b(1) -> c(5)
// b -> c(2)           --> d: [  0,   1,   3]
// c                       e: [  -,   a,   b]
//--------------------------------------------------------------------------
void dijkstra(pnode G, char s, double *d, char *e)
{
	printf("Hej vi bara testar\n");
	int size = node_cardinality(G);
	init_single_source(G, s);
	queue Q = malloc(sizeof(pnode)* size);
	pnode curr = G;
	for (int i = 0; i < size; i++)
	{
		Q[i]= curr;
		curr = get_next(curr);
		
	}
	while (!Q_is_empty(Q, size))
    {
        pnode u = Q_extract_min(Q, size);
        if (u == NULL || get_d(u) == INFINITY)
        {
            continue;
        }
        pedge edge = get_edges(u);
        while (edge != NULL)
        {
            pnode v = get_node(G, (*edge).to);
            double new_dist = get_d(u) + (*edge).weight;
            if (Q_exists(Q, size, (*edge).to) && new_dist < get_d(v))
            {
                set_d(v, new_dist);
                set_pi(v, get_name(u));
            }

            edge = (*edge).next_edge;
        }
    }
    curr = G;
    while (curr != NULL)
    {
        int pos = name_to_pos(G, get_name(curr));
        d[pos] = get_d(curr);
        char p = get_pi(curr);
        if (p == '\0')
        {
            e[pos] = 45; 
        }
        else
        {
            e[pos] = p;
        }
        curr = get_next(curr);
    }
    free(Q);
}
//--------------------------------------------------------------------------
// Prim's algorithm - Minimum Spanning Tree generator
// start_node: a
// a -> b(1) -> c(5)
// b -> c(2)           --> d: [inf,   1,   2]
// c                       e: [  -,   a,   b]
//--------------------------------------------------------------------------
void prim(pnode G, char start_node, double *d, char *e)
{
   
    init_single_source(G, start_node);
    int size = node_cardinality(G);
    queue Q = malloc(sizeof(pnode) * size);
    pnode curr = G;
    for (int i = 0; i < size; i++)
    {
        Q[i] = curr;
        curr = get_next(curr);
    }

    while (!Q_is_empty(Q, size))
    {
        pnode u = Q_extract_min(Q, size);
        if (u == NULL) break;

        pedge edge = get_edges(u);
        while (edge != NULL)
        {
            pnode v = get_node(G, edge->to);
            if (Q_exists(Q, size, get_name(v)) && edge->weight < get_d(v))
            {
                set_d(v, edge->weight);
                set_pi(v, get_name(u));
            }
            edge = edge->next_edge;
        }
    }

    curr = G;
    while (curr != NULL)
    {
        int pos = name_to_pos(G, get_name(curr));
        if (get_name(curr) == start_node) 
        {
            d[pos] = INFINITY;
        }
        else 
        {
            d[pos] = get_d(curr);
        }

        char p = get_pi(curr);
        if (p == '\0' || p == 0)
        {
            e[pos] = 45; 
        }
        else
        {
            e[pos] = p;
        }
        curr = get_next(curr);
    }

    free(Q);
}

//--------------------------------------------------------------------------
// Floyd's algorithm: returns matrix of distances
// a -> b(1)
// |                   |0  |1  |3  |
// b -> c(2)    -->    |INF|0  |2  |
// |                   |INF|INF|0  |
// c
//--------------------------------------------------------------------------
void floyd(pnode G, double W[MAXNODES][MAXNODES])
{
    for (int i = 0; i < MAXNODES; i++)
    {
        for (int j = 0; j < MAXNODES; j++)
        {
            if (i == j)
            {
                W[i][j] = 0.0; 
            }
            else
            {
                W[i][j] = INFINITY; 
			}
        }
    }

    pnode u = G;
    while (u != NULL)
    {
        int i = name_to_pos(G, get_name(u));
        
        pedge edge = get_edges(u);
        while (edge != NULL)
        {
            int j = name_to_pos(G, edge->to);
            if (i >= 0 && i < MAXNODES && j >= 0 && j < MAXNODES)
            {
                W[i][j] = edge->weight;
            }
            edge = edge->next_edge;
        }
        u = get_next(u);
    }

    int size = node_cardinality(G);
    
    for (int k = 0; k < size; k++)
    {
        for (int i = 0; i < size; i++)
        {
            for (int j = 0; j < size; j++)
            {
                if (W[i][k] != INFINITY && W[k][j] != INFINITY)
                {
                    if (W[i][k] + W[k][j] < W[i][j])
                    {
                        W[i][j] = W[i][k] + W[k][j];
                    }
                }
            }
        }
    }
}
//--------------------------------------------------------------------------
// Warshall's algorithm: returns matrix of closures, i.e. if paths exists
// a -> b(1)
// |                   |T  |T  |T  |
// b -> c(2)    -->    |F  |T  |T  |
// |                   |F  |F  |T  |
// c
//--------------------------------------------------------------------------
void warshall(pnode G, double W[MAXNODES][MAXNODES])
{
	for (int i = 0; i < MAXNODES; i++)
    {
        for (int j = 0; j < MAXNODES; j++)
        {
            W[i][j] = 0.0;
        }
    }
    pnode u = G;
    while (u != NULL)
    {
        int i = name_to_pos(G, get_name(u));
        if (i >= 0 && i < MAXNODES)
        {
            W[i][i] = 1.0;
        }

        pedge edge = get_edges(u);
        while (edge != NULL)
        {
            int j = name_to_pos(G, edge->to);
            if (i >= 0 && i < MAXNODES && j >= 0 && j < MAXNODES)
            {
                W[i][j] = 1.0; 
            }
            edge = edge->next_edge;
        }
        u = get_next(u);
    }
    int size = node_cardinality(G);
    
    for (int k = 0; k < size; k++)
    {
        for (int i = 0; i < size; i++)
        {
            for (int j = 0; j < size; j++)
            {
                if (W[i][j] == 1.0 || (W[i][k] == 1.0 && W[k][j] == 1.0))
                {
                    W[i][j] = 1.0;
                }
            }
        }
    }
}
