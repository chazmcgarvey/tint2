/**************************************************************************
*
* Tint2 : freespace
*
* Copyright (C) 2011 Mishael A Sibiryakov (death@junki.org)
*
* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License version 2
* as published by the Free Software Foundation.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
**************************************************************************/


#include <string.h>
#include <stdio.h>
#include <cairo.h>
#include <cairo-xlib.h>
#include <pango/pangocairo.h>
#include <stdlib.h>

#include "window.h"
#include "server.h"
#include "panel.h"
#include "freespace.h"
#include "common.h"

typedef enum {
	COUNT_WIDTH,
	COUNT_HEIGHT
} SpaceType ;

void init_freespace_panel(void *p)
{
	Panel *panel =(Panel*)p;
	FreeSpace *fsp= &panel->freespace;

	if (fsp->area.bg == 0)
		fsp->area.bg = &g_array_index(backgrounds, Background, 0);
	fsp->area.parent = p;
	fsp->area.panel = p;
	fsp->area.size_mode = SIZE_BY_CONTENT;
	fsp->area.resize = 1;
	fsp->area.on_screen = 1;
	fsp->area._resize = resize_freespace;
}

int freespace_get_max_size(Panel *p, SpaceType t) {

	GSList *walk;
	Area *a;
	int sz = 0;

	// Get space used by every element except the freespace
	for (walk = p->area.list; walk; walk = g_slist_next(walk)) {
		a = (Area *)walk->data;

		// Skip self
		if (a->_resize == resize_freespace)
			continue;

		if (t == COUNT_WIDTH)
			sz += a->width + (a->bg->border.width * 2) + p->area.paddingx;

		if (t == COUNT_HEIGHT)
			sz += a->height + (a->bg->border.width * 2) + p->area.paddingy;
	}

	if (t == COUNT_WIDTH)
		sz = p->area.width - sz - (p->area.bg->border.width * 2) - p->area.paddingxlr;

	if (t == COUNT_HEIGHT)
		sz = p->area.height - sz - (p->area.bg->border.width * 2) - p->area.paddingxlr; // Not sure about paddingxlr

	return sz;
}

int resize_freespace(void *obj) {

	FreeSpace *fsp= (FreeSpace *)obj;
	Panel *panel = (Panel *)fsp->area.panel;

	fsp->area.redraw = 1;

	if (panel_horizontal)
		fsp->area.width = freespace_get_max_size(panel, COUNT_WIDTH);
	else
		fsp->area.height= freespace_get_max_size(panel, COUNT_HEIGHT);

	// Always resize
	fsp->area.resize = 1;

	return 1;
}
