/*******************************************************************************

  Copyright (C) 2010, 2013 LIFIA - Facultad de Informatica - Univ. Nacional de La Plata

********************************************************************************

  This file is part of DTV-util implementation.

    DTV-util is free software: you can redistribute it and/or modify it
  under the terms of the GNU Lesser General Public License as published by the Free
  Software Foundation, either version 2 of the License.

    DTV-util is distributed in the hope that it will be useful, but WITHOUT ANY
  WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
  A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License along with
  this program. If not, see <http://www.gnu.org/licenses/>.

********************************************************************************

  Este archivo es parte de la implementación de DTV-util.

    DTV-util es Software Libre: Ud. puede redistribuirlo y/o modificarlo
  bajo los términos de la Licencia Pública General Reducida GNU como es publicada por la
  Free Software Foundation, según la versión 2 de la licencia.

    DTV-util se distribuye esperando que resulte de utilidad, pero SIN NINGUNA
  GARANTÍA; ni siquiera la garantía implícita de COMERCIALIZACIÓN o ADECUACIÓN
  PARA ALGÚN PROPÓSITO PARTICULAR. Para más detalles, revise la Licencia Pública
  General Reducida GNU.

    Ud. debería haber recibido una copia de la Licencia Pública General Reducida GNU
  junto a este programa. Si no, puede verla en <http://www.gnu.org/licenses/>.

*******************************************************************************/

#pragma once

#define RUN_TOOL(cls,prj) \
	int run_tvd_tool( int argc, char *argv[] ) { \
		std::string build (prj ## _BUILD_VERSION); \
		std::string tag (prj ## _TAG_VERSION); \
		std::string opts (prj ## _BUILD_OPTS); \
		std::string version(prj ## _VERSION); \
		std::string prefix(prj ## _PREFIX); \
		cls obj; \
		obj.initialize(prj ## _NAME, prj ## _DESCRIPTION, version + " (" + tag + ")", "Source: " + tag + '(' + build + ") \nConfig: " + opts, prefix ); \
		return obj.execute(argc,argv); \
	}

#ifdef __APPLE__
int apple_main( int argc, char *argv[] );
#endif


#ifndef __APPLE__
#define CREATE_TOOL(cls,prj) \
	RUN_TOOL(cls,prj) \
	int main(int argc, char *argv[]) { return run_tvd_tool(argc,argv); }
#else
#define CREATE_TOOL(cls,prj) \
	RUN_TOOL(cls,prj) \
	int main(int argc, char *argv[]) { return apple_main(argc,argv); }
#endif
