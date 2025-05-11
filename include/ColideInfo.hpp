#ifndef  COLIDE_INFO_HPP
#define COLIDE_INFO_HPP

#include "ColideType.hpp"

class ColideInfo
{
public:
	ColideInfo(ColideType type, int nodeId = 0, int firstNodeId = 0, int secondNodeId = 0, int arcId = 0)
		:mColideType(type), NodeId(nodeId), SecondNodeId(secondNodeId), ArcId(arcId)
	{

	}

	ColideType mColideType;
	int NodeId;
	int FirstNodeId;
	int SecondNodeId;
	int ArcId;
};

#endif
