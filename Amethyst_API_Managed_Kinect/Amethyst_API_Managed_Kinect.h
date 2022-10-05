#pragma once
#include <Amethyst_API_Devices.h>
#include <array>
#include <map>

inline ktvr::K2TrackingDeviceBase_SkeletonBasis* _this_Device;

// Typedefs to save line space
using _TextBlock = Managed_SkeletonBasis_Handler::AmethystDevice::TextBlock;
using _Button = Managed_SkeletonBasis_Handler::AmethystDevice::Button;
using _NumberBox = Managed_SkeletonBasis_Handler::AmethystDevice::NumberBox;
using _ComboBox = Managed_SkeletonBasis_Handler::AmethystDevice::ComboBox;
using _CheckBox = Managed_SkeletonBasis_Handler::AmethystDevice::CheckBox;
using _ToggleSwitch = Managed_SkeletonBasis_Handler::AmethystDevice::ToggleSwitch;
using _TextBox = Managed_SkeletonBasis_Handler::AmethystDevice::TextBox;
using _ProgressRing = Managed_SkeletonBasis_Handler::AmethystDevice::ProgressRing;
using _ProgressBar = Managed_SkeletonBasis_Handler::AmethystDevice::ProgressBar;

// Native functions
inline std::function<void(std::wstring, unsigned)> LogToAme_Handler;

inline std::function<std::pair<Eigen::Vector3d, Eigen::Quaterniond>()> GetHMDPose_Handler;
inline std::function<std::pair<Eigen::Vector3d, Eigen::Quaterniond>()> GetHMDPoseCalibrated_Handler;
inline std::function<std::pair<Eigen::Vector3d, Eigen::Quaterniond>()> GetLeftControllerPose_Handler;
inline std::function<std::pair<Eigen::Vector3d, Eigen::Quaterniond>()> GetLeftControllerPoseCalibrated_Handler;
inline std::function<std::pair<Eigen::Vector3d, Eigen::Quaterniond>()> GetRightControllerPose_Handler;
inline std::function<std::pair<Eigen::Vector3d, Eigen::Quaterniond>()> GetRightControllerPoseCalibrated_Handler;
inline std::function<double()> GetHMDOrientationYaw_Handler;
inline std::function<double()> GetHMDOrientationYawCalibrated_Handler;
inline std::function<std::vector<ktvr::K2TrackedJoint>()> GetAppJointPoses_Handler;

inline void MarshalString(System::String^ s, std::wstring& os)
{
	const auto chars = static_cast<const wchar_t*>(
		System::Runtime::InteropServices::Marshal::StringToHGlobalUni(s).ToPointer());

	os = chars;
	System::Runtime::InteropServices::Marshal::FreeHGlobal(System::IntPtr((void*)chars));
}

inline std::wstring GUIDString(System::Guid _guid)
{
	std::wstring out; // Dummy placeholder
	MarshalString(_guid.ToString(), out);
	return out;
}

inline System::Guid StringGUID(std::wstring _guid)
{
	return System::Guid::Parse(gcnew System::String(_guid.c_str()));
}


namespace Interface
{
	// Container of our UI elements / GUIDs
	inline std::map<std::wstring, std::shared_ptr<ktvr::Interface::TextBlock>> _TextBlock_Map;
	inline std::map<std::wstring, std::shared_ptr<ktvr::Interface::Button>> _Button_Map;
	inline std::map<std::wstring, std::shared_ptr<ktvr::Interface::NumberBox>> _NumberBox_Map;
	inline std::map<std::wstring, std::shared_ptr<ktvr::Interface::ComboBox>> _ComboBox_Map;
	inline std::map<std::wstring, std::shared_ptr<ktvr::Interface::CheckBox>> _CheckBox_Map;
	inline std::map<std::wstring, std::shared_ptr<ktvr::Interface::ToggleSwitch>> _ToggleSwitch_Map;
	inline std::map<std::wstring, std::shared_ptr<ktvr::Interface::TextBox>> _TextBox_Map;
	inline std::map<std::wstring, std::shared_ptr<ktvr::Interface::ProgressRing>> _ProgressRing_Map;
	inline std::map<std::wstring, std::shared_ptr<ktvr::Interface::ProgressBar>> _ProgressBar_Map;

	// Creation helpers
	inline System::Guid CreateTextBlock(System::String^ text)
	{
		const auto _m_guid = System::Guid::NewGuid();
		std::wstring out; // Dummy placeholder
		MarshalString(text, out);

		_TextBlock_Map.insert_or_assign(
			GUIDString(_m_guid),
			std::shared_ptr<ktvr::Interface::TextBlock>(
				_this_Device->CreateTextBlock(out)));

		return _m_guid;
	}

	inline System::Guid CreateButton(System::String^ content)
	{
		const auto _m_guid = System::Guid::NewGuid();
		const auto _m_guid_str = GUIDString(_m_guid);

		std::wstring out; // Dummy placeholder
		MarshalString(content, out);

		_Button_Map.insert_or_assign(_m_guid_str,
		                             std::shared_ptr<ktvr::Interface::Button>(
			                             _this_Device->CreateButton(out)));

		_Button_Map[GUIDString(_m_guid)]->OnClick =
			[_m_guid_str](auto)
			{
				Managed_SkeletonBasis_Handler::AmethystDevice::ExecuteButtonHandler(
					StringGUID(_m_guid_str));
			};

		return _m_guid;
	}

	inline System::Guid CreateNumberBox(const int value)
	{
		const auto _m_guid = System::Guid::NewGuid();
		const auto _m_guid_str = GUIDString(_m_guid);

		_NumberBox_Map.insert_or_assign(_m_guid_str,
		                                std::shared_ptr<ktvr::Interface::NumberBox>(
			                                _this_Device->CreateNumberBox(value)));

		_NumberBox_Map[GUIDString(_m_guid)]->OnValueChanged =
			[_m_guid_str](auto, auto val)
			{
				Managed_SkeletonBasis_Handler::AmethystDevice::ExecuteNumberBoxHandler(
					StringGUID(_m_guid_str), val);
			};

		return _m_guid;
	}

	inline System::Guid CreateComboBox(System::Collections::Generic::List<System::String^>^ items)
	{
		const auto _m_guid = System::Guid::NewGuid();
		const auto _m_guid_str = GUIDString(_m_guid);

		std::vector<std::wstring> out;
		for each (const auto& item in items)
		{
			std::wstring _out; // Dummy placeholder
			MarshalString(item, _out);
			out.push_back(_out); // Append
		}

		_ComboBox_Map.insert_or_assign(_m_guid_str,
		                               std::shared_ptr<ktvr::Interface::ComboBox>(
			                               _this_Device->CreateComboBox(out)));

		_ComboBox_Map[GUIDString(_m_guid)]->OnSelectionChanged =
			[_m_guid_str](auto, auto val)
			{
				Managed_SkeletonBasis_Handler::AmethystDevice::ExecuteComboBoxHandler(
					StringGUID(_m_guid_str), val);
			};

		return _m_guid;
	}

	inline System::Guid CreateCheckBox()
	{
		const auto _m_guid = System::Guid::NewGuid();
		const auto _m_guid_str = GUIDString(_m_guid);

		_CheckBox_Map.insert_or_assign(_m_guid_str,
		                               std::shared_ptr<ktvr::Interface::CheckBox>(
			                               _this_Device->CreateCheckBox()));

		_CheckBox_Map[GUIDString(_m_guid)]->OnChecked =
			[_m_guid_str](auto)
			{
				Managed_SkeletonBasis_Handler::AmethystDevice::ExecuteCheckBoxHandler(
					StringGUID(_m_guid_str), true);
			};

		_CheckBox_Map[GUIDString(_m_guid)]->OnUnchecked =
			[_m_guid_str](auto)
			{
				Managed_SkeletonBasis_Handler::AmethystDevice::ExecuteCheckBoxHandler(
					StringGUID(_m_guid_str), false);
			};

		return _m_guid;
	}

	inline System::Guid CreateToggleSwitch()
	{
		const auto _m_guid = System::Guid::NewGuid();
		const auto _m_guid_str = GUIDString(_m_guid);

		_ToggleSwitch_Map.insert_or_assign(_m_guid_str,
		                                   std::shared_ptr<ktvr::Interface::ToggleSwitch>(
			                                   _this_Device->CreateToggleSwitch()));

		_ToggleSwitch_Map[GUIDString(_m_guid)]->OnChecked =
			[_m_guid_str](auto)
			{
				Managed_SkeletonBasis_Handler::AmethystDevice::ExecuteCheckBoxHandler(
					StringGUID(_m_guid_str), true);
			};

		_ToggleSwitch_Map[GUIDString(_m_guid)]->OnUnchecked =
			[_m_guid_str](auto)
			{
				Managed_SkeletonBasis_Handler::AmethystDevice::ExecuteCheckBoxHandler(
					StringGUID(_m_guid_str), false);
			};

		return _m_guid;
	}

	inline System::Guid CreateTextBox()
	{
		const auto _m_guid = System::Guid::NewGuid();
		const auto _m_guid_str = GUIDString(_m_guid);

		_TextBox_Map.insert_or_assign(_m_guid_str,
		                              std::shared_ptr<ktvr::Interface::TextBox>(
			                              _this_Device->CreateTextBox()));

		_TextBox_Map[GUIDString(_m_guid)]->OnEnterKeyDown =
			[_m_guid_str](ktvr::Interface::TextBox* element)
			{
				Managed_SkeletonBasis_Handler::AmethystDevice::ExecuteTextBoxHandler(
					StringGUID(_m_guid_str),
					gcnew System::String(element->Text().c_str()));
			};

		return _m_guid;
	}

	inline System::Guid CreateProgressRing()
	{
		const auto _m_guid = System::Guid::NewGuid();
		_ProgressRing_Map.insert_or_assign(
			GUIDString(_m_guid),
			std::shared_ptr<ktvr::Interface::ProgressRing>(
				_this_Device->CreateProgressRing()));

		return _m_guid;
	}

	inline System::Guid CreateProgressBar()
	{
		const auto _m_guid = System::Guid::NewGuid();
		_ProgressBar_Map.insert_or_assign(
			GUIDString(_m_guid),
			std::shared_ptr<ktvr::Interface::ProgressBar>(
				_this_Device->CreateProgressBar()));

		return _m_guid;
	}

	// Addition helpers
	inline void AppendSingleElement(System::Object^ element, int alignment)
	{
		if (element->GetType() == Managed_SkeletonBasis_Handler::AmethystDevice::TextBlock::typeid &&
			_TextBlock_Map.find(GUIDString(safe_cast<_TextBlock^>(element)->_guid)) != _TextBlock_Map.end())
			_this_Device->layoutRoot->AppendSingleElement(
				_TextBlock_Map[GUIDString(safe_cast<_TextBlock^>(element)->_guid)].get(),
				static_cast<ktvr::Interface::SingleLayoutHorizontalAlignment>(alignment));

		else if (element->GetType() == Managed_SkeletonBasis_Handler::AmethystDevice::Button::typeid &&
			_Button_Map.find(GUIDString(safe_cast<_Button^>(element)->_guid)) != _Button_Map.end())
			_this_Device->layoutRoot->AppendSingleElement(
				_Button_Map[GUIDString(safe_cast<_Button^>(element)->_guid)].get(),
				static_cast<ktvr::Interface::SingleLayoutHorizontalAlignment>(alignment));

		else if (element->GetType() == Managed_SkeletonBasis_Handler::AmethystDevice::NumberBox::typeid &&
			_NumberBox_Map.find(GUIDString(safe_cast<_NumberBox^>(element)->_guid)) != _NumberBox_Map.end())
			_this_Device->layoutRoot->AppendSingleElement(
				_NumberBox_Map[GUIDString(safe_cast<_NumberBox^>(element)->_guid)].get(),
				static_cast<ktvr::Interface::SingleLayoutHorizontalAlignment>(alignment));

		else if (element->GetType() == Managed_SkeletonBasis_Handler::AmethystDevice::ComboBox::typeid &&
			_ComboBox_Map.find(GUIDString(safe_cast<_ComboBox^>(element)->_guid)) != _ComboBox_Map.end())
			_this_Device->layoutRoot->AppendSingleElement(
				_ComboBox_Map[GUIDString(safe_cast<_ComboBox^>(element)->_guid)].get(),
				static_cast<ktvr::Interface::SingleLayoutHorizontalAlignment>(alignment));

		else if (element->GetType() == Managed_SkeletonBasis_Handler::AmethystDevice::CheckBox::typeid &&
			_CheckBox_Map.find(GUIDString(safe_cast<_CheckBox^>(element)->_guid)) != _CheckBox_Map.end())
			_this_Device->layoutRoot->AppendSingleElement(
				_CheckBox_Map[GUIDString(safe_cast<_CheckBox^>(element)->_guid)].get(),
				static_cast<ktvr::Interface::SingleLayoutHorizontalAlignment>(alignment));

		else if (element->GetType() == Managed_SkeletonBasis_Handler::AmethystDevice::ToggleSwitch::typeid &&
			_ToggleSwitch_Map.find(GUIDString(safe_cast<_ToggleSwitch^>(element)->_guid)) != _ToggleSwitch_Map.end())
			_this_Device->layoutRoot->AppendSingleElement(
				_ToggleSwitch_Map[GUIDString(safe_cast<_ToggleSwitch^>(element)->_guid)].get(),
				static_cast<ktvr::Interface::SingleLayoutHorizontalAlignment>(alignment));

		else if (element->GetType() == Managed_SkeletonBasis_Handler::AmethystDevice::TextBox::typeid &&
			_TextBox_Map.find(GUIDString(safe_cast<_TextBox^>(element)->_guid)) != _TextBox_Map.end())
			_this_Device->layoutRoot->AppendSingleElement(
				_TextBox_Map[GUIDString(safe_cast<_TextBox^>(element)->_guid)].get(),
				static_cast<ktvr::Interface::SingleLayoutHorizontalAlignment>(alignment));

		else if (element->GetType() == Managed_SkeletonBasis_Handler::AmethystDevice::ProgressRing::typeid &&
			_ProgressRing_Map.find(GUIDString(safe_cast<_ProgressRing^>(element)->_guid)) != _ProgressRing_Map.end())
			_this_Device->layoutRoot->AppendSingleElement(
				_ProgressRing_Map[GUIDString(safe_cast<_ProgressRing^>(element)->_guid)].get(),
				static_cast<ktvr::Interface::SingleLayoutHorizontalAlignment>(alignment));

		else if (element->GetType() == Managed_SkeletonBasis_Handler::AmethystDevice::ProgressBar::typeid &&
			_ProgressBar_Map.find(GUIDString(safe_cast<_ProgressBar^>(element)->_guid)) != _ProgressBar_Map.end())
			_this_Device->layoutRoot->AppendSingleElement(
				_ProgressBar_Map[GUIDString(safe_cast<_ProgressBar^>(element)->_guid)].get(),
				static_cast<ktvr::Interface::SingleLayoutHorizontalAlignment>(alignment));
	}

	inline void AppendElementPair(System::Object^ element_1, System::Object^ element_2)
	{
		ktvr::Interface::Element _e1, _e2;

		// Element_1
		{
			if (element_1->GetType() == Managed_SkeletonBasis_Handler::AmethystDevice::TextBlock::typeid &&
				_TextBlock_Map.find(GUIDString(safe_cast<_TextBlock^>(element_1)->_guid)) != _TextBlock_Map.end())
				_e1 = _TextBlock_Map[GUIDString(safe_cast<_TextBlock^>(element_1)->_guid)].get();

			else if (element_1->GetType() == Managed_SkeletonBasis_Handler::AmethystDevice::Button::typeid &&
				_Button_Map.find(GUIDString(safe_cast<_Button^>(element_1)->_guid)) != _Button_Map.end())
				_e1 = _Button_Map[GUIDString(safe_cast<_Button^>(element_1)->_guid)].get();

			else if (element_1->GetType() == Managed_SkeletonBasis_Handler::AmethystDevice::NumberBox::typeid &&
				_NumberBox_Map.find(GUIDString(safe_cast<_NumberBox^>(element_1)->_guid)) != _NumberBox_Map.end())
				_e1 = _NumberBox_Map[GUIDString(safe_cast<_NumberBox^>(element_1)->_guid)].get();

			else if (element_1->GetType() == Managed_SkeletonBasis_Handler::AmethystDevice::ComboBox::typeid &&
				_ComboBox_Map.find(GUIDString(safe_cast<_ComboBox^>(element_1)->_guid)) != _ComboBox_Map.end())
				_e1 = _ComboBox_Map[GUIDString(safe_cast<_ComboBox^>(element_1)->_guid)].get();

			else if (element_1->GetType() == Managed_SkeletonBasis_Handler::AmethystDevice::CheckBox::typeid &&
				_CheckBox_Map.find(GUIDString(safe_cast<_CheckBox^>(element_1)->_guid)) != _CheckBox_Map.end())
				_e1 = _CheckBox_Map[GUIDString(safe_cast<_CheckBox^>(element_1)->_guid)].get();

			else if (element_1->GetType() == Managed_SkeletonBasis_Handler::AmethystDevice::ToggleSwitch::typeid &&
				_ToggleSwitch_Map.find(GUIDString(safe_cast<_ToggleSwitch^>(element_1)->_guid)) != _ToggleSwitch_Map.
				end())
				_e1 = _ToggleSwitch_Map[GUIDString(safe_cast<_ToggleSwitch^>(element_1)->_guid)].get();

			else if (element_1->GetType() == Managed_SkeletonBasis_Handler::AmethystDevice::TextBox::typeid &&
				_TextBox_Map.find(GUIDString(safe_cast<_TextBox^>(element_1)->_guid)) != _TextBox_Map.end())
				_e1 = _TextBox_Map[GUIDString(safe_cast<_TextBox^>(element_1)->_guid)].get();

			else if (element_1->GetType() == Managed_SkeletonBasis_Handler::AmethystDevice::ProgressRing::typeid &&
				_ProgressRing_Map.find(GUIDString(safe_cast<_ProgressRing^>(element_1)->_guid)) != _ProgressRing_Map.
				end())
				_e1 = _ProgressRing_Map[GUIDString(safe_cast<_ProgressRing^>(element_1)->_guid)].get();

			else if (element_1->GetType() == Managed_SkeletonBasis_Handler::AmethystDevice::ProgressBar::typeid &&
				_ProgressBar_Map.find(GUIDString(safe_cast<_ProgressBar^>(element_1)->_guid)) != _ProgressBar_Map.end())
				_e1 = _ProgressBar_Map[GUIDString(safe_cast<_ProgressBar^>(element_1)->_guid)].get();
		}

		// Element_2
		{
			if (element_2->GetType() == Managed_SkeletonBasis_Handler::AmethystDevice::TextBlock::typeid &&
				_TextBlock_Map.find(GUIDString(safe_cast<_TextBlock^>(element_2)->_guid)) != _TextBlock_Map.end())
				_e2 = _TextBlock_Map[GUIDString(safe_cast<_TextBlock^>(element_2)->_guid)].get();

			else if (element_2->GetType() == Managed_SkeletonBasis_Handler::AmethystDevice::Button::typeid &&
				_Button_Map.find(GUIDString(safe_cast<_Button^>(element_2)->_guid)) != _Button_Map.end())
				_e2 = _Button_Map[GUIDString(safe_cast<_Button^>(element_2)->_guid)].get();

			else if (element_2->GetType() == Managed_SkeletonBasis_Handler::AmethystDevice::NumberBox::typeid &&
				_NumberBox_Map.find(GUIDString(safe_cast<_NumberBox^>(element_2)->_guid)) != _NumberBox_Map.end())
				_e2 = _NumberBox_Map[GUIDString(safe_cast<_NumberBox^>(element_2)->_guid)].get();

			else if (element_2->GetType() == Managed_SkeletonBasis_Handler::AmethystDevice::ComboBox::typeid &&
				_ComboBox_Map.find(GUIDString(safe_cast<_ComboBox^>(element_2)->_guid)) != _ComboBox_Map.end())
				_e2 = _ComboBox_Map[GUIDString(safe_cast<_ComboBox^>(element_2)->_guid)].get();

			else if (element_2->GetType() == Managed_SkeletonBasis_Handler::AmethystDevice::CheckBox::typeid &&
				_CheckBox_Map.find(GUIDString(safe_cast<_CheckBox^>(element_2)->_guid)) != _CheckBox_Map.end())
				_e2 = _CheckBox_Map[GUIDString(safe_cast<_CheckBox^>(element_2)->_guid)].get();

			else if (element_2->GetType() == Managed_SkeletonBasis_Handler::AmethystDevice::ToggleSwitch::typeid &&
				_ToggleSwitch_Map.find(GUIDString(safe_cast<_ToggleSwitch^>(element_2)->_guid)) != _ToggleSwitch_Map.
				end())
				_e2 = _ToggleSwitch_Map[GUIDString(safe_cast<_ToggleSwitch^>(element_2)->_guid)].get();

			else if (element_2->GetType() == Managed_SkeletonBasis_Handler::AmethystDevice::TextBox::typeid &&
				_TextBox_Map.find(GUIDString(safe_cast<_TextBox^>(element_2)->_guid)) != _TextBox_Map.end())
				_e2 = _TextBox_Map[GUIDString(safe_cast<_TextBox^>(element_2)->_guid)].get();

			else if (element_2->GetType() == Managed_SkeletonBasis_Handler::AmethystDevice::ProgressRing::typeid &&
				_ProgressRing_Map.find(GUIDString(safe_cast<_ProgressRing^>(element_2)->_guid)) != _ProgressRing_Map.
				end())
				_e2 = _ProgressRing_Map[GUIDString(safe_cast<_ProgressRing^>(element_2)->_guid)].get();

			else if (element_2->GetType() == Managed_SkeletonBasis_Handler::AmethystDevice::ProgressBar::typeid &&
				_ProgressBar_Map.find(GUIDString(safe_cast<_ProgressBar^>(element_2)->_guid)) != _ProgressBar_Map.end())
				_e2 = _ProgressBar_Map[GUIDString(safe_cast<_ProgressBar^>(element_2)->_guid)].get();
		}

		// Append
		_this_Device->layoutRoot->AppendElementPair(_e1, _e2);
	}

	inline void AppendElementPairStack(System::Object^ element_1, System::Object^ element_2)
	{
		ktvr::Interface::Element _e1, _e2;

		// Element_1
		{
			if (element_1->GetType() == Managed_SkeletonBasis_Handler::AmethystDevice::TextBlock::typeid &&
				_TextBlock_Map.find(GUIDString(safe_cast<_TextBlock^>(element_1)->_guid)) != _TextBlock_Map.end())
				_e1 = _TextBlock_Map[GUIDString(safe_cast<_TextBlock^>(element_1)->_guid)].get();

			else if (element_1->GetType() == Managed_SkeletonBasis_Handler::AmethystDevice::Button::typeid &&
				_Button_Map.find(GUIDString(safe_cast<_Button^>(element_1)->_guid)) != _Button_Map.end())
				_e1 = _Button_Map[GUIDString(safe_cast<_Button^>(element_1)->_guid)].get();

			else if (element_1->GetType() == Managed_SkeletonBasis_Handler::AmethystDevice::NumberBox::typeid &&
				_NumberBox_Map.find(GUIDString(safe_cast<_NumberBox^>(element_1)->_guid)) != _NumberBox_Map.end())
				_e1 = _NumberBox_Map[GUIDString(safe_cast<_NumberBox^>(element_1)->_guid)].get();

			else if (element_1->GetType() == Managed_SkeletonBasis_Handler::AmethystDevice::ComboBox::typeid &&
				_ComboBox_Map.find(GUIDString(safe_cast<_ComboBox^>(element_1)->_guid)) != _ComboBox_Map.end())
				_e1 = _ComboBox_Map[GUIDString(safe_cast<_ComboBox^>(element_1)->_guid)].get();

			else if (element_1->GetType() == Managed_SkeletonBasis_Handler::AmethystDevice::CheckBox::typeid &&
				_CheckBox_Map.find(GUIDString(safe_cast<_CheckBox^>(element_1)->_guid)) != _CheckBox_Map.end())
				_e1 = _CheckBox_Map[GUIDString(safe_cast<_CheckBox^>(element_1)->_guid)].get();

			else if (element_1->GetType() == Managed_SkeletonBasis_Handler::AmethystDevice::ToggleSwitch::typeid &&
				_ToggleSwitch_Map.find(GUIDString(safe_cast<_ToggleSwitch^>(element_1)->_guid)) != _ToggleSwitch_Map.
				end())
				_e1 = _ToggleSwitch_Map[GUIDString(safe_cast<_ToggleSwitch^>(element_1)->_guid)].get();

			else if (element_1->GetType() == Managed_SkeletonBasis_Handler::AmethystDevice::TextBox::typeid &&
				_TextBox_Map.find(GUIDString(safe_cast<_TextBox^>(element_1)->_guid)) != _TextBox_Map.end())
				_e1 = _TextBox_Map[GUIDString(safe_cast<_TextBox^>(element_1)->_guid)].get();

			else if (element_1->GetType() == Managed_SkeletonBasis_Handler::AmethystDevice::ProgressRing::typeid &&
				_ProgressRing_Map.find(GUIDString(safe_cast<_ProgressRing^>(element_1)->_guid)) != _ProgressRing_Map.
				end())
				_e1 = _ProgressRing_Map[GUIDString(safe_cast<_ProgressRing^>(element_1)->_guid)].get();

			else if (element_1->GetType() == Managed_SkeletonBasis_Handler::AmethystDevice::ProgressBar::typeid &&
				_ProgressBar_Map.find(GUIDString(safe_cast<_ProgressBar^>(element_1)->_guid)) != _ProgressBar_Map.end())
				_e1 = _ProgressBar_Map[GUIDString(safe_cast<_ProgressBar^>(element_1)->_guid)].get();
		}

		// Element_2
		{
			if (element_2->GetType() == Managed_SkeletonBasis_Handler::AmethystDevice::TextBlock::typeid &&
				_TextBlock_Map.find(GUIDString(safe_cast<_TextBlock^>(element_2)->_guid)) != _TextBlock_Map.end())
				_e2 = _TextBlock_Map[GUIDString(safe_cast<_TextBlock^>(element_2)->_guid)].get();

			else if (element_2->GetType() == Managed_SkeletonBasis_Handler::AmethystDevice::Button::typeid &&
				_Button_Map.find(GUIDString(safe_cast<_Button^>(element_2)->_guid)) != _Button_Map.end())
				_e2 = _Button_Map[GUIDString(safe_cast<_Button^>(element_2)->_guid)].get();

			else if (element_2->GetType() == Managed_SkeletonBasis_Handler::AmethystDevice::NumberBox::typeid &&
				_NumberBox_Map.find(GUIDString(safe_cast<_NumberBox^>(element_2)->_guid)) != _NumberBox_Map.end())
				_e2 = _NumberBox_Map[GUIDString(safe_cast<_NumberBox^>(element_2)->_guid)].get();

			else if (element_2->GetType() == Managed_SkeletonBasis_Handler::AmethystDevice::ComboBox::typeid &&
				_ComboBox_Map.find(GUIDString(safe_cast<_ComboBox^>(element_2)->_guid)) != _ComboBox_Map.end())
				_e2 = _ComboBox_Map[GUIDString(safe_cast<_ComboBox^>(element_2)->_guid)].get();

			else if (element_2->GetType() == Managed_SkeletonBasis_Handler::AmethystDevice::CheckBox::typeid &&
				_CheckBox_Map.find(GUIDString(safe_cast<_CheckBox^>(element_2)->_guid)) != _CheckBox_Map.end())
				_e2 = _CheckBox_Map[GUIDString(safe_cast<_CheckBox^>(element_2)->_guid)].get();

			else if (element_2->GetType() == Managed_SkeletonBasis_Handler::AmethystDevice::ToggleSwitch::typeid &&
				_ToggleSwitch_Map.find(GUIDString(safe_cast<_ToggleSwitch^>(element_2)->_guid)) != _ToggleSwitch_Map.
				end())
				_e2 = _ToggleSwitch_Map[GUIDString(safe_cast<_ToggleSwitch^>(element_2)->_guid)].get();

			else if (element_2->GetType() == Managed_SkeletonBasis_Handler::AmethystDevice::TextBox::typeid &&
				_TextBox_Map.find(GUIDString(safe_cast<_TextBox^>(element_2)->_guid)) != _TextBox_Map.end())
				_e2 = _TextBox_Map[GUIDString(safe_cast<_TextBox^>(element_2)->_guid)].get();

			else if (element_2->GetType() == Managed_SkeletonBasis_Handler::AmethystDevice::ProgressRing::typeid &&
				_ProgressRing_Map.find(GUIDString(safe_cast<_ProgressRing^>(element_2)->_guid)) != _ProgressRing_Map.
				end())
				_e2 = _ProgressRing_Map[GUIDString(safe_cast<_ProgressRing^>(element_2)->_guid)].get();

			else if (element_2->GetType() == Managed_SkeletonBasis_Handler::AmethystDevice::ProgressBar::typeid &&
				_ProgressBar_Map.find(GUIDString(safe_cast<_ProgressBar^>(element_2)->_guid)) != _ProgressBar_Map.end())
				_e2 = _ProgressBar_Map[GUIDString(safe_cast<_ProgressBar^>(element_2)->_guid)].get();
		}

		// Append
		_this_Device->layoutRoot->AppendElementPairStack(_e1, _e2);
	}

	inline void AppendElementVector(System::Collections::Generic::List<System::Object^>^ elements)
	{
		std::vector<ktvr::Interface::Element> _e;

		// Loop over and cache
		for each (const auto& element in elements)
		{
			if (element->GetType() == Managed_SkeletonBasis_Handler::AmethystDevice::TextBlock::typeid &&
				_TextBlock_Map.find(GUIDString(safe_cast<_TextBlock^>(element)->_guid)) != _TextBlock_Map.end())
				_e.push_back(_TextBlock_Map[GUIDString(safe_cast<_TextBlock^>(element)->_guid)].get());

			else if (element->GetType() == Managed_SkeletonBasis_Handler::AmethystDevice::Button::typeid &&
				_Button_Map.find(GUIDString(safe_cast<_Button^>(element)->_guid)) != _Button_Map.end())
				_e.push_back(_Button_Map[GUIDString(safe_cast<_Button^>(element)->_guid)].get());

			else if (element->GetType() == Managed_SkeletonBasis_Handler::AmethystDevice::NumberBox::typeid &&
				_NumberBox_Map.find(GUIDString(safe_cast<_NumberBox^>(element)->_guid)) != _NumberBox_Map.end())
				_e.push_back(_NumberBox_Map[GUIDString(safe_cast<_NumberBox^>(element)->_guid)].get());

			else if (element->GetType() == Managed_SkeletonBasis_Handler::AmethystDevice::ComboBox::typeid &&
				_ComboBox_Map.find(GUIDString(safe_cast<_ComboBox^>(element)->_guid)) != _ComboBox_Map.end())
				_e.push_back(_ComboBox_Map[GUIDString(safe_cast<_ComboBox^>(element)->_guid)].get());

			else if (element->GetType() == Managed_SkeletonBasis_Handler::AmethystDevice::CheckBox::typeid &&
				_CheckBox_Map.find(GUIDString(safe_cast<_CheckBox^>(element)->_guid)) != _CheckBox_Map.end())
				_e.push_back(_CheckBox_Map[GUIDString(safe_cast<_CheckBox^>(element)->_guid)].get());

			else if (element->GetType() == Managed_SkeletonBasis_Handler::AmethystDevice::ToggleSwitch::typeid &&
				_ToggleSwitch_Map.find(GUIDString(safe_cast<_ToggleSwitch^>(element)->_guid)) != _ToggleSwitch_Map.
				end())
				_e.push_back(_ToggleSwitch_Map[GUIDString(safe_cast<_ToggleSwitch^>(element)->_guid)].get());

			else if (element->GetType() == Managed_SkeletonBasis_Handler::AmethystDevice::TextBox::typeid &&
				_TextBox_Map.find(GUIDString(safe_cast<_TextBox^>(element)->_guid)) != _TextBox_Map.end())
				_e.push_back(_TextBox_Map[GUIDString(safe_cast<_TextBox^>(element)->_guid)].get());

			else if (element->GetType() == Managed_SkeletonBasis_Handler::AmethystDevice::ProgressRing::typeid &&
				_ProgressRing_Map.find(GUIDString(safe_cast<_ProgressRing^>(element)->_guid)) != _ProgressRing_Map.
				end())
				_e.push_back(_ProgressRing_Map[GUIDString(safe_cast<_ProgressRing^>(element)->_guid)].get());

			else if (element->GetType() == Managed_SkeletonBasis_Handler::AmethystDevice::ProgressBar::typeid &&
				_ProgressBar_Map.find(GUIDString(safe_cast<_ProgressBar^>(element)->_guid)) != _ProgressBar_Map.end())
				_e.push_back(_ProgressBar_Map[GUIDString(safe_cast<_ProgressBar^>(element)->_guid)].get());
		}

		// Append
		_this_Device->layoutRoot->AppendElementVector(_e);
	}

	inline void AppendElementVectorStack(System::Collections::Generic::List<System::Object^>^ elements)
	{
		std::vector<ktvr::Interface::Element> _e;

		// Loop over and cache
		for each (const auto& element in elements)
		{
			if (element->GetType() == Managed_SkeletonBasis_Handler::AmethystDevice::TextBlock::typeid &&
				_TextBlock_Map.find(GUIDString(safe_cast<_TextBlock^>(element)->_guid)) != _TextBlock_Map.end())
				_e.push_back(_TextBlock_Map[GUIDString(safe_cast<_TextBlock^>(element)->_guid)].get());

			else if (element->GetType() == Managed_SkeletonBasis_Handler::AmethystDevice::Button::typeid &&
				_Button_Map.find(GUIDString(safe_cast<_Button^>(element)->_guid)) != _Button_Map.end())
				_e.push_back(_Button_Map[GUIDString(safe_cast<_Button^>(element)->_guid)].get());

			else if (element->GetType() == Managed_SkeletonBasis_Handler::AmethystDevice::NumberBox::typeid &&
				_NumberBox_Map.find(GUIDString(safe_cast<_NumberBox^>(element)->_guid)) != _NumberBox_Map.end())
				_e.push_back(_NumberBox_Map[GUIDString(safe_cast<_NumberBox^>(element)->_guid)].get());

			else if (element->GetType() == Managed_SkeletonBasis_Handler::AmethystDevice::ComboBox::typeid &&
				_ComboBox_Map.find(GUIDString(safe_cast<_ComboBox^>(element)->_guid)) != _ComboBox_Map.end())
				_e.push_back(_ComboBox_Map[GUIDString(safe_cast<_ComboBox^>(element)->_guid)].get());

			else if (element->GetType() == Managed_SkeletonBasis_Handler::AmethystDevice::CheckBox::typeid &&
				_CheckBox_Map.find(GUIDString(safe_cast<_CheckBox^>(element)->_guid)) != _CheckBox_Map.end())
				_e.push_back(_CheckBox_Map[GUIDString(safe_cast<_CheckBox^>(element)->_guid)].get());

			else if (element->GetType() == Managed_SkeletonBasis_Handler::AmethystDevice::ToggleSwitch::typeid &&
				_ToggleSwitch_Map.find(GUIDString(safe_cast<_ToggleSwitch^>(element)->_guid)) != _ToggleSwitch_Map.
				end())
				_e.push_back(_ToggleSwitch_Map[GUIDString(safe_cast<_ToggleSwitch^>(element)->_guid)].get());

			else if (element->GetType() == Managed_SkeletonBasis_Handler::AmethystDevice::TextBox::typeid &&
				_TextBox_Map.find(GUIDString(safe_cast<_TextBox^>(element)->_guid)) != _TextBox_Map.end())
				_e.push_back(_TextBox_Map[GUIDString(safe_cast<_TextBox^>(element)->_guid)].get());

			else if (element->GetType() == Managed_SkeletonBasis_Handler::AmethystDevice::ProgressRing::typeid &&
				_ProgressRing_Map.find(GUIDString(safe_cast<_ProgressRing^>(element)->_guid)) != _ProgressRing_Map.
				end())
				_e.push_back(_ProgressRing_Map[GUIDString(safe_cast<_ProgressRing^>(element)->_guid)].get());

			else if (element->GetType() == Managed_SkeletonBasis_Handler::AmethystDevice::ProgressBar::typeid &&
				_ProgressBar_Map.find(GUIDString(safe_cast<_ProgressBar^>(element)->_guid)) != _ProgressBar_Map.end())
				_e.push_back(_ProgressBar_Map[GUIDString(safe_cast<_ProgressBar^>(element)->_guid)].get());
		}

		// Append
		_this_Device->layoutRoot->AppendElementVectorStack(_e);
	}

	// Management helpers
	inline void TextBlock_SetVisibility(System::Guid guid, const bool value)
	{
		if (_TextBlock_Map.find(GUIDString(guid)) != _TextBlock_Map.end())
			_TextBlock_Map[GUIDString(guid)]->Visibility(value);
	}

	inline void TextBlock_SetIsPrimary(System::Guid guid, const bool value)
	{
		if (_TextBlock_Map.find(GUIDString(guid)) != _TextBlock_Map.end())
			_TextBlock_Map[GUIDString(guid)]->IsPrimary(value);
	}

	inline void TextBlock_SetWidth(System::Guid guid, const uint32_t value)
	{
		if (_TextBlock_Map.find(GUIDString(guid)) != _TextBlock_Map.end())
			_TextBlock_Map[GUIDString(guid)]->Width(value);
	}

	inline void TextBlock_SetHeight(System::Guid guid, const uint32_t value)
	{
		if (_TextBlock_Map.find(GUIDString(guid)) != _TextBlock_Map.end())
			_TextBlock_Map[GUIDString(guid)]->Height(value);
	}

	inline void TextBlock_SetText(System::Guid guid, System::String^ _value)
	{
		std::wstring value; // Dummy placeholder
		MarshalString(_value, value);

		if (_TextBlock_Map.find(GUIDString(guid)) != _TextBlock_Map.end())
			_TextBlock_Map[GUIDString(guid)]->Text(value);
	}

	inline void Button_SetVisibility(System::Guid guid, const bool value)
	{
		if (_Button_Map.find(GUIDString(guid)) != _Button_Map.end())
			_Button_Map[GUIDString(guid)]->Visibility(value);
	}

	inline void Button_SetWidth(System::Guid guid, const uint32_t value)
	{
		if (_Button_Map.find(GUIDString(guid)) != _Button_Map.end())
			_Button_Map[GUIDString(guid)]->Width(value);
	}

	inline void Button_SetHeight(System::Guid guid, const uint32_t value)
	{
		if (_Button_Map.find(GUIDString(guid)) != _Button_Map.end())
			_Button_Map[GUIDString(guid)]->Height(value);
	}

	inline void Button_SetIsEnabled(System::Guid guid, const bool value)
	{
		if (_Button_Map.find(GUIDString(guid)) != _Button_Map.end())
			_Button_Map[GUIDString(guid)]->IsEnabled(value);
	}

	inline void Button_SetContent(System::Guid guid, System::String^ _value)
	{
		std::wstring value; // Dummy placeholder
		MarshalString(_value, value);

		if (_Button_Map.find(GUIDString(guid)) != _Button_Map.end())
			_Button_Map[GUIDString(guid)]->Content(value);
	}

	inline void NumberBox_SetVisibility(System::Guid guid, const bool value)
	{
		if (_NumberBox_Map.find(GUIDString(guid)) != _NumberBox_Map.end())
			_NumberBox_Map[GUIDString(guid)]->Visibility(value);
	}

	inline void NumberBox_SetWidth(System::Guid guid, const uint32_t value)
	{
		if (_NumberBox_Map.find(GUIDString(guid)) != _NumberBox_Map.end())
			_NumberBox_Map[GUIDString(guid)]->Width(value);
	}

	inline void NumberBox_SetHeight(System::Guid guid, const uint32_t value)
	{
		if (_NumberBox_Map.find(GUIDString(guid)) != _NumberBox_Map.end())
			_NumberBox_Map[GUIDString(guid)]->Height(value);
	}

	inline void NumberBox_SetIsEnabled(System::Guid guid, const bool value)
	{
		if (_NumberBox_Map.find(GUIDString(guid)) != _NumberBox_Map.end())
			_NumberBox_Map[GUIDString(guid)]->IsEnabled(value);
	}

	inline void NumberBox_SetValue(System::Guid guid, const int value)
	{
		if (_NumberBox_Map.find(GUIDString(guid)) != _NumberBox_Map.end())
			_NumberBox_Map[GUIDString(guid)]->Value(value);
	}

	inline void ComboBox_SetVisibility(System::Guid guid, const bool value)
	{
		if (_ComboBox_Map.find(GUIDString(guid)) != _ComboBox_Map.end())
			_ComboBox_Map[GUIDString(guid)]->Visibility(value);
	}

	inline void ComboBox_SetWidth(System::Guid guid, const uint32_t value)
	{
		if (_ComboBox_Map.find(GUIDString(guid)) != _ComboBox_Map.end())
			_ComboBox_Map[GUIDString(guid)]->Width(value);
	}

	inline void ComboBox_SetHeight(System::Guid guid, const uint32_t value)
	{
		if (_ComboBox_Map.find(GUIDString(guid)) != _ComboBox_Map.end())
			_ComboBox_Map[GUIDString(guid)]->Height(value);
	}

	inline void ComboBox_SetIsEnabled(System::Guid guid, const bool value)
	{
		if (_ComboBox_Map.find(GUIDString(guid)) != _ComboBox_Map.end())
			_ComboBox_Map[GUIDString(guid)]->IsEnabled(value);
	}

	inline void ComboBox_SetSelectedIndex(System::Guid guid, const uint32_t value)
	{
		if (_ComboBox_Map.find(GUIDString(guid)) != _ComboBox_Map.end())
			_ComboBox_Map[GUIDString(guid)]->SelectedIndex(value);
	}

	inline void ComboBox_SetItems(System::Guid guid,
	                              System::Collections::Generic::List<System::String^>^ _values)
	{
		std::vector<std::wstring> out;
		for each (const auto& item in _values)
		{
			std::wstring _out; // Dummy placeholder
			MarshalString(item, _out);
			out.push_back(_out); // Append
		}

		if (_ComboBox_Map.find(GUIDString(guid)) != _ComboBox_Map.end())
			_ComboBox_Map[GUIDString(guid)]->Items(out);
	}

	inline void CheckBox_SetVisibility(System::Guid guid, const bool value)
	{
		if (_CheckBox_Map.find(GUIDString(guid)) != _CheckBox_Map.end())
			_CheckBox_Map[GUIDString(guid)]->Visibility(value);
	}

	inline void CheckBox_SetWidth(System::Guid guid, const uint32_t value)
	{
		if (_CheckBox_Map.find(GUIDString(guid)) != _CheckBox_Map.end())
			_CheckBox_Map[GUIDString(guid)]->Width(value);
	}

	inline void CheckBox_SetHeight(System::Guid guid, const uint32_t value)
	{
		if (_CheckBox_Map.find(GUIDString(guid)) != _CheckBox_Map.end())
			_CheckBox_Map[GUIDString(guid)]->Height(value);
	}

	inline void CheckBox_SetIsEnabled(System::Guid guid, const bool value)
	{
		if (_CheckBox_Map.find(GUIDString(guid)) != _CheckBox_Map.end())
			_CheckBox_Map[GUIDString(guid)]->IsEnabled(value);
	}

	inline void CheckBox_SetIsChecked(System::Guid guid, const bool value)
	{
		if (_CheckBox_Map.find(GUIDString(guid)) != _CheckBox_Map.end())
			_CheckBox_Map[GUIDString(guid)]->IsChecked(value);
	}

	inline void ToggleSwitch_SetVisibility(System::Guid guid, const bool value)
	{
		if (_ToggleSwitch_Map.find(GUIDString(guid)) != _ToggleSwitch_Map.end())
			_ToggleSwitch_Map[GUIDString(guid)]->Visibility(value);
	}

	inline void ToggleSwitch_SetWidth(System::Guid guid, const uint32_t value)
	{
		if (_ToggleSwitch_Map.find(GUIDString(guid)) != _ToggleSwitch_Map.end())
			_ToggleSwitch_Map[GUIDString(guid)]->Width(value);
	}

	inline void ToggleSwitch_SetHeight(System::Guid guid, const uint32_t value)
	{
		if (_ToggleSwitch_Map.find(GUIDString(guid)) != _ToggleSwitch_Map.end())
			_ToggleSwitch_Map[GUIDString(guid)]->Height(value);
	}

	inline void ToggleSwitch_SetIsEnabled(System::Guid guid, const bool value)
	{
		if (_ToggleSwitch_Map.find(GUIDString(guid)) != _ToggleSwitch_Map.end())
			_ToggleSwitch_Map[GUIDString(guid)]->IsEnabled(value);
	}

	inline void ToggleSwitch_SetIsChecked(System::Guid guid, const bool value)
	{
		if (_ToggleSwitch_Map.find(GUIDString(guid)) != _ToggleSwitch_Map.end())
			_ToggleSwitch_Map[GUIDString(guid)]->IsChecked(value);
	}

	inline void TextBox_SetVisibility(System::Guid guid, const bool value)
	{
		if (_TextBox_Map.find(GUIDString(guid)) != _TextBox_Map.end())
			_TextBox_Map[GUIDString(guid)]->Visibility(value);
	}

	inline void TextBox_SetWidth(System::Guid guid, const uint32_t value)
	{
		if (_TextBox_Map.find(GUIDString(guid)) != _TextBox_Map.end())
			_TextBox_Map[GUIDString(guid)]->Width(value);
	}

	inline void TextBox_SetHeight(System::Guid guid, const uint32_t value)
	{
		if (_TextBox_Map.find(GUIDString(guid)) != _TextBox_Map.end())
			_TextBox_Map[GUIDString(guid)]->Height(value);
	}

	inline void TextBox_SetText(System::Guid guid, System::String^ _value)
	{
		std::wstring value; // Dummy placeholder
		MarshalString(_value, value);

		if (_TextBox_Map.find(GUIDString(guid)) != _TextBox_Map.end())
			_TextBox_Map[GUIDString(guid)]->Text(value);
	}

	inline void ProgressRing_SetVisibility(System::Guid guid, const bool value)
	{
		if (_ProgressRing_Map.find(GUIDString(guid)) != _ProgressRing_Map.end())
			_ProgressRing_Map[GUIDString(guid)]->Visibility(value);
	}

	inline void ProgressRing_SetWidth(System::Guid guid, const uint32_t value)
	{
		if (_ProgressRing_Map.find(GUIDString(guid)) != _ProgressRing_Map.end())
			_ProgressRing_Map[GUIDString(guid)]->Width(value);
	}

	inline void ProgressRing_SetHeight(System::Guid guid, const uint32_t value)
	{
		if (_ProgressRing_Map.find(GUIDString(guid)) != _ProgressRing_Map.end())
			_ProgressRing_Map[GUIDString(guid)]->Height(value);
	}

	inline void ProgressRing_SetProgress(System::Guid guid, const int value)
	{
		if (_ProgressRing_Map.find(GUIDString(guid)) != _ProgressRing_Map.end())
			_ProgressRing_Map[GUIDString(guid)]->Progress(value);
	}

	inline void ProgressBar_SetVisibility(System::Guid guid, const bool value)
	{
		if (_ProgressBar_Map.find(GUIDString(guid)) != _ProgressBar_Map.end())
			_ProgressBar_Map[GUIDString(guid)]->Visibility(value);
	}

	inline void ProgressBar_SetWidth(System::Guid guid, const uint32_t value)
	{
		if (_ProgressBar_Map.find(GUIDString(guid)) != _ProgressBar_Map.end())
			_ProgressBar_Map[GUIDString(guid)]->Width(value);
	}

	inline void ProgressBar_SetHeight(System::Guid guid, const uint32_t value)
	{
		if (_ProgressBar_Map.find(GUIDString(guid)) != _ProgressBar_Map.end())
			_ProgressBar_Map[GUIDString(guid)]->Height(value);
	}

	inline void ProgressBar_SetProgress(System::Guid guid, const int value)
	{
		if (_ProgressBar_Map.find(GUIDString(guid)) != _ProgressBar_Map.end())
			_ProgressBar_Map[GUIDString(guid)]->Progress(value);
	}

	inline void ProgressBar_SetShowPaused(System::Guid guid, const bool value)
	{
		if (_ProgressBar_Map.find(GUIDString(guid)) != _ProgressBar_Map.end())
			_ProgressBar_Map[GUIDString(guid)]->ShowPaused(value);
	}

	inline void ProgressBar_SetShowError(System::Guid guid, const bool value)
	{
		if (_ProgressBar_Map.find(GUIDString(guid)) != _ProgressBar_Map.end())
			_ProgressBar_Map[GUIDString(guid)]->ShowError(value);
	}
}

namespace Helpers
{
	inline void LogToAme(System::String^ message, unsigned severity)
	{
		std::wstring out; // Dummy placeholder
		MarshalString(message, out);

		// Finally call the logger
		switch (severity)
		{
		case 0:
			_this_Device->logInfoMessage(out);
			return;
		case 1:
			_this_Device->logWarningMessage(out);
			return;
		case 2:
			_this_Device->logErrorMessage(out);
		}
	}

	inline void RequestStatusUIRefresh()
	{
		_this_Device->requestStatusUIRefresh();
	}

	inline bool SetLocalizationResourcesRoot(System::String^ path)
	{
		std::wstring out; // Dummy placeholder
		MarshalString(path, out);

		return _this_Device->setLocalizationResourcesRoot(out);
	}

	inline System::String^ RequestLocalizedString(System::String^ key)
	{
		std::wstring out; // Dummy placeholder
		MarshalString(key, out);

		return gcnew System::String(
			_this_Device->requestLocalizedString(out).c_str());
	}

	inline System::Tuple<System::Numerics::Vector3, System::Numerics::Quaternion>^ GetHMDPose()
	{
		const auto& [position, orientation] = _this_Device->getHMDPose();
		return gcnew System::Tuple<System::Numerics::Vector3, System::Numerics::Quaternion>(
			System::Numerics::Vector3{
				static_cast<float>(position.x()),
				static_cast<float>(position.y()),
				static_cast<float>(position.z())
			},
			System::Numerics::Quaternion{
				static_cast<float>(orientation.w()),
				static_cast<float>(orientation.x()),
				static_cast<float>(orientation.y()),
				static_cast<float>(orientation.z())
			}
		);
	}

	inline System::Tuple<System::Numerics::Vector3, System::Numerics::Quaternion>^ GetHMDPoseCalibrated()
	{
		const auto& [position, orientation] = _this_Device->getHMDPoseCalibrated();
		return gcnew System::Tuple<System::Numerics::Vector3, System::Numerics::Quaternion>(
			System::Numerics::Vector3{
				static_cast<float>(position.x()),
				static_cast<float>(position.y()),
				static_cast<float>(position.z())
			},
			System::Numerics::Quaternion{
				static_cast<float>(orientation.w()),
				static_cast<float>(orientation.x()),
				static_cast<float>(orientation.y()),
				static_cast<float>(orientation.z())
			}
		);
	}

	inline System::Tuple<System::Numerics::Vector3, System::Numerics::Quaternion>^ GetLeftControllerPose()
	{
		const auto& [position, orientation] = _this_Device->getLeftControllerPose();
		return gcnew System::Tuple<System::Numerics::Vector3, System::Numerics::Quaternion>(
			System::Numerics::Vector3{
				static_cast<float>(position.x()),
				static_cast<float>(position.y()),
				static_cast<float>(position.z())
			},
			System::Numerics::Quaternion{
				static_cast<float>(orientation.w()),
				static_cast<float>(orientation.x()),
				static_cast<float>(orientation.y()),
				static_cast<float>(orientation.z())
			}
		);
	}

	inline System::Tuple<System::Numerics::Vector3, System::Numerics::Quaternion>^ GetLeftControllerPoseCalibrated()
	{
		const auto& [position, orientation] = _this_Device->getLeftControllerPoseCalibrated();
		return gcnew System::Tuple<System::Numerics::Vector3, System::Numerics::Quaternion>(
			System::Numerics::Vector3{
				static_cast<float>(position.x()),
				static_cast<float>(position.y()),
				static_cast<float>(position.z())
			},
			System::Numerics::Quaternion{
				static_cast<float>(orientation.w()),
				static_cast<float>(orientation.x()),
				static_cast<float>(orientation.y()),
				static_cast<float>(orientation.z())
			}
		);
	}

	inline System::Tuple<System::Numerics::Vector3, System::Numerics::Quaternion>^ GetRightControllerPose()
	{
		const auto& [position, orientation] = _this_Device->getRightControllerPose();
		return gcnew System::Tuple<System::Numerics::Vector3, System::Numerics::Quaternion>(
			System::Numerics::Vector3{
				static_cast<float>(position.x()),
				static_cast<float>(position.y()),
				static_cast<float>(position.z())
			},
			System::Numerics::Quaternion{
				static_cast<float>(orientation.w()),
				static_cast<float>(orientation.x()),
				static_cast<float>(orientation.y()),
				static_cast<float>(orientation.z())
			}
		);
	}

	inline System::Tuple<System::Numerics::Vector3, System::Numerics::Quaternion>^ GetRightControllerPoseCalibrated()
	{
		const auto& [position, orientation] = _this_Device->getRightControllerPoseCalibrated();
		return gcnew System::Tuple<System::Numerics::Vector3, System::Numerics::Quaternion>(
			System::Numerics::Vector3{
				static_cast<float>(position.x()),
				static_cast<float>(position.y()),
				static_cast<float>(position.z())
			},
			System::Numerics::Quaternion{
				static_cast<float>(orientation.w()),
				static_cast<float>(orientation.x()),
				static_cast<float>(orientation.y()),
				static_cast<float>(orientation.z())
			}
		);
	}

	inline double GetHMDOrientationYaw()
	{
		return _this_Device->getHMDOrientationYaw();
	}

	inline double GetHMDOrientationYawCalibrated()
	{
		return _this_Device->getHMDOrientationYawCalibrated();
	}

	inline array<Managed_SkeletonBasis_Handler::API_Projection_Files::TrackedJoint^>^ GetAppJointPoses()
	{
		const auto joints = _this_Device->getAppJointPoses();
		auto tracker_array = gcnew array<Managed_SkeletonBasis_Handler::API_Projection_Files::TrackedJoint^>(
			joints.size());

		for (size_t i = 0; i < joints.size(); i++)
		{
			const auto& position = joints[i].getJointPosition();
			const auto& orientation = joints[i].getJointOrientation();

			tracker_array[i] = gcnew Managed_SkeletonBasis_Handler::API_Projection_Files::TrackedJoint{
				System::Numerics::Vector3{
					static_cast<float>(position.x()),
					static_cast<float>(position.y()),
					static_cast<float>(position.z())
				},
				System::Numerics::Quaternion{
					static_cast<float>(orientation.w()),
					static_cast<float>(orientation.x()),
					static_cast<float>(orientation.y()),
					static_cast<float>(orientation.z())
				},
				gcnew System::String(joints[i].getJointName().c_str())
			};
		}

		return tracker_array;
	}
}

namespace Amethyst_API_Managed_Wrapper
{
	public ref class Amethyst_API_Managed_Device : Managed_SkeletonBasis_Handler::AmethystDevice
	{
	public:
		Amethyst_API_Managed_Device()
		{
			LoggerAction = gcnew System::Action<System::String^, unsigned>(&Helpers::LogToAme);
			RefreshStatusUI = gcnew System::Action(&Helpers::RequestStatusUIRefresh);

			SetLocalizationResourcesRoot = gcnew System::Func<System::String^, bool>(
				&Helpers::SetLocalizationResourcesRoot);
			RequestLocalizedString = gcnew System::Func<System::String^, System::String^>(
				&Helpers::RequestLocalizedString);

			GetHMDPose = gcnew System::Func<System::Tuple<
				System::Numerics::Vector3, System::Numerics::Quaternion>^>(&Helpers::GetHMDPose);
			GetHMDPoseCalibrated = gcnew System::Func<System::Tuple<
				System::Numerics::Vector3, System::Numerics::Quaternion>^>(&Helpers::GetHMDPoseCalibrated);
			GetLeftControllerPose = gcnew System::Func<System::Tuple<
				System::Numerics::Vector3, System::Numerics::Quaternion>^>(&Helpers::GetLeftControllerPose);
			GetLeftControllerPoseCalibrated = gcnew System::Func<System::Tuple<
				System::Numerics::Vector3, System::Numerics::Quaternion>^>(&Helpers::GetLeftControllerPoseCalibrated);
			GetRightControllerPose = gcnew System::Func<System::Tuple<
				System::Numerics::Vector3, System::Numerics::Quaternion>^>(&Helpers::GetRightControllerPose);
			GetRightControllerPoseCalibrated = gcnew System::Func<System::Tuple<
				System::Numerics::Vector3, System::Numerics::Quaternion>^>(&Helpers::GetRightControllerPoseCalibrated);
			GetHMDOrientationYaw = gcnew System::Func<double>(&Helpers::GetHMDOrientationYaw);
			GetHMDOrientationYawCalibrated = gcnew System::Func<double>(&Helpers::GetHMDOrientationYawCalibrated);
			GetAppJointPoses = gcnew System::Func<array<
				Managed_SkeletonBasis_Handler::API_Projection_Files::TrackedJoint^>^>(&Helpers::GetAppJointPoses);

			CreateTextBlock = gcnew System::Func<System::String^, System::Guid>(&Interface::CreateTextBlock);
			CreateButton = gcnew System::Func<System::String^, System::Guid>(&Interface::CreateButton);
			CreateNumberBox = gcnew System::Func<int, System::Guid>(&Interface::CreateNumberBox);
			CreateComboBox = gcnew System::Func<System::Collections::Generic::List<System::String^>^, System::Guid>(
				&Interface::CreateComboBox);
			CreateCheckBox = gcnew System::Func<System::Guid>(&Interface::CreateCheckBox);
			CreateToggleSwitch = gcnew System::Func<System::Guid>(&Interface::CreateToggleSwitch);
			CreateTextBox = gcnew System::Func<System::Guid>(&Interface::CreateTextBox);
			CreateProgressRing = gcnew System::Func<System::Guid>(&Interface::CreateProgressRing);
			CreateProgressBar = gcnew System::Func<System::Guid>(&Interface::CreateProgressBar);

			AppendSingleElement = gcnew System::Action<Object^, int>(&Interface::AppendSingleElement);
			AppendElementPair = gcnew System::Action<Object^, Object^>(&Interface::AppendElementPair);
			AppendElementPairStack = gcnew System::Action<Object^, Object^>(&Interface::AppendElementPairStack);
			AppendElementVector = gcnew System::Action<System::Collections::Generic::List<Object^>^>(
				&Interface::AppendElementVector);
			AppendElementVectorStack = gcnew System::Action<System::Collections::Generic::List<Object^>^>(
				&Interface::AppendElementVectorStack);

			TextBlock_SetVisibility = gcnew System::Action<System::Guid, bool>(&Interface::TextBlock_SetVisibility);
			TextBlock_SetIsPrimary = gcnew System::Action<System::Guid, bool>(&Interface::TextBlock_SetIsPrimary);
			TextBlock_SetWidth = gcnew System::Action<System::Guid, uint32_t>(&Interface::TextBlock_SetWidth);
			TextBlock_SetHeight = gcnew System::Action<System::Guid, uint32_t>(&Interface::TextBlock_SetHeight);
			TextBlock_SetText = gcnew System::Action<System::Guid, System::String^>(&Interface::TextBlock_SetText);

			Button_SetVisibility = gcnew System::Action<System::Guid, bool>(&Interface::Button_SetVisibility);
			Button_SetWidth = gcnew System::Action<System::Guid, uint32_t>(&Interface::Button_SetWidth);
			Button_SetHeight = gcnew System::Action<System::Guid, uint32_t>(&Interface::Button_SetHeight);
			Button_SetIsEnabled = gcnew System::Action<System::Guid, bool>(&Interface::Button_SetIsEnabled);
			Button_SetContent = gcnew System::Action<System::Guid, System::String^>(&Interface::Button_SetContent);

			NumberBox_SetVisibility = gcnew System::Action<System::Guid, bool>(&Interface::NumberBox_SetVisibility);
			NumberBox_SetWidth = gcnew System::Action<System::Guid, uint32_t>(&Interface::NumberBox_SetWidth);
			NumberBox_SetHeight = gcnew System::Action<System::Guid, uint32_t>(&Interface::NumberBox_SetHeight);
			NumberBox_SetIsEnabled = gcnew System::Action<System::Guid, bool>(&Interface::NumberBox_SetIsEnabled);
			NumberBox_SetValue = gcnew System::Action<System::Guid, int>(&Interface::NumberBox_SetValue);

			ComboBox_SetVisibility = gcnew System::Action<System::Guid, bool>(&Interface::ComboBox_SetVisibility);
			ComboBox_SetWidth = gcnew System::Action<System::Guid, uint32_t>(&Interface::ComboBox_SetWidth);
			ComboBox_SetHeight = gcnew System::Action<System::Guid, uint32_t>(&Interface::ComboBox_SetHeight);
			ComboBox_SetIsEnabled = gcnew System::Action<System::Guid, bool>(&Interface::ComboBox_SetIsEnabled);
			ComboBox_SetSelectedIndex = gcnew System::Action<System::Guid, uint32_t>(
				&Interface::ComboBox_SetSelectedIndex);
			ComboBox_SetItems = gcnew System::Action<
				System::Guid, System::Collections::Generic::List<System::String^>^>(&Interface::ComboBox_SetItems);

			CheckBox_SetVisibility = gcnew System::Action<System::Guid, bool>(&Interface::CheckBox_SetVisibility);
			CheckBox_SetWidth = gcnew System::Action<System::Guid, uint32_t>(&Interface::CheckBox_SetWidth);
			CheckBox_SetHeight = gcnew System::Action<System::Guid, uint32_t>(&Interface::CheckBox_SetHeight);
			CheckBox_SetIsEnabled = gcnew System::Action<System::Guid, bool>(&Interface::CheckBox_SetIsEnabled);
			CheckBox_SetIsChecked = gcnew System::Action<System::Guid, bool>(&Interface::CheckBox_SetIsChecked);

			ToggleSwitch_SetVisibility = gcnew System::Action<System::Guid, bool>(
				&Interface::ToggleSwitch_SetVisibility);
			ToggleSwitch_SetWidth = gcnew System::Action<System::Guid, uint32_t>(&Interface::ToggleSwitch_SetWidth);
			ToggleSwitch_SetHeight = gcnew System::Action<System::Guid, uint32_t>(&Interface::ToggleSwitch_SetHeight);
			ToggleSwitch_SetIsEnabled = gcnew System::Action<System::Guid, bool>(&Interface::ToggleSwitch_SetIsEnabled);
			ToggleSwitch_SetIsChecked = gcnew System::Action<System::Guid, bool>(&Interface::ToggleSwitch_SetIsChecked);

			TextBox_SetVisibility = gcnew System::Action<System::Guid, bool>(&Interface::TextBox_SetVisibility);
			TextBox_SetWidth = gcnew System::Action<System::Guid, uint32_t>(&Interface::TextBox_SetWidth);
			TextBox_SetHeight = gcnew System::Action<System::Guid, uint32_t>(&Interface::TextBox_SetHeight);
			TextBox_SetText = gcnew System::Action<System::Guid, System::String^>(&Interface::TextBox_SetText);

			ProgressRing_SetVisibility = gcnew System::Action<System::Guid, bool>(
				&Interface::ProgressRing_SetVisibility);
			ProgressRing_SetWidth = gcnew System::Action<System::Guid, uint32_t>(&Interface::ProgressRing_SetWidth);
			ProgressRing_SetHeight = gcnew System::Action<System::Guid, uint32_t>(&Interface::ProgressRing_SetHeight);
			ProgressRing_SetProgress = gcnew System::Action<System::Guid, int>(&Interface::ProgressRing_SetProgress);

			ProgressBar_SetVisibility = gcnew System::Action<System::Guid, bool>(&Interface::ProgressBar_SetVisibility);
			ProgressBar_SetWidth = gcnew System::Action<System::Guid, uint32_t>(&Interface::ProgressBar_SetWidth);
			ProgressBar_SetHeight = gcnew System::Action<System::Guid, uint32_t>(&Interface::ProgressBar_SetHeight);
			ProgressBar_SetProgress = gcnew System::Action<System::Guid, int>(&Interface::ProgressBar_SetProgress);
			ProgressBar_SetShowPaused = gcnew System::Action<System::Guid, bool>(&Interface::ProgressBar_SetShowPaused);
			ProgressBar_SetShowError = gcnew System::Action<System::Guid, bool>(&Interface::ProgressBar_SetShowError);
		}
	};
}

ref struct IAmethyst_API_Managed
{
	static Amethyst_API_Managed_Wrapper::Amethyst_API_Managed_Device^ wrapper =
		gcnew Amethyst_API_Managed_Wrapper::Amethyst_API_Managed_Device;
};

namespace Amethyst_API_Managed
{
	__declspec(dllexport) void RegisterDevice(ktvr::K2TrackingDeviceBase_SkeletonBasis* handler)
	{
		_this_Device = handler;
	}

	__declspec(dllexport) bool Initialize()
	{
		return IAmethyst_API_Managed::wrapper->Initialize();
	}

	__declspec(dllexport) void Update()
	{
		IAmethyst_API_Managed::wrapper->Update();
	}

	__declspec(dllexport) bool Shutdown()
	{
		return IAmethyst_API_Managed::wrapper->Shutdown();
	}

	__declspec(dllexport) std::wstring GetDeviceName()
	{
		std::wstring out; // Dummy placeholder
		MarshalString(IAmethyst_API_Managed::wrapper->DeviceName, out);
		return out; // Return what we've got
	}

	__declspec(dllexport) std::wstring GetDeviceGUID()
	{
		std::wstring out; // Dummy placeholder
		MarshalString(IAmethyst_API_Managed::wrapper->DeviceGUID, out);
		return out; // Return what we've got
	}

	__declspec(dllexport) long GetDeviceStatus()
	{
		return IAmethyst_API_Managed::wrapper->GetDeviceStatus();
	}

	__declspec(dllexport) std::wstring GetDeviceStatusWString()
	{
		std::wstring out; // Dummy placeholder
		MarshalString(IAmethyst_API_Managed::wrapper->GetDeviceStatusString(), out);
		return out; // Return what we've got
	}

	__declspec(dllexport) uint32_t GetDeviceCharacteristics()
	{
		return IAmethyst_API_Managed::wrapper->DeviceCharacteristics;
	}

	__declspec(dllexport) bool GetIsFlipSupported()
	{
		return IAmethyst_API_Managed::wrapper->IsFlipSupported;
	}

	__declspec(dllexport) bool GetIsAppOrientationSupported()
	{
		return IAmethyst_API_Managed::wrapper->IsAppOrientationSupported;
	}

	__declspec(dllexport) bool GetIsSkeletonTracked()
	{
		return IAmethyst_API_Managed::wrapper->IsSkeletonTracked;
	}

	__declspec(dllexport) bool GetIsSettingsDaemonSupported()
	{
		return IAmethyst_API_Managed::wrapper->IsSettingsDaemonSupported;
	}

	__declspec(dllexport) bool GetIsOverridesJointPhysicsEnabled()
	{
		return IAmethyst_API_Managed::wrapper->IsOverridesJointPhysicsEnabled;
	}

	__declspec(dllexport) std::array<ktvr::K2TrackedBaseJoint, 25> GetTrackedJoints()
	{
		std::array<ktvr::K2TrackedBaseJoint, 25> cluster_joints;
		const auto joints = IAmethyst_API_Managed::wrapper->JointsList;

		for (size_t i = 0; i < std::min(
			     static_cast<size_t>(joints->Count),
			     cluster_joints.size()); i++)
		{
			std::wstring joint_name; // Dummy placeholder
			MarshalString(joints[i]->JointName, joint_name);

			ktvr::K2TrackedBaseJoint _joint;
			_joint.update(
				{
					static_cast<double>(joints[i]->Position.X),
					static_cast<double>(joints[i]->Position.Y),
					static_cast<double>(joints[i]->Position.Z)
				},
				{
					static_cast<double>(joints[i]->Orientation.W),
					static_cast<double>(joints[i]->Orientation.X),
					static_cast<double>(joints[i]->Orientation.Y),
					static_cast<double>(joints[i]->Orientation.Z),
				},
				{
					static_cast<double>(joints[i]->Velocity.X),
					static_cast<double>(joints[i]->Velocity.Y),
					static_cast<double>(joints[i]->Velocity.Z),
				},
				{
					static_cast<double>(joints[i]->Acceleration.X),
					static_cast<double>(joints[i]->Acceleration.Y),
					static_cast<double>(joints[i]->Acceleration.Z),
				},
				{
					static_cast<double>(joints[i]->AngularVelocity.X),
					static_cast<double>(joints[i]->AngularVelocity.Y),
					static_cast<double>(joints[i]->AngularVelocity.Z),
				},
				{
					static_cast<double>(joints[i]->AngularAcceleration.X),
					static_cast<double>(joints[i]->AngularAcceleration.Y),
					static_cast<double>(joints[i]->AngularAcceleration.Z),
				},
				static_cast<ktvr::ITrackedJointState>(joints[i]->TrackingState)
			);

			cluster_joints[i] = _joint;
		}

		return cluster_joints;
	}

	__declspec(dllexport) void OnLoad()
	{
		IAmethyst_API_Managed::wrapper->OnLoad();
	}
}
