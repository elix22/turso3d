// For conditions of distribution and use, see copyright notice in License.txt

#pragma once

#include "../../Math/Color.h"
#include "../../Math/IntRect.h"
#include "../../Math/IntVector2.h"
#include "../../Object/Object.h"
#include "../GraphicsDefs.h"

namespace Turso3D
{

class BlendState;
class ConstantBuffer;
class DepthState;
class GLContext;
class GPUObject;
class IndexBuffer;
class RasterizerState;
class ShaderProgram;
class ShaderVariation;
class Texture;
class VertexBuffer;
class Window;
class WindowResizeEvent;

typedef HashMap<Pair<ShaderVariation*, ShaderVariation*>, AutoPtr<ShaderProgram> > ShaderProgramMap;

static const size_t MAX_VERTEX_ATTRIBUTES = 16;

/// 3D graphics rendering context. Manages the rendering window and GPU objects.
class TURSO3D_API Graphics : public Object
{
    OBJECT(Graphics);

public:
    /// Construct and register subsystem. The graphics mode is not set & window is not opened yet.
    Graphics();
    /// Destruct. Clean up the window, rendering context and GPU objects.
    ~Graphics();

    /// Set graphics mode. Create the window and rendering context if not created yet. Return true on success.
    bool SetMode(int width, int height, bool fullscreen, bool resizable);
    /// Set fullscreen mode on/off while retaining previous resolution. The initial graphics mode must have been set first. Return true on success.
    bool SetFullscreen(bool enable);
    /// Set vertical sync on/off.
    void SetVSync(bool enable);
    /// Close the window and destroy the rendering context and GPU objects.
    void Close();
    /// Present the contents of the backbuffer.
    void Present();
    /// Set the color rendertarget and depth stencil buffer.
    void SetRenderTarget(Texture* renderTarget, Texture* stencilBuffer);
    /// Set multiple color rendertargets and the depth stencil buffer.
    void SetRenderTargets(const Vector<Texture*>& renderTargets, Texture* stencilBuffer); 
    /// Set the viewport rectangle. On window resize the viewport will automatically revert to full window.
    void SetViewport(const IntRect& viewport);
    /// Bind a vertex buffer.
    void SetVertexBuffer(size_t index, VertexBuffer* buffer);
    /// Bind an index buffer.
    void SetIndexBuffer(IndexBuffer* buffer);
    /// Bind a constant buffer.
    void SetConstantBuffer(ShaderStage stage, size_t index, ConstantBuffer* buffer);
    /// Bind a texture.
    void SetTexture(size_t index, Texture* texture);
    /// Bind vertex and pixel shaders.
    void SetShaders(ShaderVariation* vs, ShaderVariation* ps);
    /// Bind blend state object.
    void SetBlendState(BlendState* state);
    /// Bind depth state object and set stencil ref value.
    void SetDepthState(DepthState* state, unsigned stencilRef = 0);
    /// Bind rasterizer state object.
    void SetRasterizerState(RasterizerState* state);
    /// Set scissor rectangle. Is only effective if scissor test is enabled in the rasterizer state.
    void SetScissorRect(const IntRect& scissorRect);
    /// Reset rendertarget and depth stencil buffer to the backbuffer.
    void ResetRenderTargets();
    /// Reset all bound vertex buffers.
    void ResetVertexBuffers();
    /// Reset all bound constant buffers.
    void ResetConstantBuffers();
    /// Clear the current rendertarget. This is not affected by the defined viewport, but will always clear the whole target.
    void Clear(unsigned clearFlags, const Color& clearColor = Color::BLACK, float clearDepth = 1.0f, unsigned char clearStencil = 0);
    /// Draw non-indexed geometry.
    void Draw(PrimitiveType type, size_t vertexStart, size_t vertexCount);
    /// Draw indexed geometry.
    void Draw(PrimitiveType type, size_t indexStart, size_t indexCount, size_t vertexStart);
    /// Draw instanced non-indexed geometry.
    void DrawInstanced(PrimitiveType type, size_t vertexStart, size_t vertexCount, size_t instanceStart, size_t instanceCount);
    /// Draw instanced indexed geometry.
    void DrawInstanced(PrimitiveType type, size_t indexStart, size_t indexCount, size_t vertexStart, size_t instanceStart, size_t instanceCount);

    /// Return whether has the rendering window and context.
    bool IsInitialized() const;
    /// Return backbuffer width, or 0 if not initialized.
    int Width() const { return backbufferSize.x; }
    /// Return backbuffer height, or 0 if not initialized.
    int Height() const { return backbufferSize.y; }
    /// Return current rendertarget width.
    int RenderTargetWidth() const { return renderTargetSize.x; }
    /// Return current rendertarget height.
    int RenderTargetHeight() const { return renderTargetSize.y; }
    /// Return whether is using fullscreen mode.
    bool IsFullscreen() const { return fullscreen; }
    /// Return whether the window is resizable.
    bool IsResizable() const;
    /// Return whether is using vertical sync.
    bool VSync() const { return vsync; }
    /// Return the rendering window.
    Window* RenderWindow() const;
    /// Return the current color rendertarget by index, or null if rendering to the backbuffer.
    Texture* RenderTarget(size_t index) const;
    /// Return the current depth-stencil buffer, or null if rendering to the backbuffer.
    Texture* DepthStencil() const { return depthStencil; }
    /// Return the current viewport rectangle.
    const IntRect Viewport() const { return viewport; }
    /// Return currently bound vertex buffer by index.
    VertexBuffer* GetVertexBuffer(size_t index) const;
    /// Return currently bound index buffer.
    IndexBuffer* GetIndexBuffer() const { return indexBuffer; }
    /// Return currently bound constant buffer by shader stage and index.
    ConstantBuffer* GetConstantBuffer(ShaderStage stage, size_t index) const;
    /// Return currently bound texture by texture unit.
    Texture* GetTexture(size_t index) const;
    /// Return currently bound vertex shader.
    ShaderVariation* GetVertexShader() const { return vertexShader; }
    /// Return currently bound pixel shader.
    ShaderVariation* GetPixelShader() const { return pixelShader; }
    /// Return currently bound blend state.
    BlendState* GetBlendState() const { return blendState; }
    /// Return currently bound depth state.
    DepthState* GetDepthState() const { return depthState; }
    /// Return currently bound rasterizer state.
    RasterizerState* GetRasterizerState() const { return rasterizerState; }
    /// Return current scissor rectangle.
    IntRect ScissorRect() const { return scissorRect; }
    /// Return current stencil ref value.
    unsigned StencilRef() const { return stencilRef; }
    /// Return number of supported constant buffer bindings for vertex shaders.
    size_t NumVSConstantBuffers() const { return vsConstantBuffers; }
    /// Return number of supported constant buffer bindings for pixel shaders.
    size_t NumPSConstantBuffers() const { return psConstantBuffers; }

    /// Register a GPU object to keep track of.
    void AddGPUObject(GPUObject* object);
    /// Remove a GPU object.
    void RemoveGPUObject(GPUObject* object);
    /// Cleanup shader programs when a vertex or pixel shader is destroyed.
    void CleanupShaderPrograms(ShaderVariation* shader);
    /// Bind a VBO for editing or applying as a vertex source. Avoids redundant assignment.
    void BindVBO(unsigned vbo);
    /// Return the currently bound VBO.
    unsigned BoundVBO() const { return boundVBO; }

private:
    /// Resize the backbuffer when window size changes.
    void HandleResize(WindowResizeEvent& event);
    /// Prepare to execute a draw call.
    void PrepareDraw(bool instanced = false, size_t instanceStart = 0);
    /// Reset internally tracked state.
    void ResetState();

    /// OpenGL context.
    AutoPtr<GLContext> context;
    /// OS-level rendering window.
    AutoPtr<Window> window;
    /// Current size of the backbuffer.
    IntVector2 backbufferSize;
    /// Current size of the active rendertarget.
    IntVector2 renderTargetSize;
    /// Current viewport rectangle.
    IntRect viewport;
    /// GPU objects.
    Vector<GPUObject*> gpuObjects;
    /// Shader programs.
    ShaderProgramMap shaderPrograms;
    /// Bound vertex buffers.
    VertexBuffer* vertexBuffers[MAX_VERTEX_STREAMS];
    /// Enabled vertex attributes.
    bool vertexAttributes[MAX_VERTEX_ATTRIBUTES];
    /// Vertex attribute instancing divisors.
    unsigned vertexAttributeDivisors[MAX_VERTEX_ATTRIBUTES];
    /// Current mapping of vertex attributes by semantic
    Vector<Vector<unsigned> > attributeBySemantic;
    /// Bound index buffer.
    IndexBuffer* indexBuffer;
    /// Bound constant buffers by shader stage.
    ConstantBuffer* constantBuffers[MAX_SHADER_STAGES][MAX_CONSTANT_BUFFERS];
    /// Bound textures by texture unit.
    Texture* textures[MAX_TEXTURE_UNITS];
    /// OpenGL active texture targets by texture unit.
    unsigned textureTargets[MAX_TEXTURE_UNITS];
    /// Bound rendertarget textures.
    Texture* renderTargets[MAX_RENDERTARGETS];
    /// Bound depth-stencil texture.
    Texture* depthStencil;
    /// Helper vector for defining just one color rendertarget.
    Vector<Texture*> renderTargetVector;
    /// Bound vertex shader.
    ShaderVariation* vertexShader;
    /// Bound pixel shader.
    ShaderVariation* pixelShader;
    /// Bound shader program.
    ShaderProgram* shaderProgram;
    /// Bound blend state.
    BlendState* blendState;
    /// Bound depth state.
    DepthState* depthState;
    /// Bound rasterizer state.
    RasterizerState* rasterizerState;
    /// Current primitive type.
    PrimitiveType primitiveType;
    /// Current scissor rectangle.
    IntRect scissorRect;
    /// Current stencil ref value.
    unsigned stencilRef;
    /// Number of supported constant buffer bindings for vertex shaders.
    size_t vsConstantBuffers;
    /// Number of supported constant buffer bindings for pixel shaders.
    size_t psConstantBuffers;
    /// Last used OpenGL texture unit.
    unsigned activeTexture;
    /// Last bound vertex buffer object.
    unsigned boundVBO;
    /// Fullscreen flag.
    bool fullscreen;
    /// Vertical sync flag.
    bool vsync;
    /// Resize handling flag to prevent recursion.
    bool inResize;
    /// Vertex attributes dirty (shader program changed) flag.
    bool vertexAttributesDirty;
    /// Vertex buffers dirty flag.
    bool vertexBuffersDirty;
};

/// Register Graphics related object factories and attributes.
TURSO3D_API void RegisterGraphicsLibrary();

}