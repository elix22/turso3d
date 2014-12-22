// For conditions of distribution and use, see copyright notice in License.txt

#pragma once

#include "../Base/AutoPtr.h"
#include "../Graphics/GraphicsDefs.h"
#include "../Resource/Resource.h"

namespace Turso3D
{

class ConstantBuffer;
class JSONFile;
class JSONValue;
class Material;
class Shader;
class ShaderVariation;
class Texture;

/// Render pass, which defines render state and shaders. A material may define several of these.
class Pass
{
public:
    /// Construct.
    Pass(Material* parent, const String& name);
    /// Destruct.
    ~Pass();

    /// Load from JSON data. Return true on success.
    bool LoadJSON(const JSONValue& source);
    /// Save to JSON data. Return true on success.
    bool SaveJSON(JSONValue& dest);
    /// Set a predefined blend mode.
    void SetBlendMode(BlendMode mode);
    /// Set shader names and defines.
    void SetShaders(const String& vsName, const String& psName, const String& vsDefines = String::EMPTY, const String& psDefines = String::EMPTY);
    /// Reset render state to defaults.
    void Reset();
    /// Reset cached shader variations. Called by Material. Needs to be called manually if changing shader names / defines manually.
    void ClearCachedShaders();

    /// Return parent material resource.
    Material* Parent() const;
    /// Return pass name.
    const String& Name() const { return name; }

    /// Depth compare function.
    CompareFunc depthFunc;
    /// Depth write enable.
    bool depthWrite;
    /// Depth clipping enable.
    bool depthClip;
    /// Alpha to coverage enable.
    bool alphaToCoverage;
    /// Blend mode parameters.
    BlendModeDesc blendMode;
    /// Polygon culling mode.
    CullMode cullMode;
    /// Polygon fill mode.
    FillMode fillMode;
    /// Shader names.
    String shaderNames[MAX_SHADER_STAGES];
    /// Shader defines.
    String shaderDefines[MAX_SHADER_STAGES];
    /// Combined shader defines from both the pass and material. Filled by Renderer.
    String combinedShaderDefines[MAX_SHADER_STAGES];
    /// Shader resources. Filled by Renderer.
    SharedPtr<Shader> shaders[MAX_SHADER_STAGES];
    /// Cached shader variations by geometry type. Filled by Renderer.
    Vector<WeakPtr<ShaderVariation> > shaderVariations[MAX_SHADER_STAGES];
    /// Shader load attempted flag.
    bool shadersLoaded;

private:
    /// Parent material resource.
    WeakPtr<Material> parent;
    /// Pass name.
    String name;
};

/// Description of how to render 3D geometry. Split in several passes.
class Material : public Resource
{
    OBJECT(Material);

public:
    /// Construct.
    Material();
    /// Destruct.
    ~Material();

    /// Register object factory.
    static void RegisterObject();

    /// Load material from a stream. Return true on success.
    bool BeginLoad(Stream& source) override;
    /// Finalize material loading in the main thread. Return true on success.
    bool EndLoad() override;
    /// Save the material to a stream. Return true on success.
    bool Save(Stream& dest) override;

    /// Create and return a new pass. If pass with same name exists, it will be returned.
    Pass* CreatePass(const String& name);
    /// Remove a pass.
    void RemovePass(const String& name);
    /// Set a texture.
    void SetTexture(size_t index, Texture* texture);
    /// Remove a texture.
    void RemoveTexture(size_t index);
    /// Remove all textures.
    void ResetTextures();
    /// Set a constant buffer.
    void SetConstantBuffer(ShaderStage stage, ConstantBuffer* buffer);
    /// Set global shader defines. Clears existing shader cached variations from all passes.
    void SetShaderDefines(ShaderStage stage, const String& defines);

    /// Return pass by name or null if not found. Should not be called in performance-sensitive rendering loops.
    Pass* GetPass(const String& name) const;
    /// Return pass by index or null if not found.
    Pass* GetPass(size_t index) const;
    /// Return texture by texture unit.
    Texture* GetTexture(size_t index) const;
    /// Return constant buffer by stage.
    ConstantBuffer* GetConstantBuffer(ShaderStage stage) const;
    /// Return shader defines by stage.
    const String& ShaderDefines(ShaderStage stage) const;

    /// Return pass index from name.
    static unsigned PassIndex(const String& name, bool createNew = true);
    /// Return pass name by index.
    static const String& PassName(size_t index);

    /// Material textures.
    HashMap<size_t, SharedPtr<Texture> > textures;
    /// Constant buffers.
    SharedPtr<ConstantBuffer> constantBuffers[MAX_SHADER_STAGES];
    /// Global shader defines.
    String shaderDefines[MAX_SHADER_STAGES];

private:
    /// Passes by index.
    Vector<AutoPtr<Pass> > passes;
    /// JSON data used for loading.
    AutoPtr<JSONFile> loadJSON;

    /// Pass name to index mapping.
    static HashMap<String, unsigned> passIndices;
    /// Pass names by index.
    static Vector<String> passNames;
    /// Next free pass index.
    static unsigned nextPassIndex;
};

}
