//this is needed to circumvent the horrendous hierarchy of DayZPlayerCamera...
class SCameraManager : Managed {
	
	protected float m_time;
	

	protected DayZPlayerCameraBase m_camera;
	protected PlayerBase m_player;
	
	void SCameraManager(){
		onInit();
	}
	
	void onInit(){}
	
	void onUpdate(float pDt, out DayZPlayerCameraResult pOutResult){
		m_time += pDt;
	}
	
	void setCurrentCamera(DayZPlayerCameraBase newCamera){
		m_camera = newCamera;
	}
	
	DayZPlayerCameraBase getCurrentCamera(){
		return m_camera;
	}
	
	void setPlayer(DayZPlayer player){
		m_player = PlayerBase.Cast(player);
	}
	
	bool isCameraTransitioning(){
		return m_player.IsCameraBlending();
	}
	
	bool canUpdateFrom(DayZPlayerCameraBase camera) {
		return m_camera == camera;
	}
	
}
