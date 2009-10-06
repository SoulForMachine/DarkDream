#pragma once

namespace EditorCommon
{

	public enum class NotifyMessage
	{
		ModelChanged,
		AttachmentChanged,
		AnimationChanged,
	};

	public interface class FormDirector
	{
	public:
		void FormNotify(System::Windows::Forms::Form^ form, NotifyMessage msg);
	};

}
