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

#ifndef ACSMGET_ADEPTCLIENT_H
#define ACSMGET_ADEPTCLIENT_H

#include <glibmm.h>
#include <string>
#include <thread>
#include "downloadinfo.h"
#include "drmprocessorclient.h"

namespace acsmget
{
	class AdeptClient final
	{
		public:
			AdeptClient(const std::string &prg_name);
			~AdeptClient();
			void authorize_async(bool use_adobe_account, const std::string &username, const std::string &password);
			void check_authorization_async();
			void download_async(const std::string &acsm_file);
			void erase_authorization_async();
			void return_loan_async(const std::string &acsm_file);

			bool get_authorized();
			bool get_busy();
			DownloadInfo *get_download_info();
			Glib::ustring get_error();
			double get_progress();

			Glib::Dispatcher *get_authorize_success_dispatcher();
			Glib::Dispatcher *get_authorize_failed_dispatcher();
			Glib::Dispatcher *get_authorized_dispatcher();
			Glib::Dispatcher *get_download_started_dispatcher();
			Glib::Dispatcher *get_download_success_dispatcher();
			Glib::Dispatcher *get_download_failed_dispatcher();
			Glib::Dispatcher *get_erase_authorization_started_dispatcher();
			Glib::Dispatcher *get_erase_authorization_success_dispatcher();
			Glib::Dispatcher *get_erase_authorization_failed_dispatcher();
			Glib::Dispatcher *get_return_loan_started_dispatcher();
			Glib::Dispatcher *get_return_loan_success_dispatcher();
			Glib::Dispatcher *get_return_loan_failed_dispatcher();
			Glib::Dispatcher *get_busy_dispatcher();
			Glib::Dispatcher *get_progress_dispatcher();

		private:
			void authorize(bool use_adobe_account, const std::string &username, const std::string &password);
			void check_authorization();
			void download(const std::string &acsm_file);
			void erase_authorization();
			void return_loan(const std::string &acsm_file);

			void set_authorized(bool authorized = true);
			void set_progress(double progress);

			void join_and_delete_thread();

			bool m_authorized;

			DownloadInfo m_download_info;
			Glib::ustring m_error;
			double m_progress;

			std::string m_authorization_dir;
			std::string m_activation_file;
			std::string m_device_file;
			std::string m_device_salt_file;

			std::thread *m_thread;

			DRMProcessorClient m_drmprocessorclient;

			Glib::Dispatcher m_authorize_success_dispatcher;
			Glib::Dispatcher m_authorize_failed_dispatcher;
			Glib::Dispatcher m_authorized_dispatcher;
			Glib::Dispatcher m_download_started_dispatcher;
			Glib::Dispatcher m_download_success_dispatcher;
			Glib::Dispatcher m_download_failed_dispatcher;
			Glib::Dispatcher m_erase_authorization_started_dispatcher;
			Glib::Dispatcher m_erase_authorization_success_dispatcher;
			Glib::Dispatcher m_erase_authorization_failed_dispatcher;
			Glib::Dispatcher m_return_loan_started_dispatcher;
			Glib::Dispatcher m_return_loan_success_dispatcher;
			Glib::Dispatcher m_return_loan_failed_dispatcher;
			Glib::Dispatcher m_busy_dispatcher;
			Glib::Dispatcher m_progress_dispatcher;
			Glib::Dispatcher m_thread_finished_dispatcher;
	};
}

#endif //ACSMGET_ADEPTCLIENT_H
