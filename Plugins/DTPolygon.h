#include "BasePlugin.h"
#include "RouteParameters.h"

#include "../DataStructures/QueryEdge.h"
#include "../DataStructures/StaticGraph.h"

#include "../typedefs.h"

#include "../Server/DataStructures/QueryObjectsStorage.h"

class DTPolygon: public BasePlugin {

private:

	NodeInformationHelpDesk * nodeHelpDesk;
	std::vector<std::string> & names;
	StaticGraph<QueryEdge::EdgeData> * graph;

	std::string pluginDescriptorString;
	SearchEngine<QueryEdge::EdgeData, StaticGraph<QueryEdge::EdgeData> > * searchEngine;

public:

	std::string GetDescriptor() const {
		return std::string("dtpolygon");
	}
	std::string GetVersionString() const {
		return std::string("0.1");
	}

	DTPolygon(QueryObjectsStorage * objects, std::string psd = "dtpolygon") :
			names(objects->names), pluginDescriptorString(psd) {
		nodeHelpDesk = objects->nodeHelpDesk;
		graph = objects->graph;
		searchEngine = new SearchEngine<QueryEdge::EdgeData,
				StaticGraph<QueryEdge::EdgeData> >(graph, nodeHelpDesk, names);
	}

	~DTPolygon() {
		delete searchEngine;
	}

	void HandleRequest(const RouteParameters & routeParameters,
			http::Reply& reply) {

		// loc=52.614670,8.352240
		// z = 4

		/*
		 * Get PhantomNode from Input Coordinate
		 */

		//check number of parameters
		if (!routeParameters.viaPoints.size() || routeParameters.viaPoints.size() > 1) {
			reply = http::Reply::stockReply(http::Reply::badRequest);
			return;
		}
		std::vector<std::string> textCoord;
		stringSplit(routeParameters.viaPoints[0], ',', textCoord);
		if (textCoord.size() != 2) {
			reply = http::Reply::stockReply(http::Reply::badRequest);
			return;
		}

		int lat = 100000. * atof(textCoord[0].c_str());
		int lon = 100000. * atof(textCoord[1].c_str());
		_Coordinate myCoordinate(lat, lon);
		if (false == checkCoord(myCoordinate)) {
			reply = http::Reply::stockReply(http::Reply::badRequest);
			return;
		}

		unsigned zoomLevel = 18;
		if (routeParameters.options.Find("z") != "") {
			zoomLevel = atoi(routeParameters.options.Find("z").c_str());
			if (18 < zoomLevel)
				zoomLevel = 18;
		}

		/*
		 * PhantomNode for further processing
		 */
		PhantomNode workingNode;
		nodeHelpDesk->FindPhantomNodeForCoordinate(myCoordinate, workingNode,
				zoomLevel);


		unsigned int edges = graph->FindEdgeInEitherDirection(workingNode.edgeBasedNode, workingNode.edgeBasedNode);
		graph->GetEdgeData(workingNode.edgeBasedNode);

		std::cout << "dtpolygon Plugin running!" << std::endl;
		reply.status = http::Reply::ok;
		reply.content.append(
				"<html><head><title>Hello World Demonstration Document of DTPlugin</title></head><body><h1>DTPolygon</h1>");
		std::stringstream content;


			content << "EdgeID: " << edges << "<br>";


		content << "Number of Nodes: " << graph->GetNumberOfNodes() << "<br>";
		content << "Number of Edges: " << graph->GetNumberOfEdges() << "<br>";
		content << "Found Node: " << workingNode.nodeBasedEdgeNameID << "<br>";
		content << "Zoom Level: " << zoomLevel << "<br>";
		content << "Number of parameters: " << routeParameters.parameters.size()
				<< "<br>";
		for (unsigned i = 0; i < routeParameters.parameters.size(); i++) {
			content << routeParameters.parameters[i] << "<br>";
		}
		content << "Number Of Options: " << routeParameters.options.Size()
				<< "<br>";
		RouteParameters::OptionsIterator optionsIT =
				routeParameters.options.begin();
		for (; optionsIT != routeParameters.options.end(); optionsIT++) {
			content << "param  = " << optionsIT->first << ": ";
			content << "option = " << optionsIT->second << "<br>";
		}
		reply.content.append(content.str());
		reply.content.append("</body></html>");
	}

private:
	inline bool checkCoord(const _Coordinate & c) {
		if (c.lat > 90 * 100000 || c.lat < -90 * 100000 || c.lon > 180 * 100000
				|| c.lon < -180 * 100000) {
			return false;
		}
		return true;
	}

};
