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

#include "../storage.h"
#include "../../buffer.h"
#include "../../types.h"
#include <boost/type_traits/is_fundamental.hpp>
#include <boost/type_traits/is_enum.hpp>

struct __db;
typedef struct __db DB;

namespace util {
namespace storage {
namespace bdb {

class Storage : public storage::Storage<Storage> {
public:
	explicit Storage( const std::string &filename );
	virtual ~Storage( void );

	virtual bool initialize( void );
	virtual void finalize( void );

	virtual void destroyDB();

	// save/load primitive types
	template<typename T>
	typename boost::enable_if_c< boost::is_fundamental<T>::value || boost::is_enum<T>::value >::type
	saveImpl( const std::string &key, const T &data ) {
		startCall( key, data, false );
		_buf->append( (char *)&data, sizeof(data) );
		endCall( key, false );
	}

	template<typename T>
	typename boost::enable_if_c< boost::is_fundamental<T>::value || boost::is_enum<T>::value >::type
	loadImpl( const std::string &key, T &data ) {
		startCall( key, data );
		memcpy( &data, _buf->buffer()+_pos, sizeof(data) );
		_pos += sizeof(data);
		endCall( key );
	}

	// save/load complex types
	template<typename T>
	typename boost::disable_if_c< boost::is_fundamental<T>::value || boost::is_enum<T>::value >::type
	saveImpl( const std::string &key, const T &data ) {
		startCall( key, data, false );
		util::storage::serialize( *this, const_cast<T&>(data) );
		endCall( key, false );
	}

	template<typename T>
	typename boost::disable_if_c< boost::is_fundamental<T>::value || boost::is_enum<T>::value >::type
	loadImpl( const std::string &key, T &data ) {
		startCall( key, data );
		util::storage::serialize( *this, data );
		endCall( key );
	}

	// save/load vector
	template<typename T>
	void saveImpl( const std::string &key, const std::vector<T> &data );

	template<typename T>
	void saveImpl( const std::string &key, const std::vector<T*> &data );

	template<typename T>
	void loadImpl( const std::string &key, std::vector<T> &data );

	template<typename T>
	void loadImpl( const std::string &key, std::vector<T*> &data );

protected:
	void saveKey( const std::string &key );
	bool getKey( const std::string &key );

	template<typename T>
	void startCall( const std::string &key, T &data, bool load=true );
	void endCall( const std::string &key, bool load=true );

private:
	DB *_dbp;
	std::string _dbName;
	Buffer *_buf;
	int _pos;
	int _nestedCalls;
};

template<typename T>
void Storage::startCall( const std::string &key, T &data, bool load /*=true*/ ) {
	_nestedCalls++;
	if (_nestedCalls==1) {
		_buf = new Buffer();
		_pos = 0;
		if (load) {
			if (!getKey( key )) {
				bool tmp = _loading;
				_loading = false;
				saveImpl( key, data );
				saveKey( key );
				_loading = tmp;
			}
		}
	}
}

template<>
inline void Storage::saveImpl<std::string>( const std::string &key, const std::string &data ) {
	startCall( key, data, false );
	_buf->append( (char *)data.c_str(), data.length()+1 );
	endCall( key, false );
}

template<>
inline void Storage::loadImpl<std::string>( const std::string &key, std::string &data ) {
	startCall( key, data );
	data = _buf->buffer()+_pos;
	_pos += data.length()+1;
	endCall( key );
}

template<typename T>
void Storage::saveImpl( const std::string &key, const std::vector<T> &data ) {
	int count = data.size();

	startCall( key, data, false );
	saveImpl( key, count );
	for (int i=0; i<count; i++) {
		saveImpl( key, data[i] );
	}
	endCall( key, false );
}

template<typename T>
void Storage::loadImpl( const std::string &key, std::vector<T> &data ) {
	int count = 0;

	startCall( key, data );
	loadImpl( key, count );
	data.resize( count );
	for (int i=0; i<count; i++) {
		loadImpl( key, data[i] );
	}
	endCall( key );
}

template<typename T>
void Storage::saveImpl( const std::string &key, const std::vector<T*> &data ) {
	int count = data.size();

	startCall( key, data, false );
	saveImpl( key, count );
	for (int i=0; i<count; i++) {
		saveImpl( key, *data[i] );
	}
	endCall( key, false );
}

template<typename T>
void Storage::loadImpl( const std::string &key, std::vector<T*> &data ) {
	int count = 0;

	startCall( key, data );
	loadImpl( key, count );
	data.resize( count );
	CLEAN_ALL(T *, data);
	for (int i=0; i<count; i++) {
		T *obj = new T();
		loadImpl( key, *obj );
		data.push_back( obj );
	}
	endCall( key );
}

}
}
}
