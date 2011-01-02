
#include <maya/MFnPlugin.h>
#include "ModelExporter.h"

#pragma comment(lib, "OpenMaya.lib")
#pragma comment(lib, "Foundation.lib")
#pragma comment(lib, "OpenMayaAnim.lib")


PLUGIN_EXPORT
MStatus initializePlugin( MObject obj )
{ 
	MFnPlugin plugin( obj, "Milan Davidovic", "1.0", "Any");

	MStatus status =  plugin.registerFileTranslator("Dr. Shader's Model Exporter", "", ModelExporter::creator, "", "option1=1", true);
	if(!status)
	{
		status.perror("Model exporter registration failed.");
		return status;
	}

	return status;
}

PLUGIN_EXPORT
MStatus uninitializePlugin( MObject obj )
{
	MStatus   status;
	MFnPlugin plugin( obj );

	status =  plugin.deregisterFileTranslator("Dr. Shader's Model Exporter");
	if(!status)
	{
		status.perror("Model exporter deregistration failed.");
		return status;
	}

	return status;
}
