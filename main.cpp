#include <Windows.h>
#include <stdio.h>
#include <exception>
#include "pugixml.hpp"

#include "VehiclesMeta.h"
#include "HandlingMeta.h"
#include "CarVariationsMeta.h"
#include "CarColsMeta.h"

static BOOL WINAPI abortHandler( DWORD signal ) {
	CarVariationsMeta::inst()->renumerate();

	VehiclesMeta::inst()->save();
	HandlingMeta::inst()->save();
	CarVariationsMeta::inst()->save();
	CarColsMeta::inst()->save();

	return FALSE;
}

void scanTrunk( const wchar_t* dir ) {
	wchar_t path[512];
	swprintf( path, L"%s\\*", dir );

	WIN32_FIND_DATA fdata;
	HANDLE hnd = FindFirstFile( path, &fdata );

	if( hnd == INVALID_HANDLE_VALUE )
		return;

	do {
		const wchar_t* name = fdata.cFileName;

		if( wcscmp( name, L"." ) == 0 || wcscmp( name, L".." ) == 0 )
			continue;

		wchar_t filepath[512];
		swprintf( filepath, L"%s\\%s", dir, name );

		if( wcscmp( name, L"vehicles.meta" ) == 0 ) {
			VehiclesMeta::inst()->merge( filepath );
			DeleteFile( filepath );
		}
		else if( wcscmp( name, L"handling.meta" ) == 0 ) {
			HandlingMeta::inst()->merge( filepath );
			DeleteFile( filepath );
		}
		else if( wcscmp( name, L"carvariations.meta" ) == 0 ) {
			CarVariationsMeta::inst()->merge( filepath );
			DeleteFile( filepath );
		}
		else if( wcscmp( name, L"carcols.meta" ) == 0 ) {
			CarColsMeta::inst()->merge( filepath );
			DeleteFile( filepath );
		}

		if( fdata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) {
			scanTrunk( filepath );
		}
	}
	while( FindNextFile( hnd, &fdata ) );

	FindClose( hnd );
}

int main( int argc, char* args[] ) {
	SetConsoleCtrlHandler( abortHandler, TRUE );

	CreateDirectory( L"_trunk", NULL );

	while( true ) {
		Sleep( 150 );

		try {
			scanTrunk( L"_trunk" );
		}
		catch( std::exception &e ) {
			printf( "%s\n\n", e.what() );
			system( "PAUSE" );
		}
	}

	return 0;
}