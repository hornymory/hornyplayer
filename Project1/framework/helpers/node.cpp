#include "../headers/node.h"
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <cmath>

struct node_cb_data_t 
{
    float resolution[2]; 
    float time;          
    float line_weight;   
    float base_color[4];
    float speed;         
    float padding[3];    
};

static const char* node_shader_source = R"(
cbuffer NodeParams : register(b0) {
    float2 resolution;
    float time;
    float line_weight;
    float4 base_color;
    float speed;
};

float N21(float2 p) {
    float3 a = frac(float3(p.xyx) * float3(213.897, 653.453, 253.098));
    a += dot(a, a.yzx + 79.76);
    return frac((a.x + a.y) * a.z);
}
float roundedBoxSDF(float2 p, float2 b, float r)
{
    float2 q = abs(p) - b + r;
    return length(max(q, 0.0)) + min(max(q.x, q.y), 0.0) - r;
}

float2 GetPos(float2 id, float2 offs, float t) {
    float n = N21(id + offs);
    float n1 = frac(n * 10.0);
    float n2 = frac(n * 100.0);
    float a = t + n;
    return offs + float2(sin(a * n1), cos(a * n2)) * 0.4;
}

float df_line(float2 a, float2 b, float2 p) {
    float2 pa = p - a, ba = b - a;
    float h = clamp(dot(pa, ba) / dot(ba, ba), 0.0, 1.0);
    return length(pa - ba * h);
}

float draw_line(float2 a, float2 b, float2 uv) {
    float r1 = 0.04 * line_weight;
    float r2 = 0.01 * line_weight;
    float d = df_line(a, b, uv);
    float d2 = length(a - b);
    float fade = smoothstep(1.5, 0.5, d2);
    fade += smoothstep(0.05, 0.02, abs(d2 - 0.75));
    return smoothstep(r1, r2, d) * fade;
}

float NetLayer(float2 st, float n, float t) {
    float2 id = floor(st) + n;
    st = frac(st) - 0.5;
    float2 p[9];
    int idx = 0;
    for(float y = -1.0; y <= 1.0; y++) {
        for(float x = -1.0; x <= 1.0; x++) {
            p[idx++] = GetPos(id, float2(x, y), t);
        }
    }
    float m = 0.0;
    float sparkle = 0.0;
    for(int i = 0; i < 9; i++) {
        m += draw_line(p[4], p[i], st);
        float d = length(st - p[i]);
        float s = (0.005 / (d * d));
        s *= smoothstep(1.0, 0.7, d);
        float pulse = sin((frac(p[i].x) + frac(p[i].y) + t) * 5.0) * 0.4 + 0.6;
        s *= pow(pulse, 20.0);
        sparkle += s;
    }
    m += draw_line(p[1], p[3], st);
    m += draw_line(p[1], p[5], st);
    m += draw_line(p[7], p[5], st);
    m += draw_line(p[7], p[3], st);
    return m + sparkle * ((sin(t) + sin(t * 0.1)) * 0.25 + 0.5);
}

float4 main(float4 pos : SV_POSITION, float2 uv : TEXCOORD) : SV_Target {
    float2 st = (uv - 0.5) * (resolution.xy / resolution.y);
    float t = time * speed;
    float m = 0.0;
    
    for(float i = 0.0; i < 1.0; i += 0.25) {
        float z = frac(t * 0.1 + i);
        float size = lerp(15.0, 1.0, z);
        float fade = smoothstep(0.0, 0.6, z) * smoothstep(1.0, 0.8, z);
        m += fade * NetLayer(st * size, i, t);
    }
    float2 uv2 = (uv - 0.5) * 2.0; 
    float d = roundedBoxSDF(uv2, float2(1.0, 1.0), 0.2);

    float mask = smoothstep(0.01, 0.0, d);
    float3 col = base_color.rgb * m;
    
    float finalAlpha = m * base_color.a * mask;
    
    return float4(col, finalAlpha);
}
)";

struct node_state_backup_t
{
    ID3D11RasterizerState* rs = nullptr;
    ID3D11BlendState* blend_state = nullptr;
    FLOAT blend_factor[4] = { 0 };
    UINT sample_mask = 0;
    ID3D11DepthStencilState* depth_stencil_state = nullptr;
    UINT stencil_ref = 0;
    ID3D11ShaderResourceView* ps_shader_resource = nullptr;
    ID3D11SamplerState* ps_sampler = nullptr;
    ID3D11PixelShader* ps = nullptr;
    ID3D11VertexShader* vs = nullptr;
    UINT ps_instances_count = 0, vs_instances_count = 0;
    ID3D11ClassInstance* ps_instances[256] = { nullptr }, * vs_instances[256] = { nullptr };
    D3D11_PRIMITIVE_TOPOLOGY primitive_topology = D3D_PRIMITIVE_TOPOLOGY_UNDEFINED;
    ID3D11Buffer* index_buffer = nullptr, * vertex_buffer = nullptr;
    UINT index_buffer_offset = 0, vertex_buffer_stride = 0, vertex_buffer_offset = 0;
    DXGI_FORMAT index_buffer_format = DXGI_FORMAT_UNKNOWN;
    ID3D11InputLayout* input_layout = nullptr;
    ID3D11RenderTargetView* rtv = nullptr;
    ID3D11DepthStencilView* dsv = nullptr;
    D3D11_VIEWPORT viewports[D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE] = { {0} };
    UINT viewports_count = 0;

    ~node_state_backup_t()
    {
        if (rs) rs->Release();
        if (blend_state) blend_state->Release();
        if (depth_stencil_state) depth_stencil_state->Release();
        if (ps_shader_resource) ps_shader_resource->Release();
        if (ps_sampler) ps_sampler->Release();
        if (ps) ps->Release();
        if (vs) vs->Release();
        for (UINT i = 0; i < ps_instances_count; i++) if (ps_instances[i]) ps_instances[i]->Release();
        for (UINT i = 0; i < vs_instances_count; i++) if (vs_instances[i]) vs_instances[i]->Release();
        if (index_buffer) index_buffer->Release();
        if (vertex_buffer) vertex_buffer->Release();
        if (input_layout) input_layout->Release();
        if (rtv) rtv->Release();
        if (dsv) dsv->Release();
    }
};

static void node_save_state(ID3D11DeviceContext* ctx, node_state_backup_t* backup)
{
    ctx->IAGetPrimitiveTopology(&backup->primitive_topology);
    ctx->IAGetVertexBuffers(0, 1, &backup->vertex_buffer, &backup->vertex_buffer_stride, &backup->vertex_buffer_offset);
    ctx->IAGetIndexBuffer(&backup->index_buffer, &backup->index_buffer_format, &backup->index_buffer_offset);
    ctx->IAGetInputLayout(&backup->input_layout);
    backup->vs_instances_count = 256;
    ctx->VSGetShader(&backup->vs, backup->vs_instances, &backup->vs_instances_count);
    backup->ps_instances_count = 256;
    ctx->PSGetShader(&backup->ps, backup->ps_instances, &backup->ps_instances_count);
    ctx->PSGetShaderResources(0, 1, &backup->ps_shader_resource);
    ctx->PSGetSamplers(0, 1, &backup->ps_sampler);
    ctx->OMGetBlendState(&backup->blend_state, backup->blend_factor, &backup->sample_mask);
    ctx->OMGetDepthStencilState(&backup->depth_stencil_state, &backup->stencil_ref);
    ctx->RSGetState(&backup->rs);
    backup->viewports_count = D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE;
    ctx->RSGetViewports(&backup->viewports_count, backup->viewports);
    ctx->OMGetRenderTargets(1, &backup->rtv, &backup->dsv);
}

static void node_restore_state(ID3D11DeviceContext* ctx, node_state_backup_t* backup)
{
    ctx->IASetPrimitiveTopology(backup->primitive_topology);
    ctx->IASetVertexBuffers(0, 1, &backup->vertex_buffer, &backup->vertex_buffer_stride, &backup->vertex_buffer_offset);
    ctx->IASetIndexBuffer(backup->index_buffer, backup->index_buffer_format, backup->index_buffer_offset);
    ctx->IASetInputLayout(backup->input_layout);
    ctx->VSSetShader(backup->vs, backup->vs_instances, backup->vs_instances_count);
    ctx->PSSetShader(backup->ps, backup->ps_instances, backup->ps_instances_count);
    ctx->PSSetShaderResources(0, 1, &backup->ps_shader_resource);
    ctx->PSSetSamplers(0, 1, &backup->ps_sampler);
    ctx->OMSetBlendState(backup->blend_state, backup->blend_factor, backup->sample_mask);
    ctx->OMSetDepthStencilState(backup->depth_stencil_state, backup->stencil_ref);
    ctx->RSSetState(backup->rs);
    ctx->RSSetViewports(backup->viewports_count, backup->viewports);
    ctx->OMSetRenderTargets(1, &backup->rtv, backup->dsv);
}



struct nodes_callback_data
{
    ID3D11Device* device;
    ID3D11DeviceContext* ctx;
    node_state_backup_t* state;
    node_cb_data_t cb_data;
};

static ID3D11VertexShader* node_vs = nullptr;
static ID3D11PixelShader* node_ps = nullptr;
static ID3D11Buffer* node_vb = nullptr;
static ID3D11Buffer* node_cb = nullptr;
static ID3D11InputLayout* node_layout = nullptr;
static ID3D11BlendState* node_blend = nullptr;

static void nodes_render_pass(const ImDrawList* parent_list, const ImDrawCmd* cmd)
{
    nodes_callback_data* data = (nodes_callback_data*)cmd->UserCallbackData;
    if (!data || !data->ctx || !node_cb || !node_ps) return; 

    data->ctx->UpdateSubresource(node_cb, 0, nullptr, &data->cb_data, 0, 0);
    data->ctx->VSSetShader(node_vs, nullptr, 0);
    data->ctx->PSSetShader(node_ps, nullptr, 0);
    data->ctx->PSSetConstantBuffers(0, 1, &node_cb);
    data->ctx->IASetInputLayout(node_layout);

    UINT stride = sizeof(float) * 4;
    UINT offset = 0;
    data->ctx->IASetVertexBuffers(0, 1, &node_vb, &stride, &offset);
    data->ctx->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

    float blend_factor[4] = { 0.f,0.f,0.f,0.f };
    data->ctx->OMSetBlendState(node_blend, blend_factor, 0xffffffff);

    data->ctx->Draw(4, 0);
}
static void begin_node(const ImDrawList* parent_list, const ImDrawCmd* cmd)
{
    nodes_callback_data* data = (nodes_callback_data*)cmd->UserCallbackData;
    if (!data) return;

    // Keep exactly one backup object per callback chain.
    if (!data->state)
        data->state = new node_state_backup_t();
    node_save_state(data->ctx, data->state);

    float blend_factor[4] = { 0.f, 0.f, 0.f, 0.f };
    data->ctx->OMSetBlendState(node_blend, blend_factor, 0xffffffff);
    data->ctx->IASetInputLayout(node_layout);
    data->ctx->VSSetShader(node_vs, nullptr, 0);
    data->ctx->PSSetShader(node_ps, nullptr, 0);
    data->ctx->PSSetConstantBuffers(0, 1, &node_cb);

    UINT stride = sizeof(float) * 4;
    UINT offset = 0;
    data->ctx->IASetVertexBuffers(0, 1, &node_vb, &stride, &offset);
    data->ctx->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
}
static void end_node(const ImDrawList* parent_list, const ImDrawCmd* cmd)
{
    nodes_callback_data* data = (nodes_callback_data*)cmd->UserCallbackData;
    if (!data) return;

    if (data->state && data->ctx)
    {
        node_restore_state(data->ctx, data->state);
        delete data->state;
        data->state = nullptr;
    }
    delete data;
}

static bool ensure_node_resources(ID3D11Device* device)
{
    if (node_vb && node_ps && node_cb && node_layout && node_blend)
        return true;

    if (node_layout) { node_layout->Release(); node_layout = nullptr; }
    if (node_ps) { node_ps->Release(); node_ps = nullptr; }
    if (node_cb) { node_cb->Release(); node_cb = nullptr; }
    if (node_blend) { node_blend->Release(); node_blend = nullptr; }

    const char* vs_src =
        "struct VS_INPUT { float2 pos : POSITION; float2 uv : TEXCOORD; };"
        "struct PS_INPUT { float4 pos : SV_POSITION; float2 uv : TEXCOORD; };"
        "PS_INPUT main(VS_INPUT input) {"
        "    PS_INPUT output;"
        "    output.pos = float4(input.pos, 0.0, 1.0);"
        "    output.uv = input.uv;"
        "    return output;"
        "}";

    ID3DBlob* vs_blob = nullptr;
    if (FAILED(D3DCompile(vs_src, strlen(vs_src), nullptr, nullptr, nullptr, "main", "vs_5_0", 0, 0, &vs_blob, nullptr)))
        return false;

    device->CreateVertexShader(vs_blob->GetBufferPointer(), vs_blob->GetBufferSize(), nullptr, &node_vs);

    D3D11_INPUT_ELEMENT_DESC layout_desc[] = {
        { "POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 8, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };
    device->CreateInputLayout(layout_desc, 2, vs_blob->GetBufferPointer(), vs_blob->GetBufferSize(), &node_layout);
    vs_blob->Release();

    float vertices[] = {
        -1.0f,  1.0f, 0.0f, 0.0f,
         1.0f,  1.0f, 1.0f, 0.0f,
        -1.0f, -1.0f, 0.0f, 1.0f,
         1.0f, -1.0f, 1.0f, 1.0f
    };
    D3D11_BUFFER_DESC vbd = {};
    vbd.Usage = D3D11_USAGE_DEFAULT;
    vbd.ByteWidth = sizeof(vertices);
    vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    D3D11_SUBRESOURCE_DATA vsd = { vertices };
    device->CreateBuffer(&vbd, &vsd, &node_vb);

    ID3DBlob* ps_blob = nullptr;
    if (FAILED(D3DCompile(node_shader_source, strlen(node_shader_source), nullptr, nullptr, nullptr, "main", "ps_5_0", 0, 0, &ps_blob, nullptr)))
        return false;
    device->CreatePixelShader(ps_blob->GetBufferPointer(), ps_blob->GetBufferSize(), nullptr, &node_ps);
    ps_blob->Release();

    D3D11_BUFFER_DESC cbd = {};
    cbd.ByteWidth = sizeof(node_cb_data_t);
    cbd.Usage = D3D11_USAGE_DEFAULT;
    cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    device->CreateBuffer(&cbd, nullptr, &node_cb);

    D3D11_BLEND_DESC bld = {};
    bld.RenderTarget[0].BlendEnable = TRUE;
    bld.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
    bld.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
    bld.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
    bld.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
    bld.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
    bld.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
    bld.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
    device->CreateBlendState(&bld, &node_blend);

    return true;
}

void draw_nodes_background(ImDrawList* draw_list, ID3D11Device* device, ID3D11DeviceContext* ctx)
{
    if (!device || !ctx || !ensure_node_resources(device)) return;

    nodes_callback_data* data = new nodes_callback_data();
    data->device = device;
    data->ctx = ctx;
    data->state = nullptr;

    data->cb_data.resolution[0] = ImGui::GetIO().DisplaySize.x;
    data->cb_data.resolution[1] = ImGui::GetIO().DisplaySize.y;
    data->cb_data.time = (float)ImGui::GetTime();
    data->cb_data.line_weight = elements->node_settings.line_weight;
    data->cb_data.speed = elements->node_settings.speed;
    data->cb_data.base_color[0] = clr->window.node_color.Value.x;
    data->cb_data.base_color[1] = clr->window.node_color.Value.y;
    data->cb_data.base_color[2] = clr->window.node_color.Value.z;
    data->cb_data.base_color[3] = clr->window.node_color.Value.w;
    draw_list->AddCallback(begin_node, data);
    draw_list->AddCallback(nodes_render_pass, data);
    draw_list->AddCallback(end_node, data);
}

void release_node_resources()
{
    if (node_vs) { node_vs->Release(); node_vs = nullptr; }
    if (node_ps) { node_ps->Release(); node_ps = nullptr; }
    if (node_vb) { node_vb->Release(); node_vb = nullptr; }
    if (node_cb) { node_cb->Release(); node_cb = nullptr; }
    if (node_layout) { node_layout->Release(); node_layout = nullptr; }
    if (node_blend) { node_blend->Release(); node_blend = nullptr; }
}
