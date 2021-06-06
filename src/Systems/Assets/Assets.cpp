#include "./Assets.h"

const ExtensionList ImageExtensions = {".png",".jpg"};
const ExtensionList SoundExtensions = { ".wav",".mp3",".ogg" };
const ExtensionList ShaderExtensions = { ".frag" };
const ExtensionList FontExtensions = { ".ttf" };

ResourceList textureList(ImageExtensions, "./Resources/Textures", "Block Textures");
ResourceList soundList(SoundExtensions, "./Resources/Sounds", "Sounds");
ResourceList fontList(FontExtensions, "./Resources/Fonts", "Fonts");
ResourceList shaderList(ShaderExtensions, "./Resources/BGShaders", "Background Shaders");

AssetCache<BGShader> shaderCache;
AssetCache<Texture> textureCache;
AssetCache<Sound> soundCache;
AssetCache<Font> fontCache;

ActiveAssets activeAssets;
