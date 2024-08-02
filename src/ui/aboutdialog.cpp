/*
	Copyright (c) 2023-2024 Jan de Vries

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
#include "../main.h"
#include "aboutdialog.h"

namespace acsmget
{
	void show_about_dialog(Gtk::Window &parent)
	{
		auto bytes = Gio::Resource::lookup_data_global("/acsmget/licenses.txt");
		gsize size = 0;
		Glib::ustring licenses(static_cast<const gchar*>(bytes->get_data(size)));
    	bytes.reset();

		Gtk::AboutDialog dialog;
		dialog.set_comments(texts_desc_app);
		dialog.set_copyright(ACSMGET_COPYRIGHT);
		dialog.set_license(licenses);
		dialog.set_logo_default();
		dialog.set_modal();
		dialog.set_transient_for(parent);
		dialog.set_version(ACSMGET_VERSION);
		dialog.set_website(ACSMGET_WEBSITE);
		dialog.run();
	}
}
