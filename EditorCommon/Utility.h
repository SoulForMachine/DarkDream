
#pragma once

#pragma make_public(Engine::EngineAPI)


namespace EditorCommon
{

	public ref class EditorUtil
	{
	public:
		static void Init(Engine::EngineAPI* api);

		static tchar* GetRelativePath(System::String^ full_path);
		static bool IsInGameBaseDir(System::String^ file_name);

	private:
		static Engine::EngineAPI* _engineAPI;
	};

}
