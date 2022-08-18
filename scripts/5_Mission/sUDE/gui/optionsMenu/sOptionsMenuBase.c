class SOptionsMenuBase : ScriptedWidgetEventHandler {
	
	string getName();
	string getLayout();
	string getInfoBoxRootContainer();
	
	string getInfoBoxLayout() {
		return "MyMODS/sFramework/GUI/layouts/user_config_info_box.layout";
	}
	
	protected SUserConfigBase m_sUserConfig;
	protected ref map<Widget, SUserConfigOptionBase> m_optionsWidgets = new map<Widget, SUserConfigOptionBase>;
	protected ref map<Widget, ref array<int>> m_indecesLinks = new map<Widget, ref array<int>>;
	
	protected ref Widget m_root;
	protected ref Widget m_infoBoxRoot;
	protected ref Widget m_infoBoxWarning;
	
	void SOptionsMenuBase() {
		onInit();
	}
	
	protected void onInit() {
	}
	
	void setRoot(Widget root) {
		m_root = root;
		m_root.SetHandler(this);
	}
	
	/**
	*	@brief Set the user config this menu is related to
	*	@param user config
	*/
	void setUserConfig(SUserConfigBase userConfig) {
		m_sUserConfig = userConfig;
	}
	
	protected SUserConfigBase getUserConfig() {
		return m_sUserConfig;
	}
	
	/**
	*	@brief Called when the menu root has been built
	*/
	void onBuild() {
		m_infoBoxRoot = m_root.FindAnyWidget(getInfoBoxRootContainer());
		if (m_infoBoxRoot) {
			GetGame().GetWorkspace().CreateWidgets(getInfoBoxLayout(), m_infoBoxRoot);
			m_infoBoxWarning = m_infoBoxRoot.FindAnyWidget("c_warning");
			hideInfoBoxWarning();
		}
	}
	
	/**
	*	@brief Init slider widget with a float option
	*	@param widget to initialize
	*	@param widget name
	*	@param option to initialize widget with
	*/
	protected void initOptionWidget(out SliderWidget slider, string name, SUserConfigOption<float> option) {		
		slider = SliderWidget.Cast(m_root.FindAnyWidget(name));
		slider.SetCurrent(option.get());
		slider.SetHandler(this);
		
		TextWidget txt = TextWidget.Cast(slider.FindAnyWidget(name + "_value")); //@todo hardcode goes brrrr. Change this
		if (!txt) return;
		txt.SetText(slider.GetCurrent().ToString());
		txt.SetHandler(this);
		
		m_optionsWidgets.Set(slider, option);
	}
	
	/**
	*	@brief Init slider widget with a float array option
	*	@param widget to initialize
	*	@param widget name
	*	@param option to initialize widget with
	*	@param indeces of the array option that the slider will change
	*/
	protected void initOptionWidget(out SliderWidget slider, string name, SUserConfigOptionArray<float> option, array<int> indices) {
		slider = SliderWidget.Cast(m_root.FindAnyWidget(name));
		
		if (indices && indices.Count() > 0) {
			slider.SetCurrent(option.get()[indices[0]]);
			m_indecesLinks.Set(slider, indices);
		} else {
			slider.SetCurrent(option.get()[0]);
			m_indecesLinks.Set(slider, {0});
		}
		slider.SetHandler(this);
				
		TextWidget txt = TextWidget.Cast(slider.FindAnyWidget(name + "_value")); //@todo hardcode goes brrrr. Change this
		if (!txt) return;
		txt.SetText(slider.GetCurrent().ToString());
		txt.SetHandler(this);
		
		m_optionsWidgets.Set(slider, option);
	}
	
	/**
	*	@brief Init slider widget with a float array option
	*	@param widget to initialize
	*	@param widget name
	*	@param option to initialize widget with
	*	@param index of the array option that the slider will change
	*/
	protected void initOptionWidget(out SliderWidget slider, string name, SUserConfigOptionArray<float> option, int index) {		
		array<int> indeces = {index};
		initOptionWidget(slider, name, option, indeces);
	}
	
	/**
	*	@brief Init Checkbox widget with a bool option
	*	@param widget to initialize
	*	@param widget name
	*	@param option to initialize widget with
	*/
	protected void initOptionWidget(out CheckBoxWidget checkbox, string name, SUserConfigOption<bool> option) {
		checkbox = CheckBoxWidget.Cast(m_root.FindAnyWidget( name ));
		checkbox.SetChecked(option.get());
		checkbox.SetHandler(this);
		
		m_optionsWidgets.Set(checkbox, option);
	}
	
	/**
	*	@brief Init combo widget with an int option (index of combo item)
	*	@param widget to initialize
	*	@param widget name
	*	@param option to initialize widget with
	*	@param array of items
	*/
	protected void initOptionWidget(out XComboBoxWidget combo, string name, SUserConfigOption<int> option, array<string> items) {
		combo = XComboBoxWidget.Cast(m_root.FindAnyWidget(name));
		combo.SetCurrentItem(option.get());
		//combo.SetHandler(this); //@todo what?!
		if (items) {
			combo.ClearAll();
			foreach (string item : items) {
				combo.AddItem(item);
			}
		}
		m_optionsWidgets.Set(combo, option);
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

	void show() {
		m_root.Show(true);
	}
	
	void hide() {
		m_root.Show(false);	
		hideInfoBox();
	}
	
	void showInfoBox() {
		m_infoBoxRoot.Show(true);
	}
	
	void hideInfoBox() {
		m_infoBoxRoot.Show(false);	
	}
	
	void showInfoBoxWarning() {
		m_infoBoxWarning.Show(true);
	}
	
	void hideInfoBoxWarning() {
		m_infoBoxWarning.Show(false);	
	}
	
	override bool OnMouseButtonUp(Widget w, int x, int y, int button) {
		if ( button == MouseState.LEFT ) {
			onConfigChange();
		}
		return true;
	}
	
	protected void onConfigChange() { //@todo lol change this shit
		m_sUserConfig.save();
	}
	
	override bool OnChange(Widget w, int x, int y, bool finished) {
		if (!w) return false;
		
		switch (w.Type()) {
			case SliderWidget:   return onChange(SliderWidget.Cast(w));
			case CheckBoxWidget: return onChange(CheckBoxWidget.Cast(w));
			case XComboBoxWidget: return onChange(XComboBoxWidget.Cast(w));
		}
		
		return true;
	}

	protected bool onChange(SliderWidget w) {
		onChangeFloat(w, SUserConfigOption<float>.Cast(m_optionsWidgets.Get(w)));
		onChangeArrayFloat(w, SUserConfigOptionArray<float>.Cast(m_optionsWidgets.Get(w)));
		return true;
	}
	
	// this is the most unsafe code of the history of unsafe code, forgive me
	protected bool onChangeArrayFloat(SliderWidget w, SUserConfigOptionArray<float> option) {
		if (!option) return false;
		
		array<int> indices = m_indecesLinks.Get(w);
		if (!indices || indices.Count() <= 0) return false;
		
		float current = w.GetCurrent();
		array<float> temp = option.get();
		foreach (int i : indices) {
			temp.Set(i, current);
		}
		option.set(temp);
		w.SetCurrent(option.get()[indices[0]]);
		
		TextWidget txt = TextWidget.Cast(w.FindAnyWidget(w.GetName()+"_value"));
		if (txt) txt.SetText(w.GetCurrent().ToString());
		
		return true;
	}
	
	protected bool onChangeFloat(SliderWidget w, SUserConfigOption<float> option) {
		if (!option) return false;
		
		option.set(w.GetCurrent());
		w.SetCurrent(option.get());
		
		TextWidget txt = TextWidget.Cast(w.FindAnyWidget(w.GetName()+"_value"));
		if (txt) txt.SetText(w.GetCurrent().ToString());
		return true;
	}
		
	
	protected bool onChange(CheckBoxWidget w) {
		auto option = SUserConfigOption<bool>.Cast(m_optionsWidgets.Get(w));
		if (!option) return false;
		
		option.set(w.IsChecked());
		w.SetChecked(option.get());
		return true;
	}
	
	protected int onChange(XComboBoxWidget w) {
		auto option = SUserConfigOption<int>.Cast(m_optionsWidgets.Get(w));
		if (!option) return false;
		
		option.set(w.GetCurrentItem());
		w.SetCurrentItem(option.get());
		
		return true;
	}
	

}