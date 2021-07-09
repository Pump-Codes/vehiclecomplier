#include "CarVariationsMeta.h"
#include "CarColsMeta.h"

CarVariationsMeta* CarVariationsMeta::m_inst = NULL;

CarVariationsMeta* CarVariationsMeta::inst( void ) {
	if( !m_inst )
		m_inst = new CarVariationsMeta();

	return m_inst;
}

CarVariationsMeta::CarVariationsMeta() : Meta( L"carvariations.meta" ) {
	// Ensure required root node
	pugi::xml_node root = metaFile.child( "CVehicleModelInfoVariation" );

	if( !root ) {
		root = metaFile.append_child( "CVehicleModelInfoVariation" );
	}

	// Ensure nodes used in merge
	variations = root.child( "variationData" );

	if( !variations ) {
		variations = root.append_child( "variationData" );
	}
}

void CarVariationsMeta::renumerate( void ) {
	int index = 1000;

	for( pugi::xml_node item : variations.children() ) {
		std::string modelName = item.child( "modelName" ).child_value();
		std::string newModKitName = std::to_string( index ).append( "_" ).append( modelName ).append( "_" ).append( "modkit" );
		std::string oldModKitName = item.child( "kits" ).child( "Item" ).child_value();

		item.child( "kits" ).child( "Item" ).text().set( newModKitName.c_str() );
		item.child( "lightSettings" ).attribute( "value" ).set_value( index );

		CarColsMeta::inst()->renumerate( index, modelName.c_str(), oldModKitName.c_str(), newModKitName.c_str() );

		index++;
	}
}

void CarVariationsMeta::addVariation( pugi::xml_node item ) {
	variations.prepend_copy( item );
}

void CarVariationsMeta::merge( const wchar_t* fileToMerge, pugi::xml_document* newdoc ) {
	// Required vehicle handling data
	pugi::xml_node vehicles = (*newdoc).child( "CVehicleModelInfoVariation" ).child( "variationData" );

	if( !vehicles ) {
		char err[512];
		sprintf( err, "Error merging '%ls': Incompatible DOM: Expected to find 'CVehicleModelInfoVariation > variationData', none found. Look at the XML of '%ls' before continuing or delete the file to cancel.", getFilePath(), fileToMerge );
		throw std::exception( err );
	}

	for( pugi::xml_node item : vehicles.children() ) {
		printf( "%ls: Merging vehicle '%s'\n", getFilePath(), item.child( "modelName" ).child_value() );
		addVariation( item );
	}
}