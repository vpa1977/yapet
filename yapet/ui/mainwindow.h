// -*- c++ -*-
//
// $Id$
//
// Copyright (C) 2008-2013  Rafael Ostertag
//
// This file is part of YAPET.
//
// YAPET is free software: you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the Free Software
// Foundation, either version 3 of the License, or (at your option) any later
// version.
//
// YAPET is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
// details.
//
// You should have received a copy of the GNU General Public License along with
// YAPET.  If not, see <http://www.gnu.org/licenses/>.
//

#ifndef _MAINWINDOW_H
#define _MAINWINDOW_H 1

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef HAVE_STRINGS_H
#include <strings.h>
#endif

#include <cstring>
#include <string>

// Crypt
#include <file.h>

#include <yacurs.h>

#include "help.h"
#include "info.h"
#include "passworddialog.h"
#include "passwordrecord.h"

#ifdef ENABLE_PWGEN
#include "pwgendialog.h"
#endif

namespace INTERNAL {
    
    class Finder {
	private:
	    std::string needle;
#ifdef USE_WCHAR
# warning "USE_WCHAR"
	    std::string mbstolower(const std::string& mbs) {
		size_t reqsize = mbstowcs(0, mbs.c_str(), 0);
		if (reqsize == (size_t)-1)
		    throw std::runtime_error(_("Finder(): Unable to convert to wide character string"));

		wchar_t* wstr = new wchar_t[reqsize+1];
		if (wstr==0)
		    throw std::runtime_error(_("Finder(): out of memory"));
		if (mbstowcs(wstr, mbs.c_str(), reqsize+1)!=reqsize) {
		    delete[] wstr;
		    throw std::runtime_error(_("Finder(): Unable to convert to wide character string"));
		}

		for(size_t i=0; i<reqsize; i++)
		    wstr[i]=towlower(wstr[i]);

		reqsize = wcstombs(0, wstr, 0);
		if (reqsize==(size_t)-1) {
		    delete[] wstr;
		    throw std::runtime_error(_("Finder(): Unable to convert to multibyte string"));
		}

		char* str = new char[reqsize+1];
		if (str==0) {
		    delete[] wstr;
		    throw std::runtime_error(_("Finder(): out of memory"));
		}

		if (wcstombs(str, wstr, reqsize+1)!=reqsize) {
		    delete[] wstr;
		    delete[] str;
		    throw std::runtime_error(_("Finder(): Unable to convert to multibyte string"));
		}

		std::string retval(str);
		delete[] str;
		delete[] wstr;
		return retval;
	    }
#endif

	public:
#ifdef HAVE_STRCASESTR
#warning "HAVE_STRCASESTR"
	    Finder(std::string n): needle(n) {}
#else // HAVE_STRCASESTR
# if defined(USE_WCHAR) && defined(HAVE_TOWLOWER)
# warning "defined(USE_WCHAR) && defined(HAVE_TOWLOWER)"
	    Finder(std::string n): needle(mbstolower(n)) {}
# else // defined(USE_WCHAR) && defined(HAVE_TOWLOWER)
#  ifdef HAVE_TOLOWER
#  warning "HAVE_TOLOWER"
	    Finder(std::string n): needle() {
		std::string tmp(n);
		for(std::string::size_type i=0;
		    i<tmp.size();
		    i++)
		    needle.push_back(tolower(tmp[i]));
	    }
#  else // defined(HAVE_TOLOWER)
#  warning "!HAVE_TOLOWER"
	    Finder(std::string n): needle(n) {}
#  endif // defined(HAVE_TOLOWER)
# endif // defined(USE_WCHAR) && defined(HAVE_TOWLOWER)
#endif // HAVE_STRCASESTR

	    operator const std::string&() {
		return needle;
	    }

	    bool operator() (const YAPET::PartDec& haystack) {
#ifdef HAVE_STRCASESTR
		return strcasestr(
				  reinterpret_cast<const char*>(haystack.getName()),
				  needle.c_str()) != 0;
#else // HAVE_STRCASESTR
# if defined(USE_WCHAR) && defined(HAVE_TOWLOWER)
		std::string _hay(mbstolower(reinterpret_cast<const char*>(haystack.getName())));
		return strstr(_hay.c_str(), needle.c_str()) != 0;
# else // defined(USE_WCHAR) && defined(HAVE_TOWLOWER)
#  if defined(HAVE_TOLOWER)
		std::string tmp(reinterpret_cast<const char*>(haystack.getName()));
		std::string lower;
		for(std::string::size_type i=0;
		    i<tmp.size();
		    i++)
		    lower.push_back(tolower(tmp[i]));
	    return strstr(lower.c_str(), needle.c_str()) != 0;
#  else // defined(HAVE_TOLOWER)
		return strstr(
				  reinterpret_cast<const char*>(haystack.getName()),
				  needle.c_str()) != 0;
#  endif // defined(HAVE_TOLOWER)
# endif // defined(USE_WCHAR) && defined(HAVE_TOWLOWER)
#endif // HAVE_STRCASESTR
	    }
    };

}

class MainWindow : public YACURS::Window {
    private:
	YACURS::ListBox<YAPET::PartDec>* recordlist;
	HelpDialog* helpdialog;
	InfoDialog* infodialog;
	YACURS::MessageBox* confirmdelete;
	YACURS::MessageBox2* confirmquit;
	PasswordRecord* passwordrecord;
	YACURS::MessageBox2* errormsgdialog;
	YACURS::InputBox* searchdialog;
#ifdef ENABLE_PWGEN
	PwGenDialog* pwgendialog;
#endif
	INTERNAL::Finder* finder;
	// Used when opening an existing record or deleting a record
	// to store the index, so that it can be reselect in case of a
	// window resize.
	YACURS::ListBox<YAPET::PartDec>::lsz_t record_index;
	// Used for search. Keeps the position of the last succesfull
	// match.
	YACURS::ListBox<YAPET::PartDec>::lsz_t last_search_index;


        MainWindow(const MainWindow&) {
        }

        const MainWindow& operator=(const MainWindow&) {
            return *this;
        }

	void apoptosis_handler(YACURS::Event& e);

	void window_close_handler(YACURS::Event& e);

	void listbox_enter_handler(YACURS::Event& e);

    public:
        MainWindow();
        virtual ~MainWindow();

	/**
	 * Load password file.
	 *
	 * Unconditionally load a password file, i.e. changes not
	 * committed to disk on an already shown file will be lost.
	 *
	 * @param file YAPET file
	 *
	 * @param key corresponding key to @c file
	 */
	void load_password_file(YAPET::File* file, YAPET::Key* key);

	/**
	 * @param selected whether or not to preload the dialog with
	 * the currently selected password record.
	 */
	void show_password_record(bool selected);

	void delete_selected();

	/**
	 * Save records.
	 *
	 * @return @c false if there were errors, @c true otherwise.
	 */
	bool save_records();

	void change_password(YAPET::Key* nk);

	void show_help();

	void show_info();

#ifdef ENABLE_PWGEN	
	void show_pwgen();
#endif

	void quit();

	/**
	 * Get the number of password records.
	 */
	YACURS::ListBox<>::lsz_t size() {
	    return recordlist->list().size();
	}

	void sort_asc(bool f);
	bool sort_asc() const;

	void search_first();

	void search_next();
};

#endif // _MAINWINDOW_H
