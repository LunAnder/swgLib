/** -*-c++-*-
 *  \class  shot
 *  \file   shot.cpp
 *  \author Kenneth R. Sewell III

 meshLib is used for the parsing and exporting .msh models.
 Copyright (C) 2006,2007 Kenneth R. Sewell III

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

/**
   This class still needs work.
 */

#include <meshLib/base.hpp>
#include <meshLib/shot.hpp>

#include <iostream>
#include <cstdlib>

using namespace ml;

shot::shot()
{
}

shot::~shot()
{
}

unsigned int shot::readSHOT( std::istream &file )
{
    unsigned int total = 0;
    std::string form;
    unsigned int shotSize;
    std::string type;

    total += readFormHeader( file, form, shotSize, type );
    shotSize += 8;
    if( form != "FORM" || type != "SHOT" )
    {
	std::cout << "Expected Form of type SHOT: " << type << std::endl;
	exit( 0 );
    }
    std::cout << "Found SHOT form" << std::endl;

    // Peek at next record, but keep file at same place.
    unsigned int position = file.tellg();
    unsigned int size;
    readFormHeader( file, form, size, type );
    file.seekg( position, std::ios_base::beg );
    
    if( "DERV" == form )
      {
	total += readDERV( file, shotBaseObjectFilename );
      }

    total += readFormHeader( file, form, size, type );
    if( form != "FORM" )
    {
	std::cout << "Expected Form" << std::endl;
	exit( 0 );
    }
    std::cout << "Found form of type: " << type<< std::endl;

    total += readPCNT( file, numAttributes );
    
    for( unsigned int i = 0; i < numAttributes; ++i )
      {
	total += readXXXX( file );
      }

    if( shotSize == total )
    {
	std::cout << "Finished reading SHOT" << std::endl;
    }
    else
    {
	std::cout << "FAILED in reading SHOT" << std::endl;
	std::cout << "Read " << total << " out of " << shotSize
                  << std::endl;
     }

    return total;
}

unsigned int shot::readDERV( std::istream &file, std::string &filename )
{
    unsigned int total = 0;
    std::string form;
    unsigned int dervSize;
    std::string type;

    total += readFormHeader( file, form, dervSize, type );
    dervSize += 8;
    if( form != "FORM" || type != "DERV" )
    {
	std::cout << "Expected Form of type DERV: " << type << std::endl;
	exit( 0 );
    }
    std::cout << "Found DERV form" << std::endl;

    unsigned int xxxxSize;
    total += readRecordHeader( file, type, xxxxSize );
    if( type != "XXXX" )
    {
        std::cout << "Expected record of type XXXX: " << type << std::endl;
        exit( 0 );
    }
    std::cout << "Found " << type << std::endl;

    char temp[255];
    file.getline( temp, 255, 0 );
    filename = temp;
    total += filename.size() + 1;

    if( dervSize == total )
    {
	std::cout << "Finished reading DERV" << std::endl;
    }
    else
    {
	std::cout << "FAILED in reading DERV" << std::endl;
	std::cout << "Read " << total << " out of " << dervSize
                  << std::endl;
     }

    return total;
}


void shot::print() const
{
}

unsigned int shot::readPCNT( std::istream &file, unsigned int &num )
{
    unsigned int total = 0;

    std::string form;
    unsigned int pcntSize;
    std::string type;

    total += readRecordHeader( file, type, pcntSize );
    if( type != "PCNT" )
    {
        std::cout << "Expected record of type PCNT: " << type << std::endl;
        exit( 0 );
    }
    std::cout << "Found " << type << std::endl;

    if( 4 != pcntSize )
      {
        std::cout << "Expected size 4: " << pcntSize << std::endl;
        exit( 0 );
      }
    pcntSize += 8;

    file.read( (char *)&num, sizeof( num ) );
    total += sizeof( num );
    std::cout << "num: " << num << std::endl;

    if( pcntSize == total )
    {
        std::cout << "Finished reading PCNT" << std::endl;
    }
    else
    {
        std::cout << "FAILED in reading PCNT" << std::endl;
        std::cout << "Read " << total << " out of " << pcntSize
                  << std::endl;
    }

    return total;
}

unsigned int shot::readXXXX( std::istream &file )
{
    unsigned int total = 0;

    std::string form;
    unsigned int xxxxSize;
    std::string type;

    total += readRecordHeader( file, type, xxxxSize );
    if( type != "XXXX" )
    {
        std::cout << "Expected record of type XXXX: " << type << std::endl;
        exit( 0 );
    }
    std::cout << "Found " << type << std::endl;

    char temp[255];
    file.getline( temp, 255, 0 );
    std::string property( temp );
    std::cout << "Property: " << property << std::endl;
    total += property.size() + 1;

    unsigned char enabled;
    unsigned char junk;

    if( property == "objectName" )
      {
	file.read( (char *)&enabled, 1 ); ++total;
	if( enabled > 0 )
	  {
	    file.read( (char *)&junk, 1 ); ++total;
	    file.getline( temp, 255, 0 );
	    std::string tempS( temp );
	    total += tempS.size() + 1;
	    objectName.push_back( tempS );
	    
	    file.read( (char *)&junk, 1 ); ++total;
	    file.getline( temp, 255, 0 );
	    tempS = temp;
	    total += tempS.size() + 1;
	    objectName.push_back( tempS );

	    std::cout << property << ": "
		      << objectName[0]
		      << " "
		      << objectName[1]
		      << std::endl;
	  }
      }
    else if( property == "detailedDescription" )
      {
	file.read( (char *)&enabled, 1 ); ++total;
	if( enabled > 0 )
	  {
	    file.read( (char *)&junk, 1 ); ++total;
	    file.getline( temp, 255, 0 );
	    std::string tempS( temp );
	    total += tempS.size() + 1;
	    detailedDescription.push_back( tempS );
	    
	    file.read( (char *)&junk, 1 ); ++total;
	    file.getline( temp, 255, 0 );
	    tempS = temp;
	    total += tempS.size() + 1;
	    detailedDescription.push_back( tempS );

	    std::cout << property << ": "
		      << detailedDescription[0]
		      << " "
		      << detailedDescription[1]
		      << std::endl;
	  }
      }
    else if( property == "lookAtText" )
      {
	file.read( (char *)&enabled, 1 ); ++total;
	if( enabled > 0 )
	  {
	    file.read( (char *)&junk, 1 ); ++total;
	    file.getline( temp, 255, 0 );
	    std::string tempS( temp );
	    total += tempS.size() + 1;
	    lookAtText.push_back( tempS );
	    
	    file.read( (char *)&junk, 1 ); ++total;
	    file.getline( temp, 255, 0 );
	    tempS = temp;
	    total += tempS.size() + 1;
	    lookAtText.push_back( tempS );

	    std::cout << property << ": "
		      << lookAtText[0]
		      << " "
		      << lookAtText[1]
		      << std::endl;
	  }
      }
    else if( property == "snapToTerrain" )
      {
	file.seekg( xxxxSize - (property.size() + 1 ), std::ios_base::cur );
	total += xxxxSize - ( property.size() + 1 );
      }
    else if( property == "containerType" )
      {
	file.seekg( xxxxSize - (property.size() + 1 ), std::ios_base::cur );
	total += xxxxSize - ( property.size() + 1 );
      }
    else if( property == "containerVolumeLimit" )
      {
	file.seekg( xxxxSize - (property.size() + 1 ), std::ios_base::cur );
	total += xxxxSize - ( property.size() + 1 );
      }
    else if( property == "tintPalette" )
      {
	file.seekg( xxxxSize - (property.size() + 1 ), std::ios_base::cur );
	total += xxxxSize - ( property.size() + 1 );
      }
    else if( property == "slotDescriptorFilename" )
      {
	file.seekg( xxxxSize - (property.size() + 1 ), std::ios_base::cur );
	total += xxxxSize - ( property.size() + 1 );
      }
    else if( property == "arrangementDescriptorFilename" )
      {
	file.seekg( xxxxSize - (property.size() + 1 ), std::ios_base::cur );
	total += xxxxSize - ( property.size() + 1 );
      }
    else if( property == "appearanceFilename" )
      {
	file.read( (char *)&enabled, 1 ); ++total;
	if( enabled > 0 )
	  {
	    file.getline( temp, 255, 0 );
	    appearanceFilename = temp;
	    total += appearanceFilename.size() + 1;
	    
	    std::cout << property << ": "
		      << appearanceFilename
		      << std::endl;
	  }
      }
    else if( property == "portalLayoutFilename" )
      {
	file.read( (char *)&enabled, 1 ); ++total;
	if( enabled > 0 )
	  {
	    file.getline( temp, 255, 0 );
	    portalLayoutFilename = temp;
	    total += portalLayoutFilename.size() + 1;
	    
	    std::cout << property << ": "
		      << portalLayoutFilename
		      << std::endl;
	  }
      }
    else if( property == "clientDataFile" )
      {
	file.seekg( xxxxSize - (property.size() + 1 ), std::ios_base::cur );
	total += xxxxSize - ( property.size() + 1 );
      }
    else if( property == "collisionMaterialFlags" )
      {
	file.seekg( xxxxSize - (property.size() + 1 ), std::ios_base::cur );
	total += xxxxSize - ( property.size() + 1 );
      }
    else if( property == "collisionMaterialPassFlags" )
      {
	file.seekg( xxxxSize - (property.size() + 1 ), std::ios_base::cur );
	total += xxxxSize - ( property.size() + 1 );
      }
    else if( property == "collisionMaterialBlockFlags" )
      {
	file.seekg( xxxxSize - (property.size() + 1 ), std::ios_base::cur );
	total += xxxxSize - ( property.size() + 1 );
      }
    else if( property == "collisionActionFlags" )
      {
	file.seekg( xxxxSize - (property.size() + 1), std::ios_base::cur );
	total += xxxxSize - ( property.size() + 1 );
      }
    else if( property == "collisionActionPassFlags" )
      {
	file.seekg( xxxxSize - (property.size() + 1 ), std::ios_base::cur );
	total += xxxxSize - ( property.size() + 1 );
      }
    else if( property == "collisionActionBlockFlags" )
      {
	file.seekg( xxxxSize - (property.size() + 1 ), std::ios_base::cur );
	total += xxxxSize - ( property.size() + 1 );
      }
    else if( property == "scale" )
      {
	file.seekg( xxxxSize - (property.size() + 1 ), std::ios_base::cur );
	total += xxxxSize - ( property.size() + 1 );
      }
    else if( property == "gameObjectType" )
      {
	file.seekg( xxxxSize - (property.size() + 1 ), std::ios_base::cur );
	total += xxxxSize - ( property.size() + 1 );
      }
    else if( property == "sendToClient" )
      {
	file.seekg( xxxxSize - (property.size() + 1 ), std::ios_base::cur );
	total += xxxxSize - ( property.size() + 1 );
      }
    else if( property == "scaleThresholdBeforeExtentTest" )
      {
	file.seekg( xxxxSize - (property.size() + 1 ), std::ios_base::cur );
	total += xxxxSize - ( property.size() + 1 );
      }
    else if( property == "clearFloraRadius" )
      {
	file.seekg( xxxxSize - (property.size() + 1 ), std::ios_base::cur );
	total += xxxxSize - ( property.size() + 1 );
      }
    else if( property == "surfaceType" )
      {
	file.seekg( xxxxSize - (property.size() + 1 ), std::ios_base::cur );
	total += xxxxSize - ( property.size() + 1 );
      }
    else if( property == "noBuildRadius" )
      {
	file.seekg( xxxxSize - (property.size() + 1 ), std::ios_base::cur );
	total += xxxxSize - ( property.size() + 1 );
      }
    else if( property == "onlyVisibleInTools" )
      {
	file.seekg( xxxxSize - (property.size() + 1 ), std::ios_base::cur );
	total += xxxxSize - ( property.size() + 1 );
      }
    else if( property == "locationReservationRadius" )
      {
	file.seekg( xxxxSize - (property.size() + 1 ), std::ios_base::cur );
	total += xxxxSize - ( property.size() + 1 );
      }
    else
      {
	std::cout << "Unknown: " << property << std::endl;
	exit( 0 );
      }

    if( xxxxSize == (total-8) )
    {
        std::cout << "Finished reading XXXX" << std::endl;
    }
    else
    {
        std::cout << "FAILED in reading XXXX" << std::endl;
        std::cout << "Read " << total << " out of " << xxxxSize
                  << std::endl;
    }

    return total;
}
