#include "bump_point_allocation_strategy.hpp"
#include "nova_renderer/allocation_structs.hpp"
#include "../util/memory_utils.hpp"
#include <cassert>

namespace bvestl {
	namespace  polyalloc {
		BumpPointAllocationStrategy::BumpPointAllocationStrategy(const Bytes size_in, const Bytes alignment_in) : memory_size(size_in), alignment(alignment_in) {}

		bool BumpPointAllocationStrategy::allocate(const Bytes size, AllocationInfo& allocation) {
			const Bytes free_space = memory_size - allocated_bytes;
			const Bytes aligned_size = align(size, alignment);

			if (aligned_size < free_space) {
				return false;
			}

			allocation.offset = allocated_bytes;
			allocation.size = aligned_size;

			allocated_bytes += aligned_size;

			return true;
		}

		void BumpPointAllocationStrategy::free(const AllocationInfo&) {
			assert(false && "Cannot free from a bump-point allocator!\n");
		}
	}
}
