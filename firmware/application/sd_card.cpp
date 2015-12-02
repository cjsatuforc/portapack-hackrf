/*
 * Copyright (C) 2015 Jared Boone, ShareBrained Technology, Inc.
 *
 * This file is part of PortaPack.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#include "sd_card.hpp"

#include <hal.h>

#include "ff.h"

namespace sd_card {

namespace {

bool card_present = false;

FATFS fs;

FRESULT mount() {
	return f_mount(&fs, "", 0);
}

FRESULT unmount() {
	return f_mount(NULL, "", 0);
}

} /* namespace */

Signal<Status> status_signal;

void poll_inserted() {
	const auto card_present_now = sdc_lld_is_card_inserted(&SDCD1);
	if( card_present_now != card_present ) {
		card_present = card_present_now;

		Status status { card_present ? Status::Present : Status::NotPresent };

		if( card_present ) {
			if( sdcConnect(&SDCD1) == CH_SUCCESS ) {
				if( mount() == FR_OK ) {
					status = Status::Mounted;
				} else {
					status = Status::MountError;
				}
			} else {
				status = Status::ConnectError;
			}
		} else {
			sdcDisconnect(&SDCD1);
		}

		status_signal.emit(status);
	}
}

} /* namespace sd_card */