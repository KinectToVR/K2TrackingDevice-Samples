using System.Numerics;

namespace Managed_SkeletonBasis_Handler.API_Projection_Files;

public class TrackedJoint
{
    public string JointName = "[NAME NOT SET]";

    public Quaternion Orientation;

    public Vector3 Position,
        Velocity,
        Acceleration,
        AngularVelocity,
        AngularAcceleration;

    public uint TrackingState =
        (uint)TrackedJointState.State_NotTracked;

    public TrackedJoint()
    {
    }

    public TrackedJoint(
        Vector3 position,
        Quaternion orientation,
        string name)
    {
        Position = position;
        Orientation = orientation;
        JointName = name;
    }
}

public abstract class AmethystManagedDevice_Kinect
{
    // LayoutRoot appending enum:
    // when appending a single element,
    // you can choose which side it should
    // snap to (HorizontalAlignment)
    public enum SingleLayoutHorizontalAlignment
    {
        // Snap to the left
        Left,

        // Snap to the right
        Right,

        // Try to be centered
        Center
    }

    // Append a One-Row element pair : */* column space
    public Action<object, object>? AppendElementPair;

    // Append a One-Row element pair : horizontal stack
    public Action<object, object>? AppendElementPairStack;

    // Append a One-Row element vector : */* column space
    public Action<List<object>>? AppendElementVector;

    // Append a One-Row element vector : horizontal stack
    public Action<List<object>>? AppendElementVectorStack;

    // PluginSettings layout's root pane
    // This is a xaml vertical stack panel,
    // with additional methods involving grids,
    // allowing us to push different config layouts
    public Action<object, int>? AppendSingleElement;

    public uint DeviceCharacteristics = (uint)TrackingDeviceCharacteristics.K2_Character_Unknown;
    public string DeviceGUID = "INVALID"; // Make sure you set this!
    public string DeviceName = "[NAME NOT SET]"; // Make sure you set this!

    public Func<TrackedJoint[]>? GetAppJointPoses;
    public Func<double>? GetHMDOrientationYaw;
    public Func<double>? GetHMDOrientationYawCalibrated;

    public Func<Tuple<Vector3, Quaternion>>? GetHMDPose;
    public Func<Tuple<Vector3, Quaternion>>? GetHMDPoseCalibrated;
    public Func<Tuple<Vector3, Quaternion>>? GetLeftControllerPose;
    public Func<Tuple<Vector3, Quaternion>>? GetLeftControllerPoseCalibrated;
    public Func<Tuple<Vector3, Quaternion>>? GetRightControllerPose;
    public Func<Tuple<Vector3, Quaternion>>? GetRightControllerPoseCalibrated;
    public bool IsOverridesJointPhysicsEnabled = false; // No physics

    public bool IsSettingsDaemonSupported = false; // No custom settings

    public bool IsAppOrientationSupported = false;

    public bool IsFlipSupported = false;
    public bool IsSkeletonTracked = false;

    public List<TrackedJoint> JointsList = new();

    // Log a message to Amethyst logs : handler
    public Action<string, uint>? LoggerAction;

    // Refresh Amethyst device status
    public Action? RefreshStatusUI;

    // Request a string from AME resources, empty for no match
    // Warning: The primarily searched resource is the device-provided one!
    public Func<string, string>? RequestLocalizedString;

    // Request a folder to be set as device's AME resources,
    // you can access these resources with the lower function later (after onLoad)
    // Warning: Resources are containerized and can't be accessed in-between devices!
    // Warning: The default root is "[device_folder_path]/resources/Strings"!
    public Func<string, bool>? SetLocalizationResourcesRoot;

    // Log a message to Amethyst logs : wrapper
    public void Log(string msg, LogSeverity sev)
    {
        LoggerAction?.Invoke(msg, (uint)sev);
    }

    public abstract void OnLoad();

    public abstract bool Initialize();

    public abstract void Update();

    public abstract bool Shutdown();

    public abstract long GetDeviceStatus();

    public abstract string GetDeviceStatusString();

    // Elements classes

    #region Elements

    public class TextBlock
    {
        private uint _height;
        private bool _isPrimary = true;

        private string _text = "";

        private bool _visibility = true;

        private uint _width;

        public TextBlock(string text)
        {
            _guid = CreateTextBlock(text);
            _text = text;
        }

        public bool Visibility
        {
            get => _visibility;
            set
            {
                _visibility = value;
                TextBlock_SetVisibility?.Invoke(_guid, value);
            }
        }

        public bool IsPrimary
        {
            get => _isPrimary;
            set
            {
                _isPrimary = value;
                TextBlock_SetIsPrimary?.Invoke(_guid, value);
            }
        }

        public uint Width
        {
            get => _width;
            set
            {
                _width = value;
                TextBlock_SetWidth?.Invoke(_guid, value);
            }
        }

        public uint Height
        {
            get => _height;
            set
            {
                _height = value;
                TextBlock_SetHeight?.Invoke(_guid, value);
            }
        }

        public string Text
        {
            get => _text;
            set
            {
                _text = value;
                TextBlock_SetText?.Invoke(_guid, value);
            }
        }

        public Guid _guid { get; }
    }

    public class Button
    {
        private string _content;
        private uint _height;
        private bool _isEnabled = true;

        private bool _visibility = true;

        private uint _width;

        public Action OnClick = () => { };

        public Button(string content)
        {
            _guid = CreateButton(content);
            _content = content;

            ButtonsDictionary[_guid] = this;
        }

        public bool Visibility
        {
            get => _visibility;
            set
            {
                _visibility = value;
                Button_SetVisibility?.Invoke(_guid, value);
            }
        }

        public uint Width
        {
            get => _width;
            set
            {
                _width = value;
                Button_SetWidth?.Invoke(_guid, value);
            }
        }

        public uint Height
        {
            get => _height;
            set
            {
                _height = value;
                Button_SetHeight?.Invoke(_guid, value);
            }
        }

        public bool IsEnabled
        {
            get => _isEnabled;
            set
            {
                _isEnabled = value;
                Button_SetIsEnabled?.Invoke(_guid, value);
            }
        }

        public string Content
        {
            get => _content;
            set
            {
                _content = value;
                Button_SetContent?.Invoke(_guid, value);
            }
        }

        public Guid _guid { get; }
    }

    public class NumberBox
    {
        private uint _height;
        private bool _isEnabled = true;

        private int _value;

        private bool _visibility = true;

        private uint _width;

        public Action<int> OnValueChanged = _ => { };

        public NumberBox(int value)
        {
            _guid = CreateNumberBox(value);
            _value = value;

            NumberBoxesDictionary[_guid] = this;
        }

        public bool Visibility
        {
            get => _visibility;
            set
            {
                _visibility = value;
                NumberBox_SetVisibility?.Invoke(_guid, value);
            }
        }

        public uint Width
        {
            get => _width;
            set
            {
                _width = value;
                NumberBox_SetWidth?.Invoke(_guid, value);
            }
        }

        public uint Height
        {
            get => _height;
            set
            {
                _height = value;
                NumberBox_SetHeight?.Invoke(_guid, value);
            }
        }

        public bool IsEnabled
        {
            get => _isEnabled;
            set
            {
                _isEnabled = value;
                NumberBox_SetIsEnabled?.Invoke(_guid, value);
            }
        }

        public int Value
        {
            get => _value;
            set
            {
                _value = value;
                NumberBox_SetValue?.Invoke(_guid, value);
            }
        }

        public Guid _guid { get; }
    }

    public class ComboBox
    {
        private uint _height;
        private bool _isEnabled = true;
        private List<string> _items;

        private uint _selectedIndex;

        private bool _visibility = true;

        private uint _width;

        public Action<uint> OnSelectionChanged = _ => { };

        public ComboBox(List<string> items)
        {
            _guid = CreateComboBox(items);
            _items = items;

            ComboBoxesDictionary[_guid] = this;
        }

        public bool Visibility
        {
            get => _visibility;
            set
            {
                _visibility = value;
                ComboBox_SetVisibility?.Invoke(_guid, value);
            }
        }

        public uint Width
        {
            get => _width;
            set
            {
                _width = value;
                ComboBox_SetWidth?.Invoke(_guid, value);
            }
        }

        public uint Height
        {
            get => _height;
            set
            {
                _height = value;
                ComboBox_SetHeight?.Invoke(_guid, value);
            }
        }

        public bool IsEnabled
        {
            get => _isEnabled;
            set
            {
                _isEnabled = value;
                ComboBox_SetIsEnabled?.Invoke(_guid, value);
            }
        }

        public uint SelectedIndex
        {
            get => _selectedIndex;
            set
            {
                _selectedIndex = value;
                ComboBox_SetSelectedIndex?.Invoke(_guid, value);
            }
        }

        public List<string> Items
        {
            get => _items;
            set
            {
                _items = value;
                ComboBox_SetItems?.Invoke(_guid, value);
            }
        }

        public Guid _guid { get; }
    }

    public class CheckBox
    {
        private uint _height;
        private bool _isChecked = true;
        private bool _isEnabled = true;

        private bool _visibility = true;

        private uint _width;

        public Action OnChecked = () => { };
        public Action OnUnchecked = () => { };

        public CheckBox()
        {
            _guid = CreateCheckBox();

            CheckBoxesDictionary[_guid] = this;
        }

        public bool Visibility
        {
            get => _visibility;
            set
            {
                _visibility = value;
                CheckBox_SetVisibility?.Invoke(_guid, value);
            }
        }

        public uint Width
        {
            get => _width;
            set
            {
                _width = value;
                CheckBox_SetWidth?.Invoke(_guid, value);
            }
        }

        public uint Height
        {
            get => _height;
            set
            {
                _height = value;
                CheckBox_SetHeight?.Invoke(_guid, value);
            }
        }

        public bool IsEnabled
        {
            get => _isEnabled;
            set
            {
                _isEnabled = value;
                CheckBox_SetIsEnabled?.Invoke(_guid, value);
            }
        }

        public bool IsChecked
        {
            get => _isChecked;
            set
            {
                _isChecked = value;
                CheckBox_SetIsChecked?.Invoke(_guid, value);
            }
        }

        public Guid _guid { get; }
    }

    public class ToggleSwitch
    {
        private uint _height;
        private bool _isChecked = true;
        private bool _isEnabled = true;

        private bool _visibility = true;

        private uint _width;

        public Action OnChecked = () => { };
        public Action OnUnchecked = () => { };

        public ToggleSwitch()
        {
            _guid = CreateToggleSwitch();

            ToggleSwitchesDictionary[_guid] = this;
        }

        public bool Visibility
        {
            get => _visibility;
            set
            {
                _visibility = value;
                ToggleSwitch_SetVisibility?.Invoke(_guid, value);
            }
        }

        public uint Width
        {
            get => _width;
            set
            {
                _width = value;
                ToggleSwitch_SetWidth?.Invoke(_guid, value);
            }
        }

        public uint Height
        {
            get => _height;
            set
            {
                _height = value;
                ToggleSwitch_SetHeight?.Invoke(_guid, value);
            }
        }

        public bool IsEnabled
        {
            get => _isEnabled;
            set
            {
                _isEnabled = value;
                ToggleSwitch_SetIsEnabled?.Invoke(_guid, value);
            }
        }

        public bool IsChecked
        {
            get => _isChecked;
            set
            {
                _isChecked = value;
                ToggleSwitch_SetIsChecked?.Invoke(_guid, value);
            }
        }

        public Guid _guid { get; }
    }

    public class TextBox
    {
        private uint _height;

        private string _text = "";

        private bool _visibility = true;

        private uint _width;

        public Action<string> OnEnterKeyDown = _ => { };

        public TextBox()
        {
            _guid = CreateTextBox();

            TextBoxesDictionary[_guid] = this;
        }

        public bool Visibility
        {
            get => _visibility;
            set
            {
                _visibility = value;
                TextBox_SetVisibility?.Invoke(_guid, value);
            }
        }

        public uint Width
        {
            get => _width;
            set
            {
                _width = value;
                TextBox_SetWidth?.Invoke(_guid, value);
            }
        }

        public uint Height
        {
            get => _height;
            set
            {
                _height = value;
                TextBox_SetHeight?.Invoke(_guid, value);
            }
        }

        public string Text
        {
            get => _text;
            set
            {
                _text = value;
                TextBox_SetText?.Invoke(_guid, value);
            }
        }

        public Guid _guid { get; }
    }

    public class ProgressRing
    {
        private uint _height;

        private int _progress;

        private bool _visibility = true;

        private uint _width;

        public ProgressRing()
        {
            _guid = CreateProgressRing();
        }

        public bool Visibility
        {
            get => _visibility;
            set
            {
                _visibility = value;
                ProgressRing_SetVisibility?.Invoke(_guid, value);
            }
        }

        public uint Width
        {
            get => _width;
            set
            {
                _width = value;
                ProgressRing_SetWidth?.Invoke(_guid, value);
            }
        }

        public uint Height
        {
            get => _height;
            set
            {
                _height = value;
                ProgressRing_SetHeight?.Invoke(_guid, value);
            }
        }

        public int Progress
        {
            get => _progress;
            set
            {
                _progress = value;
                ProgressRing_SetProgress?.Invoke(_guid, value);
            }
        }

        public Guid _guid { get; }
    }

    public class ProgressBar
    {
        private uint _height;

        private int _progress;
        private bool _showError;
        private bool _showPaused;

        private bool _visibility = true;

        private uint _width;

        public ProgressBar()
        {
            _guid = CreateProgressBar();
        }

        public bool Visibility
        {
            get => _visibility;
            set
            {
                _visibility = value;
                ProgressBar_SetVisibility?.Invoke(_guid, value);
            }
        }

        public uint Width
        {
            get => _width;
            set
            {
                _width = value;
                ProgressBar_SetWidth?.Invoke(_guid, value);
            }
        }

        public uint Height
        {
            get => _height;
            set
            {
                _height = value;
                ProgressBar_SetHeight?.Invoke(_guid, value);
            }
        }

        public int Progress
        {
            get => _progress;
            set
            {
                _progress = value;
                ProgressBar_SetProgress?.Invoke(_guid, value);
            }
        }

        public bool ShowPaused
        {
            get => _showPaused;
            set
            {
                _showPaused = value;
                ProgressBar_SetShowPaused?.Invoke(_guid, value);
            }
        }

        public bool ShowError
        {
            get => _showError;
            set
            {
                _showError = value;
                ProgressBar_SetShowError?.Invoke(_guid, value);
            }
        }

        public Guid _guid { get; }
    }

    #endregion

    // Helpers

    #region Helpers

    // Management Helpers

    public static Action<Guid, bool>? TextBlock_SetVisibility;
    public static Action<Guid, bool>? TextBlock_SetIsPrimary;
    public static Action<Guid, uint>? TextBlock_SetWidth;
    public static Action<Guid, uint>? TextBlock_SetHeight;
    public static Action<Guid, string>? TextBlock_SetText;

    public static Action<Guid, bool>? Button_SetVisibility;
    public static Action<Guid, uint>? Button_SetWidth;
    public static Action<Guid, uint>? Button_SetHeight;
    public static Action<Guid, bool>? Button_SetIsEnabled;
    public static Action<Guid, string>? Button_SetContent;

    public static Action<Guid, bool>? NumberBox_SetVisibility;
    public static Action<Guid, uint>? NumberBox_SetWidth;
    public static Action<Guid, uint>? NumberBox_SetHeight;
    public static Action<Guid, bool>? NumberBox_SetIsEnabled;
    public static Action<Guid, int>? NumberBox_SetValue;

    public static Action<Guid, bool>? ComboBox_SetVisibility;
    public static Action<Guid, uint>? ComboBox_SetWidth;
    public static Action<Guid, uint>? ComboBox_SetHeight;
    public static Action<Guid, bool>? ComboBox_SetIsEnabled;
    public static Action<Guid, uint>? ComboBox_SetSelectedIndex;
    public static Action<Guid, List<string>>? ComboBox_SetItems;

    public static Action<Guid, bool>? CheckBox_SetVisibility;
    public static Action<Guid, uint>? CheckBox_SetWidth;
    public static Action<Guid, uint>? CheckBox_SetHeight;
    public static Action<Guid, bool>? CheckBox_SetIsEnabled;
    public static Action<Guid, bool>? CheckBox_SetIsChecked;

    public static Action<Guid, bool>? ToggleSwitch_SetVisibility;
    public static Action<Guid, uint>? ToggleSwitch_SetWidth;
    public static Action<Guid, uint>? ToggleSwitch_SetHeight;
    public static Action<Guid, bool>? ToggleSwitch_SetIsEnabled;
    public static Action<Guid, bool>? ToggleSwitch_SetIsChecked;

    public static Action<Guid, bool>? TextBox_SetVisibility;
    public static Action<Guid, uint>? TextBox_SetWidth;
    public static Action<Guid, uint>? TextBox_SetHeight;
    public static Action<Guid, string>? TextBox_SetText;

    public static Action<Guid, bool>? ProgressRing_SetVisibility;
    public static Action<Guid, uint>? ProgressRing_SetWidth;
    public static Action<Guid, uint>? ProgressRing_SetHeight;
    public static Action<Guid, int>? ProgressRing_SetProgress;

    public static Action<Guid, bool>? ProgressBar_SetVisibility;
    public static Action<Guid, uint>? ProgressBar_SetWidth;
    public static Action<Guid, uint>? ProgressBar_SetHeight;
    public static Action<Guid, int>? ProgressBar_SetProgress;
    public static Action<Guid, bool>? ProgressBar_SetShowPaused;
    public static Action<Guid, bool>? ProgressBar_SetShowError;

    // Creation helpers

    public static Func<string, Guid>? CreateTextBlock;
    public static Func<string, Guid>? CreateButton;
    public static Func<int, Guid>? CreateNumberBox;
    public static Func<List<string>, Guid>? CreateComboBox;
    public static Func<Guid>? CreateCheckBox;
    public static Func<Guid>? CreateToggleSwitch;
    public static Func<Guid>? CreateTextBox;
    public static Func<Guid>? CreateProgressRing;
    public static Func<Guid>? CreateProgressBar;

    // Handler helpers

    public static Dictionary<Guid, Button> ButtonsDictionary = new();
    public static Dictionary<Guid, NumberBox> NumberBoxesDictionary = new();
    public static Dictionary<Guid, ComboBox> ComboBoxesDictionary = new();
    public static Dictionary<Guid, CheckBox> CheckBoxesDictionary = new();
    public static Dictionary<Guid, ToggleSwitch> ToggleSwitchesDictionary = new();
    public static Dictionary<Guid, TextBox> TextBoxesDictionary = new();

    public static void ExecuteButtonHandler(Guid guid)
    {
        ButtonsDictionary[guid].OnClick.Invoke();
    }

    public static void ExecuteNumberBoxHandler(Guid guid, int value)
    {
        NumberBoxesDictionary[guid].Value = value;
        NumberBoxesDictionary[guid].OnValueChanged.Invoke(value);
    }

    public static void ExecuteComboBoxHandler(Guid guid, uint index)
    {
        ComboBoxesDictionary[guid].SelectedIndex = index;
        ComboBoxesDictionary[guid].OnSelectionChanged.Invoke(index);
    }

    public static void ExecuteCheckBoxHandler(Guid guid, bool check)
    {
        CheckBoxesDictionary[guid].IsChecked = check;
        if (check)
            CheckBoxesDictionary[guid].OnChecked.Invoke();
        else
            CheckBoxesDictionary[guid].OnUnchecked.Invoke();
    }

    public static void ExecuteToggleSwitchHandler(Guid guid, bool check)
    {
        ToggleSwitchesDictionary[guid].IsChecked = check;
        if (check)
            ToggleSwitchesDictionary[guid].OnChecked.Invoke();
        else
            ToggleSwitchesDictionary[guid].OnUnchecked.Invoke();
    }

    public static void ExecuteTextBoxHandler(Guid guid, string text)
    {
        TextBoxesDictionary[guid].Text = text;
        TextBoxesDictionary[guid].OnEnterKeyDown.Invoke(text);
    }

    #endregion
}

public enum TrackingDeviceCharacteristics
{
    // Not set???
    K2_Character_Unknown,

    // NO mathbased, only [ head, waist, ankles ]
    K2_Character_Basic,

    // SUP mathbased, only [ head, elbows, waist, knees, ankles, foot_tips ]
    K2_Character_Simple,

    // SUP mathbased, [ everything ]
    K2_Character_Full
}

public enum TrackedJointState
{
    State_NotTracked,
    State_Inferred,
    State_Tracked
}

public enum LogSeverity
{
    INFO, // 0
    WARN, // 1
    ERROR // 2
}