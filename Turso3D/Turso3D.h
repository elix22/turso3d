// For conditions of distribution and use, see copyright notice in License.txt

#pragma once

// Convenience header file for including commonly needed engine classes. Note: intentionally does not include Debug/DebugNew.h
// so that placement new works as expected.

#include "Base/HashSet.h"
#include "Base/List.h"
#include "Base/Ptr.h"
#include "Debug/Log.h"
#include "Debug/Profiler.h"
#include "Graphics/ConstantBuffer.h"
#include "Graphics/Graphics.h"
#include "Graphics/IndexBuffer.h"
#include "Graphics/Shader.h"
#include "Graphics/ShaderVariation.h"
#include "Graphics/Texture.h"
#include "Graphics/VertexBuffer.h"
#include "IO/Arguments.h"
#include "IO/Console.h"
#include "IO/File.h"
#include "IO/FileSystem.h"
#include "IO/MemoryBuffer.h"
#include "IO/VectorBuffer.h"
#include "Math/Frustum.h"
#include "Math/Polyhedron.h"
#include "Math/Random.h"
#include "Math/Ray.h"
#include "Object/Serializable.h"
#include "Renderer/Camera.h"
#include "Renderer/GeometryNode.h"
#include "Renderer/Light.h"
#include "Renderer/Material.h"
#include "Renderer/Octree.h"
#include "Renderer/Renderer.h"
#include "Resource/Image.h"
#include "Resource/JSONFile.h"
#include "Resource/ResourceCache.h"
#include "Scene/Scene.h"
#include "Thread/Condition.h"
#include "Thread/Mutex.h"
#include "Thread/Thread.h"
#include "Thread/Timer.h"
#include "Window/Input.h"
#include "Window/Window.h"
