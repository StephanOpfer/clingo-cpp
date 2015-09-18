// This file is part of Clasp. See http://www.cs.uni-potsdam.de/clasp/ 
// 
// Clasp is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// 
// Clasp is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with Clasp; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//
#ifndef CLASP_HASH_MAP_H_INCLUDED
#define CLASP_HASH_MAP_H_INCLUDED
#include <utility>
#if (defined(__cplusplus) && __cplusplus >= 201103L) || (defined(_MSC_VER) && _MSC_VER > 1500) || (defined(_LIBCPP_VERSION))
#include <unordered_map>
#define HASH_NS std
#else
#if defined(_MSC_VER)
#include <unordered_map>
#else
#include <tr1/unordered_map>
#endif
#define HASH_NS std::tr1
#endif

namespace Clasp {
	template<class K, class V, class H = HASH_NS::hash<K>, class E = std::equal_to<K>, class A = std::allocator< std::pair<const K, V> > >
	struct HashMap_t { 
		typedef HASH_NS::unordered_map<K, V, H, E,A>      map_type; 
		typedef HASH_NS::unordered_multimap<K, V, H, E,A> multi_map_type; 
	};
}

#undef HASH_NS
#endif
