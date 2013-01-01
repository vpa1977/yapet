// $Id$
//
// Test basic functionality of Curses class

#include <unistd.h>

#include <iostream>

#include "curs.h"
#include "lineobject.h"
#include "window.h"

int main() {
    try {
	Curses::init();

	LineObject* title = new LineObject(LineObject::POS_TOP,
					   NULL, "Basic 1");
	Curses::setTitle(title);

	Window* w1 = new Window;
	w1->setFrame(true);

	Curses::setWindow(w1);

	Curses::show();

	sleep(2);

	delete title;
	delete w1;
	Curses::end();
    } catch (std::exception& e) {
	std::cerr << e.what() << std::endl;
	sleep(2);
	goto _ERR;
    }

    return 0;
 _ERR:
    Curses::end();
    return 1;
}