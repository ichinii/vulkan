#pragma once

#include <utility>

/* Resource is not meant to own.
 * It rather helps that no Resource that was moved from is invalid.
*/

template <typename T>
struct Resource {
	Resource() noexcept = default;
	Resource(const Resource&) noexcept = delete;
	Resource(Resource&& other) noexcept { std::swap(item, other.item); }
	Resource(const T& other) noexcept : item(other) {}

	Resource& operator= (const Resource&) = delete;
	Resource& operator= (Resource&& other) { std::swap(item, other.item); return *this; }

	operator const T& () const { return item; }
	operator T& () { return item; }

	const T& operator* () const { return item; }
	T& operator * () { return item; }

	const T* operator& () const { return &item; }
	T* operator& () { return &item; }

	T item = {};
};
