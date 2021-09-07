class SCOAnimationDebug : SCameraOverlayAnimated {
	
	protected float m_velX[1];
	protected float m_velY[1];
	
	override void onInit(){
		//setImage(SCOMasks.DEBUG);
		//setMask(SCOMasks.DEBUG);
		setImage("MyMODS/sFramework/GUI/icons/logo/sUDE.paa");
		setSize(1);
		setPriority(eSCOPriority.UI);
	}
	
	override void onPreBuild(){
		getWidget().ClearFlags(WidgetFlags.STRETCH);
	}
	
	override void onAnimate(float deltaTime){
		getWidget().SetFlags(WidgetFlags.STRETCH);
		getWidget().ClearFlags(WidgetFlags.EXACTPOS);
		int mouseX, mouseY, screenSizeX, screenSizeY;
		float wPosX, wPosY;
		GetMousePos(mouseX,mouseY);
		GetScreenSize(screenSizeX, screenSizeY);
		getWidget().GetScreenPos(wPosX, wPosY);

		float distanceX = mouseX - (wPosX + screenSizeX * 0.5);
		float distanceY = mouseY - (wPosY + screenSizeY * 0.5);
		setPosition(
			Math.SmoothCD(getPosition()[0], SMath.mapTo(mouseX, 0, screenSizeX) - 0.5, m_velX, 0.3, 0.1, deltaTime),
			Math.SmoothCD(getPosition()[1], SMath.mapTo(mouseY, 0, screenSizeY) - 0.5, m_velY, 0.3, 0.1, deltaTime));
			
		setRotation(
			-SMath.mapTo(distanceX, 0, screenSizeX, 0, 90),
			SMath.mapTo(distanceY, 0, screenSizeY, 0, 90),
			0);
	}
}

class SCOTimedDebug : SCameraOverlayTimed {
	
	override void onInit(){
		setImage("MyMODS/sFramework/GUI/icons/logo/sUDE.paa");
		setMask(SCOMasks.CIRCULAR);
		setDuration(60);
	}
	
	override void onAnimate(float deltaTime){
		float revolution = Math.AbsFloat(Math.Sin(getTime())) * 360;
		setImage("MyMODS/sFramework/GUI/icons/logo/sUDE.paa");
		setSize(0.075, 0.1);
		setPosition(-0.45, -0.35);
		setRotation(revolution, 0, 0);
	}
}



class SCOAnimationLogo : SCameraOverlayAnimated {
	
	override void onInit(){
		setImage("MyMODS/sFramework/GUI/icons/logo/sUDE.paa");
		setSize(0.05, 0.1);
		setPosition(0.45, -0.45);
		setPriority(999);
	}
	
	override void onAnimate(float deltaTime){
		setRotation(Math.Sin(getTime()) * 360, 0, 0);
	}
	
	/*
	protected vector followCursor(){
		int mouseX, mouseY, sizeX, sizeY;
		float posX, posY;
		GetMousePos(mouseX,mouseY);
		GetScreenSize(sizeX, sizeY);
		getWidget().GetScreenPos(posX, posY);
		
		return Vector(
			-SMath.mapTo(mouseX, posX, sizeX, 0, 90),
			SMath.mapTo(mouseY, posY, sizeY, 0, 90),
			0);
	}
	*/
}