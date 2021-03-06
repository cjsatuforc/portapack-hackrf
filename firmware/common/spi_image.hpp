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

#ifndef __SPI_IMAGE_H__
#define __SPI_IMAGE_H__

#include <cstdint>
#include <cstddef>

#include "memory_map.hpp"

namespace portapack {
namespace spi_flash {

struct image_tag_t {
	constexpr image_tag_t(
	) : c { 0, 0, 0, 0 }
	{
	}

	constexpr image_tag_t(
		char c0, char c1, char c2, char c3
	) : c { c0, c1, c2, c3 }
	{
	}

	image_tag_t& operator=(const image_tag_t& other) {
		c[0] = other.c[0];
		c[1] = other.c[1];
		c[2] = other.c[2];
		c[3] = other.c[3];
		return *this;
	}

	bool operator==(const image_tag_t& other) const {
		return (c[0] == other.c[0]) && (c[1] == other.c[1]) && (c[2] == other.c[2]) && (c[3] == other.c[3]);
	}

	operator bool() const {
		return (c[0] != 0) || (c[1] != 0) || (c[2] != 0) || (c[3] != 0);
	}

private:
	char c[4];
};

constexpr image_tag_t image_tag_ais					{ 'P', 'A', 'I', 'S' };
constexpr image_tag_t image_tag_am_audio			{ 'P', 'A', 'M', 'A' };
constexpr image_tag_t image_tag_capture				{ 'P', 'C', 'A', 'P' };
constexpr image_tag_t image_tag_ert					{ 'P', 'E', 'R', 'T' };
constexpr image_tag_t image_tag_nfm_audio			{ 'P', 'N', 'F', 'M' };
constexpr image_tag_t image_tag_tpms				{ 'P', 'T', 'P', 'M' };
constexpr image_tag_t image_tag_wfm_audio			{ 'P', 'W', 'F', 'M' };
constexpr image_tag_t image_tag_wideband_spectrum	{ 'P', 'S', 'P', 'E' };
constexpr image_tag_t image_tag_hackrf				{ 'H', 'R', 'F', '1' };

struct chunk_t {
	const image_tag_t tag;
	const uint32_t length;
	const uint8_t data[];

	const chunk_t* next() const {
		return reinterpret_cast<const chunk_t*>(&data[length]);
	}
};

struct region_t {
	const size_t offset;
	const size_t size;

	constexpr const void* base() const {
		return reinterpret_cast<void*>(portapack::memory::map::spifi_cached.base() + offset);
	}
};

constexpr region_t bootstrap {
	.offset = 0x00000,
	.size = 0x10000,
};

constexpr region_t images {
	.offset = 0x10000,
	.size = 0x30000,
};

constexpr region_t application {
	.offset = 0x40000,
	.size = 0x40000,
};

} /* namespace spi_flash */
} /* namespace portapack */

#endif/*__SPI_IMAGE_H__*/
