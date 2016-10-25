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

#include "../any.h"
#include "../log.h"
#include <string>
#include <exception>
#include <map>

namespace util {
namespace storage {

template<class Storage, typename T>
void serialize( Storage &storage, T &data ) {
	data.serialize( storage );
}

class StorageError : public std::exception {
public:
	StorageError( const std::string reason ) : _reason( reason ) {}
	virtual ~StorageError() throw() {}

	virtual const char* what() const throw() {
		return _reason.c_str();
	}

	std::string _reason;
};

template<class Impl>
class Storage {
public:
	Storage() : _loading(false) {};
	virtual ~Storage() {};

	virtual bool initialize()=0;
	virtual void finalize()=0;

	virtual void destroyDB()=0;

	template<typename T>
	void save( const std::string &key, const T &data );

	template<typename T>
	void load( const std::string &key, T &data );

	template<typename T>
	void serialize( const std::string &key , T &value );

	bool isLoading() { return _loading; }

protected:
	bool _loading;

private:
	typedef std::map<std::string,Any> Cache;
	Cache _cache;
};

template<class Impl>
template<typename T>
void Storage<Impl>::save( const std::string &key, const T &data ) {
	_loading = false;
	_cache[key] = data;
	try {
		static_cast<Impl*>(this)->saveImpl( key, data );
	} catch ( std::exception &e ) {
		LWARN( "Storage", "Exception ocurred: %s", e.what() );
	}
}

template<class Impl>
template<typename T>
void Storage<Impl>::load( const std::string &key, T &data ) {
	_loading = true;
	Cache::const_iterator it = _cache.find( key );
	if (it != _cache.end()) {
		//	Key is in cache
		data = _cache[key].template get<T>();
	} else {
		static_cast<Impl*>(this)->loadImpl(key, data);
		//	Load key from StorageImpl and save in cache
		_cache[key] = data;
	}
}

template<class Impl>
template<typename T>
void Storage<Impl>::serialize( const std::string &key , T &value ) {
	if (_loading) {
		static_cast<Impl*>(this)->loadImpl( key, value );
	} else {
		static_cast<Impl*>(this)->saveImpl( key, value );
	}
}

}//	end storage namespace
}//	end util namespace
