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

#ifndef ACSMGET_TEXTS_H
#define ACSMGET_TEXTS_H

#include <string>

namespace acsmget
{
	const std::string texts_action_about                      = "About";
	const std::string texts_action_authorize                  = "Authorize";
	const std::string texts_action_authorize_computer         = "Authorize computer";
	const std::string texts_action_cancel                     = "Cancel";
	const std::string texts_action_create_or_manage_account   = "Create or manage Adobe account";
	const std::string texts_action_erase_authorization        = "Erase authorization";
	const std::string texts_action_open                       = "Open";
	const std::string texts_action_return_loan                = "Return loan";
	const std::string texts_action_show_in_folder             = "Show in folder";
	const std::string texts_desc_app                          = "acsm-get is an open source tool to download e-books with Adobe DRM";
	const std::string texts_desc_authorization                = "An acsm file is a personal download link to an e-book protected with Adobe DRM (digital rights management). "
																"To download it, your computer needs to be authorized by Adobe. If you authorize with an Adobe account, you "
																"may download the e-book on up to six different devices. If you authorize without account, it can not be downloaded "
																"on any other devices anymore.\n\nacsm-get will remove the DRM from the downloaded e-book so that you can read "
																"it on any device, but if you ever lose it, it is useful to be able to download it again.";
	const std::string texts_lbl_adobe_account                 = "Adobe account";
	const std::string texts_lbl_no_account                    = "No account";
	const std::string texts_lbl_username                      = "Username";
	const std::string texts_lbl_password                      = "Password";
	const std::string texts_msg_ask_confirm_erase_auth        = "If you erase authorization you can no longer download acsm files, unless you authorize again.\n\nContinue?";
	const std::string texts_msg_err_authorization             = "Unable to authorize computer.";
	const std::string texts_msg_err_erase_authorization       = "Unable to erase authorization.";
	const std::string texts_msg_err_home_variable_not_set     = "The HOME environment variable is not set.";
	const std::string texts_msg_err_not_a_loan                = "This e-book does not appear to be a loan.";
	const std::string texts_msg_inf_auth_computer_to_download = "Authorize your computer to download acsm files.";
	const std::string texts_msg_inf_authorization_erased      = "Authorization erased.";
	const std::string texts_msg_inf_book_is_loan              = "The downloaded e-book is a loan. When you have finished reading, you can return it via the menu so other people may "
																"borrow it.";
	const std::string texts_msg_inf_download_completed        = "Download completed.";
	const std::string texts_msg_inf_download_failed           = "Download failed.";
	const std::string texts_msg_inf_downloading               = "Downloading…";
	const std::string texts_msg_inf_erasing_authorization     = "Erasing authorization…";
	const std::string texts_msg_inf_loan_returned             = "Loan returned.";
	const std::string texts_msg_inf_open_file_to_download     = "Open an acsm file to download.";
	const std::string texts_msg_inf_return_loan_failed        = "Unable to return loan.";
	const std::string texts_msg_inf_returning_loan            = "Returning loan…";
	const std::string texts_msg_inf_successfully_authorized   = "Your computer has been authorized.";
	const std::string texts_other_acsm_file_filter            = "Adobe Content Server Message files (*.acsm)";
	const std::string texts_other_untitled_book_name          = "Untitled";
	const std::string texts_title_app                         = "acsm-get";
	const std::string texts_title_error                       = "Error";
	const std::string texts_title_download_completed          = "Download completed";
	const std::string texts_title_file_chooser                = "Open";
}

#endif //ACSMGET_TEXTS_H
