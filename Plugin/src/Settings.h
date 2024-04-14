#pragma once
#include "Offsets.h"
#include "RE/Buffers.h"

#include "reshade/reshade.hpp"

#define CONFIG_ENTRY ""
#include "DKUtil/Config.hpp"

#include <d3d12.h>

// TODO: set to false in release builds (use the build configuation to automatically define it)
#define DEVELOPMENT 0

namespace Settings
{
    using namespace DKUtil::Alias;

    enum class SettingID : unsigned int
    {
		kSTART = 600,

		kDisplayMode,
		kForceSDROnHDR,
		kHDR_PeakBrightness,
		kHDR_GamePaperWhite,
		kHDR_UIPaperWhite,
		kHDR_ExtendGamut,

		kSecondaryBrightness,

		kToneMapperType,
		kToneMapperSaturation,
		kToneMapperContrast,
		kToneMapperHighlights,
		kToneMapperShadows,
		kToneMapperBloom,

		kColorGradingStrength,
		kLUTCorrectionStrength,
		kVanillaMenuLUTs,
		kStrictLUTApplication,

		kGammaCorrectionStrength,
		kFilmGrainType,
		kFilmGrainFPSLimit,
		kPostSharpen,
		kHDRScreenshots,
		kHDRScreenshotsLossless,

		kEND,

		kDevSetting01,
		kDevSetting02,
		kDevSetting03,
		kDevSetting04,
		kDevSetting05,
    };

	class Setting
	{
	public:
		SettingID id;
	    std::string name;
		std::string description;

		Setting(SettingID a_id, const std::string& a_name, const std::string& a_description) :
			id(a_id), name(a_name), description(a_description)
		{}

		virtual ~Setting() = default;
		virtual bool IsDefault() const = 0;
        
	};

	class Checkbox : public Setting
	{
	public:
	    Boolean value;
		bool defaultValue;

		Checkbox(SettingID a_id, const std::string& a_name, const std::string& a_description, const std::string& a_key, const std::string& a_section, bool a_defaultValue) :
			Setting{ a_id, a_name, a_description }, value{ a_key, a_section }, defaultValue(a_defaultValue) {}

		bool IsDefault() const override { return value.get_data() == defaultValue; }
	};

	class Stepper : public Setting
	{
	public:
		Integer value;
		int32_t defaultValue;

		Stepper(SettingID a_id, const std::string& a_name, const std::string& a_description, const std::string& a_key, const std::string& a_section, int32_t a_defaultValue) :
			Setting{ a_id, a_name, a_description }, value{ a_key, a_section }, defaultValue(a_defaultValue) {}

		bool IsDefault() const override { return value.get_data() == defaultValue; }

		virtual std::string GetStepperText(int32_t a_value) const = 0;
		virtual int32_t GetNumOptions() const = 0;
		virtual int32_t GetValueFromStepper(int32_t a_value) const = 0;
		virtual int32_t GetCurrentStepFromValue() const = 0;
		virtual void SetValueFromStepper(int32_t a_value) = 0;
	};

	class EnumStepper : public Stepper
	{
	public:
		std::vector<std::string> optionNames;

		EnumStepper(SettingID a_id, const std::string& a_name, const std::string& a_description, const std::string& a_key, const std::string& a_section, int32_t a_defaultValue, const std::vector<std::string>& a_optionNames) :
			Stepper{ a_id, a_name, a_description, a_key, a_section, a_defaultValue }, optionNames(a_optionNames) {}

		std::string GetStepperText(int32_t a_value) const override;
		int32_t GetNumOptions() const override{ return optionNames.size(); }
		int32_t GetValueFromStepper(int32_t a_value) const override { return a_value; }
		int32_t GetCurrentStepFromValue() const override { return value.get_data(); }
		void SetValueFromStepper(int32_t a_value) override { *value = a_value; }
	};

	class ValueStepper : public Stepper
	{
	public:
	    int32_t minValue;
		int32_t maxValue;
		int32_t stepSize;

		ValueStepper(SettingID a_id, const std::string& a_name, const std::string& a_description, const std::string& a_key, const std::string& a_section, int32_t a_defaultValue, int32_t a_minValue, int32_t a_maxValue, int32_t a_stepSize) :
			Stepper{ a_id, a_name, a_description, a_key, a_section, a_defaultValue }, minValue(a_minValue), maxValue(a_maxValue), stepSize(a_stepSize) {}

		std::string GetStepperText(int32_t a_value) const override;
		int32_t GetNumOptions() const override { return (maxValue - minValue) / stepSize + 1; }
		int32_t GetValueFromStepper(int32_t a_value) const override { return a_value * stepSize + minValue; }
		int32_t GetCurrentStepFromValue() const override { return (value.get_data() - minValue) / stepSize; }
		void SetValueFromStepper(int32_t a_value) override { *value = GetValueFromStepper(a_value); }
	};

	class Slider : public Setting
	{
	public:
	    Double value;
		float  defaultValue;
		float sliderMin;
		float sliderMax;
		std::string suffix = "";

		Slider(SettingID a_id, const std::string& a_name, const std::string& a_description, const std::string& a_key, const std::string& a_section, float a_defaultValue, float a_sliderMin, float a_sliderMax, std::string_view a_suffix = "") :
			Setting{ a_id, a_name, a_description }, value{ a_key, a_section }, defaultValue(a_defaultValue), sliderMin(a_sliderMin), sliderMax(a_sliderMax), suffix(a_suffix) {}

		bool IsDefault() const override { return value.get_data() == defaultValue; }

		float GetSliderPercentage() const;
		std::string GetSliderText() const;
		float GetValueFromSlider(float a_percentage) const;
		void SetValueFromSlider(float a_percentage);
	};

	// Has to match StructHdrDllPluginConstants in HLSL.
	// Bools are set as uint to avoid padding inconsistencies between c++ and hlsl.
	struct ShaderConstants
	{
		int32_t  DisplayMode;
		float    PeakBrightness;
		float    GamePaperWhite;
		float    UIPaperWhite;
		float    ExtendGamut;

		float    SDRSecondaryBrightness;

		uint32_t ToneMapperType;
		float    Saturation;
		float    Contrast;
		float    Highlights;
		float    Shadows;
		float    Bloom;

		float    ColorGradingStrength;
		float    LUTCorrectionStrength;
		uint32_t StrictLUTApplication;

		float    GammaCorrectionStrength;
		uint32_t FilmGrainType;
		float    FilmGrainFPSLimit;
		uint32_t PostSharpen;
		uint32_t bIsAtEndOfFrame;
		uint32_t RuntimeMS;
		float    DevSetting01;
		float    DevSetting02;
		float    DevSetting03;
		float    DevSetting04;
		float    DevSetting05;
	};
	constexpr static uint32_t shaderConstantsCount = sizeof(ShaderConstants) / sizeof(uint32_t); // Number of dwords

    class Main : public DKUtil::model::Singleton<Main>
    {
    public:
		EnumStepper DisplayMode {
		    SettingID::kDisplayMode,
		    "显示模式",
		    "将游戏的显示模式设置为 SDR (Gamma 2.2 Rec.709)、HDR10 BT.2020 PQ 或 HDR scRGB。"
					"\n"
					"\nHDR scRGB 提供最高质量，但与 DLSS 帧生成等技术不兼容。"
					"\n"
					"\n如果打开“帧生成”，无论此设置如何，scRGB 都会在内部回退到 HDR10。"
					"\n"
					"\nLuma - 原生 HDR 及更多自定义 [v1.10.32.0] [JuiJ 汉化] [2024-04-14]"
					"\n汉化更新 - https://juij.fun/#Starfield_Mod_Luma"
					"\n公众号更新 - 非线性列车",
		    "DisplayMode", "Main",
		    0,
		    { "SDR", "HDR10", "HDR scRGB" }
		};
		Checkbox ForceSDROnHDR{
			SettingID::kForceSDROnHDR,
			"在 scRGB HDR 上强制 SDR",
			"启用后，游戏仍会将色调映射到 SDR，但在 HDR scRGB 交换链上输出。",
			"ForceSDROnHDR", "Dev",
			false
		};
		ValueStepper PeakBrightness{
			SettingID::kHDR_PeakBrightness,
			"峰值亮度",
			"设置 HDR 模式下的峰值亮度"
				"\n"
				"\n该值应与显示器的峰值亮度相匹配"
				"\n"
				"\n这不会影响游戏的平均亮度",
			"PeakBrightness", "HDR",
			1000,
			80,
			10000,
			10
		};
		ValueStepper GamePaperWhite{
			SettingID::kHDR_GamePaperWhite,
			"游戏纸张白",
			"设置 HDR 模式下游戏中的白色亮度"
				"\n"
				"\n此设置代表游戏中白纸的亮度（100\% 漫反射白色）"
				"\n"
				"\n默认值为 200",
			"GamePaperWhite", "HDR",
			200, /*ITU reference default is 203 but we don't want to confuse users*/
			80,
			500,
			10
		};
		ValueStepper UIPaperWhite{
			SettingID::kHDR_UIPaperWhite,
			"UI 纸张白",
			"设置 HDR 模式下的用户界面亮度"
				"\n"
				"\n此设置代表 UI 元素的亮度"
				"\n"
				"\n默认值为 200",
			"UIPaperWhite", "HDR",
			200, /*ITU reference default is 203 but we don't want to confuse users*/
			80,
			500,
			10
		};
		Slider ExtendGamut{
			SettingID::kHDR_ExtendGamut,
			"扩展色域",
			"将明亮饱和颜色从 SDR 转变为 HDR，本质上充当“智能”饱和度。"
				"\n"
				"\n默认为 0\%",
			"ExtendGamut", "HDR",
			33.333f,
			0.f,
			100.f,
			"%"
		};

		Slider SecondaryBrightness{
			SettingID::kSecondaryBrightness,
			"亮度",
			"调节 SDR 模式中的亮度"
				"\n"
				"\n默认为 50\%",
			"SecondaryBrightness", "Main",
			50.f,
			0.f,
			100.f,
			"%"
		};

		EnumStepper ToneMapperType{
			SettingID::kToneMapperType,
			"色调映射器",
			"选择色调映射器"
				"\n"
				"\n原生增强 提供了原生色调映射器以实现 HDR 体验"
				"\n"
				"\nOpenDRT 是一个可定制的 SDR 和 HDR 色调映射器，经过修改以复制原生色调。",
			"ToneMapperType", "ToneMapper",
			0,
			{ "原生增强", "OpenDRT" }
		};
		Slider Saturation{
			SettingID::kToneMapperSaturation,
			"饱和度",
			"设置色调映射器中的饱和度强度"
				"\n"
				"\n默认为 50\%",
			"Saturation", "ToneMapper",
			50.f,
			0.f,
			100.f,
			"%"
		};
		Slider Contrast{
			SettingID::kToneMapperContrast,
			"对比度",
			"设置色调映射器中的对比度强度"
				"\n"
				"\n默认为 50\%",
			"Contrast", "ToneMapper",
			50.f,
			0.f,
			100.f,
			"%"
		};
		Slider Highlights{
			SettingID::kToneMapperHighlights,
			"高光",
			"设置色调映射器中的高光强度"
				"\n"
				"\n默认为 50\%",
			"Highlights", "ToneMapper",
			50.f,
			0.f,
			100.f,
			"%"
		};
		Slider Shadows{
			SettingID::kToneMapperShadows,
			"阴影",
			"设置色调映射器中的阴影强度"
				"\n"
				"\n默认为 50\%",
			"Shadows", "ToneMapper",
			50.f,
			0.f,
			100.f,
			"%"
		};
		Slider Bloom{
			SettingID::kToneMapperBloom,
			"泛光",
			"设置色调映射器中的泛光强度"
				"\n"
				"\n默认为 50\%",
			"Bloom", "ToneMapper",
			50.f,
			0.f,
			100.f,
			"%"
		};

		Slider ColorGradingStrength{
			SettingID::kColorGradingStrength,
			"颜色分级强度",
			"设置用于应用游戏色调的游戏颜色分级强度",
			"ColorGradingStrength", "Main",
			100.f,
			0.f,
			100.f,
			"%"
		};
		Slider LUTCorrectionStrength{
			SettingID::kLUTCorrectionStrength,
			"颜色分级范围",
			"将颜色分级 LUT 扩展到全范围"
				"\n"
				"\n增加将消除阴影中存在的低对比度雾以及由限制 LUT 引起的高光中亮度限制",
			"LUTCorrectionStrength", "Main",
			100.f,
			0.f,
			100.f,
			"%"
		};
		Checkbox VanillaMenuLUTs{
			SettingID::kVanillaMenuLUTs,
			"原生菜单颜色分级",
			"启用后，菜单使用原生颜色分级，并且不受 “颜色分级强度” 和 “颜色分级范围” 设置的影响。",
			"VanillaMenuLUTs", "Main",
			true
		};
		Checkbox StrictLUTApplication{
			SettingID::kStrictLUTApplication,
			"严格颜色分级",
			"以更类似于原生 SDR 色调的方式应用颜色分级，保留更多 HDR 色调。",
			"StrictLUTApplication", "HDR",
			false
		};

		Slider GammaCorrectionStrength{
			SettingID::kGammaCorrectionStrength,
			"伽玛校正",
			"设置伽马校正强度"
				"\n"
				"\n该游戏使用 sRGB 伽玛公式，但在伽玛 2.2 显示器上进行了校准。"
				"\n这主要影响接近黑色的颜色，如果不使用，可能会导致黑色突兀。"
				"\n"
				"\n100\% 应该符合预期的原生色调"
				"\n在 SDR 中，Luma 旨在 伽玛 2.2 显示器上游玩。",
			"GammaCorrectionStrength", "Main",
			100.f,
			0.f,
			100.f,
			"%"
		};
		EnumStepper FilmGrainType{
			SettingID::kFilmGrainType,
			"胶片颗粒类型",
			"设置胶片颗粒类型"
				"\n"
				"\n感知应用基于真实电影中颗粒感感知方式的电影颗粒。"
				"\n"
				"\n值得注意的是，感知不会提高黑色或使高光变色。",
			"FilmGrainType", "Main",
			1,
			{ "原生", "感知" }
		};
		Slider FilmGrainFPSLimit{
			SettingID::kFilmGrainFPSLimit,
			"胶片颗粒 FPS 限制",
			"允许对感知胶片颗粒进行帧限制以抵消运动敏感性，"
				"\n"
				"\n使用 0 表示无上限的胶片颗粒帧速率。",
			"FilmGrainFPSLimit", "Main",
			0.f,
			0.f,
			100.f
		};
		Checkbox PostSharpen{
			SettingID::kPostSharpen,
			"后期锐化",
			"切换游戏默认的后期锐化锐化处理"
				"\n默认情况下，这是在某些锐化/放大方法之后运行的（强制启用）。",
			"PostSharpen", "Main",
			true
		};
		Checkbox HDRScreenshots{
			SettingID::kHDRScreenshots,
			"HDR 截图",
			"在 HDR 下使用照片模式时，捕获额外的 HDR 屏幕截图 (.jxr)。",
			"HDRScreenshots", "HDR",
			true
		};
		Checkbox HDRScreenshotsLossless{
			SettingID::kHDRScreenshotsLossless,
			"无损截图",
			"启用无损参数保存 HDR 截图。这会大大增加文件大小，但不会有明显差别。",
			"HDRScreenshotsLossless", "HDR",
			false
		};
		Slider DevSetting01{ SettingID::kDevSetting01, "DevSetting01", "Development setting", "DevSetting01", "Dev", 0.f, 0.f, 100.f };
		Slider DevSetting02{ SettingID::kDevSetting02, "DevSetting02", "Development setting", "DevSetting02", "Dev", 0.f, 0.f, 100.f };
		Slider DevSetting03{ SettingID::kDevSetting03, "DevSetting03", "Development setting", "DevSetting03", "Dev", 0.f, 0.f, 100.f };
		Slider DevSetting04{ SettingID::kDevSetting04, "DevSetting04", "Development setting", "DevSetting04", "Dev", 50.f, 0.f, 100.f };
		Slider DevSetting05{ SettingID::kDevSetting05, "DevSetting05", "Development setting", "DevSetting05", "Dev", 50.f, 0.f, 100.f };
		String RenderTargetsToUpgrade{ "RenderTargetsToUpgrade", "RenderTargets" };

		Boolean PeakBrightnessAutoDetected { "PeakBrightnessAutoDetected", "HDR" };

		bool InitCompatibility(RE::BGSSwapChainObject* a_swapChainObject);
		void RefreshHDRDisplaySupportState();
		void RefreshHDRDisplayEnableState();

		bool IsHDRSupported() const { return bIsHDRSupported; }
		bool IsSDRForcedOnHDR(bool bAcknowledgeScreenshots = false) const;
		bool IsDisplayModeSetToHDR() const;
		bool IsGameRenderingSetToHDR(bool bAcknowledgeScreenshots = false) const;
		bool IsCustomToneMapper() const;
		bool IsFilmGrainTypeImproved() const;

		void SetAtEndOfFrame(bool a_bIsAtEndOfFrame) { bIsAtEndOfFrame.store(a_bIsAtEndOfFrame); }

		RE::BGSSwapChainObject* GetSwapChainObject() const { return swapChainObject; }
		int32_t GetActualDisplayMode(bool bAcknowledgeScreenshots = false, std::optional<RE::FrameGenerationTech> a_frameGenerationTech = std::nullopt) const;
		RE::BS_DXGI_FORMAT GetDisplayModeFormat(std::optional<RE::FrameGenerationTech> a_frameGenerationTech = std::nullopt) const;
        DXGI_COLOR_SPACE_TYPE GetDisplayModeColorSpaceType() const;

		void RefreshSwapchainFormat(std::optional<RE::FrameGenerationTech> a_frameGenerationTech = std::nullopt);
		void OnDisplayModeChanged();

		void GetShaderConstants(ShaderConstants& a_outShaderConstants) const;

		void InitConfig(bool a_bIsSFSE);

		void RegisterReshadeOverlay();

        void Load() noexcept;
		void Save() noexcept;

		static void DrawReshadeSettings(reshade::api::effect_runtime*);

		std::atomic_bool bRequestedSDRScreenshot = false;
		std::atomic_bool bRequestedHDRScreenshot = false;

        std::atomic_bool bNeedsToRefreshFSR3 = false;

    private:
		TomlConfig sfseConfig = COMPILE_PROXY("Data\\SFSE\\Plugins\\Luma.toml");
		TomlConfig asiConfig = COMPILE_PROXY("Luma.toml");
		TomlConfig* config = nullptr;

		std::atomic_bool bIsAtEndOfFrame = false;
		std::atomic_bool bIsHDRSupported = false;
		std::atomic_bool bIsHDREnabled = false;

		RE::BGSSwapChainObject* swapChainObject = nullptr;

		bool bReshadeSettingsOverlayRegistered = false;
		bool bIsDLSSGTOFSR3Present = false;

		void DrawReshadeTooltip(const char* a_desc);
		bool DrawReshadeCheckbox(Checkbox& a_checkbox);
		bool DrawReshadeEnumStepper(EnumStepper& a_stepper);
		bool DrawReshadeValueStepper(ValueStepper& a_stepper);
		bool DrawReshadeSlider(Slider& a_slider);
		bool DrawReshadeResetButton(Setting& a_setting);
		void DrawReshadeSettings();
    };

	
}
