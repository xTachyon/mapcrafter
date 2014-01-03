/*
 * Copyright 2012-2014 Moritz Hilscher
 *
 * This file is part of mapcrafter.
 *
 * mapcrafter is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * mapcrafter is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with mapcrafter.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef WORLDCACHE_H_
#define WORLDCACHE_H_

#include "pos.h"
#include "chunk.h"
#include "region.h"
#include "world.h"

namespace mapcrafter {
namespace mc {

/**
 * A block with id/data/biome/lighting data.
 */
struct Block {
	Block(uint16_t id = 0, uint16_t data = 0, uint8_t biome = 0,
			uint8_t block_light = 0, uint8_t sky_light = 15);

	uint16_t id, data;
	uint8_t biome;
	uint8_t block_light, sky_light;

	bool isFullWater() const;
};

const int GET_ID = 1;
const int GET_DATA = 2;
const int GET_BIOME = 4;
const int GET_BLOCK_LIGHT = 8;
const int GET_SKY_LIGHT = 16;
const int GET_LIGHT = GET_BLOCK_LIGHT | GET_SKY_LIGHT;

/**
 * Some cache statistics for debugging. Not used at the moment.
 *
 * Maybe add a set of corrupt chunks/regions to dump them at the end of the rendering.
 */
struct CacheStats {
	CacheStats()
			: hits(0), misses(0), region_not_found(0), not_found(0), invalid(0) {
	}

	void print(const std::string& name) const {
		std::cout << name << ":" << std::endl;
		std::cout << "  hits: " << hits << std::endl
				  << "  misses: " << misses << std::endl
				  << "  region_not_found: " << region_not_found << std::endl
				  << "  not_found: " << not_found << std::endl
				  << "  invalid: " << invalid << std::endl;
	}

	int hits;
	int misses;

	int region_not_found;
	int not_found;
	int invalid;
};

/**
 * An entry in the cache with a Key and a Value type. Used with regions and chunks.
 */
template<typename Key, typename Value>
struct CacheEntry {
	Key key;
	Value value;
	bool used;
};

#define RBITS 2
#define RWIDTH (1 << RBITS)
#define RSIZE (RWIDTH*RWIDTH)
#define RMASK (RSIZE-1)

#define CBITS 5
#define CWIDTH (1 << CBITS)
#define CSIZE (CWIDTH*CWIDTH)
#define CMASK (CSIZE-1)

/**
 * This is a world cache with regions and chunks.
 *
 * Every region and chunk has a fixed position in the cache. The position in the cache is
 * calculated by using the first few bits of the region/chunk coordinates. Then the
 * regions/chunks are stored with the "smaller" coordinates in a 2D-like array.
 *
 * For the regions the first 2 bits are used, this are 4x4 regions (4 = 1 << 2) in the
 * cache. The regions store only the raw region file data and are used to read the chunks
 * when necessary.
 *
 * For the chunks the first 5 bits are used, this are 16x16 chunk (also the size of a
 * region) in the cache.
 *
 * When someone is trying to access the cache, the cache calculates the position of a
 * region/chunk coordinate in the cache. Then the cache checks if there is already
 * something stored on this position and if the real coordinate of this cache entry is
 * the coordinate of the requested region/chunk. If yes, the cache returns the objects.
 * If not, the cache tries to load the chunk/region and puts it in this cache entry
 * (overwrites an already loaded region/chunk at this cache position).
 */
class WorldCache {
private:
	World world;

	CacheEntry<RegionPos, RegionFile> regioncache[RSIZE];
	CacheEntry<ChunkPos, Chunk> chunkcache[CSIZE];

	CacheStats regionstats;
	CacheStats chunkstats;

	int getRegionCacheIndex(const RegionPos& pos) const;
	int getChunkCacheIndex(const ChunkPos& pos) const;

public:
	WorldCache(const World& world = World());

	RegionFile* getRegion(const RegionPos& pos);
	Chunk* getChunk(const ChunkPos& pos);

	Block getBlock(const mc::BlockPos& pos, const mc::Chunk* chunk, int get = GET_ID | GET_DATA);

	const CacheStats& getRegionCacheStats() const;
	const CacheStats& getChunkCacheStats() const;
};

}
}

#endif /* WORLDCACHE_H_ */
