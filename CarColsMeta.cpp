#include "CarColsMeta.h"

CarColsMeta* CarColsMeta::m_inst = NULL;

CarColsMeta* CarColsMeta::inst( void ) {
	if( !m_inst )
		m_inst = new CarColsMeta();

	return m_inst;
}

CarColsMeta::CarColsMeta() : Meta( L"carcols.meta" ) {
	// Ensure required root node
	pugi::xml_node root = metaFile.child( "CVehicleModelInfoVarGlobal" );

	if( !root ) {
		root = metaFile.append_child( "CVehicleModelInfoVarGlobal" );
	}

	// Ensure nodes used in merge
	kits = root.child( "Kits" );
	lights = root.child( "Lights" );

	if( !kits ) {
		kits = root.append_child( "Kits" );
	}

	if( !lights ) {
		lights = root.append_child( "Lights" );
	}
}

void CarColsMeta::renumerate( int index, const char* modelName, const char* oldModKitName, const char* newModKitName ) {
	for( pugi::xml_node item : kits.children() ) {
		const char* tmp = item.child( "kitName" ).text().get();

		if( strcmpi( item.child( "kitName" ).text().get(), oldModKitName ) != 0 )
			continue;

		item.child( "kitName" ).text().set( newModKitName );
		item.child( "id" ).attribute( "value" ).set_value( index );
	}

	for( pugi::xml_node item : lights.children() ) {
		if( strcmpi( item.child( "name" ).text().get(), modelName ) != 0 )
			continue;

		item.child( "id" ).attribute( "value" ).set_value( index );
	}
}

void CarColsMeta::addKit( pugi::xml_node item ) {
	kits.prepend_copy( item );
}

void CarColsMeta::addLight( pugi::xml_node item ) {
	lights.prepend_copy( item );
}

void CarColsMeta::merge( const wchar_t* fileToMerge, pugi::xml_document* newdoc ) {
	// Required vehicle data
	pugi::xml_node kits = (*newdoc).child( "CVehicleModelInfoVarGlobal" ).child( "Kits" );

	if( !kits ) {
		char err[512];
		sprintf( err, "Error merging '%ls': Incompatible DOM: Expected to find 'CVehicleModelInfoVarGlobal > Kits', none found. Look at the XML of '%ls' before continuing or delete the file to cancel.", getFilePath(), fileToMerge );
		throw std::exception( err );
	}

	for( pugi::xml_node item : kits.children() ) {
		printf( "%ls: Merging vehicle '%s'\n", getFilePath(), item.child( "kitName" ).child_value() );
		addKit( item );
	}

	// Optional relationship data
	pugi::xml_node lights = (*newdoc).child( "CVehicleModelInfoVarGlobal" ).child( "Lights" );

	if( lights ) {
		for( pugi::xml_node item : lights.children() ) {
			addLight( item );
		}
	}
}