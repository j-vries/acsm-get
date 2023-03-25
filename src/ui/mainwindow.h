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

#ifndef ACSMGET_MAINWINDOW_H
#define ACSMGET_MAINWINDOW_H

#include <gtkmm.h>
#include "../core/adeptclient.h"
#include "authorizationwindow.h"

namespace acsmget
{
	class MainWindow final : public Gtk::Window
	{
		public:
			MainWindow(AdeptClient *adeptclient, const std::string &command_line_filename);

		private:
			void on_action_open_acsm();
			void on_action_authorize();
			void on_action_erase_authorization();
			void on_action_return_loan();
			void on_action_about();

			void on_authorized_update();
			void on_busy_update();
			void on_download_started();
			void on_download_success();
			void on_download_failed();
			void on_erase_authorization_started();
			void on_erase_authorization_success();
			void on_erase_authorization_failed();
			void on_progress_update();
			void on_return_loan_started();
			void on_return_loan_success();
			void on_return_loan_failed();

			Gtk::HeaderBar m_headerbar;
			Gtk::Button m_open_button;
			Gtk::MenuButton m_menu_button;
			Gtk::Box m_main_box;
			Gtk::Box m_status_box;
			Gtk::Label m_status_label;
			Gtk::LinkButton m_open_download_linkbutton;
			Gtk::LinkButton m_open_download_folder_linkbutton;
			Gtk::Frame m_additional_info_frame;
			Gtk::Label m_additional_info_label;
			Gtk::ProgressBar m_progressbar;

			AuthorizationWindow m_authorization_window;

			Glib::RefPtr<Gio::SimpleActionGroup> m_actiongroup;

			AdeptClient *m_adeptclient;

			bool m_authorization_unknown = true;
			std::string m_command_line_filename;
	};
}

#endif //ACSMGET_MAINWINDOW_H
