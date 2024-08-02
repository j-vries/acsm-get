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

#include "texts/texts.h"
#include "ui/mainwindow.h"
#include "application.h"
#include "main.h"

namespace acsmget
{
	Application::Application() :
		Gtk::Application(ACSMGET_APP_ID, Gio::APPLICATION_HANDLES_OPEN),
		m_adeptclient(ACSMGET_PRG_NAME)
	{
		g_set_application_name(texts_title_app.c_str());
		g_set_prgname(ACSMGET_PRG_NAME);
	}

	Glib::RefPtr<Application> Application::create()
	{
		return Glib::RefPtr<Application>(new Application());
	}

	Gtk::Window *Application::create_mainwindow(const std::string &command_line_filename = "")
	{
		auto window = new MainWindow(&m_adeptclient, command_line_filename);
		add_window(*window);
		window->signal_hide().connect(sigc::bind<Gtk::Window *>(sigc::mem_fun(*this, &Application::on_hide_window), window));
		return window;
	}

	void Application::on_activate()
	{
		auto window = create_mainwindow();
		window->present();
	}

	void Application::on_open(const Gio::Application::type_vec_files &files, const Glib::ustring &hint)
	{
		auto command_line_filename = std::string("");
		if(files.size() > 0) {
			command_line_filename = files.front()->get_path();	// Only one download at a time is supported; discard the rest.
		}
		auto window = create_mainwindow(command_line_filename);
		window->present();
	}

	void Application::on_hide_window(Gtk::Window *window)
	{
		delete window;
	}
}
