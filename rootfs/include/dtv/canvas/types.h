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

#include "size.h"
#include <util/types.h>
#include <util/id/ident.h>
#include <util/keydefs.h>
#include <boost/function.hpp>

namespace canvas {

class System;
class Rect;
class Point;
class Canvas;
class Color;
class Surface;
typedef Surface * SurfacePtr;
struct ImageDataType {
	Size size;
	unsigned char *data;
	int length;
	int bitsPerPixel;
	int bytesPerPixel;
	int stride;
	int dataOffset;
};
typedef struct ImageDataType ImageData;

//	Video
class Window;

#ifdef _WIN32
typedef HANDLE VideoDescription;
#else
typedef struct {
  void *display;
  int   screenID;
  unsigned long winID;
} VideoDescription;
#endif
class VideoOverlay;
typedef VideoOverlay *VideoOverlayPtr;

//	Volume
#define MIN_VOLUME     0
#define MAX_VOLUME     20
typedef long Volume;

//	Player
class Player;
class MediaPlayer;
typedef MediaPlayer *MediaPlayerPtr;

//	HTML
class WebViewer;

//	Event notifications callback
namespace event {

namespace type {
enum evtType {
	key,
	button,
	shutdown
};
}

//	Keys
struct KeyStruct {
	type::evtType type;
	util::key::type key;
	bool isUp;
};
typedef struct KeyStruct KeyEvent;

//	Buttons
struct ButtonStruct {
	type::evtType type;
	unsigned int button;
	int x, y;
	bool isPress;
};
typedef struct ButtonStruct ButtonEvent;

union NotifyStruct {
	type::evtType type;
	KeyEvent keyEvt;
	ButtonEvent buttonEvt;
};
typedef union NotifyStruct Notify;

typedef boost::function<void ( Notify *evt )> Callback;

}	//	namespace event notification

//	Events
template<typename T> class Event;
typedef boost::function<void (util::id::Ident &id)> EventCallback;

namespace composition {

enum mode {
	source_over,
	destination_over,
	clear,
	source,
	destination,
	source_in,
	destination_in,
	source_out,
	destination_out,
	source_atop,
	destination_atop,
	x_or,
	plus,
	multiply,
	screen,
	overlay,
	darken,
	lighten,
	color_dodge,
	color_burn,
	hard_light,
	soft_light,
	difference,
	exclusion
};

}	//namespace composition

}
