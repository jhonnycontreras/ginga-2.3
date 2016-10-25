/*******************************************************************************

  Copyright (C) 2010, 2013 LIFIA - Facultad de Informatica - Univ. Nacional de La Plata

********************************************************************************

  This file is part of DTV-canvas implementation.

    DTV-canvas is free software: you can redistribute it and/or modify it
  under the terms of the GNU Lesser General Public License as published by the Free
  Software Foundation, either version 2 of the License.

    DTV-canvas is distributed in the hope that it will be useful, but WITHOUT ANY
  WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
  A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License along with
  this program. If not, see <http://www.gnu.org/licenses/>.

********************************************************************************

  Este archivo es parte de la implementación de DTV-canvas.

    DTV-canvas es Software Libre: Ud. puede redistribuirlo y/o modificarlo
  bajo los términos de la Licencia Pública General Reducida GNU como es publicada por la
  Free Software Foundation, según la versión 2 de la licencia.

    DTV-canvas se distribuye esperando que resulte de utilidad, pero SIN NINGUNA
  GARANTÍA; ni siquiera la garantía implícita de COMERCIALIZACIÓN o ADECUACIÓN
  PARA ALGÚN PROPÓSITO PARTICULAR. Para más detalles, revise la Licencia Pública
  General Reducida GNU.

    Ud. debería haber recibido una copia de la Licencia Pública General Reducida GNU
  junto a este programa. Si no, puede verla en <http://www.gnu.org/licenses/>.

*******************************************************************************/

#pragma once

#include "types.h"
#include <string>
#include <boost/function.hpp>

namespace util {
	class Buffer;
}

namespace canvas {

/**
 * Permite la reproducción de un video en particular.
 */
class MediaPlayer {
public:
	MediaPlayer();
	virtual ~MediaPlayer();

	typedef boost::function<void (void)> OnStopCallback;
	void onStop( const OnStopCallback &callback );

	/**
	 * Inicializa el estado del @c MediaPlayer, creando una superficie para reproducir el video indicado en @b url
	 * que ocupa la región indicada en @b rect.
	 * @param rect La región que ocupará el video.
	 * @param url La ruta al archivo de video.
	 * @return true si la inicialización finalizo correctamente, false en caso contrario.
	 */
	virtual bool initialize( const Rect &rect, const std::string &url )=0;

	/**
	 * Inicializa el estado del @c MediaPlayer, seteando a @b url como el video a reproducir.
	 * @param url La ruta al archivo de video.
	 * @return true si la inicialización finalizo correctamente, false en caso contrario.
	 */
	virtual bool initialize( const std::string &url )=0;

	/**
	 * Finaliza el estado del @c MediaPlayer.
	 */
	virtual void finalize()=0;

	/**
	 * Inicia la reproducción del video.
	 */
	virtual void play()=0;

	/**
	 * Frena la reproducción del video.
	 */
	virtual void stop()=0;

	/**
	 * Pausa/continua la reproducción del video.
	 * @param needPause Indica si se debe frenar o continuar con la reproducción del video.
	 */
	virtual void pause( bool needPause )=0;

	/**
	 * Silencia/habilita el audio del video.
	 * @param needMute Indica si se debe silenciar o habilitar el audio del video.
	 */
	virtual void mute( bool needMute )=0;

	void volume( Volume vol );

	/**
	 * Mueve la superficie de video.
	 * @param point La nueva posicion del @c MediaPlayer.
	 */
	virtual void move( const Point &point )=0;

	/**
	 * Cambia el tamaño de la superficie de video.
	 * @param size El nuevo tamaño del @c MediaPlayer.
	 */
	virtual void resize( const Size &size )=0;

	/**
	 * Pone al @c MediaPlayer en modo pantalla completa.
	 */
	virtual void setFullScreen()=0;

	/**
	 * Cambia el tamaño y posición del @c MediaPlayer.
	 * @param rect La nueva región (posición y tamaño) del @c MediaPlayer.
	 */
	virtual void bounds( const Rect &rect );

	//	Feed data
	virtual void feed( util::Buffer *buf );

protected:
	//	Aux volume
	virtual void setVolume( Volume vol ) = 0;
	virtual Volume maxVol();
	virtual Volume minVol();

	void onMediaStopped();
	
private:
	OnStopCallback _onStop;	
};

}

