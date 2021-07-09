#pragma once
#include "pugixml.hpp"

class Meta {
public:
	const wchar_t* getFilePath( void );
	void merge( const char* fileToMerge );
	void merge( const wchar_t* fileToMerge );
	void save( void );

protected:
	Meta( const wchar_t* filePath );

	virtual void merge( const wchar_t* fileToMerge, pugi::xml_document* newdoc ) = 0;

protected:
	pugi::xml_document metaFile;

	const wchar_t* metaFilePath;
};