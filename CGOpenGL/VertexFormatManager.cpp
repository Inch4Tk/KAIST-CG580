#include "VertexFormatManager.h"


std::unordered_map<std::string, VertexFormat> VertexFormatManager::formats = 
	std::unordered_map<std::string, VertexFormat>();


VertexFormatManager::VertexFormatManager()
{
}


VertexFormatManager::~VertexFormatManager()
{
}

/// <summary>
/// Gets the vertex format with just 3 standard floats
/// </summary>
/// <returns></returns>
const VertexFormat& VertexFormatManager::Get3F()
{
	if( formats.find( "3F" ) != formats.end() )
		return formats["3F"];

	VertexFormat form;
	form.bytesize = 12;
	form.sizes.push_back( 3 );
	form.types.push_back( GL_FLOAT );
	form.offsets.push_back( 0 );
	form.asInt.push_back( false );
	form.normalized.push_back( false );

	formats["3F"] = form;
	return formats["3F"];
}
