modded class DayZPlayerCameraBase{

	
	protected float m_enteringTransitionTime = DayZPlayerCameras.DEFAULT_UNREGISTERED_TIME; 
	protected static ref SCameraManager m_camManager;
	
	void DayZPlayerCameraBase(DayZPlayer pPlayer, HumanInputController pInput){
		if (!m_camManager) m_camManager = new SCameraManager();
		m_camManager.setCurrentCamera(this);
		m_camManager.setPlayer(pPlayer);
	}
	
	override void OnActivate(DayZPlayerCamera pPrevCamera, DayZPlayerCameraResult pPrevCameraResult){
		super.OnActivate(pPrevCamera, pPrevCameraResult);
		if(DayZPlayerCameraBase.Cast(pPrevCamera)){
			m_enteringTransitionTime = DayZPlayerCameras.GetTransitionTime(DayZPlayerCameraBase.Cast(pPrevCamera).getRegisteredCameraID(), this.getRegisteredCameraID());
		}
	}
	
	override void OnUpdate(float pDt, out DayZPlayerCameraResult pOutResult){
		super.OnUpdate(pDt, pOutResult);
		m_camManager.onUpdate(pDt, pOutResult);  
	}
	
	float getTransitionTime(){
		return m_enteringTransitionTime;
	}
	
	int getRegisteredCameraID(){
		return -1;
	}

}