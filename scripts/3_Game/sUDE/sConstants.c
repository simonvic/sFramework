class SRPCIDs {
	const int SYNC_USER_CONFIG_CONSTRAINTS = 0x79B78296;
}

enum PPEMergeFlags {
	DEFAULT = 0,
	COPY = 1,
	INTERPOLATE = 2,
	MAX = 4,
	MIN = 8,
	ADDITIVE = 16,

	UNION = 32,
	INTERSECTION = 64
}

class PPEMaterialsNames {
	static const TPPEMaterial GLOW             = "Graphics/Materials/postprocess/glow";
	static const TPPEMaterial RADIAL_BLUR      = "graphics/materials/postprocess/radialblur";
	static const TPPEMaterial MOTION_BLUR      = "graphics/materials/postprocess/rotblur";
	static const TPPEMaterial GAUSS_BLUR       = "graphics/materials/postprocess/gauss";
	static const TPPEMaterial CHROM_ABER       = "graphics/materials/postprocess/chromaber";
	static const TPPEMaterial FILM_GRAIN       = "Graphics/Materials/postprocess/filmgrainNV";
	static const TPPEMaterial GODRAYS_SUN      = "graphics/materials/postprocess/godrayssun";
	static const TPPEMaterial GODRAYS_SUN_MASK = "graphics/materials/postprocess/godrayssunmask";
	static const TPPEMaterial HBAO             = "graphics/materials/postprocess/hbao";
	static const TPPEMaterial RAIN             = "graphics/materials/postprocess/rain";
}

class PPEParamNames { //just a precaution for future updates... because who knows :shrug:
	static const TPPEParamName VIGNETTE_STRENGTH = "Vignette";
	static const TPPEParamName VIGNETTE_COLOR = "VignetteColor";
	
	static const TPPEParamName MOTION_BLUR_POWER = "Power";
	static const TPPEParamName MOTION_BLUR_MINDEPTH = "MinDepth";
	static const TPPEParamName MOTION_BLUR_MAXDEPTH = "MaxDepth";
	static const TPPEParamName MOTION_BLUR_MIN_ANGLE_PER_SEC = "MinAnglePerSec";
	static const TPPEParamName MOTION_BLUR_MAX_ANGLE_PER_SEC = "MaxAnglePerSec";
	
	static const TPPEParamName RADIAL_BLUR_POWER_X = "PowerX";
	static const TPPEParamName RADIAL_BLUR_POWER_Y = "PowerY";
	static const TPPEParamName RADIAL_BLUR_OFFSET_X = "OffsetX";
	static const TPPEParamName RADIAL_BLUR_OFFSET_Y = "OffsetY";
	
	static const TPPEParamName LENS_DISTORT = "LensDistort";
	static const TPPEParamName LENS_CENTER_X = "LensCenterX";
	static const TPPEParamName LENS_CENTER_Y = "LensCenterY";
	static const TPPEParamName LENS_CHROM_ABER = "MaxChromAbberation";
	
	static const TPPEParamName GLOW_COLORIZATION = "ColorizationColor";
	static const TPPEParamName SATURATION = "Saturation";
	
	static const TPPEParamName OVERLAY_FACTOR = "OverlayFactor";
	static const TPPEParamName OVERLAY_COLOR = "OverlayColor";
	
	static const TPPEParamName BLOOM_THRESHOLD = "BloomThreshold";
	static const TPPEParamName BLOOM_STEEPNESS = "BloomSteepness";
	static const TPPEParamName BLOOM_INTENSITY = "BloomIntensity";
	
	static const TPPEParamName CHROM_ABER_POWER_X = "PowerX";
	static const TPPEParamName CHROM_ABER_POWER_Y = "PowerY";
	
	static const TPPEParamName FILM_GRAIN_SHARPNESS = "Sharpness";
	static const TPPEParamName FILM_GRAIN_GRAIN_SIZE = "GrainSize";
	static const TPPEParamName FILM_GRAIN_INTENSITY = "Intensity";
	static const TPPEParamName FILM_GRAIN_INTENSITY_X0 = "IntensityX0";
	static const TPPEParamName FILM_GRAIN_INTENSITY_X1 = "IntensityX1";
	static const TPPEParamName FILM_GRAIN_MONOCHROMATIC = "Monochromatic";
	static const TPPEParamName FILM_GRAIN_SIMPLE = "Simple";
	static const TPPEParamName FILM_GRAIN_DISTORT = "Distort";
	static const TPPEParamName FILM_GRAIN_FREQUENCY = "Frequency";
	
	static const TPPEParamName GAUSS_BLUR_INTENSITY = "Intensity";
	
	static const TPPEParamName GODRAYS_SUN_INTENSITY = "Intensity";
	static const TPPEParamName GODRAYS_SUN_OVERBURN_INTENSITY = "OverBurnIntensity";
	static const TPPEParamName GODRAYS_SUN_OVERBURN_START = "OverBurnStart";
	static const TPPEParamName GODRAYS_SUN_OVERBURN_END = "OverBurnEnd";
	
	static const TPPEParamName GODRAYS_SUN_MASK = "Intensity";
	static const TPPEParamName GODRAYS_SUN_SIZE = "SunSize";
	static const TPPEParamName GODRAYS_SUN_V_INTENSITY = "VerticalStreakIntensity";
	static const TPPEParamName GODRAYS_SUN_D_INTENSITY = "DiagonalStreakIntensity";
	
	static const TPPEParamName HBAO_RADIUS = "RadiusMeters";
	static const TPPEParamName HBAO_INTENSITY = "Intensity";
	static const TPPEParamName HBAO_SAMPLES = "NumSamples"; //power of 2
	
	static const TPPEParamName RAIN_SPEED = "Speed";
	static const TPPEParamName RAIN_DISTANTSPEED = "DistantSpeed";
	
}

class SPPEConstants {
	static const float ACTIVATION_SPEED = 0.25; //how fast post process effects will reach the desired value. (0.01 = slow | 0.5 = almost immediate | 0.9 = immediate)
	static const float RESET_SPEED = 0.1;       //how fast post process effects will return to normal. (0.01 = slow | 0.5 = almost immediate | 0.9 = immediate)
	//static const float VANILLA_COEFF = 0.91; //coefficient used when merging with vanilla values. (0.1 = very high | 0.5 = high | 0.90 normal | 0.999 very low)
	
	static const float EPSILON = 1e-3; //used when comparing two float values (PPEffects.equals())
	
	static const float EXHAUSTED_DEACTIVATION_TRESHOLD = 25; //[0% - 100%] treshold for Exhausted PPEffect. Percentage of stamina available [0 - m_StaminaCap]
	
	static const float HIT_RECEIVED_MIN_DURATION = 4.5;        //How long (seconds) the animation will last at least
	static const float HIT_RECEIVED_MAX_DURATION = 30.0;       //Maximum duration (seconds) of the animation
	static const float HIT_RECEIVED_DURATION_MULTIPLIER = 2;   //How much the duration will increment when being hit multiple time
	static const float HIT_RECEIVED_MIN_STRENGTH = 1;          //Minimum strength of the effects
	static const float HIT_RECEIVED_STRENGTH_MULTIPLIER = 1.5; //How much the effect strength will increment when being hit multiple time
	static const float HIT_RECEIVED_MAX_CHROM_ABER = 0.5;      //Maximum value of chromatic aberration applied when being hit
	static const float HIT_RECEIVED_RED_SPEED = 5.0;           //How fast the red overlay will dissipate
	
	static const float HIT_RECEIVED_MAX_STRENGTH = 10.0; //Maximum value of hit strength
	
	static const float FOCUS_VIGNETTE_MULTIPLIER = 1; // multiplier for vignette effect when focusing
	
}