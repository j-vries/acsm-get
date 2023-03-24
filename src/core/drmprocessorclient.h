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

#ifndef ACSMGET_DRMPROCESSORCLIENT_H
#define ACSMGET_DRMPROCESSORCLIENT_H

#include <curl/curl.h>
#include <drmprocessorclientimpl.h>
#include <filesystem>
#include <glibmm.h>

namespace acsmget
{
	class DRMProcessorClient final : public DRMProcessorClientImpl
	{
		public:
			DRMProcessorClient();
			std::string sendHTTPRequest(const std::string &url, const std::string &post_data = std::string(""), const std::string &content_type = std::string(""), std::map<std::string, std::string> *response_headers = 0, int fd = 0, bool resume = false) override;
			sigc::signal<void(double)> signal_progress_update();
		
		private:
			sigc::signal<void(double)> m_signal_progress_update;
	};
}

#endif //ACSMGET_DRMPROCESSORCLIENT_H
