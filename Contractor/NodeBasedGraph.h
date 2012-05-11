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
    _NodeBasedEdgeData() : distance(INT_MAX), edgeBasedNodeID(UINT_MAX), nameID(UINT_MAX), type(SHRT_MAX),
            shortcut(false), forward(false), backward(false), roundabout(false), ignoreInGrid(false), isAccessRestricted(false), belongsToTinyComponent(false) {}
	int distance;
	unsigned edgeBasedNodeID;
	unsigned nameID;
    short type;
	bool shortcut:1;
	bool forward:1;
	bool backward:1;
	bool roundabout:1;
	bool ignoreInGrid:1;
    bool isAccessRestricted:1;
    bool belongsToTinyComponent:1;
};
typedef DynamicGraph< _NodeBasedEdgeData > _NodeBasedDynamicGraph;
typedef _NodeBasedDynamicGraph::InputEdge _NodeBasedEdge;



#endif /* NODEBASEDGRAPH_H_ */
