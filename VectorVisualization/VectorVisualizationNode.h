#ifndef _VectorVisualizationNode
#define _VectorVisualizationNode
//
// Copyright (C) Brian Torres
// 
// File: VectorVisualizationNode.h
//
// Dependency Graph Node: VectorVisualization
//
// Author: Maya Plug-in Wizard 2.0
//

#include <maya/MPxLocatorNode.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnCompoundAttribute.h>
#include <maya/MTypeId.h> 

 
class VectorVisualization : public MPxLocatorNode
{
public:
						VectorVisualization();
	virtual				~VectorVisualization(); 

	virtual MStatus		compute( const MPlug& plug, MDataBlock& data );
	virtual void		postConstructor();
	static  void*		creator();
	static  MStatus		initialize();
	virtual void		draw(M3dView & view, const MDagPath & path, M3dView::DisplayStyle style, M3dView::DisplayStatus status);

public:


	static  MObject		inputX;	
	static  MObject		inputY;	
	static  MObject		inputZ;			
	static  MObject		width;
	static  MObject		height;
	static  MObject		depth;
	static  MObject		densityX;
	static  MObject		densityY;
	static  MObject		densityZ;
	static  MObject		drawLength;

	static MObject		fldData;
	static MObject		pntArray;
	static MObject		velArray;
	static MObject		massArray;
	static MObject		deltaTime;

	static MObject		inTime;
	static MObject		inputForce;

	static	MTypeId		id;
};

#endif
