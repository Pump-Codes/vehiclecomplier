#pragma once
#include "Meta.h"

class CarVariationsMeta : public Meta {
public:
	static CarVariationsMeta *m_inst;

public:
	static CarVariationsMeta* inst( void );

	using Meta::merge;

	void renumerate( void );
	void addVariation( pugi::xml_node item );
	void merge( const wchar_t* fileToMerge, pugi::xml_document* newdoc );

private:
	CarVariationsMeta();

private:
	pugi::xml_node variations;
};