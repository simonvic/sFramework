class SCameraManager : Managed {
	
	protected float m_time;
	

	protected DayZPlayerCameraBase m_camera;
	protected PlayerBase m_player;
	
	private static ref SCameraManager INSTANCE = new SCameraManager();
	static SCameraManager getInstance(){
		return INSTANCE;
	}
	
	private void SCameraManager(){
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
	
	float getHeadLeanAngle(){
		return 0;
	}
	
}
