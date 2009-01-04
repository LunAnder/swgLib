/** -*-c++-*-
 *  \class  base
 *  \file   base.cpp
 *  \author Kenneth R. Sewell III

 meshLib is used for the parsing and exporting .msh models.
 Copyright (C) 2006-2009 Kenneth R. Sewell III

 This file is part of meshLib.

 meshLib is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.

 meshLib is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with meshLib; if not, write to the Free Software
 Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include <meshLib/base.hpp>
#include <iostream>
#include <cstdlib>

using namespace ml;

std::string base::getType( std::istream &file )
{
  std::string form;
  unsigned int size;
  std::string type;
    
  // Get current file position...
  unsigned int position = file.tellg();
    
  // Read first FORM
  readFormHeader( file, form, size, type );
    
  // Reposition file to original location
  file.seekg( position, std::ios_base::beg );
    
  if( "FORM" == form )
    {
      return type;
    }
  else
    {
      unsigned int x;
      file.read( (char*)&x, sizeof( x ) );

      // Reposition file to original location
      file.seekg( position, std::ios_base::beg );

      // .str string file
      if( x == 0xabcd )
	{
	  return std::string("ABCD");
	}

      return std::string("");
    }
}

bool base::isOfType( std::istream &file, const std::string &Type )
{
  std::string form;
  unsigned int size;
  std::string type;

  // Get current file position...
  unsigned int position = file.tellg();

  // Read first FORM
  readFormHeader( file, form, size, type );

  // Reposition file to original location
  file.seekg( position, std::ios_base::beg );

  return( "FORM" == form && Type == type );
}

unsigned char base::readBigEndian( std::istream &file,
				   const unsigned int &size,
				   char *buffer
				   )
{
#if BYTE_ORDER == LITTLE_ENDIAN
  for( unsigned int i=0; i<size; ++i )
    {
      file.read( &(buffer[size -1 - i]), 1 );
    }
#else
  file.read( buffer, size );
#endif
    
  return size;
}

unsigned char base::writeBigEndian( std::ofstream &file,
				    const unsigned int &size,
				    char *buffer
				    )
{
#if BYTE_ORDER == LITTLE_ENDIAN
  for( unsigned int i=0; i<size; ++i )
    {
      file.write( &(buffer[size -1 - i]), 1 );
    }
#else
  file.write( buffer, size );
#endif
    
  return size;
}

unsigned int base::readRecordHeader( std::istream &file,
				     std::string &type,
				     unsigned int &size )
{
  char tempType[5];
  file.read( tempType, 4 );
  tempType[4] = 0;
  type = tempType;
  readBigEndian( file, sizeof( size ), (char *)&size );

  return 8;
}

unsigned int base::writeRecordHeader( std::ofstream &file,
				      const std::string &type,
				      const unsigned int &size )
{
  file.write( type.c_str(), 4 );
  writeBigEndian( file, sizeof( size ), (char *)&size );

  return 8;
}

unsigned int base::readFormHeader( std::istream &file,
				   std::string &form,
				   unsigned int &size,
				   std::string &type )
{
  unsigned total = 0;
  total += readRecordHeader( file,
			     form,
			     size );
  char tempType[5];
  file.read( tempType, 4 );
  total += 4;

  tempType[4] = 0;
  type = tempType;

  return total;
}

unsigned int base::writeFormHeader( std::ofstream &file,
				    const std::string &form,
				    const unsigned int &size,
				    const std::string &type )
{
  unsigned total = 0;
  total += writeRecordHeader( file,
			      form,
			      size );

  file.write( type.c_str(), 4 );
  total += 4;

  return total;
}

unsigned int base::readUnknown( std::istream &file,
				const unsigned int size )
{
  for( unsigned int i = 0; i < size; ++i )
    {
      unsigned char data;
      file.read( (char*)&data, 1 );
      if(
	 ( data >= '.' && data <= 'z' )
	 || ( data == '\\' )
	 )
        {
	  std::cout << data;
        }
      else
        {
	  std::cout<<std::hex<<"0x"<<(unsigned int)data<<" "<< std::dec;
        }
    }
  std::cout << std::endl;
  return size;
}

unsigned int base::readBOX( std::istream &file,
			    float &x1, float &y1, float &z1,
			    float &x2, float &y2, float &z2
			    )
{
  unsigned int total = 0;

  std::string form;
  unsigned int size;
  std::string type;

  total += readRecordHeader( file, type, size );
  size += 8; // Size of header
  if( type != "BOX " )
    {
      std::cout << "Expected record of type BOX: " << type << std::endl;
      exit( 0 );
    }
  std::cout << "Found BOX record" << std::endl;

  // Read center and radius of bounding sphere
  file.read( (char*)&x1, sizeof( float ) );
  file.read( (char*)&y1, sizeof( float ) );
  file.read( (char*)&z1, sizeof( float ) );
  file.read( (char*)&x2, sizeof( float ) );
  file.read( (char*)&y2, sizeof( float ) );
  file.read( (char*)&z2, sizeof( float ) );
  total += sizeof( float ) * 6;

  std::cout << "Bounding box corners: " << std::endl;
  std::cout << "   " << x1 << ", " << y1 << ", " << z1 << std::endl;
  std::cout << "   " << x2 << ", " << y2 << ", " << z2 << std::endl;

  if( total == size )
    {
      std::cout << "Finished reading BOX." << std::endl;
    }
  else
    {
      std::cout << "Error reading BOX!" << std::endl;
      std::cout << "Read " << total << " out of " << size
		<< std::endl;
    }

  return total;
}

unsigned int base::readSPHR( std::istream &file,
			     float &cx, float &cy, float &cz,
			     float &radius
			     )
{
  unsigned int total = 0;

  std::string form;
  unsigned int size;
  std::string type;

  total += readRecordHeader( file, type, size );
  size += 8; // Size of header
  if( type != "SPHR" )
    {
      std::cout << "Expected record of type SPHR: " << type << std::endl;
      exit( 0 );
    }
  std::cout << "Found SPHR record" << std::endl;

  // Read center and radius of bounding sphere
  file.read( (char*)&cx, sizeof( float ) );
  file.read( (char*)&cy, sizeof( float ) );
  file.read( (char*)&cz, sizeof( float ) );
  file.read( (char*)&radius, sizeof( float ) );
  total += sizeof( float ) * 4;

  std::cout << "Bounding sphere center and radius: " << std::endl;
  std::cout << "  ( " << cx << ", " << cy << ", " << cz
	    << " ) ( " << radius << " ) " << std::endl;

  if( total == size )
    {
      std::cout << "Finished reading SPHR." << std::endl;
    }
  else
    {
      std::cout << "Error reading SPHR!" << std::endl;
      std::cout << "Read " << total << " out of " << size
		<< std::endl;
    }

  return total;
}

unsigned int base::readEXSP( std::istream &file,
			     float &cx, float &cy, float &cz,
			     float &radius
			     )
{
  unsigned int total = 0;

  std::string form;
  unsigned int exspSize;
  std::string type;

  total += readFormHeader( file, form, exspSize, type );
  exspSize += 8;
  if( form != "FORM" || type != "EXSP" )
    {
      std::cout << "Expected Form of type EXSP: " << type << std::endl;
      exit( 0 );
    }
  std::cout << "Found EXSP form" << std::endl;

  unsigned int size;
  total += readFormHeader( file, form, size, type );
  if( form != "FORM" || type != "0001" )
    {
      std::cout << "Expected Form of type 0001: " << type << std::endl;
      exit( 0 );
    }
  std::cout << "Found 0001 form" << std::endl;

  total += base::readSPHR( file, cx, cy, cz, radius );

  if( total == exspSize )
    {
      std::cout << "Finished reading EXSP." << std::endl;
    }
  else
    {
      std::cout << "Error reading EXSP!" << std::endl;
      std::cout << "Read " << total << " out of " << exspSize
		<< std::endl;
    }

  return total;
}

unsigned int base::readEXBX( std::istream &file,
			     float &cx, float &cy, float &cz,
			     float &radius,
			     float &x1, float &y1, float &z1,
			     float &x2, float &y2, float &z2
			     )
{
  unsigned int total = 0;

  std::string form;
  unsigned int exbxSize;
  std::string type;

  unsigned int position = file.tellg();
  total += readFormHeader( file, form, exbxSize, type );
  exbxSize += 8;
  if( form != "FORM" || type != "EXBX" )
    {
      std::cout << "Expected Form of type EXBX: " << type << std::endl;
      file.seekg( position, std::ios_base::beg );
      return 0;
    }
  std::cout << "Found EXBX form" << std::endl;

  unsigned int size;
  total += readFormHeader( file, form, size, type );
  if( form != "FORM" || type != "0001" )
    {
      std::cout << "Expected Form of type 0001: " << type << std::endl;
      exit( 0 );
    }
  std::cout << "Found 0001 form" << std::endl;

  total += base::readEXSP( file, cx, cy, cz, radius );
  total += base::readBOX( file, x1, y1, z1, x2, y2, z2 );

  if( total == exbxSize )
    {
      std::cout << "Finished reading EXBX." << std::endl;
    }
  else
    {
      std::cout << "Error reading EXBX!" << std::endl;
      std::cout << "Read " << total << " out of " << exbxSize
		<< std::endl;
    }

  return total;
}

unsigned int base::readMatrixAndPosition( std::istream &file,
					  matrix3 &matrix,
					  vector3 &position
					  )
{
  unsigned total = 0;
  
  float x, y, z;
  float v[9];
  
  // First row of matrix
  file.read( (char *)v, sizeof( float ) * 3 );
  total += sizeof( float ) * 3;
  
  // X position
  file.read( (char *)&x, sizeof( x ) );
  total += sizeof( x );
  
  // Second row of matrix
  file.read( (char *)(v+3), sizeof( float ) * 3 );
  total += sizeof( float ) * 3;
  
  // Y position
  file.read( (char *)&y, sizeof( y ) );
  total += sizeof( y );
  
  // Third row of matrix
  file.read( (char *)(v+6), sizeof( float ) * 3 );
  total += sizeof( float ) * 3;
  
  // Y position
  file.read( (char *)&z, sizeof( z ) );
  total += sizeof( z );

  matrix.set( v );
  position.set( x, y, z );

  return total;
}

unsigned int base::readIDTL( std::istream &file,
			     std::vector<vector3> &vec,
			     std::vector<unsigned int> &index
			     )
{
  unsigned int total = 0;

  std::string form;
  unsigned int idtlSize;
  std::string type;

  total += readFormHeader( file, form, idtlSize, type );
  idtlSize += 8;
  if( form != "FORM" || type != "IDTL" )
    {
      std::cout << "Expected FORM of type IDTL: " << type << std::endl;
      exit( 0 );
    }
  std::cout << "Found IDTL FORM" << std::endl;

  unsigned int size;
  total += readFormHeader( file, form, size, type );
  if( form != "FORM" )
    {
      std::cout << "Expected FORM: " << form << std::endl;
      exit( 0 );
    }
  std::cout << "Found FORM" << std::endl;

  total += readVERT( file, vec );
  total += readINDX( file, index );

  if( idtlSize == total )
    {
      std::cout << "Finished reading IDTL" << std::endl;
    }
  else
    {
      std::cout << "Failed in reading IDTL" << std::endl;
      std::cout << "Read " << total << " out of " << idtlSize
		<< std::endl;
    }
    
  return total;
}


unsigned int base::readVERT( std::istream &file,
			     std::vector<vector3> &vec
			     )
{
  unsigned int total = 0;
  
  std::string form;
  unsigned int vertSize;
  std::string type;
  
  total += readRecordHeader( file, type, vertSize );
  vertSize += 8;
  if( type != "VERT" )
    {
      std::cout << "Expected record of type VERT: " << type << std::endl;
      exit( 0 );
    }
  std::cout << "Found VERT record" << std::endl;

  float x, y, z;
  unsigned int numVerts = (vertSize-8)/(sizeof( float ) * 3);
  for( unsigned int i = 0; i < numVerts; ++i )
    {
      file.read( (char*)&x, sizeof( x ) );
      total += sizeof( x );

      file.read( (char*)&y, sizeof( y ) );
      total += sizeof( y );

      file.read( (char*)&z, sizeof( z ) );
      total += sizeof( z );
#if 0
      std::cout << "Vert: " << std::fixed
		<< x << ", "
		<< y << ", "
		<< z << std::endl;
#endif
      vec.push_back( vector3( x, y, z ) );
    }

  if( vertSize == total )
    {
      std::cout << "Finished reading VERT" << std::endl;
    }
  else
    {
      std::cout << "Failed in reading VERT" << std::endl;
      std::cout << "Read " << total << " out of " << vertSize
		<< std::endl;
    }
    
  return total;

}

unsigned int base::readINDX( std::istream &file,
			     std::vector<unsigned int> &index )
{
  unsigned int total = 0;

  std::string form;
  unsigned int indxSize;
  std::string type;

  total += readRecordHeader( file, type, indxSize );
  indxSize += 8;
  if( type != "INDX" )
    {
      std::cout << "Expected record of type INDX: " << type << std::endl;
      exit( 0 );
    }
  std::cout << "Found INDX record" << std::endl;

  unsigned int x;
  unsigned int numIndex = (indxSize-8)/sizeof( x );
  for( unsigned int i = 0; i < numIndex; ++i )
    {
      file.read( (char*)&x, sizeof( x ) );
      total += sizeof( x );
      //std::cout << "Index: " << x << std::endl;
      index.push_back( x );
    }

  if( indxSize == total )
    {
      std::cout << "Finished reading INDX" << std::endl;
    }
  else
    {
      std::cout << "Failed in reading INDX" << std::endl;
      std::cout << "Read " << total << " out of " << indxSize
		<< std::endl;
    }
    
  return total;
}

bool base::fixSlash( std::string &filename )
{
  for( unsigned int i = 0; i < filename.size(); ++i )
    {
      if( filename[i] == '\\' )
	{
	  filename[i] = '/';
	}
    }

  return true;
}
