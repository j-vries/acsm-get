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
#include <glibmm.h>
#include "../texts/texts.h"
#include "filesystemutils.h"

namespace acsmget
{
	std::string get_config_dir()
	{
		auto home = std::getenv("HOME");
		return home != nullptr ? std::string(home) + "/.config" : throw std::runtime_error(texts_msg_err_home_variable_not_set);
	}

	std::string get_available_filename(const std::string &directory, const std::string &suggested, const std::string &extension)
	{
		auto filename = directory + "/" + suggested + extension;
		if(!std::filesystem::exists(filename)) {
			return filename;
		}
		for(int i = 1; ; i++) {
			filename = directory + "/" + suggested + " (" + std::to_string(i) + ")" + extension;
			if(!std::filesystem::exists(filename)) {
				return filename;
			}
		}
	}

	std::string remove_illegal_chars(const std::string &suggested)
	{
		std::string cleaned(suggested);
		char illegal_chars[] = "\\/:?\"<>|";
		for(char c : illegal_chars) {
			cleaned.erase(remove(cleaned.begin(), cleaned.end(), c), cleaned.end());
		}
		return cleaned;
	}
}
