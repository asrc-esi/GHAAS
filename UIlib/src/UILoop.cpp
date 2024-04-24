/******************************************************************************

GHAAS User Interface library V2.1
Global Hydrological Archive and Analysis System
Copyright 1994-2024, UNH - CCNY

UILoop.cpp

bfekete@ccny.cuny.edu

*******************************************************************************/

#include <UI.hpp>

static int _UILoopStop = false;

int UILoop() {
    XEvent event;

    XtAppNextEvent(UIApplicationContext(), &event);
    XtDispatchEvent(&event);

    if (_UILoopStop) {
        _UILoopStop = false;
        return (false);
    }
    else return (true);
}

void UILoopStopCBK(Widget widget, void *data, XmAnyCallbackStruct *callData) {
    _UILoopStop = true;
}
