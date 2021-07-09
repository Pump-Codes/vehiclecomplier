#pragma once
#include "Meta.h"

class HandlingMeta : public Meta {
public:
	static HandlingMeta *m_inst;

public:
	static HandlingMeta* inst( void );

	using Meta::merge;

	void addHandlingData( pugi::xml_node item );
	void merge( const wchar_t* fileToMerge, pugi::xml_document* newdoc );

private:
	HandlingMeta();

private:
	pugi::xml_node handlingData;
};