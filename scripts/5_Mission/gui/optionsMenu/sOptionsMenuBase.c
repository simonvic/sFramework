class SOptionsMenuBase : ScriptedWidgetEventHandler {
	
	string getName();
	string getLayout();
	string getInfoBoxRootContainer();
	
	string getInfoBoxLayout() {
		return "MyMODS/sFramework/GUI/layouts/user_config_info_box.layout";
	}
	
	protected ref map<Widget, SUserConfigOptionBase> m_optionsWidgets = new map<Widget, SUserConfigOptionBase>;
	
	protected Widget m_root;
	protected ref Widget m_infoBoxRoot;
	protected ref Widget m_infoBoxWarning;
	
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
	
	
	protected void initOptionWidget(out SSliderWidget slider, string name, SUserConfigOption<float> option){
		slider = new SSliderWidget(m_root, name, option.get(), this);
		
		m_optionsWidgets.Set(slider.getSliderWidget(), option);
	}
	
	protected void initOptionWidget(out CheckBoxWidget checkbox, string name, SUserConfigOption<bool> option){
		checkbox = CheckBoxWidget.Cast(m_root.FindAnyWidget( name ));
		checkbox.SetChecked(option.get());
		checkbox.SetHandler(this);
		
		m_optionsWidgets.Set(checkbox, option);
	}

	override bool OnMouseEnter(Widget w, int x, int y) {
		updateInfoBox(m_optionsWidgets.Get(w));
		return true;
	}
	
	protected void updateInfoBox(SUserConfigOptionBase option) {
		if (!m_infoBoxRoot || !option) return;
		if (option.getInfo()) {
			initInfoBox(option);
			showInfoBox();
		} else {
			hideInfoBox();
		}
	}
	
	protected void initInfoBox(SUserConfigOptionBase option) {
		if (!m_infoBoxRoot) return;
		TextWidget.Cast(m_infoBoxRoot.FindAnyWidget("title")).SetText(option.getInfo().getTitle());
		RichTextWidget.Cast(m_infoBoxRoot.FindAnyWidget("description_body")).SetText(option.getInfo().getDescription());
		
		
		if (option.isConstrained()) {
			showInfoBoxWarning();			
			RichTextWidget.Cast(m_infoBoxWarning.FindAnyWidget("warning_body")).SetText(option.getWarningMessage());
		} else {
			hideInfoBoxWarning();
		}
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
	
	override bool OnChange(Widget w, int x, int y, bool finished){
		if(!w) return false;
		
		switch (w.Type()) {
			case SliderWidget:   return onChange(SliderWidget.Cast(w));
			case CheckBoxWidget: return onChange(CheckBoxWidget.Cast(w));
		}
		
		return true;
	}
	
	protected bool onChange(SliderWidget w);
	protected bool onChange(CheckBoxWidget w);

}