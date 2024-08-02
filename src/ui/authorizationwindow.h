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

#ifndef ACSMGET_AUTHORIZATIONWINDOW_H
#define ACSMGET_AUTHORIZATIONWINDOW_H

#include <gtkmm.h>
#include "../core/adeptclient.h"

namespace acsmget
{
	class AuthorizationWindow final : public Gtk::Window
	{
		public:
			AuthorizationWindow(AdeptClient *adeptclient);

		private:
			void on_action_authorize();
			void on_authorize_success();
			void on_authorize_failed();
			void update_widget_visibility();

			Gtk::HeaderBar m_headerbar;
			Gtk::Box m_main_box;
			Gtk::Label m_help_label;
			Gtk::Box m_account_adobe_box;
			Gtk::RadioButtonGroup m_account_radiobuttongroup;
			Gtk::RadioButton m_account_adobe_radiobutton;
			Gtk::Entry m_username_entry;
			Gtk::Entry m_password_entry;
			Gtk::Box m_adobe_link_box;
			Gtk::LinkButton m_adobe_linkbutton;
			Gtk::Box m_account_none_box;
			Gtk::RadioButton m_account_none_radiobutton;
			Gtk::Box m_bottom_buttons_box;
			Gtk::Button m_cancel_button;
			Gtk::Button m_authorize_button;
			Gtk::Spinner m_spinner;

			AdeptClient *m_adeptclient;
	};
}

#endif //ACSMGET_AUTHORIZATIONWINDOW_H
