/*
 * NodeBasedGraph.h
 *
 *  Created on: 01.05.2012
 *      Author: dennis
 */

#ifndef NODEBASEDGRAPH_H_
#define NODEBASEDGRAPH_H_

#include "../typedefs.h"
#include "../DataStructures/DynamicGraph.h"

struct _NodeBasedEdgeData {
	int distance;
	unsigned edgeBasedNodeID;
	unsigned nameID:31;
	bool shortcut:1;
	bool forward:1;
	bool backward:1;
	bool roundabout:1;
	bool ignoreInGrid:1;
	short type;
	bool isAccessRestricted;
};
typedef DynamicGraph< _NodeBasedEdgeData > _NodeBasedDynamicGraph;
typedef _NodeBasedDynamicGraph::InputEdge _NodeBasedEdge;



#endif /* NODEBASEDGRAPH_H_ */
