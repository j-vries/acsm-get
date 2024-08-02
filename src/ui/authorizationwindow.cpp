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
#include "authorizationwindow.h"
#include "messagedialogs.h"

namespace acsmget
{
	AuthorizationWindow::AuthorizationWindow(AdeptClient *adeptclient) :
		m_main_box(Gtk::Orientation::ORIENTATION_VERTICAL, 8),
		m_help_label(texts_desc_authorization),
		m_account_adobe_radiobutton(m_account_radiobuttongroup, texts_lbl_adobe_account, true),
		m_adobe_linkbutton("https://account.adobe.com/", texts_action_create_or_manage_account),
		m_account_none_radiobutton(m_account_radiobuttongroup, texts_lbl_no_account, true),
		m_bottom_buttons_box(Gtk::Orientation::ORIENTATION_HORIZONTAL, 8),
		m_cancel_button(texts_action_cancel),
		m_authorize_button(texts_action_authorize)
	{
		m_adeptclient = adeptclient;

		set_resizable(false);
		set_titlebar(m_headerbar);
		add(m_main_box);

		m_headerbar.set_show_close_button();
		m_headerbar.set_title(texts_action_authorize_computer);

		m_main_box.set_border_width(8);
		m_main_box.pack_start(m_help_label);
		m_main_box.pack_start(m_account_adobe_box);
		m_main_box.pack_start(m_username_entry);
		m_main_box.pack_start(m_password_entry);
		m_main_box.pack_start(m_adobe_link_box);
		m_main_box.pack_start(m_account_none_box);
		m_main_box.pack_start(m_bottom_buttons_box);

		m_account_adobe_box.pack_start(m_account_adobe_radiobutton, Gtk::PACK_SHRINK);

		m_account_none_box.pack_start(m_account_none_radiobutton, Gtk::PACK_SHRINK);

		m_bottom_buttons_box.pack_start(m_cancel_button);
		m_bottom_buttons_box.pack_start(m_authorize_button);
		m_bottom_buttons_box.pack_start(m_spinner);

		m_help_label.set_line_wrap();
		m_help_label.set_max_width_chars(70);

		m_account_adobe_radiobutton.signal_toggled().connect(sigc::mem_fun(*this, &AuthorizationWindow::update_widget_visibility));

		m_username_entry.set_margin_left(56);
		m_username_entry.set_margin_right(56);
		m_username_entry.set_placeholder_text(texts_lbl_username);
		m_username_entry.signal_changed().connect(sigc::mem_fun(*this, &AuthorizationWindow::update_widget_visibility));

		m_password_entry.set_margin_left(56);
		m_password_entry.set_margin_right(56);
		m_password_entry.set_placeholder_text(texts_lbl_password);
		m_password_entry.set_visibility(false);
		m_password_entry.signal_changed().connect(sigc::mem_fun(*this, &AuthorizationWindow::update_widget_visibility));

		m_adobe_link_box.set_halign(Gtk::Align::ALIGN_CENTER);
		m_adobe_link_box.pack_start(m_adobe_linkbutton, Gtk::PACK_SHRINK);

		m_adobe_linkbutton.set_margin_left(56);
		m_adobe_linkbutton.set_margin_right(56);

		m_account_none_radiobutton.signal_toggled().connect(sigc::mem_fun(*this, &AuthorizationWindow::update_widget_visibility));

		m_bottom_buttons_box.set_halign(Gtk::Align::ALIGN_END);

		m_cancel_button.set_size_request(100, -1);
		m_cancel_button.signal_clicked().connect(sigc::mem_fun(*this, &AuthorizationWindow::hide));

		m_authorize_button.set_size_request(100, -1);
		m_authorize_button.signal_clicked().connect(sigc::mem_fun(*this, &AuthorizationWindow::on_action_authorize));

		m_spinner.set_size_request(100, -1);

		m_adeptclient->get_authorize_success_dispatcher()->connect(sigc::mem_fun(*this, &AuthorizationWindow::on_authorize_success));
		m_adeptclient->get_authorize_failed_dispatcher() ->connect(sigc::mem_fun(*this, &AuthorizationWindow::on_authorize_failed));
		m_adeptclient->get_busy_dispatcher()             ->connect(sigc::mem_fun(*this, &AuthorizationWindow::update_widget_visibility));

		show_all_children();

		update_widget_visibility();
	}

	void AuthorizationWindow::on_action_authorize()
	{
		m_adeptclient->authorize_async(m_account_adobe_radiobutton.get_active(), m_username_entry.get_text(), m_password_entry.get_text());
	}

	void AuthorizationWindow::on_authorize_success()
	{
		show_info_message_dialog(*this, texts_msg_inf_successfully_authorized, texts_action_authorize_computer);
		hide();
	}

	void AuthorizationWindow::on_authorize_failed()
	{
		show_error_message_dialog(*this, texts_msg_err_authorization, m_adeptclient->get_error());
	}

	void AuthorizationWindow::update_widget_visibility()
	{
		m_headerbar.set_sensitive(!m_adeptclient->get_busy());
		m_main_box.set_sensitive(!m_adeptclient->get_busy());
		if(m_adeptclient->get_busy()) {
			m_spinner.show();
			m_spinner.start();
			m_authorize_button.hide();
		}
		else {
			m_spinner.stop();
			m_spinner.hide();
			m_authorize_button.show();

			m_username_entry.set_sensitive(m_account_adobe_radiobutton.get_active());
			m_password_entry.set_sensitive(m_account_adobe_radiobutton.get_active());
			m_authorize_button.set_sensitive(!m_account_adobe_radiobutton.get_active() || (!m_username_entry.get_text().empty() && !m_password_entry.get_text().empty()));
		}
	}
}
