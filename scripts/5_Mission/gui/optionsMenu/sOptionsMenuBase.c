class SOptionsMenuBase : ScriptedWidgetEventHandler {
	
	string getName();
	string getLayout();
	string getInfoBoxRootContainer();
	
	string getInfoBoxLayout() {
		return "MyMODS/sFramework/GUI/layouts/user_config_info_box.layout";
	}
	
	protected SUserConfigBase m_sUserConfig;
	protected ref map<Widget, SUserConfigOptionBase> m_optionsWidgets = new map<Widget, SUserConfigOptionBase>;
	
	protected ref Widget m_root;
	protected ref Widget m_infoBoxRoot;
	protected ref Widget m_infoBoxWarning;
	
	void SOptionsMenuBase() {
		onInit();
	}
	
	protected void onInit() {
	}
	
	void setUserConfig(SUserConfigBase userConfig) {
		m_sUserConfig = userConfig;
	}
	
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
	
	
	protected void initOptionWidget(out SliderWidget slider, string name, SUserConfigOption<float> option){		
		slider = SliderWidget.Cast(m_root.FindAnyWidget(name));
		slider.SetCurrent(option.get());
		slider.SetHandler(this);
		
		TextWidget txt = TextWidget.Cast(slider.FindAnyWidget(name+"_value")); //@todo hardcode goes brrrr. Change this
		if (!txt) return;
		txt.SetText(slider.GetCurrent().ToString());
		txt.SetHandler(this);
		
		m_optionsWidgets.Set(slider, option);
	}
	
	protected void initOptionWidget(out CheckBoxWidget checkbox, string name, SUserConfigOption<bool> option){
		checkbox = CheckBoxWidget.Cast(m_root.FindAnyWidget( name ));
		checkbox.SetChecked(option.get());
		checkbox.SetHandler(this);
		
		m_optionsWidgets.Set(checkbox, option);
	}
	
	protected void initOptionWidget(out SliderWidget slider, string name, SUserConfigOption<TFloatArray> option, TIntArray indices = null){		
		slider = SliderWidget.Cast(m_root.FindAnyWidget(name));
		
		if (indices && indices.Count() > 0) {
			slider.SetCurrent(option.get()[indices[0]]);
		} else {
			slider.SetCurrent(option.get()[0]);
		}
		slider.SetUserData(indices);
		slider.SetHandler(this);
				
		TextWidget txt = TextWidget.Cast(slider.FindAnyWidget(name+"_value")); //@todo hardcode goes brrrr. Change this
		if (!txt) return;
		txt.SetText(slider.GetCurrent().ToString());
		txt.SetHandler(this);
		
		m_optionsWidgets.Set(slider, option);
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
		hideInfoBox();
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
	
	override bool OnMouseButtonUp(Widget w, int x, int y, int button) {
		if ( button == MouseState.LEFT ) {
			onConfigChange();
		}
		return true;
	}
	
	protected void onConfigChange(){ //@todo lol change this shit
		m_sUserConfig.save();
	}
	
	override bool OnChange(Widget w, int x, int y, bool finished){
		if(!w) return false;
		
		switch (w.Type()) {
			case SliderWidget:   return onChange(SliderWidget.Cast(w));
			case CheckBoxWidget: return onChange(CheckBoxWidget.Cast(w));
		}
		
		return true;
	}

	protected bool onChange(SliderWidget w) {
		onChangeFloat(w, SUserConfigOption<float>.Cast(m_optionsWidgets.Get(w)));
		
		
		onChangeArrayFloat(w, SUserConfigOption<array<float>>.Cast(m_optionsWidgets.Get(w)));
		return true;
	}
	
	protected bool onChangeArrayFloat(SliderWidget w, SUserConfigOption<array<float>> option) {
		if (!option) return true;
		TIntArray indices;
		w.GetUserData(indices);
		if (!indices) return true;
		
		float current = w.GetCurrent();
		foreach (int i : indices) {
			option.get().Set(i, current);
		}
		
		TextWidget txt = TextWidget.Cast(w.FindAnyWidget(w.GetName()+"_value"));
		if (txt) txt.SetText(w.GetCurrent().ToString());
		
		return true;
	}
	
	protected bool onChangeFloat(SliderWidget w, SUserConfigOption<float> option) {
		if (!option) return true;
		
		option.set(w.GetCurrent());
		w.SetCurrent(option.get());
		
		TextWidget txt = TextWidget.Cast(w.FindAnyWidget(w.GetName()+"_value"));
		if (txt) txt.SetText(w.GetCurrent().ToString());
		return true;
	}
		
	
	protected bool onChange(CheckBoxWidget w) {
		SUserConfigOption<bool> option = SUserConfigOption<bool>.Cast(m_optionsWidgets.Get(w));
		if (!option) return true;
		
		option.set(w.IsChecked());
		w.SetChecked(option.get());
		return true;
	}
	

}