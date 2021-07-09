#include "HandlingMeta.h"

HandlingMeta* HandlingMeta::m_inst = NULL;

HandlingMeta* HandlingMeta::inst( void ) {
	if( !m_inst )
		m_inst = new HandlingMeta();

	return m_inst;
}

HandlingMeta::HandlingMeta() : Meta( L"handling.meta" ) {
	// Ensure required root node
	pugi::xml_node root = metaFile.child( "CHandlingDataMgr" );

	if( !root ) {
		root = metaFile.append_child( "CHandlingDataMgr" );
	}

	// Ensure nodes used in merge
	handlingData = root.child( "HandlingData" );

	if( !handlingData ) {
		handlingData = root.append_child( "HandlingData" );
	}
}

void HandlingMeta::addHandlingData( pugi::xml_node item ) {
	handlingData.prepend_copy( item );
}

void HandlingMeta::merge( const wchar_t* fileToMerge, pugi::xml_document* newdoc ) {
	// Required vehicle handling data
	pugi::xml_node vehicles = (*newdoc).child( "CHandlingDataMgr" ).child( "HandlingData" );

	if( !vehicles ) {
		char err[512];
		sprintf( err, "Error merging '%ls': Incompatible DOM: Expected to find 'CHandlingDataMgr > HandlingData', none found. Look at the XML of '%ls' before continuing or delete the file to cancel.", getFilePath(), fileToMerge );
		throw std::exception( err );
	}

	for( pugi::xml_node item : vehicles.children() ) {
		printf( "%ls: Merging vehicle '%s'\n", getFilePath(), item.child( "handlingName" ).child_value() );
		addHandlingData( item );
	}
}