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
#include "aboutdialog.h"
#include "acsmfilechooserdialog.h"
#include "mainwindow.h"
#include "messagedialogs.h"

namespace acsmget
{
	MainWindow::MainWindow(AdeptClient *adeptclient, const std::string &command_line_filename) :
		m_open_button(texts_action_open),
		m_main_box(Gtk::Orientation::ORIENTATION_VERTICAL, 16),
		m_status_box(Gtk::Orientation::ORIENTATION_HORIZONTAL, 8),
		m_authorization_window(adeptclient)
	{
		m_adeptclient           = adeptclient;
		m_command_line_filename = command_line_filename;

		set_default_icon(Gdk::Pixbuf::create_from_resource("/acsmget/acsm-get.svg"));
		set_resizable(false);
		set_size_request(640, 480);
		set_titlebar(m_headerbar);

		add(m_main_box);

		m_headerbar.set_show_close_button();
		m_headerbar.set_title(Glib::get_application_name());
		m_headerbar.pack_start(m_open_button);
		m_headerbar.pack_end(m_menu_button);

		m_open_button.get_style_context()->add_class("suggested-action");
		m_open_button.signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::on_action_open_acsm));

		m_menu_button.set_image_from_icon_name("open-menu-symbolic");

		auto menu = Gio::Menu::create();
		auto section1 = Gio::Menu::create();
		section1->append(texts_action_authorize_computer,  "actions.authorize");
		section1->append(texts_action_erase_authorization, "actions.erase_authorization");
		auto section2 = Gio::Menu::create();
		section2->append(texts_action_return_loan,         "actions.return_loan");
		auto section3 = Gio::Menu::create();
		section3->append(texts_action_about,               "actions.about");
		menu->append_section(section1);
		menu->append_section(section2);
		menu->append_section(section3);
		m_menu_button.set_menu_model(menu);

		m_actiongroup = Gio::SimpleActionGroup::create();
		m_actiongroup->add_action("authorize",           sigc::mem_fun(*this, &MainWindow::on_action_authorize));
		m_actiongroup->add_action("erase_authorization", sigc::mem_fun(*this, &MainWindow::on_action_erase_authorization));
		m_actiongroup->add_action("return_loan",         sigc::mem_fun(*this, &MainWindow::on_action_return_loan));
		m_actiongroup->add_action("about",               sigc::mem_fun(*this, &MainWindow::on_action_about));
		insert_action_group("actions", m_actiongroup);

		m_main_box.set_border_width(8);
		m_main_box.set_valign(Gtk::Align::ALIGN_CENTER);
		m_main_box.pack_start(m_status_box,            false, false);
		m_main_box.pack_start(m_additional_info_frame, false, false);
		m_main_box.pack_start(m_progressbar,           false, false);

		m_status_box.set_halign(Gtk::Align::ALIGN_CENTER);
		m_status_box.set_margin_left(56);
		m_status_box.set_margin_right(56);
		m_status_box.pack_start(m_status_label,                    false, false);
		m_status_box.pack_start(m_open_download_linkbutton,        false, false);
		m_status_box.pack_start(m_open_download_folder_linkbutton, false, false);

		m_open_download_linkbutton.set_label(texts_action_open);

		m_open_download_folder_linkbutton.set_label(texts_action_show_in_folder);

		m_additional_info_frame.add(m_additional_info_label);

		m_additional_info_label.set_line_wrap();
		m_additional_info_label.set_margin_left(8);
		m_additional_info_label.set_margin_right(8);
		m_additional_info_label.set_margin_top(8);
		m_additional_info_label.set_margin_bottom(8);
		m_additional_info_label.set_max_width_chars(64);
		m_additional_info_label.set_selectable();

		m_progressbar.set_margin_left(56);
		m_progressbar.set_margin_right(56);

		m_adeptclient->get_authorized_dispatcher()                 ->connect(sigc::mem_fun(*this, &MainWindow::on_authorized_update));
		m_adeptclient->get_busy_dispatcher()                       ->connect(sigc::mem_fun(*this, &MainWindow::on_busy_update));
		m_adeptclient->get_erase_authorization_started_dispatcher()->connect(sigc::mem_fun(*this, &MainWindow::on_erase_authorization_started));
		m_adeptclient->get_erase_authorization_success_dispatcher()->connect(sigc::mem_fun(*this, &MainWindow::on_erase_authorization_success));
		m_adeptclient->get_erase_authorization_failed_dispatcher() ->connect(sigc::mem_fun(*this, &MainWindow::on_erase_authorization_failed));
		m_adeptclient->get_download_started_dispatcher()           ->connect(sigc::mem_fun(*this, &MainWindow::on_download_started));
		m_adeptclient->get_download_success_dispatcher()           ->connect(sigc::mem_fun(*this, &MainWindow::on_download_success));
		m_adeptclient->get_download_failed_dispatcher()            ->connect(sigc::mem_fun(*this, &MainWindow::on_download_failed));
		m_adeptclient->get_progress_dispatcher()                   ->connect(sigc::mem_fun(*this, &MainWindow::on_progress_update));
		m_adeptclient->get_return_loan_started_dispatcher()        ->connect(sigc::mem_fun(*this, &MainWindow::on_return_loan_started));
		m_adeptclient->get_return_loan_success_dispatcher()        ->connect(sigc::mem_fun(*this, &MainWindow::on_return_loan_success));
		m_adeptclient->get_return_loan_failed_dispatcher()         ->connect(sigc::mem_fun(*this, &MainWindow::on_return_loan_failed));

		show_all_children();

		m_progressbar.hide();
		m_open_download_linkbutton.hide();
		m_open_download_folder_linkbutton.hide();
		m_additional_info_frame.hide();

		m_adeptclient->check_authorization_async();
	}

	void MainWindow::on_action_open_acsm()
	{
		AcsmFileChooserDialog dialog(*this);
		if(dialog.run() == Gtk::RESPONSE_OK) {
			m_adeptclient->download_async(dialog.get_filename());
		}
	}

	void MainWindow::on_action_authorize()
	{
		m_authorization_window.set_modal();
		m_authorization_window.set_transient_for(*this);
		m_authorization_window.show();
	}

	void MainWindow::on_action_erase_authorization()
	{
		if(show_yes_no_message_dialog(*this, texts_msg_ask_confirm_erase_auth, texts_action_erase_authorization) == Gtk::ResponseType::RESPONSE_YES) {
			m_adeptclient->erase_authorization_async();
		}
	}

	void MainWindow::on_action_return_loan()
	{
		AcsmFileChooserDialog dialog(*this);
		if(dialog.run() == Gtk::RESPONSE_OK) {
			m_adeptclient->return_loan_async(dialog.get_filename());
		}
	}

	void MainWindow::on_action_about()
	{
		show_about_dialog(*this);
	}

	void MainWindow::on_authorized_update()
	{
		m_open_button.set_sensitive(m_adeptclient->get_authorized());
		m_actiongroup->action_enabled_changed("open",                m_adeptclient->get_authorized());
		m_actiongroup->action_enabled_changed("authorize",          !m_adeptclient->get_authorized());
		m_actiongroup->action_enabled_changed("erase_authorization", m_adeptclient->get_authorized());
		m_actiongroup->action_enabled_changed("return_loan",         m_adeptclient->get_authorized());
		m_status_label.set_text(m_adeptclient->get_authorized() ? texts_msg_inf_open_file_to_download : texts_msg_inf_auth_computer_to_download);
		m_additional_info_frame.hide();

		if(m_authorization_unknown) {			// If the computer is not authorized on the first check after the application starts, show the authorization window.
			m_authorization_unknown = false;
			if(!m_adeptclient->get_authorized()) {
				on_action_authorize();				
			}
		}
	}

	void MainWindow::on_busy_update()
	{
		m_headerbar.set_sensitive(!m_adeptclient->get_busy());
		m_main_box.set_sensitive( !m_adeptclient->get_busy());

		if(!m_adeptclient->get_busy() && m_adeptclient->get_authorized()) {	// If a file was passed on the command line and the computer is authorized, download it.
			if(!m_command_line_filename.empty()) {
				m_adeptclient->download_async(m_command_line_filename);
				m_command_line_filename = "";
			}
		}
	}

	void MainWindow::on_download_started()
	{
		m_status_label.set_text(texts_msg_inf_downloading);
		m_additional_info_frame.hide();
		m_open_download_linkbutton.hide();
		m_open_download_folder_linkbutton.hide();
		m_progressbar.show();
	}

	void MainWindow::on_download_success()
	{
		m_status_label.set_text(texts_msg_inf_download_completed);
		m_additional_info_frame.hide();
		m_open_download_linkbutton.show();
		m_open_download_folder_linkbutton.show();
		m_progressbar.hide();

		auto download_info = m_adeptclient->get_download_info();
		m_open_download_linkbutton.       set_uri("file://" + download_info->get_filename());
		m_open_download_folder_linkbutton.set_uri("file://" + download_info->get_parent_dir());

		if(download_info->get_is_loan()) {
			m_additional_info_label.set_text(texts_msg_inf_book_is_loan);
			m_additional_info_frame.show();
		}
	}

	void MainWindow::on_download_failed()
	{
		m_status_label.set_text(texts_msg_inf_download_failed);
		m_additional_info_label.set_text(m_adeptclient->get_error());
		m_additional_info_frame.show();
		m_open_download_linkbutton.hide();
		m_open_download_folder_linkbutton.hide();
		m_progressbar.hide();
	}

	void MainWindow::on_erase_authorization_started()
	{
		m_status_label.set_text(texts_msg_inf_erasing_authorization);
		m_open_download_linkbutton.hide();
		m_open_download_folder_linkbutton.hide();
		m_progressbar.hide();
	}

	void MainWindow::on_erase_authorization_success()
	{
		m_status_label.set_text(texts_msg_inf_authorization_erased);
		m_open_download_linkbutton.hide();
		m_open_download_folder_linkbutton.hide();
		m_progressbar.hide();
	}

	void MainWindow::on_erase_authorization_failed()
	{
		m_status_label.set_text(texts_msg_err_erase_authorization);
		m_additional_info_label.set_text(m_adeptclient->get_error());
		m_additional_info_frame.show();
		m_open_download_linkbutton.hide();
		m_open_download_folder_linkbutton.hide();
		m_progressbar.hide();
	}

	void MainWindow::on_progress_update()
	{
		m_progressbar.set_fraction(m_adeptclient->get_progress());
	}

	void MainWindow::on_return_loan_started()
	{
		m_status_label.set_text(texts_msg_inf_returning_loan);
		m_additional_info_frame.hide();
		m_open_download_linkbutton.hide();
		m_open_download_folder_linkbutton.hide();
		m_progressbar.hide();
	}

	void MainWindow::on_return_loan_success()
	{
		m_status_label.set_text(texts_msg_inf_loan_returned);
		m_additional_info_frame.hide();
		m_open_download_linkbutton.hide();
		m_open_download_folder_linkbutton.hide();
		m_progressbar.hide();
	}

	void MainWindow::on_return_loan_failed()
	{
		m_status_label.set_text(texts_msg_inf_return_loan_failed);
		m_additional_info_label.set_text(m_adeptclient->get_error());
		m_additional_info_frame.show();
		m_open_download_linkbutton.hide();
		m_open_download_folder_linkbutton.hide();
		m_progressbar.hide();
	}
}
