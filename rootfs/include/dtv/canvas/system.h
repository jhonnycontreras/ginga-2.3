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
#include "size.h"
#include <util/task/types.h>
#include <boost/thread.hpp>
#include <vector>

namespace util {
namespace io {
	class Dispatcher;
}
}

namespace canvas {

class Canvas;
class Size;
class MediaPlayer;
class WebViewer;
class Window;
class Surface;
class Input;
class Player;

/**
 * Es la clase principal de la librería dtv-canvas. Encapsula la funcionalidad de manejo de eventos, 
 * entrada de teclado, manejo de sockets, ejecución de tareas, manejo de timers y loop principal. 
 * A partir de una instancia de System se puede acceder a las instancias de Canvas, Window, Player y WebViewer.
 */
class System {
public:
	System();
	virtual ~System();

	//	Initialization
	bool initialize();
	void finalize();

	//	Run/exit system loop
	void run();
	void exit();

	//	Dispatcher
	util::task::Dispatcher *dispatcher() const;
	util::io::Dispatcher *io() const;
	void post( util::task::Target target, const util::task::Type &task );

	//	Event notifications
	void setNotifyCallback( const event::Callback &callback );
	void dispatchNotify( event::Notify *evt );
	void dispatchMouseNotify( event::Notify *notify );

	//	Input
	void addInput( Input *in );
	
	
	void dispatchKey( util::key::type keyValue, bool isUp );

	//	Web viewer
	virtual WebViewer *createWebViewer( Surface * );

	//	Getters/Setters
	Window *window() const;
	Canvas *canvas() const;
	Player *player() const;

	//	Instance creation
	static System *create( const std::string &useParam="" );

	//	Implementation
	const boost::thread::id &getThreadId() const;

protected:
	//	Types
	typedef std::vector<Input *> InputList;

	//	Initialization
	virtual bool init();
	virtual void fin();
	void cleanup();

	//	IO dispatcher methods
	virtual util::io::Dispatcher *createIO() const;
	bool setupIO();
	
	//	Canvas methods
	virtual Canvas *createCanvas() const;
	bool setupCanvas();

	//	Player methods
	virtual Player *createPlayer() const;
	bool setupPlayer();

	//	Window methods
	virtual Window *createWindow() const;
	bool setupWindow();
	void dispatchShutdown();

	//	Aux mouse
	void fixMouseCoordinates( event::Notify *notify );

private:
	util::io::Dispatcher *_io;
	Window *_win;
	Canvas *_canvas;
	Player *_player;
	InputList _inputs;
	boost::thread::id _threadId;
	event::Callback _onNotify;
};

}
