#pragma once

#include <SDL2/SDL_log.h>
#include <string>
#include <vector>
#include <filesystem>
#include <unordered_map>

#include <SDL2/SDL.h>

#include "./Font.h"
#include "./Sound.h"
#include "./Texture.h"
#include "./BGShader.h"
#include "./LinearTexture.h"

namespace fs = std::filesystem;

using std::string;
using std::vector;

using ExtensionList = vector<string>;
	
//Scans Directories to find valid files
class ResourceList {
public:
	const ExtensionList validExtensions;
	const string path;
	const string title;
	vector<string> files;
	vector<string> names;

	ResourceList(const ExtensionList& extensions, string path, string title) : validExtensions(extensions), path(path), title(title), files() {
		scan();
	}

	void scan() {
		SDL_Log("Scanning for %s", title.c_str());
		files.clear();
		names.clear();

		for (auto& p : fs::directory_iterator(path)) {
			string file = p.path().string();

			if (!isValid(file))
				continue;

			SDL_Log("	%s", file.c_str());

			string name = p.path().stem().string();

			files.push_back(p.path().string());
			names.push_back(name);
		}

	}

	bool isValid(string& file) {
		for (auto& extension : validExtensions) {
			if (file.find(extension) != string::npos)
				return true;
		}

		return false;
	}

	bool contains(string& toCheck) {
		for (auto& s : files)
			if (s == toCheck)
				return true;
		return false;
	}
};

template <class type>
class AssetCache {
	std::unordered_map<string, type*> loadedFiles;

	public:
	//Load file if it hasn't been loaded already, otherwise return the cached item
	type* get(const string& path) {

		auto iter = loadedFiles.find(path);

		if (iter == loadedFiles.end()) {
			type* toLoad = new type(path);
			
			// SDL_Log("Loaded: %s", path.c_str());

			loadedFiles.insert({ path, toLoad });
			return toLoad;
		} else {
			return iter->second;
		}
		
		return nullptr;
	}
};

extern ResourceList textureList;
extern ResourceList soundList;
extern ResourceList fontList;
extern ResourceList shaderList;

extern AssetCache<BGShader> shaderCache;
extern AssetCache<Texture> textureCache;
extern AssetCache<Sound> soundCache;
extern AssetCache<Font> fontCache;

struct ActiveAssets {
    Texture* pieceTexture;
    BGShader* bgShader;
	Font* font;
	
	Sound* sound_lineclear = 0;
    Sound* sound_sonicdrop = 0;
    Sound* sound_harddrop = 0;
    Sound* sound_lock = 0;

	Sound* sound_combo5 = 0;
    Sound* sound_combo7 = 0;
    Sound* sound_combo10 = 0;
    Sound* sound_combo12 = 0;
    Sound* sound_combo14 = 0;
};

extern ActiveAssets activeAssets;