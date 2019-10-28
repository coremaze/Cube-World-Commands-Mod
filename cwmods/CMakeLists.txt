cmake_minimum_required (VERSION 3.8)
if(NOT CMAKE_CL_64)
   message(FATAL_ERROR "CWSDK REQUIRES clang-cl x64 to build. " )
endif()
if(CMAKE_BUILD_TYPE MATCHES "Debug")
    message(FATAL_ERROR "CWSDK cannot be built debug mode due to MSVC binary compatibility issues." )
endif()
project("CWSDK" VERSION "1.0.0")
add_library (CWSDK 
    cwmods.cpp
    common/ByteRGBA.cpp
    common/FloatRGBA.cpp
    common/FloatVector3.cpp
    common/IntVector2.cpp
    common/LongVector3.cpp
    cube/ChatWidget.cpp
    cube/Client.cpp
    cube/Creature.cpp
    cube/Database.cpp
    cube/Field.cpp
    cube/Game.cpp
    cube/GUI.cpp
    cube/Host.cpp
    cube/Item.cpp
    cube/ItemStack.cpp
    cube/Speech.cpp
    cube/Spirit.cpp
    cube/SpriteManager.cpp
    cube/World.cpp
    cube/WorldMap.cpp
    cube/Zone.cpp
    gfx/Chunk.cpp
    gfx/D3D11Graphics.cpp
    gfx/D3D11Renderer.cpp
    plasma/Array.cpp
    plasma/Attribute.cpp
    plasma/ContinuousAttribute.cpp
    plasma/D3D11Engine.cpp
    plasma/DiscreteAttribute.cpp
    plasma/Display.cpp
    plasma/Engine.cpp
    plasma/Keyable.cpp
    plasma/NamedObject.cpp
    plasma/Node.cpp
    plasma/Object.cpp
    plasma/ObjectManager.cpp
    plasma/Vector.cpp
    plasma/Widget.cpp)
target_include_directories (CWSDK PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})