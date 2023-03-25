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

#include <filesystem>
#include <libgourou.h>
#include "../texts/texts.h"
#include "../utils/filesystemutils.h"
#include "adeptclient.h"

namespace acsmget
{
	AdeptClient::AdeptClient(const std::string &prg_name)
	{
		m_authorization_dir = get_config_dir() + "/" + prg_name + "/authorization";
		m_activation_file   = m_authorization_dir + "/activation.xml";
		m_device_file       = m_authorization_dir + "/device.xml";
		m_device_salt_file  = m_authorization_dir + "/devicesalt";

		m_drmprocessorclient.signal_progress_update().connect(sigc::mem_fun(*this, &AdeptClient::set_progress));

		m_thread = nullptr;
		m_thread_finished_dispatcher.connect(sigc::mem_fun(*this, &AdeptClient::join_and_delete_thread));
	}

	AdeptClient::~AdeptClient()
	{
		if(m_thread != nullptr) {
			join_and_delete_thread();
		}
	}

	void AdeptClient::authorize_async(bool use_adobe_account, const std::string &username, const std::string &password)
	{
		if(m_thread == nullptr) {
			m_thread = new std::thread([this](bool _use_adobe_account, const std::string &_username, const std::string &_password) {
				authorize(_use_adobe_account, _username, _password);
				m_thread_finished_dispatcher.emit();
			}, use_adobe_account, username, password);
			m_busy_dispatcher.emit();
		}
	}

	void AdeptClient::check_authorization_async()
	{
		if(m_thread == nullptr) {
			m_thread = new std::thread([this] {
				check_authorization();
				m_thread_finished_dispatcher.emit();
			});
			m_busy_dispatcher.emit();
		}
	}
	
	void AdeptClient::download_async(const std::string &acsm_file)
	{
		if(m_thread == nullptr) {
			m_thread = new std::thread([this] (const std::string &_acsm_file) {
				download(_acsm_file);
				m_thread_finished_dispatcher.emit();
			}, acsm_file);
			m_busy_dispatcher.emit();
		}
	}

	void AdeptClient::erase_authorization_async()
	{
		if(m_thread == nullptr) {
			m_thread = new std::thread([this] {
				erase_authorization();
				m_thread_finished_dispatcher.emit();
			});
			m_busy_dispatcher.emit();
		}
	}

	void AdeptClient::return_loan_async(const std::string &acsm_file)
	{
		if(m_thread == nullptr) {
			m_thread = new std::thread([this] (const std::string &_acsm_file) {
				return_loan(_acsm_file);
				m_thread_finished_dispatcher.emit();
			}, acsm_file);
			m_busy_dispatcher.emit();
		}
	}

	bool AdeptClient::get_authorized()
	{
		return m_authorized;
	}

	bool AdeptClient::get_busy()
	{
		return m_thread != nullptr;
	}

	DownloadInfo *AdeptClient::get_download_info()
	{
		return &m_download_info;
	}

	Glib::ustring AdeptClient::get_error()
	{
		return m_error;
	}

	double AdeptClient::get_progress()
	{
		return m_progress;
	}

	Glib::Dispatcher *AdeptClient::get_authorize_success_dispatcher()
	{
		return &m_authorize_success_dispatcher;
	}

	Glib::Dispatcher *AdeptClient::get_authorize_failed_dispatcher()
	{
		return &m_authorize_failed_dispatcher;
	}

	Glib::Dispatcher *AdeptClient::get_authorized_dispatcher()
	{
		return &m_authorized_dispatcher;
	}

	Glib::Dispatcher *AdeptClient::get_download_started_dispatcher()
	{
		return &m_download_started_dispatcher;
	}

	Glib::Dispatcher *AdeptClient::get_download_success_dispatcher()
	{
		return &m_download_success_dispatcher;
	}

	Glib::Dispatcher *AdeptClient::get_download_failed_dispatcher()
	{
		return &m_download_failed_dispatcher;
	}

	Glib::Dispatcher *AdeptClient::get_erase_authorization_started_dispatcher()
	{
		return &m_erase_authorization_started_dispatcher;
	}

	Glib::Dispatcher *AdeptClient::get_erase_authorization_success_dispatcher()
	{
		return &m_erase_authorization_success_dispatcher;
	}

	Glib::Dispatcher *AdeptClient::get_erase_authorization_failed_dispatcher()
	{
		return &m_erase_authorization_failed_dispatcher;
	}

	Glib::Dispatcher *AdeptClient::get_return_loan_started_dispatcher()
	{
		return &m_return_loan_started_dispatcher;
	}

	Glib::Dispatcher *AdeptClient::get_return_loan_success_dispatcher()
	{
		return &m_return_loan_success_dispatcher;
	}

	Glib::Dispatcher *AdeptClient::get_return_loan_failed_dispatcher()
	{
		return &m_return_loan_failed_dispatcher;
	}

	Glib::Dispatcher *AdeptClient::get_busy_dispatcher()
	{
		return &m_busy_dispatcher;
	}

	Glib::Dispatcher *AdeptClient::get_progress_dispatcher()
	{
		return &m_progress_dispatcher;
	}

	void AdeptClient::authorize(bool use_adobe_account, const std::string &username, const std::string &password)
	{
		try {
			auto processor = gourou::DRMProcessor::createDRMProcessor(&m_drmprocessorclient, false, m_authorization_dir);

			if(use_adobe_account) {
				processor->signIn(username, password);
			}
			else {
				processor->signIn("anonymous", "");
			}

			processor->activateDevice();
			set_authorized();
			m_authorize_success_dispatcher.emit();
		}
		catch(std::exception &e) {
			m_error = e.what();
			m_authorize_failed_dispatcher.emit();
		}
	}

	void AdeptClient::check_authorization()
	{
		try {
			set_authorized(std::filesystem::exists(m_activation_file)
						&& std::filesystem::exists(m_device_file)
						&& std::filesystem::exists(m_device_salt_file));
		}
		catch(std::exception &) {
			set_authorized(false);
		}
	}
	
	void AdeptClient::download(const std::string &acsm_file)
	{
		set_progress(0);
		m_download_started_dispatcher.emit();
		try {
			gourou::DRMProcessor processor(&m_drmprocessorclient, m_device_file, m_activation_file, m_device_salt_file);
			auto item = processor.fulfill(acsm_file);

			auto creator   = item->getMetadata("creator");
			auto title     = item->getMetadata("title");
			auto book_name = creator + (creator.empty() || title.empty() ? "" : " - ") + title;
			book_name      = book_name.empty() ? texts_other_untitled_book_name : book_name;
			auto directory = std::filesystem::path(acsm_file).parent_path();
			auto drm_file  = get_available_filename(directory, remove_illegal_chars(book_name), ".drm");

			auto type = processor.download(item, drm_file);
			std::string out_file;

			if(type == gourou::DRMProcessor::ITEM_TYPE::EPUB) {
				out_file = get_available_filename(directory, book_name, ".epub");
				processor.removeDRM(drm_file, drm_file, type);						// Epub DRM is removed in-place.
				std::filesystem::rename(drm_file, out_file);						// So rename the file.
			}
			else {
				out_file = get_available_filename(directory, book_name, ".pdf");
				processor.removeDRM(drm_file, out_file, type);						// Pdf DRM removal creates a new file.
				std::filesystem::remove(drm_file);									// Remove the DRM file.
			}

			m_download_info = DownloadInfo(out_file, item->getLoanToken() != nullptr);

			m_download_success_dispatcher.emit();
		}
		catch(std::exception &e) {
			m_error = e.what();
			m_download_failed_dispatcher.emit();
		}
	}

	void AdeptClient::erase_authorization()
	{
		try {
			std::filesystem::remove(m_activation_file);
			std::filesystem::remove(m_device_file);
			std::filesystem::remove(m_device_salt_file);
			set_authorized(false);
			m_erase_authorization_success_dispatcher.emit();
		}
		catch(std::exception &e) {
			m_error = e.what();
			m_erase_authorization_failed_dispatcher.emit();
		}
	}

	void AdeptClient::return_loan(const std::string &acsm_file)
	{
		m_return_loan_started_dispatcher.emit();
		try {
			gourou::DRMProcessor processor(&m_drmprocessorclient, m_device_file, m_activation_file, m_device_salt_file);
			auto item = processor.fulfill(acsm_file);

			auto token = item->getLoanToken();
			if(token) {
				processor.returnLoan((*token)["id"], (*token)["operatorURL"]);
				m_return_loan_success_dispatcher();
			}
			else {
				m_error = texts_msg_err_not_a_loan;
				m_return_loan_failed_dispatcher.emit();
			}
		}
		catch(std::exception &e) {
			m_error = e.what();
			m_return_loan_failed_dispatcher.emit();
		}
	}

	void AdeptClient::set_authorized(bool authorized)
	{
		m_authorized = authorized;
		m_authorized_dispatcher.emit();
	}

	void AdeptClient::set_progress(double progress)
	{
		m_progress = progress;
		m_progress_dispatcher.emit();
	}

	void AdeptClient::join_and_delete_thread()
	{
		if(m_thread->joinable()) {
			m_thread->join();
		}
		delete m_thread;
		m_thread = nullptr;
		m_busy_dispatcher.emit();
	}
}
