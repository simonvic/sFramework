// TODO: move to sGunplay?
modded class DayZPlayerCameraBase {

	protected float m_enteringTransitionTime = DayZPlayerCameras.DEFAULT_UNREGISTERED_TIME;

	override void OnActivate(DayZPlayerCamera pPrevCamera, DayZPlayerCameraResult pPrevCameraResult) {
		super.OnActivate(pPrevCamera, pPrevCameraResult);
		DayZPlayerCameraBase prevCamera;
		if (Class.CastTo(pPrevCamera, prevCamera)) {
			m_enteringTransitionTime = DayZPlayerCameras.GetTransitionTime(
				prevCamera.getRegisteredCameraID(),
				this.getRegisteredCameraID());
		}
	}

	float getTransitionTime() {
		return m_enteringTransitionTime;
	}

	int getRegisteredCameraID() {
		return -1;
	}

}
