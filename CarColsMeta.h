#pragma once
#include "Meta.h"

class CarColsMeta : public Meta {
public:
	static CarColsMeta *m_inst;

public:
	static CarColsMeta* inst( void );

	using Meta::merge;

	void renumerate( int index, const char* modelName, const char* oldModKitName, const char* newModKitName );
	void addKit( pugi::xml_node item );
	void addLight( pugi::xml_node item );
	void merge( const wchar_t* fileToMerge, pugi::xml_document* newdoc );

private:
	CarColsMeta();

private:
	pugi::xml_node kits;
	pugi::xml_node lights;
};