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
	uint64_t *n_edge;
	CommEdge **edges;
} CommGraph;

int commEdge_init(CommEdge *edge);
int commGraph_init(CommGraph *graph, CommEdge **edge, uint64_t len, uint64_t *n_edge);
int commGraph_del(CommGraph *graph);
int commGraph_copy(CommGraph *dst, CommGraph *src);
int commGraph_MC(CommGraph *graph);//minimum cut
int commGraph_MST(CommGraph *graph);//minimum spanning tree
int commGraph_SP(CommGraph *graph);//shortest path
#endif
