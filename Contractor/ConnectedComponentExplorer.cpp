/*
    open source routing machine
    Copyright (C) Dennis Luxen, 2010

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU AFFERO General Public License as published by
the Free Software Foundation; either version 3 of the License, or
any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU Affero General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
or see http://www.gnu.org/licenses/agpl.txt.
 */

#include <algorithm>
#include "ConnectedComponentExplorer.h"
#include "../DataStructures/BinaryHeap.h"

ConnectedComponentExplorer::ConnectedComponentExplorer(boost::shared_ptr<_NodeBasedDynamicGraph> & g, boost::unordered_map<NodeID, bool> & bn) : _graph(g), _barrierNodes(bn) {

}
void ConnectedComponentExplorer::Run(){
	unsigned componentCounter = 0;
	unsigned visitedNodeCounter = 0;
	BinaryHeap<unsigned, int, int, BFSHeapData> bfsHeap(_graph->GetNumberOfNodes());
	for(unsigned i = 0; i < _graph->GetNumberOfNodes(); ++i) {
		if(!bfsHeap.WasInserted(i)) {
			unsigned previousVisitedNodeCounter = visitedNodeCounter;
			_vectorOfComponents.resize(componentCounter+1);
			bfsHeap.Insert(i, visitedNodeCounter++, componentCounter);
			while(bfsHeap.Size()) {
				//get next element from queue
				unsigned currentNode = bfsHeap.DeleteMin();
				_vectorOfComponents.at(componentCounter).push_back(currentNode);
				//todo: continue if bollard node
//				if(_barrierNodes[currentNode])
//				    continue;
				//relax outgoing edges
				for(_NodeBasedDynamicGraph::EdgeIterator edge = _graph->BeginEdges(currentNode), end = _graph->EndEdges(currentNode); edge != end; ++edge) {
					unsigned targetNode = _graph->GetTarget(edge);
					if(!bfsHeap.WasInserted(targetNode)) {
						//add adjacent _and_ unvisited nodes to queue
						bfsHeap.Insert(targetNode, visitedNodeCounter++, componentCounter);
					}
				}
			}
			//Marking edges from tiny components as such
			if((visitedNodeCounter-previousVisitedNodeCounter) < TINY_COMPONENT_SIZE_THRESHOLD) {
			    INFO("CC: " << componentCounter << ", size: " << visitedNodeCounter-previousVisitedNodeCounter);
			    for(unsigned j = 0, endOfNodes = _vectorOfComponents.at(componentCounter).size() ; j < endOfNodes; ++j) {
			        unsigned currentNode = _vectorOfComponents.at(componentCounter)[j];
	                for(_NodeBasedDynamicGraph::EdgeIterator edge = _graph->BeginEdges(currentNode), end = _graph->EndEdges(currentNode); edge != end; ++edge) {
	                    _graph->GetEdgeData(edge).belongsToTinyComponent = true;
	                }
			    }
			}
            ++componentCounter;
		}
	}
	INFO("Finished CC exploration of input graph, marked nodes of tiny edges");
//	ProduceHistogram();
}

unsigned ConnectedComponentExplorer::GetNumberOfConnectedComponents() const {
	return _vectorOfComponents.size();
}

std::vector<unsigned> & ConnectedComponentExplorer::GetComponent(const unsigned i) {
	return _vectorOfComponents.at(i);
}

ConnectedComponentExplorer::~ConnectedComponentExplorer() {
	// TODO Auto-generated destructor stub
}

void ConnectedComponentExplorer::ProduceHistogram() const {
	std::vector<unsigned> sizes;
	for(unsigned i = 0; i < _vectorOfComponents.size(); ++i) {
		sizes.push_back(_vectorOfComponents.at(i).size());
	}
	std::sort(sizes.begin(), sizes.end());
	for(unsigned i = 0; i < sizes.size(); ++i) {
		INFO(i << ", " << sizes[i]);
	}
}
