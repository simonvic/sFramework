modded class DayZPlayerCameraBase {

	
	protected float m_enteringTransitionTime = DayZPlayerCameras.DEFAULT_UNREGISTERED_TIME; 
	
	void DayZPlayerCameraBase(DayZPlayer pPlayer, HumanInputController pInput){
	}
	
	override void OnActivate(DayZPlayerCamera pPrevCamera, DayZPlayerCameraResult pPrevCameraResult){
		super.OnActivate(pPrevCamera, pPrevCameraResult);
		if (DayZPlayerCameraBase.Cast(pPrevCamera)) {
			m_enteringTransitionTime = DayZPlayerCameras.GetTransitionTime(DayZPlayerCameraBase.Cast(pPrevCamera).getRegisteredCameraID(), this.getRegisteredCameraID());
		}
	}
	
	override void OnUpdate(float pDt, out DayZPlayerCameraResult pOutResult){
		super.OnUpdate(pDt, pOutResult);
	}
	
	float getTransitionTime() {
		return m_enteringTransitionTime;
	}
	
	int getRegisteredCameraID() {
		return -1;
	}

}