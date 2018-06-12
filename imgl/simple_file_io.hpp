#pragma once

#include <string>

#include "stdio.h"

#include "basic_typedefs.hpp"
#include "defer.hpp"

namespace imgl {
	inline bool load_text_file (::std::string const& filepath, ::std::string* text) {

		FILE* f = fopen(filepath.c_str(), "rb"); // we don't want "\r\n" to "\n" conversion, because it interferes with our file size calculation
		if (!f)
			return false;
		defer {
			fclose(f);
		};

		fseek(f, 0, SEEK_END);
		int filesize = ftell(f);
		fseek(f, 0, SEEK_SET);

		text->resize(filesize);

		uptr ret = fread(&(*text)[0], 1,text->size(), f);
		if (ret != (uptr)filesize) return false;

		return true;
	}

	struct Blob {
		void*	data = nullptr;
		uptr	size = 0;

		//
		static inline Blob alloc (uptr size) {
			Blob b;
			b.data = malloc(size);
			b.size = size;
			return b;
		}

		// default unallocated
		inline Blob () {}
		// always auto free
		inline ~Blob () {
			free(data); // ok to free nullptr
		}

		// no implicit copy
		inline Blob& operator= (Blob& r) = delete;
		inline Blob (Blob& r) = delete;
		// move operators
		friend void swap (Blob& l, Blob& r);
		inline Blob& operator= (Blob&& r) {	swap(*this, r);	return *this; }
		inline Blob (Blob&& r) {				swap(*this, r); }

	};
	inline void swap (Blob& l, Blob& r) {
		std::swap(l.data, r.data);
		std::swap(l.size, r.size);
	}


	inline bool load_binary_file (::std::string const& filepath, Blob* blob) {

		FILE* f = fopen(filepath.c_str(), "rb"); // we don't want "\r\n" to "\n" conversion
		if (!f)
			return false;
		defer {
			fclose(f);
		};

		fseek(f, 0, SEEK_END);
		int filesize = ftell(f);
		fseek(f, 0, SEEK_SET);

		auto tmp = Blob::alloc(filesize);

		uptr ret = fread(tmp.data, 1,tmp.size, f);
		if (ret != (uptr)filesize) return false;

		*blob = std::move(tmp);
		return true;
	}

	inline bool load_fixed_size_binary_file (::std::string const& filepath, void* data, uptr sz) {

		FILE* f = fopen(filepath.c_str(), "rb");
		if (!f)
			return false;
		defer {
			fclose(f);
		};

		fseek(f, 0, SEEK_END);
		int filesize = ftell(f);
		fseek(f, 0, SEEK_SET);

		if (filesize != sz)
			return false;

		uptr ret = fread(data, 1,sz, f);
		if (ret != sz)
			return false;

		return true;
	}

	inline bool write_fixed_size_binary_file (::std::string const& filepath, void const* data, uptr sz) {

		FILE* f = fopen(filepath.c_str(), "wb");
		if (!f)
			return false;
		defer {
			fclose(f);
		};

		uptr ret = fwrite(data, 1,sz, f);
		if (ret != sz)
			return false;

		return true;
	}
}
