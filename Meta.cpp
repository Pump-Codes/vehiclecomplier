#include "Meta.h"
#include <exception>

Meta::Meta( const wchar_t* filePath ) {
	metaFilePath = filePath;

	pugi::xml_parse_result res = metaFile.load_file( filePath );
}

const wchar_t* Meta::getFilePath( void ) {
	return metaFilePath;
}

void Meta::merge( const char* fileToMerge ) {
	pugi::xml_document newdoc;
	pugi::xml_parse_result res = newdoc.load_file( fileToMerge );

	if( !res ) {
		char message[512] = { 0 };
		sprintf( message, "Unable to load '%s'. Error: %s\n\nOffset: %d\nUse this offset in Notepad++ by pressing CTRL+G and selecting 'offset' to fix XML syntax error manually before continuing.", fileToMerge, res.description(), res.offset );

		throw std::exception( message );
	}

	wchar_t filePathW[160] = { 0 };
	mbstowcs( filePathW, fileToMerge, 159 );

	merge( filePathW, &newdoc );
}

void Meta::merge( const wchar_t* fileToMerge ) {
	pugi::xml_document newdoc;
	pugi::xml_parse_result res = newdoc.load_file( fileToMerge );

	if( !res ) {
		char message[512] = { 0 };
		sprintf( message, "Unable to load '%ls'. Error: %s\n\nOffset: %d\nUse this offset in Notepad++ by pressing CTRL+G and selecting 'offset' to fix XML syntax error manually before continuing.", fileToMerge, res.description(), res.offset );

		throw std::exception( message );
	}

	merge( fileToMerge, &newdoc );
}

void Meta::save( void ) {
	metaFile.save_file( getFilePath() );
}