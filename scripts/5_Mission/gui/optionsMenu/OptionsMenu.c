modded class OptionsMenu {
	
	protected ref OptionsMenuSUDE m_sUDETab;
	
	override Widget Init(){
		//super
		Widget temp = super.Init();
		
		// Add custom tab
		int newIndex = m_Tabber.GetTabCount(); //make it last so we son't have problem with possible future additions
		m_Tabber.AddTab(newIndex.ToString());
		TextWidget.Cast( layoutRoot.FindAnyWidget( "Tab_Control_"+newIndex+"_Title" )).SetText("sUDE");
		m_sUDETab = new OptionsMenuSUDE(layoutRoot.FindAnyWidget( "Tab_" + newIndex ));
		
		return temp;

	}
	
	
	OptionsMenuSUDE getSUDEMenu(){
		return m_sUDETab;
	}
	
}
