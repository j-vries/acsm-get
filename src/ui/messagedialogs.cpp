/*
	Copyright (c) 2023 Jan de Vries

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program. If not, see <https://www.gnu.org/licenses/>.
*/

#include "../texts/texts.h"
#include "messagedialogs.h"

namespace acsmget
{
	int show_error_message_dialog(Gtk::Window &parent, const Glib::ustring &message, const Glib::ustring &details)
	{
		Gtk::MessageDialog dialog(message, false, Gtk::MESSAGE_ERROR, Gtk::BUTTONS_CLOSE, true);
		dialog.set_title(texts_title_error);
		dialog.set_secondary_text(details);
		dialog.set_transient_for(parent);
		return dialog.run();
	}

	int show_info_message_dialog(Gtk::Window &parent, const Glib::ustring &message, const Glib::ustring &title)
	{
		Gtk::MessageDialog dialog(message, false, Gtk::MESSAGE_INFO, Gtk::BUTTONS_CLOSE, true);
		dialog.set_title(title);
		dialog.set_transient_for(parent);
		return dialog.run();
	}

	int show_yes_no_message_dialog(Gtk::Window &parent, const Glib::ustring &message, const Glib::ustring &title)
	{
		Gtk::MessageDialog dialog(message, false, Gtk::MESSAGE_QUESTION, Gtk::BUTTONS_YES_NO, true);
		dialog.set_title(title);
		dialog.set_transient_for(parent);
		return dialog.run();
	}
}
