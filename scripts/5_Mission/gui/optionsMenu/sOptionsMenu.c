class SOptionsMenuBase : ScriptedWidgetEventHandler {
	
	string getName();
	string getLayout();
	string getInfoBoxRootContainer();
	
	string getInfoBoxLayout() {
		return "MyMODS/sFramework/GUI/layouts/user_config_description_box.layout";
	}
	
	protected ref map<ref Widget, ref SUserConfigInfo> m_descriptions = new map<ref Widget, ref SUserConfigInfo>;
	
	protected Widget m_root;
	protected Widget m_infoBoxRoot;
	protected Widget m_infoBoxWarning;
	
	void setRoot(Widget root){
		m_root = root;
		m_root.SetHandler(this);
	}
	
	void onBuild() {
		m_infoBoxRoot = m_root.FindAnyWidget(getInfoBoxRootContainer());
		if (m_infoBoxRoot) {
			GetGame().GetWorkspace().CreateWidgets(getInfoBoxLayout(), m_infoBoxRoot);
			m_infoBoxWarning = m_infoBoxRoot.FindAnyWidget("c_warning");
			hideInfoBoxWarning();
		}
	}
	
	protected void initOptionInfo(Widget widget, SUserConfigInfo info) {
		m_descriptions.Set(widget, info);
	}
	
	protected void initInfoBox(SUserConfigInfo info) {
		if (!m_infoBoxRoot) return;
		TextWidget.Cast(m_infoBoxRoot.FindAnyWidget("title")).SetText(info.getTitle());
		RichTextWidget.Cast(m_infoBoxRoot.FindAnyWidget("description_body")).SetText(info.getDescription());
		
		
		if (info.isConstrained()) {
			showInfoBoxWarning();			
			RichTextWidget.Cast(m_infoBoxWarning.FindAnyWidget("warning_body")).SetText(info.getWarningMessage());
		} else {
			hideInfoBoxWarning();
		}
	}
	
	
	
	
	protected void initOption(out SSliderWidget slider, string name, float value){
		slider = new SSliderWidget(m_root, name, value, this);
	}
	
	protected void initOption(out CheckBoxWidget checkbox, string name, bool value){
		checkbox = CheckBoxWidget.Cast(m_root.FindAnyWidget( name ));
		checkbox.SetChecked(value);
		checkbox.SetHandler(this);
	}
	
	
		
	protected void lockOption(SSliderWidget slider, string suffix = " ( #STR_SUDE_LAYOUT_OPTIONS_LOCKED )"){
		slider.getSliderWidget().Enable(false);
		
		TextWidget txtWidget = slider.getTextWidget();
		txtWidget.SetText("" + slider.getValue() + suffix);
		txtWidget.Enable(false);
	}
	
	protected void lockOption(CheckBoxWidget widget, string suffix = " ( #STR_SUDE_LAYOUT_OPTIONS_LOCKED )") {
		widget.SetText(suffix);
		widget.Enable(false);
	}
	
	protected void lockOption(Widget widget){
		widget.Enable(false);
	}
		
	
	
	override bool OnMouseEnter(Widget w, int x, int y) {
		if (!m_infoBoxRoot) return true;
		
		SUserConfigInfo info = m_descriptions.Get(w);
		if (!info) {
			hideInfoBox();
			return true;
		}
		
		initInfoBox(info);
		showInfoBox();
		return true;
	}
		
	override bool OnChange(Widget w, int x, int y, bool finished){
		if(!w) return false;
		
		if(w.IsInherited( SliderWidget ))   return onChange(SliderWidget.Cast(w));
		if(w.IsInherited( CheckBoxWidget )) return onChange(CheckBoxWidget.Cast(w));		

		return true;
	}
	
	void show(){
		m_root.Show(true);
	}
	
	void hide(){
		m_root.Show(false);	
	}
	
	void showInfoBox(){
		m_infoBoxRoot.Show(true);
	}
	
	void hideInfoBox(){
		m_infoBoxRoot.Show(false);	
	}
	
	void showInfoBoxWarning(){
		m_infoBoxWarning.Show(true);
	}
	
	void hideInfoBoxWarning(){
		m_infoBoxWarning.Show(false);	
	}
	
	protected bool onChange(SliderWidget w);
	protected bool onChange(CheckBoxWidget w);

}