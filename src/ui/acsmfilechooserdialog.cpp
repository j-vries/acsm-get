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
#include "acsmfilechooserdialog.h"

namespace acsmget
{
	AcsmFileChooserDialog::AcsmFileChooserDialog(Gtk::Window &parent) :
		FileChooserDialog(parent, texts_title_file_chooser)
	{
		add_button(texts_action_cancel, Gtk::RESPONSE_CANCEL);
		auto open_button = add_button(texts_action_open, Gtk::RESPONSE_OK);
		open_button->get_style_context()->add_class("suggested-action");

		auto filter = Gtk::FileFilter::create();
		filter->set_name(texts_other_acsm_file_filter);
		filter->add_pattern("*.ACSM");
		filter->add_pattern("*.acsm");
		add_filter(filter);
	}
}
