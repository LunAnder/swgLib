/** -*-c++-*-
 *  \file   readSWG.cpp
 *  \author Kenneth R. Sewell III

 swgLib is used for the parsing and exporting .msh models.
 Copyright (C) 2006-2009 Kenneth R. Sewell III

 This file is part of swgLib.

 swgLib is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.

 swgLib is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with swgLib; if not, write to the Free Software
 Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>

#include <swgLib/apt.hpp>
#include <swgLib/cach.hpp>
#include <swgLib/cclt.hpp>
#include <swgLib/ckat.hpp>
#include <swgLib/cmp.hpp>
#include <swgLib/cshd.hpp>
#include <swgLib/cstb.hpp>
#include <swgLib/dtii.hpp>
#include <swgLib/eft.hpp>
#include <swgLib/flor.hpp>
#include <swgLib/foot.hpp>
#include <swgLib/ilf.hpp>
#include <swgLib/lod.hpp>
#include <swgLib/mlod.hpp>
#include <swgLib/peft.hpp>
#include <swgLib/prto.hpp>
#include <swgLib/sbot.hpp>
#include <swgLib/sd2d.hpp>
#include <swgLib/sd3d.hpp>
#include <swgLib/sht.hpp>
#include <swgLib/sktm.hpp>
#include <swgLib/skmg.hpp>
#include <swgLib/slod.hpp>
#include <swgLib/smat.hpp>
#include <swgLib/spam.hpp>
#include <swgLib/stat.hpp>
#include <swgLib/ster.hpp>
#include <swgLib/stot.hpp>
#include <swgLib/str.hpp>
#include <swgLib/swts.hpp>
#include <swgLib/ws.hpp>

int main( int argc, char **argv )
{
  // Read
  if( 2 == argc )
    {
      std::ifstream infile( argv[1], std::ios_base::binary );
      if( !infile.is_open() )
	{
	  std::cout << "Unable to open file: " << argv[1] << std::endl;
	  exit( 0 );
	}
	
      std::string fileType = ml::base::getType( infile );
	
      if( "INLY" == fileType )
	{
	  // Interior layout
	  ml::ilf interior;
	  interior.readILF( infile );
	}
      else if( "ABCD" == fileType )
	{
	  ml::str swgString;
	  swgString.readSTR( infile );
	  swgString.print();
	}
      else if( "APT " == fileType )
	{
	  ml::apt misc;
	  misc.readAPT( infile );
	  misc.print();
	}
      else if( "DTII" == fileType )
	{
	  ml::dtii datatable;
	  datatable.readDTII( infile );
	}
      else if( "CACH" == fileType )
	{
	  ml::cach misc;
	  misc.readCACH( infile );
	}
      else if( "CCLT" == fileType )
	{
	  ml::cclt misc;
	  misc.readCCLT( infile );
	}
      else if( "CKAT" == fileType )
	{
	  ml::ckat misc;
	  misc.readCKAT( infile );
	}
      else if( "CMPA" == fileType )
	{
	  // Component
	  ml::cmp misc;
	  misc.readCMP( infile );
	}
      else if( "CSHD" == fileType )
	{
	  ml::cshd shader;
	  shader.readCSHD( infile );
	}
      else if( "CSTB" == fileType )
	{
	  ml::cstb misc;
	  misc.readCSTB( infile );
	  misc.print();
	}
      else if( "EFCT" == fileType )
	{
	  ml::eft effect;
	  effect.readEFT( infile );
	}
      else if( "FOOT" == fileType )
	{
	  ml::foot misc;
	  misc.readFOOT( infile );
	}
      else if( "FLOR" == fileType )
	{
	  ml::flor misc;
	  misc.readFLOR( infile );
	}
      else if( "DTLA" == fileType )
	{
	  // LOD
	  ml::lod misc;
	  misc.readLOD( infile );
	}
      else if( "MLOD" == fileType )
	{
	  // MLOD
	  ml::mlod misc;
	  misc.readMLOD( infile );
	}
      else if( "PEFT" == fileType )
	{
	  // Particle effect
	  ml::peft misc;
	  misc.readPEFT( infile );
	}
      else if( "PRTO" == fileType )
	{
	  // Portal
	  ml::prto misc;
	  misc.readPRTO( infile );
	}
      else if( "SBOT" == fileType )
	{
	  ml::sbot misc;
	  misc.readSBOT( infile );
	  misc.print();
	}
      else if( "SD2D" == fileType )
	{
	  ml::sd2d misc;
	  misc.readSD2D( infile );
	  misc.print();
	}
      else if( "SD3D" == fileType )
	{
	  ml::sd3d misc;
	  misc.readSD3D( infile );
	  misc.print();
	}
      else if( "SPAM" == fileType )
	{
	  ml::spam misc;
	  misc.readSPAM( infile );
	  misc.print();
	}
      else if( "SMAT" == fileType )
	{
	  ml::smat misc;
	  misc.readSMAT( infile );
	  misc.print();
	}
      else if( "SLOD" == fileType )
	{
	  // Skeleton LOD
	  ml::slod misc;
	  misc.readSLOD( infile );
	}
      else if( "SKTM" == fileType )
	{
	  // 
	  ml::sktm misc;
	  misc.readSKTM( infile );
	}
      else if( "SKMG" == fileType )
	{
	  ml::skmg misc;
	  misc.readSKMG( infile );
	}
      else if( "SSHT" == fileType )
	{
	  // Shader
	  ml::sht shader;
	  shader.readSHT( infile );
	}
      else if( "STAT" == fileType )
	{
	  ml::stat misc;
	  misc.readSTAT( infile );
	  misc.print();
	}
      else if( "STER" == fileType )
	{
	  ml::ster misc;
	  misc.readSTER( infile );
	  misc.print();
	}
      else if( "STOT" == fileType )
	{
	  ml::stot misc;
	  misc.readSTOT( infile );
	  misc.print();
	}
      else if( "SWTS" == fileType )
	{
	  // Animated texture
	  ml::swts misc;
	  misc.readSWTS( infile );
	  misc.print();
	}
      else if( "WSNP" == fileType )
	{
	  // World snapshot
	  ml::ws snapshot;
	  snapshot.readWS( infile );
	}
      else
	{
	  std::cout << "Unknown type: " << fileType << std::endl;
	}
    }
  // Write
  else if( 4 == argc && std::string( argv[1] ) == "create" )
    {
      std::ifstream infile( argv[2] );
      if( !infile.is_open() )
	{
	  std::cout << "Unable to open file: " << argv[2] << std::endl;
	  exit( 0 );
	}

      char temp[512];
      infile.getline( temp, 512, ':' );
      std::string type;
      infile >> type;

      if( "INLY" == type )
	{
	  std::ofstream outfile( argv[3], std::ios_base::binary );
	  if( !outfile.is_open() )
	    {
	      std::cout << "Unable to create file: " << argv[2] << std::endl;
	      exit( 0 );
	    }

	  ml::ilf interior;
	  interior.createILF( infile, outfile );

	  outfile.close();
	}
      else if( "WSNP" == type )
	{
	  std::ofstream outfile( argv[3], std::ios_base::binary );
	  if( !outfile.is_open() )
	    {
	      std::cout << "Unable to create file: " << argv[2] << std::endl;
	      exit( 0 );
	    }

	  ml::ws exterior;
	  exterior.readMetaFile( infile );
	  exterior.createWS( outfile );

	  outfile.close();
	}
      else
	{
	  std::cout << "Cannot write files of type: " << type << std::endl;
	  infile.close();
	  return 0;
	}
	

      infile.close();
    }
  // Error
  else
    {
      std::cout << "readSWG <file>" << std::endl;
      std::cout << "readSWG create <data file> <swg file>" << std::endl;
      return 0;
    }
    
  return 0;
}
