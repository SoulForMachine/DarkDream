
#include "BaseLib/SmartPtr.h"
#include "BaseLib/Parser.h"
#include "BaseLib/Console.h"
#include "BaseLib/GL/GLRenderer.h"
#include "Engine/FileSystem.h"
#include "Engine/Resource.h"
#include "Engine/EngineInternal.h"
#include "Material.h"

using namespace math3d;


namespace Engine
{

	Material::Material()
	{
		_flags = 0;
		_emissionTexture = Texture2DResPtr::null;
		_diffuseTexture = Texture2DResPtr::null;
		_normalMap = Texture2DResPtr::null;
		_transpTexture = Texture2DResPtr::null;

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
			_emissionTexture = engineAPI.textureManager->CreateTexture2D(mat._emissionTexture.GetRes()->GetFileName());
		}
		else
			_emissionTexture = Texture2DResPtr::null;

		_diffuseColor = mat._diffuseColor;
		if(mat._diffuseTexture)
		{
			engineAPI.textureManager->ReleaseTexture(_diffuseTexture);
			_diffuseTexture = engineAPI.textureManager->CreateTexture2D(mat._diffuseTexture.GetRes()->GetFileName());
		}
		else
			_diffuseTexture = Texture2DResPtr::null;

		if(mat._normalMap)
		{
			engineAPI.textureManager->ReleaseTexture(_normalMap);
			_normalMap = engineAPI.textureManager->CreateTexture2D(mat._normalMap.GetRes()->GetFileName());
		}
		else
			_normalMap = Texture2DResPtr::null;

		_useTransparency = mat._useTransparency;
		_globalOpacity = mat._globalOpacity;
		if(mat._transpTexture)
		{
			engineAPI.textureManager->ReleaseTexture(_transpTexture);
			_transpTexture = engineAPI.textureManager->CreateTexture2D(mat._transpTexture.GetRes()->GetFileName());
		}
		else
			_transpTexture = Texture2DResPtr::null;

		return *this;
	}

	bool Material::Load(const tchar* file_name)
	{
		if(!file_name)
			return false;

		Unload();

		SmartPtr<FileUtil::File> file = engineAPI.fileSystem->Open(file_name, _t("rb"));
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

		try
		{
			parser.ExpectTokenString("material");

			parser.ExpectTokenType(Parser::TOK_PUNCTUATION, Parser::PUNCT_OPEN_BRACE);

			parser.ExpectTokenString("emissionTexture");
			parser.ReadString(emiss_tex_path, MAX_PATH);

			parser.ExpectTokenString("diffuseColor");
			parser.ReadVec3f(_diffuseColor);

			parser.ExpectTokenString("diffuseTexture");
			parser.ReadString(diff_tex_path, MAX_PATH);

			parser.ExpectTokenString("normalMap");
			parser.ReadString(nrm_tex_path, MAX_PATH);

			parser.ExpectTokenString("useTransparency");
			parser.ReadBool(_useTransparency);

			parser.ExpectTokenString("globalTransparency");
			parser.ReadFloat(_globalOpacity);

			parser.ExpectTokenString("transparencyTexture");
			parser.ReadString(transp_tex_path, MAX_PATH);

			parser.ExpectTokenType(Parser::TOK_PUNCTUATION, Parser::PUNCT_CLOSE_BRACE);
		}
		catch(ParserException& e)
		{
			Console::PrintError(e.GetDesc());
			return false;
		}

		// create texture objects
		tchar* path;

		if(*emiss_tex_path)
		{
			path = CharToWideChar(emiss_tex_path);
			_emissionTexture = engineAPI.textureManager->CreateTexture2D(path);
			delete[] path;
		}

		if(*diff_tex_path)
		{
			path = CharToWideChar(diff_tex_path);
			_diffuseTexture = engineAPI.textureManager->CreateTexture2D(path);
			delete[] path;
		}

		if(*nrm_tex_path)
		{
			path = CharToWideChar(nrm_tex_path);
			_normalMap = engineAPI.textureManager->CreateTexture2D(path);
			delete[] path;
		}

		if(*transp_tex_path)
		{
			path = CharToWideChar(transp_tex_path);
			_transpTexture = engineAPI.textureManager->CreateTexture2D(path);
			delete[] path;
		}

		UpdateFlags();

		return true;
	}

	bool Material::Save(const tchar* file_name) const
	{
		SmartPtr<FileUtil::File> file = engineAPI.fileSystem->Open(file_name, _t("wt"));
		if(!file)
		{
			Console::PrintError("Failed to write material file: %ls", file_name);
			return false;
		}

		const char* str = "";
		const tchar* tstr = _t("");

		file->Printf("// Daemonium engine material file\n\n");
		file->Printf("material\n{\n");

		tstr = _emissionTexture? _emissionTexture.GetRes()->GetFileName(): _t("");
		if(!tstr)
			tstr = _t("");
		file->Printf("\temissionTexture\t\t\"%ls\"\n", tstr);

		file->Printf("\tdiffuseColor\t\t[%f %f %f]\n", _diffuseColor.r, _diffuseColor.g, _diffuseColor.b);
		tstr = _diffuseTexture? _diffuseTexture.GetRes()->GetFileName(): _t("");
		if(!tstr)
			tstr = _t("");
		file->Printf("\tdiffuseTexture\t\t\"%ls\"\n", tstr);

		tstr = _normalMap? _normalMap.GetRes()->GetFileName(): _t("");
		if(!tstr)
			tstr = _t("");
		file->Printf("\tnormalMap\t\t\"%ls\"\n", tstr);

		file->Printf("\tuseTransparency\t\t%s\n", _useTransparency? "True": "False");
		file->Printf("\tglobalTransparency\t\t%f\n", _globalOpacity);
		tstr = _transpTexture? _transpTexture.GetRes()->GetFileName(): _t("");
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
			_emissionTexture = Texture2DResPtr::null;
		}

		if(_diffuseTexture)
		{
			engineAPI.textureManager->ReleaseTexture(_diffuseTexture);
			_diffuseTexture = Texture2DResPtr::null;
		}

		if(_normalMap)
		{
			engineAPI.textureManager->ReleaseTexture(_normalMap);
			_normalMap = Texture2DResPtr::null;
		}

		if(_transpTexture)
		{
			engineAPI.textureManager->ReleaseTexture(_transpTexture);
			_transpTexture = Texture2DResPtr::null;
		}
	}

	void Material::SetEmissionTexture(Texture2DResPtr val)
	{
		engineAPI.textureManager->ReleaseTexture(_emissionTexture);
		_emissionTexture = val;
		UpdateFlags();
	}

	void Material::SetDiffuseTexture(Texture2DResPtr val)
	{
		engineAPI.textureManager->ReleaseTexture(_diffuseTexture);
		_diffuseTexture = val;
		UpdateFlags();
	}

	void Material::SetNormalMap(Texture2DResPtr val)
	{
		engineAPI.textureManager->ReleaseTexture(_normalMap);
		_normalMap = val;
		UpdateFlags();
	}

	void Material::SetTransparencyTexture(Texture2DResPtr val)
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
