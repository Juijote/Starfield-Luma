#include "Hooks.h"
#include "Offsets.h"
#include "Utils.h"
#include "reshade/reshade.hpp"

namespace Hooks
{
    RE::BufferDefinition* Patches::GetBufferFromString(std::string_view a_bufferName)
	{
		const auto& bufferArray = *Offsets::bufferArray;
		for (const auto& bufferDefinition : bufferArray) {
		    if (bufferDefinition->bufferName == a_bufferName) {
                return bufferDefinition;
            }
		}

		return nullptr;
	}

    void Hooks::ToggleEnableHDRSubSettings(RE::SettingsDataModel* a_model, bool a_bDisplayModeHDREnable, bool a_bGameRenderingHDREnable)
    {
		if (const auto peakBrightnessSetting = a_model->FindSettingById(static_cast<int>(Settings::SettingID::kHDR_PeakBrightness))) {
			peakBrightnessSetting->m_Enabled.SetValue(a_bGameRenderingHDREnable);
		}

		if (const auto gamePaperWhiteSetting = a_model->FindSettingById(static_cast<int>(Settings::SettingID::kHDR_GamePaperWhite))) {
			gamePaperWhiteSetting->m_Enabled.SetValue(a_bDisplayModeHDREnable);
		}

		if (const auto uiPaperWhiteSetting = a_model->FindSettingById(static_cast<int>(Settings::SettingID::kHDR_UIPaperWhite))) {
			uiPaperWhiteSetting->m_Enabled.SetValue(a_bGameRenderingHDREnable);
		}

		if (const auto saturation = a_model->FindSettingById(static_cast<int>(Settings::SettingID::kHDR_Saturation))) {
			saturation->m_Enabled.SetValue(a_bGameRenderingHDREnable);
		}

		if (const auto contrast = a_model->FindSettingById(static_cast<int>(Settings::SettingID::kHDR_Contrast))) {
			contrast->m_Enabled.SetValue(a_bGameRenderingHDREnable);
		}
		
		if (const auto secondaryBrightnessSetting = a_model->FindSettingById(static_cast<int>(Settings::SettingID::kSecondaryBrightness))) {
			secondaryBrightnessSetting->m_Enabled.SetValue(!a_bGameRenderingHDREnable);
		}
    }

    void Hooks::CreateCheckboxSetting(RE::ArrayNestedUIValue<RE::SubSettingsList::GeneralSetting, 0>* a_settingList, Settings::Checkbox& a_setting, bool a_bEnabled)
    {
		auto& s = *(new (alloca(sizeof(RE::SubSettingsList::GeneralSetting))) RE::SubSettingsList::GeneralSetting());

		s.m_Text.SetStringValue(a_setting.name.c_str());
		s.m_Description.SetStringValue(a_setting.description.c_str());
		s.m_ID.SetValue(static_cast<unsigned int>(a_setting.id));
		s.m_Type.SetValue(RE::SubSettingsList::GeneralSetting::Type::Checkbox);
		s.m_Category.SetValue(RE::SubSettingsList::GeneralSetting::Category::Display);
		s.m_Enabled.SetValue(a_bEnabled);
		s.m_CheckBoxData.m_ShuttleMap.GetData().m_Value.SetValue(a_setting.value.get_data());
		a_settingList->AddItem(s);
    }

    void Hooks::CreateStepperSetting(RE::ArrayNestedUIValue<RE::SubSettingsList::GeneralSetting, 0>* a_settingList, Settings::Stepper& a_setting, bool a_bEnabled)
    {
		auto& s = *(new (alloca(sizeof(RE::SubSettingsList::GeneralSetting))) RE::SubSettingsList::GeneralSetting());

		s.m_Text.SetStringValue(a_setting.name.c_str());
		s.m_Description.SetStringValue(a_setting.description.c_str());
		s.m_ID.SetValue(static_cast<unsigned int>(a_setting.id));
		s.m_Type.SetValue(RE::SubSettingsList::GeneralSetting::Type::Stepper);
		s.m_Category.SetValue(RE::SubSettingsList::GeneralSetting::Category::Display);
		s.m_Enabled.SetValue(a_bEnabled);
		for (auto i = 0; i < a_setting.GetNumOptions(); ++i) {
			s.m_StepperData.m_ShuttleMap.GetData().m_DisplayValues.AddItem(a_setting.GetStepperText(i).c_str());
		}
		s.m_StepperData.m_ShuttleMap.GetData().m_Value.SetValue(a_setting.GetCurrentStepFromValue());
		a_settingList->AddItem(s);
    }

    void Hooks::CreateSliderSetting(RE::ArrayNestedUIValue<RE::SubSettingsList::GeneralSetting, 0>* a_settingList, Settings::Slider& a_setting, bool a_bEnabled)
    {
		auto& s = *(new (alloca(sizeof(RE::SubSettingsList::GeneralSetting))) RE::SubSettingsList::GeneralSetting());

		s.m_Text.SetStringValue(a_setting.name.c_str());
		s.m_Description.SetStringValue(a_setting.description.c_str());
		s.m_ID.SetValue(static_cast<unsigned int>(a_setting.id));
		s.m_Type.SetValue(RE::SubSettingsList::GeneralSetting::Type::Slider);
		s.m_Category.SetValue(RE::SubSettingsList::GeneralSetting::Category::Display);
		s.m_Enabled.SetValue(a_bEnabled);
		s.m_SliderData.m_ShuttleMap.GetData().m_Value.SetValue(a_setting.GetSliderPercentage());
		s.m_SliderData.m_ShuttleMap.GetData().m_DisplayValue.SetStringValue(a_setting.GetSliderText().c_str());
		a_settingList->AddItem(s);
    }

    void Hooks::CreateSeparator(RE::ArrayNestedUIValue<RE::SubSettingsList::GeneralSetting, 0>* a_settingList, Settings::SettingID a_id)
    {
		auto& s = *(new (alloca(sizeof(RE::SubSettingsList::GeneralSetting))) RE::SubSettingsList::GeneralSetting());

		s.m_Text.SetStringValue("");
		s.m_Description.SetStringValue("");
		s.m_ID.SetValue(static_cast<unsigned int>(a_id));
		s.m_Type.SetValue(RE::SubSettingsList::GeneralSetting::Type::LargeStepper);
		s.m_Category.SetValue(RE::SubSettingsList::GeneralSetting::Category::Display);
		s.m_Enabled.SetValue(false);
		a_settingList->AddItem(s);
    }

    void Hooks::CreateSettings(RE::ArrayNestedUIValue<RE::SubSettingsList::GeneralSetting, 0>* a_settingList)
    {
		const auto settings = Settings::Main::GetSingleton();

		CreateSeparator(a_settingList, Settings::SettingID::kSTART);

		// We don't expose "DevSetting*" or "ForceSDROnHDR" to the game settings, they'd just confuse users
		CreateStepperSetting(a_settingList, settings->DisplayMode, settings->IsHDRSupported() && !settings->IsSDRForcedOnHDR());
		CreateStepperSetting(a_settingList, settings->PeakBrightness, settings->IsGameRenderingSetToHDR());
		CreateStepperSetting(a_settingList, settings->GamePaperWhite, settings->IsDisplayModeSetToHDR());
		CreateStepperSetting(a_settingList, settings->UIPaperWhite, settings->IsGameRenderingSetToHDR());
		CreateSliderSetting(a_settingList, settings->Saturation, settings->IsGameRenderingSetToHDR());
		CreateSliderSetting(a_settingList, settings->Contrast, settings->IsGameRenderingSetToHDR());
		CreateSliderSetting(a_settingList, settings->SecondaryBrightness, !settings->IsGameRenderingSetToHDR());
		CreateSliderSetting(a_settingList, settings->LUTCorrectionStrength, true);
		CreateSliderSetting(a_settingList, settings->ColorGradingStrength, true);
		CreateSliderSetting(a_settingList, settings->GammaCorrectionStrength, true);
		CreateCheckboxSetting(a_settingList, settings->VanillaMenuLUTs, true);
		CreateStepperSetting(a_settingList, settings->FilmGrainType, true);
		CreateCheckboxSetting(a_settingList, settings->PostSharpen, true);

		CreateSeparator(a_settingList, Settings::SettingID::kEND);
    }

	struct DescriptorAllocation
	{
		enum class Type : uint8_t
		{
			Sampler = 0,
			CbvSrvUav = 1,
		};

		D3D12_CPU_DESCRIPTOR_HANDLE CpuHandleBase;
		D3D12_GPU_DESCRIPTOR_HANDLE GpuHandleBase;
	};

	struct Dx12Resource // Setup at 00000001432E692F
	{
		char _pad0[0x78];												// 0
		int  m_DescriptorArrayCount;									// 78 Greater than 0 (msb bit) indicates array
		union
		{
			D3D12_CPU_DESCRIPTOR_HANDLE  m_CpuDescriptor;				// 80 Possibly mip levels? No idea w.r.t. its purpose
			D3D12_CPU_DESCRIPTOR_HANDLE* m_CpuDescriptorArray;			// 80
		};
		D3D12_CPU_DESCRIPTOR_HANDLE m_CBVCpuDescriptor;                 // 88 Each handle is unioned with an array like above?
		D3D12_CPU_DESCRIPTOR_HANDLE m_SRVCpuDescriptor;                 // 90
		D3D12_CPU_DESCRIPTOR_HANDLE m_UAVCpuDescriptor;					// 98
		D3D12_CPU_DESCRIPTOR_HANDLE m_UnknownSecondaryUAVCpuDescriptor;	// A0 Probably raytracing related
		ID3D12Resource*             m_Resource;							// A8
		uint32_t                    m_TransitionState;					// B0 Flags converted to D3D12_RESOURCE_STATES enum
	};
	static_assert(offsetof(Dx12Resource, m_DescriptorArrayCount) == 0x78);
	static_assert(offsetof(Dx12Resource, m_CBVCpuDescriptor) == 0x88);
	static_assert(offsetof(Dx12Resource, m_Resource) == 0xA8);

	thread_local Dx12Resource *ScaleformCompositeRenderTarget;

	void Hooks::HookedScaleformCompositeSetRenderTarget(void* a1, void* a2, void** a_rtArray, void* a4, void* a5, void* a6, void* a7, void* a8, void* a9)
	{
		ScaleformCompositeRenderTarget = *reinterpret_cast<Dx12Resource**>(reinterpret_cast<uintptr_t>(a_rtArray[0]) + 0x48);

		// Ignore render target sets. They're going to be overwritten anyway.
		//_ScaleformCompositeSetRenderTarget(a1, a2, a_rtArray, a4, a5, a6, a7, a8, a9);
	}

	void Hooks::HookedScaleformCompositeDraw(void* a_arg1, void* a_arg2, uint32_t a_vertexCount)
	{
		auto creationRendererInstance = *reinterpret_cast<uintptr_t*>(reinterpret_cast<uintptr_t>(a_arg1) + 0x0);
		auto device = *reinterpret_cast<ID3D12Device2**>(creationRendererInstance + 0x3A0);

		auto commandList = *reinterpret_cast<ID3D12GraphicsCommandList**>(reinterpret_cast<uintptr_t>(a_arg1) + 0x10);

		auto unknown1 = *reinterpret_cast<uintptr_t*>(reinterpret_cast<uintptr_t>(a_arg1) + 0x8);
		auto descriptorManager = *reinterpret_cast<void**>(unknown1 + 0x18);

		auto getBoundShaderResource = [&](uint32_t a_index) {
			const auto v1 = *reinterpret_cast<uintptr_t*>(reinterpret_cast<uintptr_t>(a_arg2) + 0x30);
			const auto v2 = *reinterpret_cast<uintptr_t*>(v1 + 0x10) + 24 * *(uint32_t*)(*(uintptr_t*)v1 + (4 * a_index) + 0x20);
			return *reinterpret_cast<Dx12Resource**>(v2 + 0x8);
		};

		using AllocateDescriptors_t = void (*)(void*, DescriptorAllocation&, uint32_t, DescriptorAllocation::Type, uint32_t&);
		auto allocateDescriptors = reinterpret_cast<AllocateDescriptors_t>(dku::Hook::IDToAbs(207691));

		if (Hook_ApplyRenderPassRenderState1(a_arg1, a_arg2)) {
			// Remove all render targets; we're treating this pixel shader as a compute shader. All RT writes end
			// up discarded.
			//
			// The ScaleformComposite pass contains exactly one draw so we can safely unbind them without informing
			// the game. Game code also happens to unbind RTs immediately after the hook.
			commandList->OMSetRenderTargets(0, nullptr, false, nullptr);

			// Instead of creating copies and worrying about resource allocations, bind the original RT and SRV as
			// UAVs that can be modified in-place.
			DescriptorAllocation alloc;
			uint32_t             handleSizeIncrement;
			allocateDescriptors(descriptorManager, alloc, 2, DescriptorAllocation::Type::CbvSrvUav, handleSizeIncrement);

			alloc.CpuHandleBase.ptr += (0 * handleSizeIncrement);
			device->CopyDescriptorsSimple(1, alloc.CpuHandleBase, getBoundShaderResource(0)->m_CpuDescriptor, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

			alloc.CpuHandleBase.ptr += (1 * handleSizeIncrement);
			device->CopyDescriptorsSimple(1, alloc.CpuHandleBase, ScaleformCompositeRenderTarget->m_UAVCpuDescriptor, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

			D3D12_RESOURCE_BARRIER barrier = {};
			barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
			barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
			barrier.Transition.pResource = ScaleformCompositeRenderTarget->m_Resource;
			barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
			barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
			barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_UNORDERED_ACCESS;

			commandList->ResourceBarrier(1, &barrier);
			commandList->SetGraphicsRootDescriptorTable(1, alloc.GpuHandleBase); // Hardcoded in ScaleformComposite\RootSignature.hlsl
			commandList->DrawInstanced(a_vertexCount, 1, 0, 0);
			std::swap(barrier.Transition.StateBefore, barrier.Transition.StateAfter);
			commandList->ResourceBarrier(1, &barrier);
		}
	}

    void Hooks::UploadRootConstants(void* a1, void* a2)
    {
		const auto technique = *reinterpret_cast<uintptr_t*>(reinterpret_cast<uintptr_t>(a2) + 0x8);
		const auto techniqueId = *reinterpret_cast<uint64_t*>(technique + 0x78);

		auto uploadRootConstants = [&](const Settings::ShaderConstants& a_shaderConstants, uint32_t a_rootParameterIndex, bool a_bCompute) {
			auto commandList = *reinterpret_cast<ID3D12GraphicsCommandList**>(reinterpret_cast<uintptr_t>(a1) + 0x10);

			if (!a_bCompute)
				commandList->SetGraphicsRoot32BitConstants(a_rootParameterIndex, Settings::shaderConstantsSize, &a_shaderConstants, 0);
			else
				commandList->SetComputeRoot32BitConstants(a_rootParameterIndex, Settings::shaderConstantsSize, &a_shaderConstants, 0);
		};

		// Note: The following switch statement may be called several thousand times per frame. Additionally, it'll be called from multiple
		// threads concurrently. The individual cases are called at most once or twice per frame. Keep the amount of code here fairly light.
		//
		// RootParameterIndex is the index of our custom RootConstants() entry in the root signature. It's taken from the corresponding
		// RootSignature.hlsl file stored next to each technique hlsl file.
		switch (techniqueId) {
		case 0xFF1A:
		case 0x600FF1A:
		case 0x700FF1A:
		case 0x800FF1A:
		case 0xE00FF1A:
		case 0xF00FF1A:
			{
				Settings::ShaderConstants shaderConstants;
				const auto settings = Settings::Main::GetSingleton();
				settings->GetShaderConstants(shaderConstants);
				if (*settings->VanillaMenuLUTs.value && !Utils::ShouldCorrectLUTs()) {
					shaderConstants.ColorGradingStrength = 1.f;
				}
				uploadRootConstants(shaderConstants, 14, false);  // HDRComposite
				break;
			}

		case 0x400FF59:
		//case 0x2000FF59:
			{
				Settings::ShaderConstants shaderConstants;
				Settings::Main::GetSingleton()->GetShaderConstants(shaderConstants);
				uploadRootConstants(shaderConstants, 2, false);  // Copy
				break;
			}

		case 0xFF75:
			{
				Settings::ShaderConstants shaderConstants;
				Settings::Main::GetSingleton()->GetShaderConstants(shaderConstants);
				uploadRootConstants(shaderConstants, 2, false);  // FilmGrain
				break;
			}

		case 0xFF81:
			{
				Settings::ShaderConstants shaderConstants;
				const auto settings = Settings::Main::GetSingleton();
				settings->GetShaderConstants(shaderConstants);
				if (*settings->VanillaMenuLUTs.value && !Utils::ShouldCorrectLUTs()) {
				    shaderConstants.LUTCorrectionStrength = 0.f;
				    shaderConstants.ColorGradingStrength = 1.f;
				}
				uploadRootConstants(shaderConstants, 7, true);  // ColorGradingMerge
				break;
			}

		//case 0xFF94:
		case 0x100FF94:
		case 0x300FF94:
			{
				Settings::ShaderConstants shaderConstants;
				Settings::Main::GetSingleton()->GetShaderConstants(shaderConstants);
				uploadRootConstants(shaderConstants, 14, true);  // ContrastAdaptiveSharpening
				break;
			}

		case 0xFF9A:
			{
				Settings::ShaderConstants shaderConstants;
				Settings::Main::GetSingleton()->GetShaderConstants(shaderConstants);
				uploadRootConstants(shaderConstants, 14, false);  // PostSharpen
				break;
			}

		case 0xFFAA:
			{
				Settings::ShaderConstants shaderConstants;
				Settings::Main::GetSingleton()->GetShaderConstants(shaderConstants);
				uploadRootConstants(shaderConstants, 2, false);  // ScaleformComposite
				break;
			}

		case 0xFFAB:
			{
				Settings::ShaderConstants shaderConstants;
				Settings::Main::GetSingleton()->GetShaderConstants(shaderConstants);
				uploadRootConstants(shaderConstants, 1, false);  // BinkMovie
				break;
			}
		}
    }

    void Hooks::Hook_UnkFunc(uintptr_t a1, RE::BGSSwapChainObject* a_bgsSwapchainObject)
    {
		const auto settings = Settings::Main::GetSingleton();
		settings->InitCompatibility(a_bgsSwapchainObject);

		a_bgsSwapchainObject->swapChainInterface->SetColorSpace1(settings->GetDisplayModeColorSpaceType());

		settings->RegisterReshadeOverlay();

		return _UnkFunc(a1, a_bgsSwapchainObject);		
    }

    bool Hooks::Hook_TakeSnapshot(uintptr_t a1)
    {
		const auto settings = Settings::Main::GetSingleton();
		//if (settings->IsDisplayModeSetToHDR()) {
		if (true) {  // actually it's always going to be the case because we're always going to update the image space buffer format
			auto  hack = alloca(sizeof(RE::MessageBoxData));
			auto& message = *(new (hack) RE::MessageBoxData("Photo Mode", "Taking screenshots with Photo Mode is not supported with Starfield Luma. Use an external tool (e.g. Xbox Game Bar) to take a screenshot.", nullptr, 0));
			Offsets::ShowMessageBox(*Offsets::MessageMenuManagerPtr, message, false);

			// hack to refresh the UI visibility after the snapshot
			Offsets::PhotoMode_ToggleUI(a1 + 0x8);
			Offsets::PhotoMode_ToggleUI(a1 + 0x8);

			return true;
		}
		
        return _TakeSnapshot(a1);
    }

    void Hooks::Hook_RecreateSwapchain(void* a1, RE::BGSSwapChainObject* a_bgsSwapChainObject, uint32_t a_width, uint32_t a_height, uint8_t a5)
    {
		_RecreateSwapchain(a1, a_bgsSwapChainObject, a_width, a_height, a5);

		const auto settings = Settings::Main::GetSingleton();
		a_bgsSwapChainObject->swapChainInterface->SetColorSpace1(settings->GetDisplayModeColorSpaceType());
    }

    void Hooks::Hook_CreateMonitorSetting(void* a1, void* a2)
    {
		_CreateMonitorSetting(a1, a2);

		// insert our settings after
		auto* settingList = reinterpret_cast<RE::ArrayNestedUIValue<RE::SubSettingsList::GeneralSetting, 0>*>(reinterpret_cast<uintptr_t>(a1) - 0x28);
		CreateSettings(settingList);
    }

    void Hooks::Hook_SettingsDataModelCheckboxChanged(void* a_arg1, RE::SettingsDataModel::UpdateEventData& a_eventData)
    {
		const auto settings = Settings::Main::GetSingleton();

		auto HandleSetting = [&](Settings::Checkbox& a_setting) {
			const auto prevValue = a_setting.value.get_data();
			const auto newValue = a_eventData.m_Value.Bool;
			if (prevValue != newValue) {
				*a_setting.value = newValue;
				settings->Save();
				return true;
			}
			return false;
		};

		switch (a_eventData.m_SettingID) {
		case static_cast<int>(Settings::SettingID::kForceSDROnHDR):
			{
				const bool wasDisplayModeHDR = settings->IsDisplayModeSetToHDR();
				const bool wasGameRenderingHDR = settings->IsGameRenderingSetToHDR();
				if (HandleSetting(settings->ForceSDROnHDR)) {
					const bool isDisplayModeHDR = settings->IsDisplayModeSetToHDR();
					const bool isGameRenderingHDR = settings->IsGameRenderingSetToHDR();
					if (wasDisplayModeHDR != isDisplayModeHDR || wasGameRenderingHDR != isGameRenderingHDR) {
						ToggleEnableHDRSubSettings(a_eventData.m_Model, isDisplayModeHDR, isGameRenderingHDR);
					}
					if (const auto displayModeSetting = a_eventData.m_Model->FindSettingById(static_cast<int>(Settings::SettingID::kDisplayMode))) {
						displayModeSetting->m_Enabled.SetValue(settings->IsHDRSupported() && !settings->IsSDRForcedOnHDR());
					}
					settings->OnDisplayModeChanged();
				}
			}
			break;
		case static_cast<int>(Settings::SettingID::kVanillaMenuLUTs):
			HandleSetting(settings->VanillaMenuLUTs);
			break;
		case static_cast<int>(Settings::SettingID::kPostSharpen):
			HandleSetting(settings->PostSharpen);
		    break;
		}

		_SettingsDataModelCheckboxChanged(a_arg1, a_eventData);
    }

    void Hooks::Hook_SettingsDataModelStepperChanged(void* a_arg1, RE::SettingsDataModel::UpdateEventData& a_eventData)
    {
		const auto settings = Settings::Main::GetSingleton();

		auto HandleSetting = [&](Settings::Stepper& a_setting) {
			const auto prevValue = a_setting.value.get_data();
			const auto newValue = a_setting.GetValueFromStepper(a_eventData.m_Value.Int);
			if (prevValue != newValue) {
				*a_setting.value = newValue;
				settings->Save();
				return true;
			}
			return false;
		};

		switch (a_eventData.m_SettingID) {
		case static_cast<int>(Settings::SettingID::kDisplayMode):
			{
				const bool wasDisplayModeHDR = settings->IsDisplayModeSetToHDR();
				const bool wasGameRenderingHDR = settings->IsGameRenderingSetToHDR();
				if (HandleSetting(settings->DisplayMode)) {
					const bool isDisplayModeHDR = settings->IsDisplayModeSetToHDR();
					const bool isGameRenderingHDR = settings->IsGameRenderingSetToHDR();
					if (wasDisplayModeHDR != isDisplayModeHDR || wasGameRenderingHDR != isGameRenderingHDR) {
						ToggleEnableHDRSubSettings(a_eventData.m_Model, isDisplayModeHDR, isGameRenderingHDR);
					}
					if (const auto displayModeSetting = a_eventData.m_Model->FindSettingById(static_cast<int>(Settings::SettingID::kDisplayMode))) {
						displayModeSetting->m_Enabled.SetValue(settings->IsHDRSupported() && !settings->IsSDRForcedOnHDR());
					}
					settings->OnDisplayModeChanged();
				}
			}
			break;
		case static_cast<int>(Settings::SettingID::kHDR_PeakBrightness):
			HandleSetting(settings->PeakBrightness);
			break;
		case static_cast<int>(Settings::SettingID::kHDR_GamePaperWhite):
			HandleSetting(settings->GamePaperWhite);
			break;
		case static_cast<int>(Settings::SettingID::kHDR_UIPaperWhite):
			HandleSetting(settings->UIPaperWhite);
			break;
		case static_cast<int>(Settings::SettingID::kFilmGrainType):
			HandleSetting(settings->FilmGrainType);
			break;
		}

		_SettingsDataModelStepperChanged(a_arg1, a_eventData);
    }

    bool Hooks::OnSettingsDataModelSliderChanged(RE::SettingsDataModel::UpdateEventData& a_eventData)
    {
		const auto settings = Settings::Main::GetSingleton();

		auto HandleSetting = [&](Settings::Slider& a_setting) {
			const auto prevValue = a_setting.value.get_data();
			const auto newValue = a_setting.GetValueFromSlider(a_eventData.m_Value.Float);

			if (prevValue != newValue) {
				*a_setting.value = newValue;
				settings->Save();
			}

			// Skip _SettingsDataModelSliderChanged and queue the update callback ourselves. Why, you ask? Bethesda had the
			// brilliant idea to hardcode slider option text values.
			struct
			{
				int            v1;         // 0
				float          v2;         // 4
				const char*    v3;         // 8
				unsigned int   v4 = 0;     // 10
				unsigned short v5 = 1024;  // 14
			} const callbackData = {
				.v1 = a_eventData.m_SettingID,
				.v2 = a_eventData.m_Value.Float,
				.v3 = a_setting.GetSliderText().c_str(),
			};

			const auto modelData = *reinterpret_cast<void**>(reinterpret_cast<uintptr_t>(a_eventData.m_Model) + 0x8);
			const auto func = reinterpret_cast<void (*)(void*, const void*)>(dku::Hook::IDToAbs(135746));

			if (modelData) {
				func(modelData, &callbackData);
			}
		};

		switch (a_eventData.m_SettingID) {
		case static_cast<int>(Settings::SettingID::kHDR_Saturation):
			HandleSetting(settings->Saturation);
			return true;
		case static_cast<int>(Settings::SettingID::kHDR_Contrast):
			HandleSetting(settings->Contrast);
			return true;
		case static_cast<int>(Settings::SettingID::kLUTCorrectionStrength):
			HandleSetting(settings->LUTCorrectionStrength);
			return true;
		case static_cast<int>(Settings::SettingID::kColorGradingStrength):
			HandleSetting(settings->ColorGradingStrength);
			return true;
		case static_cast<int>(Settings::SettingID::kGammaCorrectionStrength):
			HandleSetting(settings->GammaCorrectionStrength);
			return true;
		case static_cast<int>(Settings::SettingID::kSecondaryBrightness):
			HandleSetting(settings->SecondaryBrightness);
			return true;
		}

		return false;
    }

    void Hooks::Hook_SettingsDataModelSliderChanged1(RE::SettingsDataModel::UpdateEventData& a_eventData)
    {
		if (!OnSettingsDataModelSliderChanged(a_eventData)) {
			_SettingsDataModelSliderChanged1(a_eventData);
		}
    }

    void Hooks::Hook_SettingsDataModelSliderChanged2(RE::SettingsDataModel::UpdateEventData& a_eventData)
    {
		if (!OnSettingsDataModelSliderChanged(a_eventData)) {
			_SettingsDataModelSliderChanged2(a_eventData);
		}
    }

    bool Hooks::Hook_ApplyRenderPassRenderState1(void* a_arg1, void* a_arg2)
	{
		const bool result = _ApplyRenderPassRenderState1(a_arg1, a_arg2);

		if (result) {
			UploadRootConstants(a_arg1, a_arg2);
		}

		return result;
	}

    bool Hooks::Hook_ApplyRenderPassRenderState2(void* a_arg1, void* a_arg2)
    {
		const bool result = _ApplyRenderPassRenderState2(a_arg1, a_arg2);

		if (result) {
			UploadRootConstants(a_arg1, a_arg2);
		}

		return result;
    }

    void Hooks::Hook_EndOfFrame(void* a1, void* a2, const char* a3)
    {
		Settings::Main::GetSingleton()->SetAtEndOfFrame(true);
		_EndOfFrame(a1, a2, a3);
    }

    void Hooks::Hook_PostEndOfFrame(void* a1)
    {
		_PostEndOfFrame(a1);
		Settings::Main::GetSingleton()->SetAtEndOfFrame(false);

		if (Utils::IsInSettingsMenu()) // TODO: is really necessary? Was the HDR support state already refreshed e.g. when the user changed windowed mode?
		{
			Settings::Main::GetSingleton()->RefreshHDRSupportState();
		}
    }

    void Install()
	{
#ifndef NDEBUG
	    Utils::LogBuffers();
#endif
		Hooks::Hook();
		Patches::Patch();
	}
}
