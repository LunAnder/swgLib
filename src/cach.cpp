/** -*-c++-*-
 *  \class  cach
 *  \file   cach.cpp
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

#include <meshLib/cach.hpp>

#include <iostream>
#include <bitset>
#include <cstdlib>

using namespace ml;

cach::cach()
{
}

cach::~cach()
{
}

unsigned int cach::readCACH( std::istream &file, std::string path )
{
  basePath = path;
  unsigned int total = 0;
  std::string form;
  unsigned int cachSize;
  std::string type;

  total += readFormHeader( file, form, cachSize, type );
  cachSize += 8;
  if( form != "FORM" || type != "CACH" )
    {
      std::cout << "Expected Form of type CACH: " << type << std::endl;
      exit( 0 );
    }
  std::cout << "Found CACH form" << std::endl;

  unsigned int size;
  total += readFormHeader( file, form, size, type );
  if( form != "FORM" )
    {
      std::cout << "Expected Form " << std::endl;
      exit( 0 );
    }
  std::cout << "Found form of type: " << type << std::endl;

  total += readUnknown( file, cachSize - total );

  if( cachSize == total )
    {
      std::cout << "Finished reading CACH" << std::endl;
    }
  else
    {
      std::cout << "Failed in reading CACH" << std::endl;
      std::cout << "Read " << total << " out of " << cachSize
		<< std::endl;
    }

  return total;
}

