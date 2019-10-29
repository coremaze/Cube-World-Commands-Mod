#ifndef CWMODS_H
#define CWMODS_H
#include <cstdint>

#define EXPORT extern "C" __declspec(dllexport) __declspec(noinline)

#include "IDA/types.h"

#include "common/ByteRGBA.h"
#include "common/DoubleVector3.h"
#include "common/FloatRGBA.h"
#include "common/FloatVector2.h"
#include "common/FloatVector3.h"
#include "common/IntVector2.h"
#include "common/LongVector3.h"
#include "common/Matrix4.h"

#include "cube/Block.h"
#include "cube/ChatWidget.h"
#include "cube/Client.h"
#include "cube/Controls.h"
#include "cube/Creature.h"
#include "cube/Database.h"
#include "cube/Field.h"
#include "cube/Game.h"
#include "cube/GUI.h"
#include "cube/Host.h"
#include "cube/Item.h"
#include "cube/ItemStack.h"
#include "cube/MapOverlayWidget.h"
#include "cube/MultiplayerWidget.h"
#include "cube/Music.h"
#include "cube/Options.h"
#include "cube/ServerUpdates.h"
#include "cube/Speech.h"
#include "cube/Sprite.h"
#include "cube/SpriteManager.h"
#include "cube/World.h"
#include "cube/WorldMap.h"
#include "cube/XAudio2Engine.h"
#include "cube/Zone.h"
#include "cube/constants.h"

#include "gfx/Chunk.h"
#include "gfx/D3D11Graphics.h"
#include "gfx/D3D11Renderer.h"
#include "gfx/D3D11IndexBuffer.h"
#include "gfx/D3D11VertexBuffer.h"
#include "gfx/IndexBuffer.h"
#include "gfx/VertexBuffer.h"

#include "msvc/_Thrd_t.h"

#include "plasma/Attribute.h"
#include "plasma/ContinuousAttribute.h"
#include "plasma/D3D11Engine.h"
#include "plasma/DiscreteAttribute.h"
#include "plasma/Display.h"
#include "plasma/Engine.h"
#include "plasma/Keyable.h"
#include "plasma/NamedObject.h"
#include "plasma/Matrix.h"
#include "plasma/Node.h"
#include "plasma/Object.h"
#include "plasma/ObjectManager.h"
#include "plasma/Transformation.h"
#include "plasma/Vector.h"
#include "plasma/Widget.h"


void* CWBase();
void* CWOffset(size_t offset);

EXPORT void ModPreInitialize();
EXPORT int ModMajorVersion();
EXPORT int ModMinorVersion();

void WriteByte(void* location, char val);
void WriteFarJMP(void* source, void* destination);

__declspec(noinline) void* operator new(size_t size);
__declspec(noinline) void* operator new[](size_t size);
__declspec(noinline) void operator delete(void* ptr) noexcept;
__declspec(noinline) void operator delete[](void* ptr) noexcept;

#endif // CWMODS_H
