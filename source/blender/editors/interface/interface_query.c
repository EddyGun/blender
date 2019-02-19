/*
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

/** \file
 * \ingroup edinterface
 *
 * Utilities to inspect the interface, extract information.
 */

#include "BLI_utildefines.h"

#include "DNA_screen_types.h"

#include "UI_interface.h"

#include "interface_intern.h"

#include "WM_api.h"
#include "WM_types.h"

/* -------------------------------------------------------------------- */
/** \name Button (uiBut)
 * \{ */

bool ui_but_is_editable(const uiBut *but)
{
	return !ELEM(
	        but->type,
	        UI_BTYPE_LABEL, UI_BTYPE_SEPR, UI_BTYPE_SEPR_LINE,
	        UI_BTYPE_ROUNDBOX, UI_BTYPE_LISTBOX, UI_BTYPE_PROGRESS_BAR);
}

bool ui_but_is_editable_as_text(const uiBut *but)
{
	return ELEM(
	        but->type,
	        UI_BTYPE_TEXT, UI_BTYPE_NUM, UI_BTYPE_NUM_SLIDER,
	        UI_BTYPE_SEARCH_MENU);

}

bool ui_but_is_toggle(const uiBut *but)
{
	return ELEM(
	        but->type,
	        UI_BTYPE_BUT_TOGGLE,
	        UI_BTYPE_TOGGLE,
	        UI_BTYPE_ICON_TOGGLE,
	        UI_BTYPE_ICON_TOGGLE_N,
	        UI_BTYPE_TOGGLE_N,
	        UI_BTYPE_CHECKBOX,
	        UI_BTYPE_CHECKBOX_N,
	        UI_BTYPE_ROW
	);
}

#ifdef USE_UI_POPOVER_ONCE
bool ui_but_is_popover_once_compat(const uiBut *but)
{
	return (
	        (but->type == UI_BTYPE_BUT) ||
	        ui_but_is_toggle(but)
	);
}
#endif

bool UI_but_is_tool(const uiBut *but)
{
	/* very evil! */
	if (but->optype != NULL) {
		static wmOperatorType *ot = NULL;
		if (ot == NULL) {
			ot = WM_operatortype_find("WM_OT_tool_set_by_name", false);
		}
		if (but->optype == ot) {
			return true;
		}
	}
	return false;
}

bool UI_but_has_tooltip_label(const uiBut *but)
{
	if ((but->drawstr[0] == '\0') && !ui_block_is_popover(but->block)) {
		return UI_but_is_tool(but);
	}
	return false;
}

/** \} */

/* -------------------------------------------------------------------- */
/** \name Block (uiBlock)
 * \{ */

bool ui_block_is_menu(const uiBlock *block)
{
	return (((block->flag & UI_BLOCK_LOOP) != 0) &&
	        /* non-menu popups use keep-open, so check this is off */
	        ((block->flag & UI_BLOCK_KEEP_OPEN) == 0));
}

bool ui_block_is_popover(const uiBlock *block)
{
	return (block->flag & UI_BLOCK_POPOVER) != 0;
}

bool ui_block_is_pie_menu(const uiBlock *block)
{
	return ((block->flag & UI_BLOCK_RADIAL) != 0);
}

bool ui_block_is_popup_any(const uiBlock *block)
{
	return (
	        ui_block_is_menu(block) ||
	        ui_block_is_popover(block) ||
	        ui_block_is_pie_menu(block)
	);
}

bool UI_block_is_empty(const uiBlock *block)
{
	for (const uiBut *but = block->buttons.first; but; but = but->next) {
		if (!ELEM(but->type, UI_BTYPE_SEPR, UI_BTYPE_SEPR_LINE)) {
			return false;
		}
	}
	return true;
}

/** \} */
