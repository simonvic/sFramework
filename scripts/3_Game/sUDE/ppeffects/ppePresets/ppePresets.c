////////////////////////////////////////////
// VALUE based presets
////////////////////////////////////////////
class PPEPresetBase{
	void PPEPresetBase(){
		init();
	}
	
	void init(){};
}

class DoFPreset : PPEPresetBase{
	float blurStrength;
	float focusDistance;
	float focusLength;
	float focusLengthNear;
	float focusDepthOffset;
	float focusMinDistance;
	float focusMaxDistance;
	
	void initPreset(DoFPreset dof){
		initPreset(dof.blurStrength, dof.focusDistance, dof.focusLength, dof.focusLengthNear, dof.focusDepthOffset, dof.focusMinDistance, dof.focusMaxDistance);
	}
	
	void initPreset(float blur, float fDistance, float fLength, float fLengthNear, float fDepthOffset, float fMinDistance, float fMaxDistance){
		blurStrength = blur;
		focusDistance = fDistance;
		focusLength = fLength;
		focusLengthNear = fLengthNear;
		focusDepthOffset = fDepthOffset;
		focusMinDistance = fMinDistance;
		focusMaxDistance = fMaxDistance;
	}
	
	void reset(){
		blurStrength = 0;
		focusDistance = 0;
		focusLength = 0;
		focusLengthNear = 0;
		focusDepthOffset = 1;
		focusMinDistance = 0;
		focusMaxDistance = 1;
	}
	
	bool equals(DoFPreset dof, float epsilon = 1e-32){
		if(this == dof) return true;
		if(dof == null) return false;
		if(this.Type() != dof.Type()) return false;
		TFloatArray f = {blurStrength, focusDistance, focusLength, focusLengthNear, focusDepthOffset, focusMinDistance, focusMaxDistance};
		TFloatArray f2 = {dof.blurStrength, dof.focusDistance, dof.focusLength, dof.focusLengthNear, dof.focusDepthOffset, dof.focusMinDistance, dof.focusMaxDistance};
		return SMath.equal(f, f2, epsilon);
	}
}

class PPEMaskPreset : PPEPresetBase{
	float posX;
	float posY;
	float radius;
	float blur;
	
	void initPreset(float positionX, float positionY, float maskRadius, float blurStrength){
		posX = positionX;
		posY = positionY;
		radius = maskRadius;
		blur = blurStrength;
	}
	
}






////////////////////////////////////////////
// Material based presets
////////////////////////////////////////////
class PPEMaterialPresetBase : PPEParams{}

class PPEDefaultPreset : PPEMaterialPresetBase{}

class PPEMotionBlurPreset : PPEMaterialPresetBase{
	void initPreset(float blurPower, float minimumDepth, float maximumDepth, float minAnglePerSec, float maxAnglePerSec){
		setMotionBlur(blurPower, minimumDepth, maximumDepth, minAnglePerSec, maxAnglePerSec);
	}
}

class PPEVignettePreset : PPEMaterialPresetBase{
	void initPreset(float strength, TPPEColor color ){
		setVignette(strength, color);
	}
}

class PPERadialBlurPreset : PPEMaterialPresetBase{
	void initPreset(float powerX, float powerY, float offsetX, float offsetY){
		setRadialBlur(powerX, powerY, offsetX, offsetY);
	}
}

class PPEChromAberPreset : PPEMaterialPresetBase{
	void initPreset(float powerX, float powerY){
		setChromAber(powerX, powerY);
	}
}

class PPEBloomPreset : PPEMaterialPresetBase{
	void initPreset(float threshold, float steepness, float intensity){
		setBloom(steepness, intensity, threshold);
	}
}

class PPEOverlayPreset : PPEMaterialPresetBase{
	void initPreset(float factor, TPPEColor color){
		setOverlay(factor, color);
	}
}

class PPEFilmGrainPreset : PPEMaterialPresetBase{
	void initPreset(float sharpness, float grainSize){
		setFilmGrain(sharpness, grainSize);
	}
}

class PPELensPreset : PPEMaterialPresetBase{
	void initPreset(float distortStrength, float centerX, float centerY, float chromAber){
		setLens(distortStrength, centerX, centerY, chromAber);
	}
}

class PPECameraEffectPreset : PPEMaterialPresetBase{
	void initPreset(float saturation, TPPEColor colorization){
		setCameraEffects(saturation, colorization);
	}
}

class PPEBlurPreset : PPEMaterialPresetBase{
	void initPreset(float intensity){
		setGausBlur(intensity);
	}
}