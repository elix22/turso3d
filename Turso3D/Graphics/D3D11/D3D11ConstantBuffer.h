// For conditions of distribution and use, see copyright notice in License.txt

#pragma once

#include "../../Base/AutoPtr.h"
#include "../GPUObject.h"
#include "../GraphicsDefs.h"

namespace Turso3D
{

/// Description of a shader constant.
struct TURSO3D_API Constant
{
    /// Construct empty.
    Constant() :
        numElements(1)
    {
    }

    /// Construct with type, name and optional number of elements.
    Constant(ConstantType type_, const String& name_, size_t numElements_ = 1) :
        type(type_),
        name(name_),
        numElements(numElements_)
    {
    }

    /// Construct with type, name and optional number of elements.
    Constant(ConstantType type_, const char* name_, size_t numElements_ = 1) :
        type(type_),
        name(name_),
        numElements(numElements_)
    {
    }

    /// Type of constant.
    ConstantType type;
    /// Name of constant.
    String name;
    /// Number of elements. Default 1.
    size_t numElements;
    /// Element size. Filled by ConstantBuffer.
    size_t elementSize;
    /// Start offset in the buffer. Filled by ConstantBuffer.
    size_t offset;
};

/// GPU buffer for shader constant data.
class TURSO3D_API ConstantBuffer : public WeakRefCounted, public GPUObject
{
public:
    /// Construct.
    ConstantBuffer();
    /// Destruct.
    virtual ~ConstantBuffer();

    /// Release the buffer.
    virtual void Release();

    /// Define the constants being used and create the GPU-side buffer. Return true on success.
    bool Define(const Vector<Constant>& srcConstants);
    /// Define the constants being used and create the GPU-side buffer. Return true on success.
    bool Define(size_t numConstants, const Constant* srcConstants);
    /// Set a constant by index. Optionally specify how many elements to update, default all. Return true on success.
    bool SetConstant(size_t index, void* data, size_t numElements = 0);
    /// Set a constant by name. Optionally specify how many elements to update, default all. Return true on success.
    bool SetConstant(const String& name, void* data, size_t numElements = 0);
    /// Set a constant by name. Optionally specify how many elements to update, default all. Return true on success.
    bool SetConstant(const char* name, void* data, size_t numElements = 0);
    /// Apply to the GPU-side buffer if has changes. Return true on success.
    bool Apply();
    /// Set a constant by index, template version
    template <class T> bool SetConstant(size_t index, const T& data, size_t numElements = 0) { return SetConstant(index, (void*)&data, numElements); }
    /// Set a constant by name, template version
    template <class T> bool SetConstant(const String& name, const T& data, size_t numElements = 0) { return SetConstant(name, (void*)&data, numElements); }
    /// Set a constant by name, template version
    template <class T> bool SetConstant(const char* name, const T& data, size_t numElements = 0) { return SetConstant(name, (void*)&data, numElements); }

    /// Return the D3D11 buffer.
    void* Buffer() const { return buffer; }
    /// Return number of constants.
    size_t NumConstants() const { return constants.Size(); }
    /// Return the constant descriptions.
    const Vector<Constant>& Constants() const { return constants; }
    /// Return total byte size of the buffer.
    size_t ByteSize() const { return byteSize; }
    /// Return whether buffer has unapplied changes.
    bool IsDirty() const { return dirty; }

    /// Element sizes by type.
    static const size_t elementSize[];
    
private:
    /// D3D11 buffer.
    void* buffer;
    /// Constant definitions.
    Vector<Constant> constants;
    /// CPU-side data where updates are collected before applying.
    AutoArrayPtr<unsigned char> shadowData;
    /// Total byte size.
    size_t byteSize;
    /// Dirty flag.
    bool dirty;
};

}