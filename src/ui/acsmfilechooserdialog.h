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

#ifndef ACSMGET_ACSMFILECHOOSERDIALOG_H
#define ACSMGET_ACSMFILECHOOSERDIALOG_H

#include <gtkmm.h>

namespace acsmget
{
	class AcsmFileChooserDialog final : public Gtk::FileChooserDialog
	{
		public:
			AcsmFileChooserDialog(Gtk::Window &parent);
	};
}

#endif //ACSMGET_ACSMFILECHOOSERDIALOG_H
