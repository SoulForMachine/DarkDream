#pragma once

namespace EditorCommon
{

	public enum class NotifyMessage
	{
		ModelChanged,
		AttachmentChanged,
		AnimationChanged,
		MaterialChanged,
		PatchAdded,
		PatchRemoved,
		CameraMoved,
	};

	public interface class FormDirector
	{
	public:
		void FormNotify(System::Windows::Forms::Form^ form, NotifyMessage msg);
	};

}
