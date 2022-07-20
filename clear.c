void
clear(Monitor *m) {
	Client *c;
	for (c = m->stack; c; c = c->snext) {
		if(ISVISIBLE(c)) {
			XMoveWindow(dpy, c->win, WIDTH(c) * -2, c->y);
		}
	}
}
