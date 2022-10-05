/**********************************************************************************        
**********************************************************************************/

#include "Image.h"
#include "Graphics.h" 

Image::Image(string filename) : textureView(nullptr), width(0), height(0)
{ 
    D3D11CreateTextureFromFile(
        Graphics::device, 
        Graphics::context, 
        filename.c_str(), 
        nullptr, 
        &textureView, 
        width, 
        height); 
} 

Image::Image(string filename, 
    uint tileWidth, uint tileHeight, uint numColumns,
    int* tiledMap, uint tiledMapCols, uint tiledMapRows)
{ 
    ID3D11Resource* tilesTexture = nullptr;
    uint mapWidth = 0;
    uint mapHeight = 0; 
    D3D11CreateTextureFromFile(
        Graphics::device, 
        Graphics::context, 
        filename.c_str(), 
        &tilesTexture, 
        nullptr, 
        mapWidth, 
        mapHeight);  
    D3D11_TEXTURE2D_DESC desc;
    desc.Width = tileWidth * tiledMapCols;
    desc.Height = tileHeight * tiledMapRows;
    desc.MipLevels = 1;
    desc.ArraySize = 1;
    desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    desc.SampleDesc.Count = 1;
    desc.SampleDesc.Quality = 0;
    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    desc.CPUAccessFlags = 0;
    desc.MiscFlags = 0; 
    ID3D11Texture2D* imgTexture;
    Graphics::device->CreateTexture2D(&desc, nullptr, &imgTexture); 
    for (uint i = 0; i < tiledMapRows; ++i)
    {
        for (uint j = 0; j < tiledMapCols; ++j)
        {   
            uint tileNum = tiledMap[j + i * tiledMapCols] - 1;

            uint x = j * tileWidth;
            uint y = i * tileHeight;

            D3D11_BOX sourceRegion;
            sourceRegion.left = (tileNum % numColumns) * tileWidth;
            sourceRegion.top = (tileNum / numColumns) * tileHeight;
            sourceRegion.right = sourceRegion.left + tileWidth;
            sourceRegion.bottom = sourceRegion.top + tileHeight;
            sourceRegion.front = 0;
            sourceRegion.back = 1; 
            Graphics::context->CopySubresourceRegion(imgTexture, 0, x, y, 0, tilesTexture, 0, &sourceRegion);
        }
    } 
    D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc;
    memset(&viewDesc, 0, sizeof(viewDesc));
    viewDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    viewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    viewDesc.Texture2D.MipLevels = 1; 
    textureView = nullptr;
    width = desc.Width;
    height = desc.Height; 
    Graphics::device->CreateShaderResourceView(imgTexture, &viewDesc, &textureView); 
    tilesTexture->Release();
    imgTexture->Release();
} 

Image::~Image()
{ 
    if (textureView)
    { 
        ID3D11Resource * resource = nullptr;
        textureView->GetResource(&resource);  
        if (resource)
        {
            resource->Release();
            resource = nullptr;
        }

        textureView->Release();
        textureView = nullptr;
    }
} 