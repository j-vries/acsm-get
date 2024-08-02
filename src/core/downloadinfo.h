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

#ifndef ACSMGET_DOWNLOADINFO_H
#define ACSMGET_DOWNLOADINFO_H

#include <string>

namespace acsmget
{
	class DownloadInfo final
	{
		public:
			DownloadInfo();
			DownloadInfo(const std::string &filename, bool is_loan);
			std::string get_filename();
			std::string get_parent_dir();
			bool get_is_loan();

		private:
			std::string m_filename;
			std::string m_parent_dir;
			bool m_is_loan;
	};
}

#endif //ACSMGET_DOWNLOADINFO_H
