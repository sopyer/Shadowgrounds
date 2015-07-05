#pragma once

#include <d3d9.h>
#include <d3dx9.h>
#include <stdint.h>

#include <VertexFormats.h>

#define NUM_FRAMES_DELAY 2

#define MAX_CONSTS 224

struct GfxDevice
{
    LPDIRECT3DDEVICE9 device;

private:
    // cache
    enum
    {
        STD_SHADER_COUNT = 8
    };

    static bool cached;

    LPDIRECT3DVERTEXDECLARATION9 vtxFmtSet[FVF_COUNT];

    uint32_t  rs_valid[8];
    DWORD     rs[256];

    uint32_t  tex_valid;
    LPDIRECT3DBASETEXTURE9 textures[16];

    enum
    {
        RES_STATE_VS,
        RES_STATE_PS,
        RES_STATE_VF,
        RES_STATE_IB,
        RES_STATE_VB0,
        RES_STATE_VB1,
        RES_STATE_VB2,
        RES_STATE_VB3,
        RES_STATE_COUNT
    };

    uint32_t res_states;

    LPDIRECT3DVERTEXSHADER9 vs;
    LPDIRECT3DPIXELSHADER9  ps;

    LPDIRECT3DVERTEXDECLARATION9 vf;

    struct
    {
        LPDIRECT3DVERTEXBUFFER9 vb;
        UINT                    offset;
        UINT                    stride;
    } vb_state[4];

    LPDIRECT3DINDEXBUFFER9  ib;

    void resetCache();

    // frame sync
    LPDIRECT3DQUERY9 frame_sync[NUM_FRAMES_DELAY];
    size_t           frame_id = 0;

    LPDIRECT3DVERTEXBUFFER9 frame_vb[NUM_FRAMES_DELAY];
    UINT                    frame_vb_used;

    LPDIRECT3DINDEXBUFFER9 frame_ib16[NUM_FRAMES_DELAY];
    UINT                   frame_ib16_used;

    D3DXMATRIX world_mat;
    D3DXMATRIX view_mat;
    D3DXMATRIX proj_mat;

    LPDIRECT3DVERTEXSHADER9 stdVS[STD_SHADER_COUNT];
    LPDIRECT3DPIXELSHADER9  stdPS[STD_SHADER_COUNT];

    void createFrameResources(D3DPRESENT_PARAMETERS& params);
    void destroyFrameResources();

    VC2   fViewportDim;
    VC2I  iViewportDim;
    VC2   pxSize;

    void setViewportSize(int w, int h);

    D3DXVECTOR4 vertex_consts[MAX_CONSTS];
    D3DXVECTOR4 pixel_consts[MAX_CONSTS];

    UINT vconsts_range_min;
    UINT vconsts_range_max;

    UINT pconsts_range_min;
    UINT pconsts_range_max;

    void SetShaderConsts();

public:
    enum
    {
        SSF_2D_POS  = (1<<0),
        SSF_COLOR   = (1<<1),
        SSF_TEXTURE = (1<<2),
    };

    bool init(LPDIRECT3D9 d3d, UINT Adapter, HWND hWnd, D3DPRESENT_PARAMETERS& params);
    void fini();


    bool reset(D3DPRESENT_PARAMETERS& params);


    void beginFrame();
    void endFrame();


    void SetWorldMatrix(const D3DXMATRIX& world);
    void SetViewMatrix(const D3DXMATRIX& view);
    void SetProjectionMatrix(const D3DXMATRIX& proj);
    void CommitConstants();
    void SetStdProgram(size_t id);


    void SetDynIdx16Buffer();
    bool lockDynIdx16(UINT count, uint16_t** ptr, UINT* baseIndex);
    void unlockDynIdx16();


    void SetDynVtxBuffer(UINT Stride);
    bool lockDynVtx(UINT count, UINT stride, void** ptr, UINT* baseVertex);
    void unlockDynVtx();

    template<typename T>
    void SetDynVtxBuffer()
    {
        SetDynVtxBuffer(sizeof(T));
    }

    template<typename T>
    bool lockDynVtx(UINT count, T** ptr, UINT* baseVertex)
    {
        return lockDynVtx(count, sizeof(T), (void**)ptr, baseVertex);
    }


    VC2 pixelSize() {return pxSize;}


    //Fixed function imitation
    void SetFVF(FVF vtxFmt);
    void DrawPrimitiveUP(D3DPRIMITIVETYPE PrimitiveType,UINT PrimitiveCount,CONST void* vertexData,UINT Stride);


    HRESULT TestCooperativeLevel();
    HRESULT EvictManagedResources();
    HRESULT GetDeviceCaps(D3DCAPS9* pCaps);
    HRESULT GetDisplayMode(UINT iSwapChain,D3DDISPLAYMODE* pMode);
    HRESULT GetBackBuffer(UINT iSwapChain,UINT iBackBuffer,D3DBACKBUFFER_TYPE Type,IDirect3DSurface9** ppBackBuffer);
    void    SetGammaRamp(UINT iSwapChain,DWORD Flags,CONST D3DGAMMARAMP* pRamp);
    void    GetGammaRamp(UINT iSwapChain,D3DGAMMARAMP* pRamp);
    HRESULT CreateTexture(UINT Width,UINT Height,UINT Levels,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,IDirect3DTexture9** ppTexture,HANDLE* pSharedHandle);
    HRESULT CreateVolumeTexture(UINT Width,UINT Height,UINT Depth,UINT Levels,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,IDirect3DVolumeTexture9** ppVolumeTexture,HANDLE* pSharedHandle);
    HRESULT CreateCubeTexture(UINT EdgeLength,UINT Levels,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,IDirect3DCubeTexture9** ppCubeTexture,HANDLE* pSharedHandle);
    HRESULT CreateVertexBuffer(UINT Length,DWORD Usage,DWORD FVF,D3DPOOL Pool,IDirect3DVertexBuffer9** ppVertexBuffer,HANDLE* pSharedHandle);
    HRESULT CreateIndexBuffer(UINT Length,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,IDirect3DIndexBuffer9** ppIndexBuffer,HANDLE* pSharedHandle);
    HRESULT CreateRenderTarget(UINT Width,UINT Height,D3DFORMAT Format,D3DMULTISAMPLE_TYPE MultiSample,DWORD MultisampleQuality,BOOL Lockable,IDirect3DSurface9** ppSurface,HANDLE* pSharedHandle);
    HRESULT CreateDepthStencilSurface(UINT Width,UINT Height,D3DFORMAT Format,D3DMULTISAMPLE_TYPE MultiSample,DWORD MultisampleQuality,BOOL Discard,IDirect3DSurface9** ppSurface,HANDLE* pSharedHandle);
    HRESULT UpdateSurface(IDirect3DSurface9* pSourceSurface,CONST RECT* pSourceRect,IDirect3DSurface9* pDestinationSurface,CONST POINT* pDestPoint);
    HRESULT UpdateTexture(IDirect3DBaseTexture9* pSourceTexture,IDirect3DBaseTexture9* pDestinationTexture);
    HRESULT GetRenderTargetData(IDirect3DSurface9* pRenderTarget,IDirect3DSurface9* pDestSurface);
    HRESULT GetFrontBufferData(UINT iSwapChain,IDirect3DSurface9* pDestSurface);
    HRESULT StretchRect(IDirect3DSurface9* pSourceSurface,CONST RECT* pSourceRect,IDirect3DSurface9* pDestSurface,CONST RECT* pDestRect,D3DTEXTUREFILTERTYPE Filter);
    HRESULT CreateOffscreenPlainSurface(UINT Width,UINT Height,D3DFORMAT Format,D3DPOOL Pool,IDirect3DSurface9** ppSurface,HANDLE* pSharedHandle);
    HRESULT SetRenderTarget(DWORD RenderTargetIndex,IDirect3DSurface9* pRenderTarget);
    HRESULT GetRenderTarget(DWORD RenderTargetIndex,IDirect3DSurface9** ppRenderTarget);
    HRESULT SetDepthStencilSurface(IDirect3DSurface9* pNewZStencil);
    HRESULT GetDepthStencilSurface(IDirect3DSurface9** ppZStencilSurface);
    HRESULT BeginScene();
    HRESULT EndScene();
    HRESULT Clear(DWORD Count,CONST D3DRECT* pRects,DWORD Flags,D3DCOLOR Color,float Z,DWORD Stencil);
    HRESULT SetTransform(D3DTRANSFORMSTATETYPE State,CONST D3DMATRIX* pMatrix);
    HRESULT SetViewport(CONST D3DVIEWPORT9* pViewport);
    HRESULT SetMaterial(CONST D3DMATERIAL9* pMaterial);
    HRESULT GetMaterial(D3DMATERIAL9* pMaterial);
    HRESULT SetLight(DWORD Index,CONST D3DLIGHT9*);
    HRESULT LightEnable(DWORD Index,BOOL Enable);
    HRESULT SetClipPlane(DWORD Index,CONST float* pPlane);
    HRESULT SetRenderState(D3DRENDERSTATETYPE State,DWORD Value);
    HRESULT CreateStateBlock(D3DSTATEBLOCKTYPE Type,IDirect3DStateBlock9** ppSB);
    HRESULT BeginStateBlock();
    HRESULT EndStateBlock(IDirect3DStateBlock9** ppSB);
    HRESULT SetTexture(DWORD Stage,IDirect3DBaseTexture9* pTexture);
    HRESULT SetTextureStageState(DWORD Stage,D3DTEXTURESTAGESTATETYPE Type,DWORD Value);
    HRESULT SetSamplerState(DWORD Sampler,D3DSAMPLERSTATETYPE Type,DWORD Value);
    HRESULT SetScissorRect(CONST RECT* pRect);
    HRESULT DrawPrimitive(D3DPRIMITIVETYPE PrimitiveType,UINT StartVertex,UINT PrimitiveCount);
    HRESULT DrawIndexedPrimitive(D3DPRIMITIVETYPE,INT BaseVertexIndex,UINT MinVertexIndex,UINT NumVertices,UINT startIndex,UINT primCount);
    HRESULT CreateVertexDeclaration(CONST D3DVERTEXELEMENT9* pVertexElements,IDirect3DVertexDeclaration9** ppDecl);
    HRESULT SetVertexDeclaration(IDirect3DVertexDeclaration9* pDecl);
    HRESULT CreateVertexShader(CONST DWORD* pFunction,IDirect3DVertexShader9** ppShader);
    HRESULT SetVertexShader(IDirect3DVertexShader9* pShader);
    HRESULT SetVertexShaderConstantF(UINT StartRegister,CONST float* pConstantData,UINT Vector4fCount);
    HRESULT SetVertexShaderConstantI(UINT StartRegister,CONST int* pConstantData,UINT Vector4iCount);
    HRESULT SetVertexShaderConstantB(UINT StartRegister,CONST BOOL* pConstantData,UINT  BoolCount);
    HRESULT SetStreamSource(UINT StreamNumber,IDirect3DVertexBuffer9* pStreamData,UINT OffsetInBytes,UINT Stride);
    HRESULT SetStreamSourceFreq(UINT StreamNumber,UINT Setting);
    HRESULT SetIndices(IDirect3DIndexBuffer9* pIndexData);
    HRESULT CreatePixelShader(CONST DWORD* pFunction,IDirect3DPixelShader9** ppShader);
    HRESULT SetPixelShader(IDirect3DPixelShader9* pShader);
    HRESULT SetPixelShaderConstantF(UINT StartRegister,CONST float* pConstantData,UINT Vector4fCount);
    HRESULT SetPixelShaderConstantI(UINT StartRegister,CONST int* pConstantData,UINT Vector4iCount);
    HRESULT SetPixelShaderConstantB(UINT StartRegister,CONST BOOL* pConstantData,UINT  BoolCount);
    HRESULT CreateQuery(D3DQUERYTYPE Type,IDirect3DQuery9** ppQuery);
};
