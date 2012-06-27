//
// Copyright (C) Brian Torres
// 
// File: VectorVisualizationNode.cpp
//
// Dependency Graph Node: VectorVisualization
//
// Author: Maya Plug-in Wizard 2.0
//

#include "VectorVisualizationNode.h"
#include <maya/MPlug.h>
#include <maya/MDataBlock.h>
#include <maya/MDataHandle.h>
#include <maya/MColor.h>
#include <maya/MFloatVector.h>
#include <maya/MMatrix.h>
#include <maya/MPxNode.h>
#include <maya/MFnDependencyNode.h>
#include <maya/MFnUnitAttribute.h>
#include <maya/MFnGenericAttribute.h>
#include <maya/MDoubleArray.h>
#include <maya/MFnDoubleArrayData.h>
#include <maya/MTime.h>
#include <maya/MVector.h>
#include <maya/MVectorArray.h>
#include <maya/MFnVectorArrayData.h>
#include <maya/MDagPath.h>
#include <maya/MFnDagNode.h>

MTypeId     VectorVisualization::id( 0xaf26c );

MObject     VectorVisualization::inputX;  
MObject     VectorVisualization::inputY;   
MObject     VectorVisualization::inputZ;        
MObject		VectorVisualization::width;
MObject		VectorVisualization::height;
MObject		VectorVisualization::depth;
MObject		VectorVisualization::densityX;
MObject		VectorVisualization::densityY;
MObject		VectorVisualization::densityZ;
MObject		VectorVisualization::drawLength;

MObject		VectorVisualization::fldData;
MObject		VectorVisualization::pntArray;
MObject		VectorVisualization::velArray;
MObject		VectorVisualization::massArray;
MObject		VectorVisualization::deltaTime;

MObject		VectorVisualization::inTime;
MObject		VectorVisualization::inputForce;

VectorVisualization::VectorVisualization() {}
VectorVisualization::~VectorVisualization() {}

MStatus VectorVisualization::initialize()
//
//	Description:
//		This method is called to create and initialize all of the attributes
//      and attribute dependencies for this node type.  This is only called 
//		once when the node type is registered with Maya.
//
//	Return Values:
//		MS::kSuccess
//		MS::kFailure
//		
{


	MFnNumericAttribute nAttr;
	MFnUnitAttribute	uAttr;
	MFnGenericAttribute gAttr;
	MStatus				stat;

	inputX = nAttr.create( "inputX", "inX", MFnNumericData::kFloat, 0.0 );
	nAttr.setStorable(true);
	nAttr.setKeyable(true);
	inputY = nAttr.create( "inputY", "inY", MFnNumericData::kFloat, 0.0 );
	nAttr.setStorable(true);
	nAttr.setKeyable(true);
	inputZ = nAttr.create( "inputZ", "inZ", MFnNumericData::kFloat, 0.0 );
 	nAttr.setStorable(true);
 	nAttr.setKeyable(true);


	// Add the attributes we have created to the node
	//
	width = nAttr.create( "width", "w", MFnNumericData::kFloat, 4.0f );
	nAttr.setKeyable(true);
	height = nAttr.create( "height", "h", MFnNumericData::kFloat, 4.0f );
	nAttr.setKeyable(true);
	depth = nAttr.create( "depth", "dp", MFnNumericData::kFloat, 4.0f );
	nAttr.setKeyable(true);
	densityX = nAttr.create( "densityX", "dnX", MFnNumericData::kInt, 4 );
	nAttr.setKeyable(true);
	densityY = nAttr.create( "densityY", "dnY", MFnNumericData::kInt, 4 );
	nAttr.setKeyable(true);
	densityZ = nAttr.create( "densityZ", "dnZ", MFnNumericData::kInt, 4 );
	nAttr.setKeyable(true);
	drawLength = nAttr.create( "drawLength", "dl", MFnNumericData::kFloat, 1.0f );
	nAttr.setKeyable(true);


	pntArray = gAttr.create("fieldDataPosition","fdp");
	gAttr.addAccept(MFnData::kVectorArray);
	gAttr.setReadable(true);
	gAttr.setHidden(true);

	velArray = gAttr.create("fieldDataVelocity","fdv");
	gAttr.addAccept(MFnData::kVectorArray);
	gAttr.setReadable(true);
	gAttr.setHidden(true);

	massArray = gAttr.create("fieldDataMass","fdm");
	gAttr.addAccept(MFnData::kDoubleArray);
	gAttr.setReadable(true);
	gAttr.setHidden(true);



	deltaTime = uAttr.create("fieldDataDeltaTime","fdt", MFnUnitAttribute::kTime, 0.0417);
	uAttr.setReadable(true);
	uAttr.setHidden(true);

	MFnCompoundAttribute fldDataList; 
	fldData = fldDataList.create("fieldData", "fd", &stat); 
	fldDataList.addChild(pntArray); 
	fldDataList.addChild(velArray); 
	fldDataList.addChild(massArray); 
	fldDataList.addChild(deltaTime); 
	fldDataList.setReadable(true); 
	fldDataList.setHidden(true);
	 

	inputForce = gAttr.create("inputForce","ifc");
	gAttr.addAccept(MFnData::kVectorArray);
	gAttr.setReadable(true);
	gAttr.setHidden(true);

	inTime = uAttr.create("inTime","it", MFnUnitAttribute::kTime, 0.0);
	uAttr.setReadable(true);


	stat = addAttribute( inputX );
	stat = addAttribute( inputY );
	stat = addAttribute( inputZ );
	stat = addAttribute(inTime);
	stat = addAttribute(pntArray);
	stat = addAttribute(velArray);
	stat = addAttribute(massArray);
	stat = addAttribute(deltaTime);
	stat = addAttribute(fldData);
	stat = addAttribute(inputForce);

	stat = addAttribute( width );
	stat = addAttribute( height );
	stat = addAttribute( depth );
	stat = addAttribute( densityX );
	stat = addAttribute( densityY );
	stat = addAttribute( densityZ );
	stat = addAttribute( drawLength );

	stat = attributeAffects( inputX, fldData );
	stat = attributeAffects( inputY, fldData );
	stat = attributeAffects( inputZ, fldData );
	stat = attributeAffects( width, fldData );
	stat = attributeAffects( height, fldData );
	stat = attributeAffects( depth, fldData );
	stat = attributeAffects( densityX, fldData );
	stat = attributeAffects( densityY, fldData );
	stat = attributeAffects( densityZ, fldData );
	stat = attributeAffects( inTime, fldData );
	stat = attributeAffects( fldData, inputForce );

	return MS::kSuccess;

}

MStatus VectorVisualization::compute( const MPlug& plug, MDataBlock& data )
//
//	Description:
//		This method computes the value of the given output plug based
//		on the values of the input attributes.
//
//	Arguments:
//		plug - the plug to compute
//		data - object that provides access to the attributes for this node
//
{
	MStatus returnStatus;
	// Check which output attribute we have been asked to compute.  If this 
	// node doesn't know how to compute it, we must return 
	// MS::kUnknownParameter.
	// 
		// Get a handle to the input attribute that we will need for the
		// computation.  If the value is being supplied via a connection 
		// in the dependency graph, then this call will cause all upstream  
		// connections to be evaluated so that the correct value is supplied.
		// 


	if( (plug == pntArray) || (plug == velArray) || (plug == massArray) || (plug == deltaTime) || (plug == fldData) || (plug == inputX)|| (plug == inputY)|| (plug == inputZ) || (plug == inTime)){

		MStatus stat;
		MObject thisNode = thisMObject();
		MDataHandle inputTime = data.inputValue( inTime, &returnStatus );
		//Fetch all plugs for figuring fieldData array length and space between lines in xyz
		MDataHandle inputWidth = data.inputValue( width, &returnStatus );
		MDataHandle inputDepth = data.inputValue( depth, &returnStatus );
		MDataHandle inputHeight = data.inputValue( height, &returnStatus );
		MDataHandle inputDensityX = data.inputValue( densityX, &returnStatus );
		MDataHandle inputDensityY = data.inputValue( densityY, &returnStatus );
		MDataHandle inputDensityZ = data.inputValue( densityZ, &returnStatus );

		int densityX = inputDensityX.asInt();
		int densityY = inputDensityY.asInt();
		int densityZ = inputDensityZ.asInt();

		MVector xStep = MFloatVector( (inputWidth.asFloat()/densityX), 0.0, 0.0 );
		MVector yStep = MFloatVector( 0.0, (inputHeight.asFloat()/densityY), 0.0 );
		MVector zStep = MFloatVector( 0.0, 0.0,(inputDepth.asFloat()/densityZ) );
		
		MVector current = MFloatVector( 0.0, 0.0, 0.0 );
		MVector wpos = MFloatVector( 0.0, 0.0, 0.0 );


		int arrLength = (int)(inputWidth.asFloat() *  inputHeight.asFloat() * inputDepth.asFloat()  * inputDensityX.asInt());

		//Create handles and MFn wrappers
		MDataHandle pointArrayData = data.outputValue(pntArray);
		MDataHandle velocityArrayData = data.outputValue(velArray);
		MDataHandle massArrayData = data.outputValue(massArray);
		MDataHandle deltaTimeData = data.outputValue(deltaTime);

		MVectorArray pointArrayOut;
		MVectorArray velocityArrayOut;
		MDoubleArray massArrayOut;

		MFnVectorArrayData pointDataFn(pointArrayData.data());
		MFnVectorArrayData velocityDataFn(velocityArrayData.data());
		MFnDoubleArrayData massDataFn(massArrayData.data());

		// Not an array, just make it
		MTime deltaTimeOut;
		deltaTimeOut.setUnit(MTime::kFilm);
		deltaTimeOut.setValue(1.0);
		deltaTimeData.set(deltaTimeOut);

		// if the array does not exist
		//		create new array
		//		
		//	else
		//		take existing array
		//  set length to the width*depth*height*density
		//	point data handle to the new/existing array
		//
		//	Needed to make sure you're not dropping pointers everytime/creating a huge memory leak
		//	Rinse and repeat for all 3 array-based main fieldData outputs
		bool pointExists;
		bool velocityExists;
		bool massExists;

		if ((pointArrayData.data()).isNull())
		{
			pointExists = false;
		}
		else
		{
			pointExists = true;
			pointArrayOut = pointDataFn.array();
		}
		pointArrayOut.setLength(arrLength);
		

		if ((velocityArrayData.data()).isNull())
		{
			velocityExists = false;
		}
		else
		{
			velocityExists = true;
			velocityArrayOut = velocityDataFn.array();
		}
		velocityArrayOut.setLength(arrLength);

		if ((massArrayData.data()).isNull())
		{
			massExists = false;
		}
		else
		{
			massExists = true;
			massArrayOut = massDataFn.array();
		}
		massArrayOut.setLength(arrLength);
		


		// Populate the arrays!
		
		//Fetch worldspace Position for sending data to fields
		MFnDagNode dagPath( thisNode );
		MDagPath thisDagNode;
		dagPath.getPath(thisDagNode);
		MMatrix worldPos = thisDagNode.inclusiveMatrix();
		float q = worldPos( 3, 0 );
		float w = worldPos( 3, 1 );
		float e = worldPos( 3, 2 );

		int count = 0;
		
		for(int x = 0; x < densityX; x++)
		{
			for(int y = 0; y < densityY; y++)
			{
				for(int z = 0; z < densityZ; z++)
				{
					pointArrayOut[count]= MFloatVector(current.x + q, current.y + w, current.z + e);
					velocityArrayOut[count]= MFloatVector(0.0, 0.0, 0.0);
					massArrayOut[count] = 1.0;
					count ++;
					current += zStep;
				}
				current.z = 0.0;
				current += yStep;
			}
			current.y = 0.0;
			current += xStep;
		}
		
		if(pointExists == false)
		{
			pointDataFn.create(pointArrayOut);
		}
		if(velocityExists == false)
		{
			velocityDataFn.create(velocityArrayOut);
		}
		if(massExists == false)
		{
			massDataFn.create(massArrayOut);
		}
		pointArrayData.set(pointDataFn.object());
		velocityArrayData.set(velocityDataFn.object());
		massArrayData.set(massDataFn.object());

	}


	data.setClean(plug);
	return MS::kSuccess;
}

void* VectorVisualization::creator()
//
//	Description:
//		this method exists to give Maya a way to create new objects
//      of this type. 
//
//	Return Value:
//		a new object of this type
//
{
	return new VectorVisualization();
}
void VectorVisualization::postConstructor()
{
  MFnDependencyNode nodeFn(thisMObject());

  nodeFn.setName("VectorVisualizationShape#");
}

void VectorVisualization::draw(M3dView & view, const MDagPath & path, M3dView::DisplayStyle style, M3dView::DisplayStatus status)
{	
	//Query all the draw related plugs
	MObject thisNode = thisMObject();
	MPlug densePlugX(thisNode, densityX);
	MPlug densePlugY(thisNode, densityY);
	MPlug densePlugZ(thisNode, densityZ);
	MPlug widthPlug(thisNode, width);
	MPlug heightPlug(thisNode, height);
	MPlug depthPlug(thisNode, depth);
	MPlug drawPlug(thisNode, drawLength);

	MPlug forcePlug(thisNode, inputForce);

	//These will be set to plug value
	int densityX;
	int densityY;
	int densityZ;
	float widthVal;
	float heightVal;
	float depthVal;
	float drawVal;

	// Settings from plugs
	densePlugX.getValue(densityX);
	densePlugY.getValue(densityY);
	densePlugZ.getValue(densityZ);
	widthPlug.getValue(widthVal);
	heightPlug.getValue(heightVal);
	depthPlug.getValue(depthVal);
	drawPlug.getValue(drawVal);

	// Query force
	MObject forceObj;
	forcePlug.getValue(forceObj);
	MFnVectorArrayData forceData(forceObj);
	MVectorArray inputForceArray = forceData.array();

	// convenience code so I can just += in the for loop.
	MVector xStep = MFloatVector( (widthVal/densityX), 0.0, 0.0 );
	MVector yStep = MFloatVector( 0.0, (heightVal/densityY), 0.0 );
	MVector zStep = MFloatVector( 0.0, 0.0,(depthVal/densityZ) );
	MVector current = MFloatVector( 0.0, 0.0, 0.0 );

	MVector forceColor;
	bool forceIsNull = forceObj.isNull();


	// BEGIN DRAW STUFF
	view.beginGL();

	glPushAttrib( GL_ALL_ATTRIB_BITS);
	glClearDepth(1.0);
	glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glShadeModel(GL_SMOOTH);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glDepthMask( GL_FALSE );
	MColor theColor(1.0, 0.0, 0.0);
	float fx, fy, fz = 0.0;
	int count = 0;
	
	for(int x = 0; x < densityX; x++)
	{
		for(int y = 0; y < densityY; y++)
		{
			for(int z = 0; z < densityZ; z++)
			{
				if(forceIsNull)
				{
					glBegin( GL_LINES );
					glVertex3f( current.x, current.y, current.z);
					glVertex3f( current.x, (current.y+drawVal), current.z);
					glEnd();
				}
				else
				{

					fx = inputForceArray[count].x;
					fy = inputForceArray[count].y;
					fz = inputForceArray[count].z;

					forceColor.x = fx;
					forceColor.y = fy;
					forceColor.z = fz;
					forceColor.normalize();
					
					forceColor += MVector(1,1,1);
					forceColor /= 2;
					theColor.r = forceColor.x;
					theColor.g = forceColor.y;
					theColor.b = forceColor.z;
					theColor.a = 1.0;
					glColor4f(forceColor.x, forceColor.y, forceColor.z, 0.0f);
					glBegin( GL_LINES );
					glVertex3f( current.x - (fx / 2) * drawVal, current.y - (fy / 2) * drawVal, current.z - (fz / 2) * drawVal);
					
					//theColor.a = 0.0;
					//view.setDrawColor(theColor);
					glColor4f(forceColor.x, forceColor.y, forceColor.z, 1.0f);
					glVertex3f( current.x + (fx / 2) * drawVal, current.y + (fy / 2) * drawVal, current.z + (fz / 2) * drawVal);
					
				}
				glEnd();
				count++;
				current += zStep;
				
			}
			current += yStep;
			current.z = 0.0;
		}
		current += xStep;
		current.y = 0.0;
	}

	glPopAttrib();
	view.endGL();
}