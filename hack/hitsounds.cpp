#include "../includes.h"

Hitsounds g_hitsounds;

void Hitsounds::WavHeader_t::ParseWavHeader( unsigned char* wavData ) {
	unsigned char buffer4[ 4 ];
	unsigned char buffer2[ 2 ];

	unsigned char* walker = wavData;

	// Retreive riff
	memcpy( riff, walker, sizeof( riff ) );
	walker += sizeof( riff );

	// overall_size
	memcpy( buffer4, walker, sizeof( buffer4 ) );
	walker += sizeof( buffer4 );

	// Convert little endian to big endian 4 byte int
	overall_size = buffer4[ 0 ] | ( buffer4[ 1 ] << 8 ) | ( buffer4[ 2 ] << 16 ) | ( buffer4[ 3 ] << 24 );

	// wave
	memcpy( wave, walker, sizeof( wave ) );
	walker += sizeof( wave );

	// fmt_chunk_marker
	memcpy( fmt_chunk_marker, walker, sizeof( fmt_chunk_marker ) );
	walker += sizeof( fmt_chunk_marker );

	// length_of_fmt
	memcpy( buffer4, walker, sizeof( buffer4 ) );
	walker += sizeof( buffer4 );

	length_of_fmt = buffer4[ 0 ] | ( buffer4[ 1 ] << 8 ) | ( buffer4[ 2 ] << 16 ) | ( buffer4[ 3 ] << 24 );

	// format_type
	memcpy( buffer2, walker, sizeof( buffer2 ) );
	walker += sizeof( buffer2 );

	format_type = buffer2[ 0 ] | ( buffer2[ 1 ] << 8 );

	// channels
	memcpy( buffer2, walker, sizeof( buffer2 ) );
	walker += sizeof( buffer2 );

	channels = buffer2[ 0 ] | ( buffer2[ 1 ] << 8 );

	// sample_rate
	memcpy( buffer4, walker, sizeof( buffer4 ) );
	walker += sizeof( buffer4 );

	sample_rate = buffer4[ 0 ] | ( buffer4[ 1 ] << 8 ) | ( buffer4[ 2 ] << 16 ) | ( buffer4[ 3 ] << 24 );

	// byterate
	memcpy( buffer4, walker, sizeof( buffer4 ) );
	walker += sizeof( buffer4 );

	byterate = buffer4[ 0 ] | ( buffer4[ 1 ] << 8 ) | ( buffer4[ 2 ] << 16 ) | ( buffer4[ 3 ] << 24 );

	// block_align
	memcpy( buffer2, walker, sizeof( buffer2 ) );
	walker += sizeof( buffer2 );

	block_align = buffer2[ 0 ] | ( buffer2[ 1 ] << 8 );

	// bits_per_sample
	memcpy( buffer2, walker, sizeof( buffer2 ) );
	walker += sizeof( buffer2 );

	bits_per_sample = buffer2[ 0 ] | ( buffer2[ 1 ] << 8 );

	// data_chunk_header
	memcpy( data_chunk_header, walker, sizeof( data_chunk_header ) );
	walker += sizeof( data_chunk_header );

	// data_size
	memcpy( buffer4, walker, sizeof( buffer4 ) );
	walker += sizeof( buffer4 );

	data_size = buffer4[ 0 ] | ( buffer4[ 1 ] << 8 ) | ( buffer4[ 2 ] << 16 ) | ( buffer4[ 3 ] << 24 );

	data = walker;
}

bool Hitsounds::AdjustHitsoundVolume( Hitsounds::WavHeader_t& header, float volume )
{
	// We can only adjust PMC (Uncompressed) WAV files
	if( header.format_type != 1 )
		return false;

	// Adjust volume to correct ratio
	float finalVolume = std::clamp( volume, 0.f, 1.f );

	// Loop through all samples and adjust their volumes
	int numOfSamples = header.data_size / header.channels;
	unsigned char* walker = header.data;

	for( int i = 0; i < numOfSamples; i++ ) {
		switch( header.channels ) {
			case sizeof( unsigned char ) :
				// Mono
				*walker = ( unsigned char )( *walker * finalVolume );
				walker += sizeof( unsigned char );
				break;
				case sizeof( short ) :
					// Stereo
					*( short* )walker = ( short )( *( short* )walker * finalVolume );
					walker += sizeof( short );
					break;
					case sizeof( int ) :
						// Surround?
						*( int* )walker = ( int )( *( int* )walker * finalVolume );
						walker += sizeof( int );
						break;
					default:
						return false;
		}
	}

	return true;
}