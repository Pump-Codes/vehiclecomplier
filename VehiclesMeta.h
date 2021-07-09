#pragma once
#include "Meta.h"

class VehiclesMeta : public Meta {
public:
	static VehiclesMeta *m_inst;

public:
	static VehiclesMeta* inst( void );

	using Meta::merge;

	void addVehicle( pugi::xml_node item );
	void addRelationship( pugi::xml_node item );
	void merge( const wchar_t* fileToMerge, pugi::xml_document* newdoc );

private:
	VehiclesMeta();

private:
	pugi::xml_node initDatas;
	pugi::xml_node relationships;
};