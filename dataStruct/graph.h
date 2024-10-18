#ifndef COMM_DATASTRUCT_GRAPH
#define COMM_DATASTRUCT_GRAPH
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
	uint64_t start;
	uint64_t end;
	int64_t weight;
} CommEdge;

typedef struct {
	uint64_t len;
	uint64_t status;
	uint64_t *vertex;
	uint64_t *n_edge;
	CommEdge **edges;
} CommGraph;

int commEdge_init(CommEdge *edge);
int commGraph_init(CommGraph *graph, CommEdge **edge, uint64_t len, uint64_t *n_edge, uint64_t *vertex);
int commGraph_del(CommGraph *graph);
int commGraph_copy(CommGraph *dst, CommGraph *src);
//Maximun Flow
int commGraph_MF(CommGraph *graph, uint64_t src, uint64_t dst);
//Minimum Spanning Tree
int commGraph_MST(CommGraph *graph);
//Shortest Path
int commGraph_SP(CommGraph *graph, uint64_t src, uint64_t dst);
//Topological Sorting
int commGraph_TS(CommGraph *graph, uint64_t *sorted);
//Strongly Connected Components
int commGraph_SCC(CommGraph *graph, CommGraph *result);
//Bipartite Graph Checking
int commGraph_BGC(CommGraph *graph);
//Reachability Queries
int commGraph_RQ(CommGraph *graph, uint64_t src, uint64_t dst);
//Traveling Salesman Problem
int commGraph_TSP(CommGraph *graph, uint64_t src, uint64_t dst);
//Clique Finding
int commGraph_CF(CommGraph *graph, uint64_t size, uint64_t *vertex);
//Random Walk
uint64_t commGraph_RW(CommGraph *graph, uint64_t src);
#endif
