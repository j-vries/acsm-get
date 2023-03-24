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

#ifndef ACSMGET_APPLICATION_H
#define ACSMGET_APPLICATION_H

#include <gtkmm.h>
#include "core/adeptclient.h"
#include "core/drmprocessorclient.h"

namespace acsmget
{
	class Application final : public Gtk::Application
	{
		protected:
			Application();

		public:
			static Glib::RefPtr<Application> create();

		protected:
			void on_activate() override;
			void on_open(const Gio::Application::type_vec_files &files, const Glib::ustring &hint) override;

		private:
			Gtk::Window *create_mainwindow(const std::string &command_line_filename);
			void on_hide_window(Gtk::Window *window);

			DRMProcessorClient m_drmprocessorclient;
			AdeptClient m_adeptclient;
	};
}

#endif //ACSMGET_APPLICATION_H
