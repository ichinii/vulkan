#pragma once

#include <utility>

/* Resource is not meant to own.
 * It rather helps that no Resource that was moved from is invalid.
*/

template <typename T>
struct Resource {
	Resource() = default;
	Resource(const Resource&) = delete;
	Resource(Resource&& other) { std::swap(item, other.item); }
	Resource(const T& other) : item(other) {}

	Resource& operator= (const Resource&) = delete;
	Resource& operator= (Resource&& other) { std::swap(item, other.item); return *this; }

	operator T&() { return item; }

	T item = {};
};
