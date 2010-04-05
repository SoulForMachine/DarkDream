#pragma once

namespace EditorCommon
{

	public enum class NotifyMessage
	{
		ModelChanged,
		AttachmentChanged,
		AnimationChanged,
		MaterialChanged,
		ClassPropertyChanged,
		PropertyChanged,
		PatchAdded,
		PatchRemoved,
		CameraMoved,
		Modified
	};

	public interface class FormDirector
	{
	public:
		void FormNotify(System::Windows::Forms::Form^ form, NotifyMessage msg, Object^ param);
	};

}
