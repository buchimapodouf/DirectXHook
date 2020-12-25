#include "Textures.h"

using namespace DirectX;
using namespace Microsoft::WRL;

Textures::Textures(ID3D11Device* device)
{
	console = DebugConsole();
	this->device = device;
	textures = std::vector<ComPtr<ID3D11ShaderResourceView>>();
}

int Textures::LoadTexture(std::string filepath)
{

	if (device == nullptr)
	{
		console.Print(MsgType::FAILED, "Could not load texture, device was nullptr");
		return -1;
	}

	console.Print("Loading texture: %s", filepath);

	ComPtr<ID3D11ShaderResourceView> texture = nullptr;

	// Convert our filepath string to a wide string, because Windows likes wide characters
	std::wstring wideString(filepath.length(), ' ');
	std::copy(filepath.begin(), filepath.end(), wideString.begin());

	std::fstream file = std::fstream(filepath);

	if (file.fail())
	{
		console.Print(MsgType::FAILED, "Texture loading failed, file was not found at: %s", filepath);
		file.close();
		return -1;
	}

	file.close();

	HRESULT texResult = CreateWICTextureFromFile(device.Get(), wideString.c_str(), nullptr, texture.GetAddressOf());

	_com_error texErr(texResult);
	console.Print("Texture HRESULT: %s", texErr.ErrorMessage());

	if (FAILED(texResult))
	{
		console.Print(MsgType::FAILED, "Texture loading failed: %s", filepath);
		return -1;
	}
	
	textures.push_back(texture);

	return textures.size() - 1;
}

ID3D11ShaderResourceView* Textures::Get(int textureIndex)
{
	if (textureIndex < 0 || textureIndex >= textures.size()) return nullptr;
	return textures.at(textureIndex).Get();
}
