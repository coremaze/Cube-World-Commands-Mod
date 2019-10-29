#ifndef ZONE_H
#define ZONE_H

#include "../IDA/types.h"
#include "World.h"
#include "Field.h"
#include "../gfx/Chunk.h"
#include "../common/IntVector2.h"

namespace cube {
class Zone {
    public:
		virtual ~Zone();

		cube::World* world;
		IntVector2 position;
		_BYTE gap18[72];
		char some_struct;
		_BYTE gap61[351];
		gfx::Chunk chunk;
		_BYTE gap448[208];
		cube::Field fields[4096];
    };
}

static_assert(sizeof(cube::Zone) == 0x58518, "cube::Zone is not the correct size.");

#endif // ZONE_H
