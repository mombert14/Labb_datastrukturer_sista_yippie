#include "adjlist.h"
// local prototypes
pnode create_node(char nname);
pnode node_cons(pnode first, pnode second);
pedge create_edge(char to, double weight);
pedge edge_cons(pedge first, pedge second);
pedge _add_edge(pedge edge, char to, double weight);
pedge _rem_edge(pedge edges, char to);
void remove_all_edges_to(pnode G, char name);
void remove_all_edges_from(pnode G, char name);

// create_node: creates node with name nname
pnode create_node(char nname)
{
	pnode new_node = (pnode)calloc(1, sizeof(node));
	new_node->name = nname;
	new_node->d = INFINITY;
	new_node->pi = '-';
	new_node->next_node = NULL;
	new_node->edges = NULL;
	return new_node;
}
// function implementations
bool is_empty(pnode G)
{
	return !G;
}
char get_name(pnode G)
{
	return is_empty(G) ? '-' : G->name;
}
pnode set_name(pnode G, char name)
{
	if (!is_empty(G))
		G->name = name;
	return G;
}
pedge get_edges(pnode G)
{
	return is_empty(G) ? NULL : G->edges;
}
pnode set_edges(pnode G, pedge E)
{
	if (!is_empty(G))
		G->edges = E;
	return G;
}
pnode get_next(pnode G)
{
	return is_empty(G) ? NULL : G->next_node;
}
// see node_cons() for the corresponding "set-next" function
double get_d(pnode G)
{
	return is_empty(G) ? INFINITY : G->d;
}
pnode set_d(pnode G, double d)
{
	if (!is_empty(G))
		G->d = d;
	return G;
}
char get_pi(pnode G)
{
	return is_empty(G) ? '-' : G->pi;
}
pnode set_pi(pnode G, char pi)
{
	if (!is_empty(G))
		G->pi = pi;
	return G;
}
// node_cons: connects two nodes in adjacency list
pnode node_cons(pnode first, pnode second)
{
	if (is_empty(first))
		return NULL;
	first->next_node = second;
	return first;
}
// add_node: adds a new node with name nname to adjacency list G
//           in lexicographical order. If it already exists
//           in graph, nothing is done
pnode add_node(pnode G, char nname)
{

	if (find_node(G, nname)) // controll if it already exist
		return G;

	pnode new_n = create_node(nname);

	if (is_empty(G)) // empty list or it should go in first
		return node_cons(new_n, G);

	if (nname < get_name(G)) // find right position
		return node_cons(new_n, G);

	pnode current = G;
	while (!is_empty(get_next(current)) && get_name(get_next(current)) < nname)
		current = get_next(current);

	node_cons(new_n, get_next(current));
	node_cons(current, new_n);

	return G;
}
// rem_node: removes node with name name from adjacency list G
//           if node does not exist, nothing happens
pnode rem_node(pnode G, char name)
{
	if (is_empty(G))
		return G;

	if (get_name(G) == name) // first node
	{
		pnode next = get_next(G);
		remove_all_edges_to(next, name);
		remove_all_edges_from(G, name);

		free(G);
		return next;
	}

	pnode current = G;

	// find right node
	while (!is_empty(get_next(current)) && get_name(get_next(current)) != name)
		current = get_next(current);

	if (!is_empty(get_next(current)))
	{
		pnode part = get_next(current);
		node_cons(current, get_next(part));
		remove_all_edges_to(G, name);
		remove_all_edges_from(part, name);
		free(part);
	}

	return G;
}

// get_node: returns pointer to node with name name from adjacency list G
pnode get_node(pnode G, char name)
{
	while (!is_empty(G))
	{
		if (get_name(G) == name)
			return G;
		G = get_next(G);
	}
	return NULL;
}
// get_node: returns true if node with name name exists in adjacency list G
//           false otherwise
bool find_node(pnode G, char name)
{
	return is_empty(G) ? false : get_name(G) == name ? true
													 : find_node(G->next_node, name);
}
// create_edge: creates edge
pedge create_edge(char to, double weight)
{
	pedge new_edge = (pedge)calloc(1, sizeof(edge));
	new_edge->to = to;
	new_edge->weight = weight;
	return new_edge;
}
bool edge_empty(pedge E)
{
	return !E;
}
char get_to(pedge E)
{
	return edge_empty(E) ? '-' : E->to;
}
pedge set_to(pedge E, char to)
{
	if (!edge_empty(E))
		E->to = to;
	return E;
}
double get_weight(pedge E)
{
	return edge_empty(E) ? INFINITY : E->weight;
}
pedge set_weight(pedge E, double weight)
{
	if (!edge_empty(E))
		E->weight = weight;
	return E;
}
pedge get_next_edge(pedge E)
{
	return edge_empty(E) ? NULL : E->next_edge;
}
// edge_cons: connects two edges in edge list
pedge edge_cons(pedge first, pedge second)
{
	if (edge_empty(first))
		return NULL;
	first->next_edge = second;
	return first;
}
// upd_edge: updates edge E to new weight
pedge upd_edge(pedge E, double weight)
{
	if (edge_empty(E))
		return E;
	else
		set_weight(E, weight);

	return E;
}
// _add_edge: creates and connects new edge to edge-list
pedge _add_edge(pedge E, char to, double weight)
{
	if (edge_empty(E)) // if edge dont exist create a new
		return create_edge(to, weight);
	else if (to < get_to(E)) // if the new should be placed before existing one
		return edge_cons(create_edge(to, weight), E);
	else if (get_to(E) == to) // if exist update weight
		return upd_edge(E, weight);

	pedge holder = _add_edge(get_next_edge(E), to, weight); // recursive through the rest
	edge_cons(E, holder); 

	return E;
}
// add_edge: adds an edge to G by finding correct start node
//           and then calling _add_edge to create new edge
void add_edge(pnode G, char from, char to, double weight)
{
	if (is_empty(G)) // if empty do nothing
		return;

	if (!find_node(G, to)) // if the desitination doesnt exit, do nothing
		return;


	while (!is_empty(G)) // go through the list
	{
		if (get_name(G) == from)
		{
			// add the edge to the list
			set_edges(G, _add_edge(get_edges(G), to, weight));
			return;
		}

		G = get_next(G);
	}
}
// _find_edge: finds edge in edge-list
bool _find_edge(pedge E, char to)
{
	while (!edge_empty(E))
	{
		if (get_to(E) == to)
			return true;
		E = get_next_edge(E);
	}
	return false;
}
// find_edge: returns true if edge between from and to exists, false otherwise
bool find_edge(pnode G, char from, char to)
{
	pnode current = G;

	// find the starting node
	while (!is_empty(current) && get_name(current) != from)
		current = get_next(current);

	// if the node dont exist
	if (is_empty(current))
		return false;

	// go through all the edges for the node
	pedge edge = get_edges(current);
	while (!edge_empty(edge))
	{
		if (get_to(edge) == to)
			return true;

		edge = get_next_edge(edge);
	}

	return false;
}
// _edge_cardinality: returns the number of edges from one node
int _edge_cardinality(pedge E)
{
	int count = 0;

	while (!edge_empty(E))
	{
		count++;
		E = get_next_edge(E);
	}

	return count;
}
// edge_cardinality: returns the total number of edges in G
int edge_cardinality(pnode G)
{
	int count = 0;

	while (!is_empty(G))
	{
		count += _edge_cardinality(get_edges(G));
		G = get_next(G);
	}
	return count;
}
// _self_loops: returns the number of edges going back to
//              source node
int _self_loops(pedge E, char src)
{
	int count = 0;

	while (!edge_empty(E))
	{
		if (get_to(E) == src)
			count++;
		E = get_next_edge(E);
	}

	return count;
}
// self_loops: counts the number of self-loops, i.e. edges to and from
//             the same node
int self_loops(pnode G)
{
	int count = 0;

	while (!is_empty(G))
	{
		count += _self_loops(get_edges(G), get_name(G));
		G = get_next(G);
	}

	return count;
}
// _rem_edge: removes edge from edge-list
pedge _rem_edge(pedge E, char to)
{
	pedge current = E;
	pedge prev = NULL;

	// go through the list
	while (!edge_empty(current))
	{
		//if right edge is found
		if (get_to(current) == to)
		{
			// edge is first
			if (edge_empty(prev))
				return get_next_edge(current);

			//skip the edge that is going to be removed
			edge_cons(prev, get_next_edge(current));
			return E;
		}

		// move pointer forward
		prev = current;
		current = get_next_edge(current);
	}

	return E;
}
// rem_edge: removes edge from G
void rem_edge(pnode G, char from, char to)
{
	// go through the list
	while (!is_empty(G))
	{
		// find the start node
		if (get_name(G) == from)
		{
			// remove edge from list
			set_edges(G, _rem_edge(get_edges(G), to));
			return;
		}

		G = get_next(G);
	}
}
// remove_all_edges_to: removes all edges going towards node with name name
void remove_all_edges_to(pnode G, char name)
{
	while (!is_empty(G))
	{

		while (_find_edge(get_edges(G), name))
			set_edges(G, _rem_edge(get_edges(G), name));

		G = get_next(G);
	}
}
// remove_all_edges_from: removes all edges from node with name name
void remove_all_edges_from(pnode G, char name)
{
	while (!is_empty(G))
	{
		if (get_name(G) == name)
		{
			set_edges(G, NULL);
			return;
		}
		G = get_next(G);
	}
}
// node_cardinality: returns the number of nodes in G
int node_cardinality(pnode G)
{
	int count = 0;

	while (!is_empty(G))
	{
		count++;
		G = get_next(G);
	}

	return count;
}
// name_to_pos: returns position of node with name c, -1 if not found
int name_to_pos(pnode G, char c)
{
	int pos = 0;

	while (!is_empty(G))
	{
		if (get_name(G) == c)
			return pos;

		G = get_next(G);
		pos++;
	}

	return -1;
}
// pos_to_name: returns name of node at position pos in G
char pos_to_name(pnode G, int pos)
{
	int i = 0;

	while (!is_empty(G))
	{
		if (i == pos)
			return get_name(G);
		G = get_next(G);
		i++;
	}

	return '-';
}
// list_to_pos: creates adjacency matrix from adjacency list
void list_to_matrix(pnode G, double matrix[MAXNODES][MAXNODES])
{
	pnode head = G; // save start node

	while (!is_empty(G))
	{
		int nodes = node_cardinality(G); //count nodes

		for (int i = 0; i < nodes; i++)
			for (int j = 0; j < nodes; j++)
				matrix[i][j] = INFINITY;

		while (!is_empty(G))
		{
			// Turn nod to a index in the matrix
			int k = name_to_pos(head, get_name(G));
			pedge edge = get_edges(G); // get list with outgoing edges

			while (!edge_empty(edge))
			{
				// dsetination node to colum index
				int l = name_to_pos(head, get_to(edge));
				matrix[k][l] = get_weight(edge); // weight in position k and l
				edge = get_next_edge(edge);
			}

			G = get_next(G);
		}
	}
}
