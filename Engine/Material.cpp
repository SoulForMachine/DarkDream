
#include "BaseLib/SmartPtr.h"
#include "BaseLib/Parser.h"
#include "BaseLib/Console.h"
#include "BaseLib/GL/GLRenderer.h"
#include "Engine/FileSystem.h"
#include "Engine/ResourceManager.h"
#include "Engine/FileResource.h"
#include "Engine/EngineInternal.h"
#include "Material.h"

using namespace math3d;


namespace Engine
{

	Material::Material()
	{
		_flags = 0;
		_emissionTexture = 0;
		_diffuseTexture = 0;
		_normalMap = 0;
		_transpTexture = 0;

		_diffuseColor.set(1.0f, 1.0f, 1.0f);
		_useTransparency = false;
		_globalOpacity = 1.0f;
	}

	Material::Material(const Material& mat)
	{
		*this = mat;
	}

	Material::~Material()
	{
		Unload();
	}

	Material& Material::operator = (const Material& mat)
	{
		_flags = mat._flags;

		if(mat._emissionTexture)
		{
			engineAPI.textureManager->ReleaseTexture(_emissionTexture);
			_emissionTexture = engineAPI.textureManager->CreateTexture(mat._emissionTexture->GetFileName());
		}
		else
			_emissionTexture = 0;

		_diffuseColor = mat._diffuseColor;
		if(mat._diffuseTexture)
		{
			engineAPI.textureManager->ReleaseTexture(_diffuseTexture);
			_diffuseTexture = engineAPI.textureManager->CreateTexture(mat._diffuseTexture->GetFileName());
		}
		else
			_diffuseTexture = 0;

		if(mat._normalMap)
		{
			engineAPI.textureManager->ReleaseTexture(_normalMap);
			_normalMap = engineAPI.textureManager->CreateTexture(mat._normalMap->GetFileName());
		}
		else
			_normalMap = 0;

		_useTransparency = mat._useTransparency;
		_globalOpacity = mat._globalOpacity;
		if(mat._transpTexture)
		{
			engineAPI.textureManager->ReleaseTexture(_transpTexture);
			_transpTexture = engineAPI.textureManager->CreateTexture(mat._transpTexture->GetFileName());
		}
		else
			_transpTexture = 0;

		return *this;
	}

	bool Material::Load(const tchar* file_name)
	{
		if(!file_name)
			return false;

		Unload();

		SmartPtr<FileSys::File> file = engineAPI.fileSystem->Open(file_name, _t("rb"));
		if(!file)
			return false;

		Parser parser;
		if(!parser.LoadFile(*file))
			return false;
		file->Close();

		char emiss_tex_path[MAX_PATH];
		char diff_tex_path[MAX_PATH];
		char nrm_tex_path[MAX_PATH];
		char transp_tex_path[MAX_PATH];
		const int MAX_IDENT_LEN = 64;
		char buf[MAX_IDENT_LEN];

		if(!parser.ExpectTokenString("material"))
			return false;

		if(!parser.ExpectTokenType(Parser::TOK_PUNCTUATION, Parser::PUNCT_OPEN_BRACE))
			return false;

		if(!parser.ExpectTokenString("emissionTexture"))
			return false;
		if(!parser.ReadString(emiss_tex_path, MAX_PATH))
			return false;

		if(!parser.ExpectTokenString("diffuseColor"))
			return false;
		if(!parser.ReadVec3f(_diffuseColor))
			return false;

		if(!parser.ExpectTokenString("diffuseTexture"))
			return false;
		if(!parser.ReadString(diff_tex_path, MAX_PATH))
			return false;

		if(!parser.ExpectTokenString("normalMap"))
			return false;
		if(!parser.ReadString(nrm_tex_path, MAX_PATH))
			return false;

		if(!parser.ExpectTokenString("useTransparency"))
			return false;
		if(!parser.ReadIdentifier(buf, MAX_IDENT_LEN))
			return false;
		if(!strcmp(buf, "True"))
			_useTransparency = true;
		else if(!strcmp(buf, "False"))
			_useTransparency = false;
		else
		{
			Console::PrintError("Invalid value for useTransparency: %s", buf);
			return false;
		}

		if(!parser.ExpectTokenString("globalTransparency"))
			return false;
		if(!parser.ReadFloat(_globalOpacity))
			return false;

		if(!parser.ExpectTokenString("transparencyTexture"))
			return false;
		if(!parser.ReadString(transp_tex_path, MAX_PATH))
			return false;

		if(!parser.ExpectTokenType(Parser::TOK_PUNCTUATION, Parser::PUNCT_CLOSE_BRACE))
			return false;

		// create texture objects
		tchar* path;

		if(*emiss_tex_path)
		{
			path = CharToWideChar(emiss_tex_path);
			_emissionTexture = engineAPI.textureManager->CreateTexture(path);
			delete[] path;
		}

		if(*diff_tex_path)
		{
			path = CharToWideChar(diff_tex_path);
			_diffuseTexture = engineAPI.textureManager->CreateTexture(path);
			delete[] path;
		}

		if(*nrm_tex_path)
		{
			path = CharToWideChar(nrm_tex_path);
			_normalMap = engineAPI.textureManager->CreateTexture(path);
			delete[] path;
		}

		if(*transp_tex_path)
		{
			path = CharToWideChar(transp_tex_path);
			_transpTexture = engineAPI.textureManager->CreateTexture(path);
			delete[] path;
		}

		UpdateFlags();

		return true;
	}

	bool Material::Save(const tchar* file_name) const
	{
		SmartPtr<FileSys::File> file = engineAPI.fileSystem->Open(file_name, _t("wt"));
		if(!file)
		{
			Console::PrintError("Failed to write material file: %ls", file_name);
			return false;
		}

		const char* str = "";
		const tchar* tstr = _t("");

		file->Printf("// Daemonium engine material file\n\n");
		file->Printf("material\n{\n");

		tstr = _emissionTexture? _emissionTexture->GetFileName(): _t("");
		if(!tstr)
			tstr = _t("");
		file->Printf("\temissionTexture\t\t\"%ls\"\n", tstr);

		file->Printf("\tdiffuseColor\t\t[%f %f %f]\n", _diffuseColor.r, _diffuseColor.g, _diffuseColor.b);
		tstr = _diffuseTexture? _diffuseTexture->GetFileName(): _t("");
		if(!tstr)
			tstr = _t("");
		file->Printf("\tdiffuseTexture\t\t\"%ls\"\n", tstr);

		tstr = _normalMap? _normalMap->GetFileName(): _t("");
		if(!tstr)
			tstr = _t("");
		file->Printf("\tnormalMap\t\t\"%ls\"\n", tstr);

		file->Printf("\tuseTransparency\t\t%s\n", _useTransparency? "True": "False");
		file->Printf("\tglobalTransparency\t\t%f\n", _globalOpacity);
		tstr = _transpTexture? _transpTexture->GetFileName(): _t("");
		if(!tstr)
			tstr = _t("");
		file->Printf("\ttransparencyTexture\t\t\"%ls\"\n", tstr);

		file->Printf("}\n");

		file->Close();

		return true;
	}

	void Material::Unload()
	{
		if(_emissionTexture)
		{
			engineAPI.textureManager->ReleaseTexture(_emissionTexture);
			_emissionTexture = 0;
		}

		if(_diffuseTexture)
		{
			engineAPI.textureManager->ReleaseTexture(_diffuseTexture);
			_diffuseTexture = 0;
		}

		if(_normalMap)
		{
			engineAPI.textureManager->ReleaseTexture(_normalMap);
			_normalMap = 0;
		}

		if(_transpTexture)
		{
			engineAPI.textureManager->ReleaseTexture(_transpTexture);
			_transpTexture = 0;
		}
	}

	void Material::SetEmissionTexture(const TextureRes* val)
	{
		engineAPI.textureManager->ReleaseTexture(_emissionTexture);
		_emissionTexture = val;
		UpdateFlags();
	}

	void Material::SetDiffuseTexture(const TextureRes* val)
	{
		engineAPI.textureManager->ReleaseTexture(_diffuseTexture);
		_diffuseTexture = val;
		UpdateFlags();
	}

	void Material::SetNormalMap(const TextureRes* val)
	{
		engineAPI.textureManager->ReleaseTexture(_normalMap);
		_normalMap = val;
		UpdateFlags();
	}

	void Material::SetTransparencyTexture(const TextureRes* val)
	{
		engineAPI.textureManager->ReleaseTexture(_transpTexture);
		_transpTexture = val;
		UpdateFlags();
	}

	void Material::UpdateFlags()
	{
		_flags = 0;

		// emission
		if(_emissionTexture)
			_flags |= FLAG_EMISSION;

		// diffuse
		if(_diffuseTexture)
			_flags |= FLAG_DIFFUSE;

		// normal map
		if(_normalMap)
			_flags |= FLAG_NORMAL_MAP;

		// transparency
		if(_useTransparency && (_transpTexture || (_globalOpacity < 1.0f)))
			_flags |= FLAG_TRANSPARENCY;
	}

}
