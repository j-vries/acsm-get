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

#include <glibmm.h>
#include <libgourou_common.h>
#include <sys/stat.h>
#include <unistd.h>
#include "drmprocessorclient.h"

#define DOWNLOAD_DISPLAY_THRESHOLD 10240
#define HTTP_REQ_MAX_RETRY	5
static unsigned downloaded_bytes;

namespace acsmget
{
	DRMProcessorClient::DRMProcessorClient()
	{
	}
	
	static size_t progress_callback(void *clientp, curl_off_t dltotal, curl_off_t dlnow, curl_off_t ultotal, curl_off_t ulnow)
	{
	    if(dltotal >= DOWNLOAD_DISPLAY_THRESHOLD) {
			auto client = static_cast<DRMProcessorClient *>(clientp);
			client->signal_progress_update().emit((double) dlnow / (double) dltotal);
		}
		return 0;
	}

	static size_t write_callback_fd(char *ptr, size_t size, size_t nmemb, void *userdata)
	{
		auto fd = *(int *)userdata;
		size_t res = write(fd, ptr, size * nmemb);
		downloaded_bytes += res;
		return res;
	}

	static size_t write_callback(char *ptr, size_t size, size_t nmemb, void *userdata)
	{
		auto reply_data = (gourou::ByteArray *)userdata;
		reply_data->append((unsigned char *)ptr, size * nmemb);
		return size * nmemb;
	}

	static size_t header_callback(char *buffer, size_t size, size_t nitems, void *userdata)
	{
		std::map<std::string, std::string> *response_headers = (std::map<std::string, std::string>*)userdata;
		std::string::size_type pos = 0;
		std::string buf(buffer, size * nitems);

		pos = buf.find(":", pos);

		if(pos != std::string::npos) {
			auto key   = std::string(buffer, pos);
			auto value = std::string(&buffer[pos+1], (size * nitems) - (pos + 1));

			key   = gourou::trim(key);
			value = gourou::trim(value);

			(*response_headers)[key] = value;
		}
		
		return size * nitems;
	}

	// Sends a HTTP request just like the base class, but with progress reported through signal_progress_update instead of stdout.
	std::string DRMProcessorClient::sendHTTPRequest(const std::string &url, const std::string &post_data, const std::string &content_type, std::map<std::string, std::string> *response_headers, int fd, bool resume)
	{
		gourou::ByteArray reply_data;
		std::map<std::string, std::string> local_headers;

		if(!response_headers) {
			response_headers = &local_headers;
		}
		
		unsigned prev_downloaded_bytes;
		downloaded_bytes = 0;
		if(fd && resume) {
			struct stat _stat;
			if(!fstat(fd, &_stat)) {
				downloaded_bytes = _stat.st_size;
			}
		}
		
		auto curl = curl_easy_init();
		CURLcode res;
		curl_easy_setopt(curl, CURLOPT_URL,            url.c_str());
		curl_easy_setopt(curl, CURLOPT_USERAGENT,      "book2png");
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
		
		struct curl_slist *list = nullptr;
		list = curl_slist_append(list, "Accept: */*");
		std::string _contentType;
		if(content_type.size()) {
			_contentType = "Content-Type: " + content_type;
			list = curl_slist_append(list, _contentType.c_str());
		}

		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, list);

		if(post_data.size()) {
			curl_easy_setopt(curl, CURLOPT_POST,          1L);
			curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, post_data.size());
			curl_easy_setopt(curl, CURLOPT_POSTFIELDS,    post_data.data());
		}

		if(fd) {
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback_fd);
			curl_easy_setopt(curl, CURLOPT_WRITEDATA,     (void *)&fd);
		}
		else {
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
			curl_easy_setopt(curl, CURLOPT_WRITEDATA,     (void *)&reply_data);
		}
		
		curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, header_callback);
		curl_easy_setopt(curl, CURLOPT_HEADERDATA,     (void *)response_headers);
		
		curl_easy_setopt(curl, CURLOPT_XFERINFOFUNCTION, progress_callback);
		curl_easy_setopt(curl, CURLOPT_XFERINFODATA,     this);
		curl_easy_setopt(curl, CURLOPT_NOPROGRESS,       0);

		for(int i = 0; i < HTTP_REQ_MAX_RETRY; i++) {
			prev_downloaded_bytes = downloaded_bytes;
			if(downloaded_bytes) {
				curl_easy_setopt(curl, CURLOPT_RESUME_FROM, downloaded_bytes);
			}
				
			res = curl_easy_perform(curl);

			if(res == CURLE_COULDNT_CONNECT) {						// Unable to connect: retry.
			}
			else if(res == CURLE_RECV_ERROR) {
				if(prev_downloaded_bytes != downloaded_bytes) {		// Receive error, but some data received: retry and decrease remaining attempts.
					i--;
				}
				else {												// Receive error, but no data received: retry.
				}
			}
			else {													// Some other error: fail.
				break;
			}

			usleep((250 * 1000) * (i + 1));
		}
		
		curl_slist_free_all(list);
		curl_easy_cleanup(curl);
	
		if(res != CURLE_OK) {
			EXCEPTION(gourou::CLIENT_NETWORK_ERROR, "Error " << curl_easy_strerror(res));
		}
		
		return std::string((char*)reply_data.data(), reply_data.length());
	}

	sigc::signal<void(double)> DRMProcessorClient::signal_progress_update()
	{
		return m_signal_progress_update;
	}
}
