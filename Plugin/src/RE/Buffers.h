#pragma once
#include <d3d12.h>
#include <dxgi1_6.h>

namespace RE
{
	enum class BS_DXGI_FORMAT
	{
		BS_DXGI_FORMAT_UNKNOWN_0 = 0,
		BS_DXGI_FORMAT_R8_UNORM = 1,
		BS_DXGI_FORMAT_R8_SNORM = 2,
		BS_DXGI_FORMAT_R8_UINT,
		BS_DXGI_FORMAT_R8_SINT,
		BS_DXGI_FORMAT_UNKNOWN_5,
		BS_DXGI_FORMAT_UNKNOWN_6,
		BS_DXGI_FORMAT_B4G4R4A4_UNORM,
		BS_DXGI_FORMAT_UNKNOWN_8,
		BS_DXGI_FORMAT_UNKNOWN_9,
		BS_DXGI_FORMAT_B5G6R5_UNORM_10,
		BS_DXGI_FORMAT_B5G6R5_UNORM_11,
		BS_DXGI_FORMAT_UNKNOWN_12,
		BS_DXGI_FORMAT_B5G5R5A1_UNORM,
		BS_DXGI_FORMAT_R8G8_UNORM,
		BS_DXGI_FORMAT_R8G8_SNORM,
		BS_DXGI_FORMAT_UNKNOWN_16,
		BS_DXGI_FORMAT_UNKNOWN_17,
		BS_DXGI_FORMAT_R8G8_UINT,
		BS_DXGI_FORMAT_R8G8_SINT,
		BS_DXGI_FORMAT_UNKNOWN_20,
		BS_DXGI_FORMAT_R16_UNORM,
		BS_DXGI_FORMAT_R16_SNORM,
		BS_DXGI_FORMAT_R16_UINT,
		BS_DXGI_FORMAT_R16_SINT,
		BS_DXGI_FORMAT_R16_FLOAT,
		BS_DXGI_FORMAT_UNKNOWN_26,
		BS_DXGI_FORMAT_UNKNOWN_27,
		BS_DXGI_FORMAT_UNKNOWN_28,
		BS_DXGI_FORMAT_UNKNOWN_29,
		BS_DXGI_FORMAT_UNKNOWN_30,
		BS_DXGI_FORMAT_UNKNOWN_31,
		BS_DXGI_FORMAT_UNKNOWN_32,
		BS_DXGI_FORMAT_UNKNOWN_33,
		BS_DXGI_FORMAT_UNKNOWN_34,
		BS_DXGI_FORMAT_UNKNOWN_35,
		BS_DXGI_FORMAT_R8G8B8A8_UNORM,
		BS_DXGI_FORMAT_R8G8B8A8_SNORM,
		BS_DXGI_FORMAT_R8G8B8A8_UINT,
		BS_DXGI_FORMAT_R8G8B8A8_SINT,
		BS_DXGI_FORMAT_R8G8B8A8_UNORM_SRGB,
		BS_DXGI_FORMAT_B8G8R8A8_UNORM,
		BS_DXGI_FORMAT_UNKNOWN_42,
		BS_DXGI_FORMAT_UNKNOWN_43,
		BS_DXGI_FORMAT_UNKNOWN_44,
		BS_DXGI_FORMAT_B8G8R8A8_UNORM_SRGB,
		BS_DXGI_FORMAT_UNKNOWN_46,
		BS_DXGI_FORMAT_B8G8R8X8_UNORM,
		BS_DXGI_FORMAT_R16G16_UNORM,
		BS_DXGI_FORMAT_UNKNOWN_49,
		BS_DXGI_FORMAT_R16G16_SNORM,
		BS_DXGI_FORMAT_UNKNOWN_51,
		BS_DXGI_FORMAT_R16G16_UINT,
		BS_DXGI_FORMAT_R16G16_SINT,
		BS_DXGI_FORMAT_R16G16_FLOAT,
		BS_DXGI_FORMAT_R32_UINT,
		BS_DXGI_FORMAT_R32_SINT,
		BS_DXGI_FORMAT_R32_FLOAT,
		BS_DXGI_FORMAT_UNKNOWN_58,
		BS_DXGI_FORMAT_UNKNOWN_59,
		BS_DXGI_FORMAT_UNKNOWN_60,
		BS_DXGI_FORMAT_UNKNOWN_61,
		BS_DXGI_FORMAT_R10G10B10A2_UNORM,
		BS_DXGI_FORMAT_R10G10B10A2_UINT,
		BS_DXGI_FORMAT_UNKNOWN_64,
		BS_DXGI_FORMAT_UNKNOWN_65,
		BS_DXGI_FORMAT_R11G11B10_FLOAT,
		BS_DXGI_FORMAT_R9G9B9E5_SHAREDEXP,
		BS_DXGI_FORMAT_UNKNOWN_68,
		BS_DXGI_FORMAT_UNKNOWN_69,
		BS_DXGI_FORMAT_UNKNOWN_70,
		BS_DXGI_FORMAT_UNKNOWN_71,
		BS_DXGI_FORMAT_UNKNOWN_72,
		BS_DXGI_FORMAT_R16G16B16A16_UNORM,
		BS_DXGI_FORMAT_R16G16B16A16_SNORM,
		BS_DXGI_FORMAT_R16G16B16A16_UINT,
		BS_DXGI_FORMAT_R16G16B16A16_SINT,
		BS_DXGI_FORMAT_R16G16B16A16_FLOAT,
		BS_DXGI_FORMAT_R32G32_UINT,
		BS_DXGI_FORMAT_R32G32_SINT,
		BS_DXGI_FORMAT_R32G32_FLOAT,
		BS_DXGI_FORMAT_R32G32B32_UINT,
		BS_DXGI_FORMAT_R32G32B32_SINT,
		BS_DXGI_FORMAT_R32G32B32_FLOAT,
		BS_DXGI_FORMAT_R32G32B32A32_UINT,
		BS_DXGI_FORMAT_R32G32B32A32_SINT,
		BS_DXGI_FORMAT_R32G32B32A32_FLOAT,
		BS_DXGI_FORMAT_UNKNOWN_87,
		BS_DXGI_FORMAT_UNKNOWN_88,
		BS_DXGI_FORMAT_UNKNOWN_89,
		BS_DXGI_FORMAT_UNKNOWN_90,
		BS_DXGI_FORMAT_UNKNOWN_91,
		BS_DXGI_FORMAT_UNKNOWN_92,
		BS_DXGI_FORMAT_UNKNOWN_93,
		BS_DXGI_FORMAT_UNKNOWN_94,
		BS_DXGI_FORMAT_UNKNOWN_95,
		BS_DXGI_FORMAT_UNKNOWN_96,
		BS_DXGI_FORMAT_UNKNOWN_97,
		BS_DXGI_FORMAT_UNKNOWN_98,
		BS_DXGI_FORMAT_D16_UNORM,
		BS_DXGI_FORMAT_D24_UNORM_S8_UINT,
		BS_DXGI_FORMAT_D32_FLOAT,
		BS_DXGI_FORMAT_D24_UNORM_S8_UINT_102,
		BS_DXGI_FORMAT_D24_UNORM_S8_UINT_103,
		BS_DXGI_FORMAT_D32_FLOAT_S8X24_UINT,
		BS_DXGI_FORMAT_BC1_UNORM_105,
		BS_DXGI_FORMAT_BC1_UNORM_SRGB_106,
		BS_DXGI_FORMAT_BC1_UNORM_107,
		BS_DXGI_FORMAT_BC1_UNORM_SRGB_108,
		BS_DXGI_FORMAT_BC2_UNORM,
		BS_DXGI_FORMAT_BC2_UNORM_SRGB,
		BS_DXGI_FORMAT_BC3_UNORM,
		BS_DXGI_FORMAT_BC3_UNORM_SRGB,
		BS_DXGI_FORMAT_BC4_UNORM,
		BS_DXGI_FORMAT_BC4_SNORM,
		BS_DXGI_FORMAT_BC5_UNORM,
		BS_DXGI_FORMAT_BC5_SNORM,
		BS_DXGI_FORMAT_BC6H_UF16,
		BS_DXGI_FORMAT_BC6H_SF16,
		BS_DXGI_FORMAT_BC7_UNORM,
		BS_DXGI_FORMAT_BC7_UNORM_SRGB
	};

	enum class Buffers
	{
		SF_ColorBuffer = 0,
		SF_StencilBuffer = 1,
		DepthBuffer,
		PostUpscaleDepthBuffer,
		TileDepthBoundsVS,
		PreviousDepthBufferDesc,
		DepthBufferDebug,
		TopDownDepthSector,
		ShadowMap,
		TerrainShadowMap,
		TerrainShadowMapBias,
		ShadowMaskDesc,
		ShadowMaskArrayDesc,
		ScaleformStencilBuffer,
		ScaleformToTextureStencilBuffer,
		Scaleform3DStencilBuffer,
		GBuffer_AlbedoMisc,
		GBuffer_Normal_EmissiveIntensity,
		GBuffer_AO_Rough_Metal,
		GBuffer_MV,
		GBuffer_Mask,
		GBuffer_Optional,
		LightingBufferY,
		LightingBufferUV,
		ContactShadowsBuffer,
		ContactShadowsSwapBuffer,
		RayTracedLightingDiffuseBuffer,
		IndirectLightingBufferFullRes,
		IndirectLightingBufferHalfRes,
		ColorBuffer01,
		ColorBufferUAV,
		NativeResolutionColorBuffer01,
		DistortionValueBuffer,
		DistortionBlurStrengthBuffer,
		DistortionColorBuffer,
		ImageSpaceBuffer,
		HDRImagespaceBuffer,
		ImageSpaceBufferR10G10B10A2,
		ImageSpaceBufferB10G11R11,
		ImageSpaceBufferE5B9G9R9,
		ImageSpaceBufferR16G16B16A16,
		ImageSpaceBufferR32G32B32A32,
		ImageSpaceHalfResBuffer,
		ImageSpaceHalfResBufferPreUpscale,
		ScaleformCompositeBuffer,
		ColorGradingLUT,
		LumaLowResPreUpscale,
		LumaLowResPostUpscale,
		BloomPyramidPreUpscale,
		BloomPyramidPostUpscale,
		HDRSingleValueAverageLum,
		SAOCameraZ,
		SAORawAO,
		SAOFinalAO,
		FrameBuffer,
		NullRenderTarget,
		SkyOcclusionMask,
		VisualizationBuffer,
		HeightClipmap,
		BlurHeightClipmap,
		ShoreClipmap,
		ShoreFlood,
		SplatClipmap,
		NormalClipmap,
		VErrorClipmap,
		MaterialIndexMap,
		ProcGenDensityMap,
		DownsampleOutputPrevFrame,
		DownsampleOutput,
		SobelOutput,
		SpaceGlareBlur,
		CGenTestTexture3D,
		DownsampleMipChainOutput,
		BlurredMipChainOutput,
		RefractionPrevFrame,
		DoFHTileMask,
		DoFCoCTarget,
		DoFColorTarget,
		DoFTAAHistoryTarget,
		TAA_idTech7HistoryColorTarget,
		TAA_idTech7HistoryDataTarget,
		RT_HighlightMask,
		RT_ColorGradeMask,
		SeparableSSSBufferY,
		SeparableSSSBufferUV,
		SHLUTAngle_High,
		SHLUTAngle_Mid,
		SHLUTAngle_Low,
		SHLUTAngleProbe,
		EnvBRDF,
		BinkYPlane,
		BinkcRcBPlane,
		DebugMotionVectorColorBufferCopy,
		AlphaBuffer,
		VRSMask,
		VRSMaskHistory,
		VRSMaskCompute,
		MaxTileVelocityBuffer,
		VelocityDepthBuffer,
		RefractionMaskCoarse,
		ReflectionProbeSHAtlas,
		DLSSExposureTexture,
		DLSSDepth,
		FSR2ReactiveMask,
		FSR2TandCMask,
		FSR2_PREPARED_INPUT_COLOR,
		FSR2_RECONSTRUCTED_PREVIOUS_NEAREST_DEPTH,
		FSR2_INTERNAL_DILATED_MOTION_VECTORS,
		FSR2_DILATED_DEPTH,
		FSR2_RESAMPLED_LUMA_HISTORY,
		FSR2_DEPTH_CLIP,
		FSR2_AUTO_EXPOSURE,
		FSR2_DILATED_REACTIVE_MASKS,
		FSR2_INTERNAL_UPSCALED_COLOR_W,
		MBOITMoments4,
		MBOITOpticalDepth,
		MBOITMoments4HalfRes,
		MBOITOpticalDepthHalfRes,
		MBOITTransparentAccumulation,
		MBOITTransparentAccumulationHalfRes,
		MBOITForwardHalfResTarget,
		ForwardHalfResTarget,
		ForwardHalfResDepth,
		ForwardHalfResOverdraw,
		BlurHeightFieldStaging,
		BlurHeightField,
		SkyOcclusionSH,
		TopDownDepthArray,
		FilteredHeightfield,
		DownsampledHeightfield,
		ProbeRayTrace,
		ProbeDistanceAtlas,
		ProbeIrradianceAtlas,
		ProbeStateMaxDistanceAtlas,
		CascadeProbeRayTrace,
		CascadeProbeDistanceAtlas,
		CascadeProbeIrradianceAtlas,
		CascadeProbeStateMaxDistanceAtlas,
		CascadeProbeScrollSpace,
		ThinDepthBuffer,
		ThinGBuffer_Albedo,
		ThinGBuffer_Normal,
		ThinGBuffer_Optional,
		ThinDepthBufferArray,
		ThinGBuffer_AlbedoArray,
		ThinGBuffer_NormalArray,
		ThinGBuffer_OptionalArray,
		ReflectionCubemapArray,
		ReflectionCubemapCaptureArray,
		ReflectionCubemapForSHArray,
		ReflectionCubemapDepthArray,
		SkyCubemapAtmosScatteringOutput,
		SkyCubemap,
		SkyCubemapThinDepthBuffer,
		SkyCubemapThinGBuffer_Albedo,
		SkyCubemapThinGBuffer_Normal,
		SkyCubemapThinGBuffer_Optional,
		CelestialBodyThinGBuffer_Albedo,
		CelestialBodyThinGBuffer_Normal,
		CelestialBodyThinGBuffer_Optional,
		CelestialBodyDepthBuffer,
		NetDensityToAtmTop,
		SingleScattering,
		ScatterRadiance,
		InscatterOrder,
		HighOrderScatter,
		EpipolarSliceEndpoints,
		EpipolarCoordinates,
		EpipolarRawDepth,
		EpipolarExtinction,
		EpipolarInscattering,
		EpipolarInterpolationSource,
		EpipolarInitialScatteredLight,
		EpipolarSliceUVDirAndOrigin,
		MinMaxShadowMap,
		AtmosphericScatteringOutput,
		PrecomputedTransmittance,
		FogMap,
		MaterialVolume,
		ScatteringVolume,
		ScatteringVolumeHistoryTarget,
		DistantScattering,
		DistantScatteringHistoryTarget,
		DistantScatteringComposite,
		PerlinNoise3D,
		PerlinNoise2D,
		VolumeDepthDownsample,
		FaceCustomizationDiffuse,
		FaceCustomizationDiffuseLinear,
		FaceCustomizationNormals,
		FaceCustomizationAO,
		FaceCustomizationRoughness,
		WaterDisplacementTarget,
		ImageProcessColorTarget,
		ImageProcessR8Target,
		TextureReadbackInput_All,
		ParticleLightingAtlas,
		CloudsHalfResTarget,
		CloudCardsOpacity,
		ImageCaptureHDR_OutputImage
	};

	struct BufferDefinition
	{
		BS_DXGI_FORMAT format;
		uint32_t unk04;
		const char* bufferName;
		uint16_t unk10;
		uint16_t unk12;
		uint32_t unk14;
		uint16_t unk18;
		uint16_t unk1A;
		uint32_t unk1C;
		uint16_t unk20;
		uint16_t unk22;
		uint32_t unk24;
		uint16_t unk28;
		uint16_t unk2A;
		uint32_t unk2C;
		uint32_t unk30;
		float unk34;
		float unk38;
		float unk3C;
		uint32_t unk40;
		uint32_t unk44;
		uint32_t unk48;
		uint32_t unk4C;
	};
	static_assert(sizeof(BufferDefinition) == 0x50);

	struct BGSSwapChainObject
	{
		/*struct UnkStruct
		{
			struct UnkInternalStruct
			{
				uint64_t			unk00;
				uint64_t            unk08;
				uint64_t            unk10;
				uint64_t            unk18;
				uint64_t            unk20;
				ID3D12CommandQueue* commandQueue;
			};

		    UnkInternalStruct* unkInternalStruct;
		};*/

		HWND hwnd;
		//UnkStruct* unkStruct;  // where'd it go? :(
		uint32_t bufferCount;
		uint32_t width;
		uint32_t height;
		uint32_t unk14;
		uint32_t sampleQuality;
		BS_DXGI_FORMAT format;
		uint64_t unk20;
		uint64_t unk28;
		uint64_t unk30;
		uint64_t unk38;
		uint64_t unk40;
		IDXGISwapChain3* swapChainInterface;
		uint64_t unk50;
		ID3D12Resource** renderTargets;
		uint64_t unk60;
		HANDLE handle;
	};
}
