#include "VehiclesMeta.h"

VehiclesMeta* VehiclesMeta::m_inst = NULL;

VehiclesMeta* VehiclesMeta::inst( void ) {
	if( !m_inst )
		m_inst = new VehiclesMeta();

	return m_inst;
}

VehiclesMeta::VehiclesMeta() : Meta( L"vehicles.meta" ) {
	// Ensure required root node
	pugi::xml_node root = metaFile.child( "CVehicleModelInfo__InitDataList" );

	if( !root ) {
		root = metaFile.append_child( "CVehicleModelInfo__InitDataList" );
	}

	// Ensure nodes used in merge
	initDatas = root.child( "InitDatas" );
	relationships = root.child( "txdRelationships" );

	if( !initDatas ) {
		initDatas = root.append_child( "InitDatas" );
	}

	if( !relationships ) {
		relationships = root.append_child( "txdRelationships" );
	}

	// Ensure other nodes
	pugi::xml_node resident = root.child( "residentTxd" );
	pugi::xml_node residentAnims = root.child( "residentAnims" );

	if( !residentAnims ) {
		root.prepend_child( "residentAnims" );
	}

	if( !resident ) {
		resident = root.prepend_child( "residentTxd" );
		resident.append_child( pugi::node_pcdata ).set_value( "vehshare" );
	}
}

void VehiclesMeta::addVehicle( pugi::xml_node item ) {
	initDatas.prepend_copy( item );
}

void VehiclesMeta::addRelationship( pugi::xml_node item ) {
	relationships.prepend_copy( item );
}

void VehiclesMeta::merge( const wchar_t* fileToMerge, pugi::xml_document* newdoc ) {
	// Required vehicle data
	pugi::xml_node vehicles = (*newdoc).child( "CVehicleModelInfo__InitDataList" ).child( "InitDatas" );

	if( !vehicles ) {
		char err[512];
		sprintf( err, "Error merging '%ls': Incompatible DOM: Expected to find 'CVehicleModelInfo__InitDataList > InitDatas', none found. Look at the XML of '%ls' before continuing or delete the file to cancel.", getFilePath(), fileToMerge );
		throw std::exception( err );
	}

	for( pugi::xml_node item : vehicles.children() ) {
		printf( "%ls: Merging vehicle '%s'\n", getFilePath(), item.child( "modelName" ).child_value() );
		addVehicle( item );
	}

	// Optional relationship data
	pugi::xml_node relationships = (*newdoc).child( "CVehicleModelInfo__InitDataList" ).child( "txdRelationships" );

	if( relationships ) {
		for( pugi::xml_node item : relationships.children() ) {
			addRelationship( item );
		}
	}
}