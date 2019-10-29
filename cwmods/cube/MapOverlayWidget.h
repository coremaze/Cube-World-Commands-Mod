#ifndef CUBE_MAPOVERLAYWIDGET_H
#define CUBE_MAPOVERLAYWIDGET_H
#include "../plasma/Widget.h"

namespace cube {
class MapOverlayWidget : public plasma::Widget {
    public:
		__int64 field_1A8;
		char field_1B0;
		char field_1B1;
		char field_1B2;
		char field_1B3;
		char field_1B4;
		char field_1B5;
		char field_1B6;
		char field_1B7;
		char field_1B8;
		char field_1B9;
		char field_1BA;
		char field_1BB;
		char field_1BC;
		char field_1BD;
		char field_1BE;
		char field_1BF;
		char field_1C0;
		char field_1C1;
		char field_1C2;
		char field_1C3;
		char field_1C4;
		char field_1C5;
		char field_1C6;
		char field_1C7;
		int field_1C8;
		char field_1CC;
		char struct_size_0x80[128];
		char field_24D;
		char field_24E;
		char field_24F;
		int field_250;
		int field_254;
		int field_258;
		int field_25C;
    };
}

static_assert(sizeof(cube::MapOverlayWidget) == 0x260, "cube::MapOverlayWidget is not the correct size.");

#endif // CUBE_MAPOVERLAYWIDGET_H
