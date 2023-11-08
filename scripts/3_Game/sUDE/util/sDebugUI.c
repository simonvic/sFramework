#ifdef DIAG_DEVELOPER
/**

	@brief Create a simple interface for debugging purposes
	It can also be used in a "per-frame context"

	@code
		// Example usage for per-frame update
		void OnUpdate(float timeslice) {
			auto dui = SDebugUI.of("TestDebugUI");
			dui.begin();
			dui.window("Debug monitor");
				dui.text("Day Time : " + GetGame().GetDayTime());
				dui.newline();
				
				dui.textrich("<image set='dayz_gui' name='icon_pin' /> ");
				dui.textrich("You can click on the slider, or you can use the mouse wheel");
				dui.textrich("If you hold shift while using mouse wheel, it will go wrooom!");
				float sliderValue;
				dui.slider("mySlider", sliderValue);
				dui.textrich("The value of <font name='gui/fonts/amorserifpro'>sliderValue</font> is: <b>"+ sliderValue +"</b>");
				
				bool checkValue
				dui.check("myCheck", checkValue);
				dui.text("CheckValue: " + checkValue);
				dui.button("click me", this, "printSum", new Param2<int,int>(69, 420));
				dui.newline();
				dui.table({
					{"Attribute",    "Value"},
					{"Time",         ""+GetGame().GetTickTime()},
					{"Radio volume", ""+GetGame().GetSoundScene().GetRadioVolume()},
					{"VoIP volume",  ""+GetGame().GetSoundScene().GetVOIPVolume()},
					{"VoIP level",   ""+GetGame().GetSoundScene().GetAudioLevel()}
				});
				dui.plotlive("Sin", Easing.EaseInBounce(Math.AbsFloat(Math.Sin(m_time))));
			dui.end();
		}

		void printSum(int x, int y) {
			Print(x + y);
		}
		
*/
class SDebugUI : ScriptedWidgetEventHandler {
	
	private static ref map<string, ref SDebugUI> instances = new map<string, ref SDebugUI>;
	
	private ref Widget root;
	private string duiName;
	
	/**
	*	Stack (LIFO) of the instantiated windows.
	*	The first element is the last created window
	*/
	private ref SStack<Widget> windows;
	
	/**
	*	Disable the debug ui
	*/
	private bool disabled;
	
	private ref map<ButtonWidget, ref SDebugButtonCallback> buttonsCallbacks;	
	private ref map<string, bool> statesCheckbox;
	private ref map<string, float> statesSlider;
	private ref map<string, ref array<ref array<float>>> plotsHistory;
	private ref map<string, string> options;
	
	private void SDebugUI(string name) {
		if (isServer()) return;
		duiName = name;
		root = GetGame().GetWorkspace().CreateWidgets("MyMODS/sFramework/GUI/layouts/debug/root.layout");
		root.SetHandler(this);
		root.SetSort(999);
		windows = new SStack<Widget>();
		buttonsCallbacks = new map<ButtonWidget, ref SDebugButtonCallback>;
		statesCheckbox = new map<string, bool>;
		statesSlider = new map<string, float>;
		options = new map<string, string>;
		plotsHistory = new map<string, ref array<ref array<float>>>;
	}

	/**
	*	@brief Get an instance of SDebugUI
	*	@param name of instance
	*	@return instance of name, or new one if it doesn't exists
	*/
	static SDebugUI of(string name) {
		SDebugUI dui = instances.Get(name);
		if (dui) return dui;
		dui = new SDebugUI(name);
		instances.Set(name, dui);
		return dui;
	}
	
	static SDebugUI of(string name, bool enabled) {
		SDebugUI dui = instances.Get(name);
		if (dui) return dui;
		dui = new SDebugUI(name);
		instances.Set(name, dui);
		if (!enabled) {
			dui.disable();
			dui.hide();
		}
		return dui;
	}
	
	static void hideAll() {
		foreach (auto dui : instances) {
			dui.hide();
		}
	}
	
	static void showAll() {
		foreach (auto dui : instances) {
			dui.show();
		}
	}
	
	static void disableAll() {
		foreach (auto dui : instances) {
			dui.disable();
		}
	}
	
	static void enableAll() {
		foreach (auto dui : instances) {
			dui.enable();
		}
	}
	

	/**
	*	@brief Specify an option
	*	@param key
	*	@param value
	*	@code
	*	dui.withOpt("key", "value");
	*/
	SDebugUI withOpt(string key, string value) {
		if (isServer()) return this;
		options.Set(key.Trim(), value);
		return this;
	}
	
	/**
	*	@brief Specify an option
	*	@param key value pair separated by '=' (uqual sign)
	*	@code
	*	dui.withOpt("key = value");
	*/
	SDebugUI withOpt(string opt) {
		if (isServer()) return this;
		int index = opt.IndexOf("=");
		if (index == -1) {
			SLog.w("option malformed: " + opt, ""+this);
		} else {
			string lhs = opt.Substring(0, index);
			string rhs = opt.Substring(index + 1, opt.Length() - 1 - index);
			options.Set(lhs.Trim(), rhs);
		}
		return this;
	}
	
	/**
	*	@brief Specify a list of options
	*	@param array of key value pair separated by '=' (uqual sign)
	*	@code
	*	dui.with({
	*		"key = value"
	*		"key2 = value2"
	*	});
	*/
	SDebugUI with(array<string> opts) {
		if (isServer()) return this;
		foreach (string opt : opts) {
			withOpt(opt);
		}
		return this;
	}
	
	/**
	*	@brief Specify a list of key=value options
	*	@param map of options
	*	@code
	*	auto opts = new map<string, string>();
	*	opts["pos"] = "69px"
	*	dui.with(opts);
	*/
	SDebugUI withOpts(map<string, string> opts) {
		if (isServer()) return this;
		foreach (string key, string value : opts) {
			withOpt(key, value);
		}
		return this;
	}
	
	SDebugUI withBg(SColor color) {
		if (isServer()) return this;
		withOpt("bg", "#" + color.getRGBA());
		return this;
	}
	
	/**
	*	@brief Build a new window
	*	@param title - title of the window
	*	@param sizePx - X and Y size defined in pixels
	*	@param posPx - X and Y position defined in pixels
	*	@return Widget - new window created
	*
	*	@note options:
	*	bg
	*		type: color
	*		default: #00000088
	*	size
	*		type: "dimension dimension"
	*		default:
	*	pos
	*		type: "dimension dimension"
	*		default:
	*	body.bg
	*		type: color
	*		default: #00000088
	*/
	Widget window(string title = "") {
		if (isServer()) return null;
		
		SColor bodyColor = SColor.of(consumeOrDefault("body.bg", "#00000088"));
		
		title = duiName + " / " + title;
				
		Widget w = GetGame().GetWorkspace().CreateWidgets("MyMODS/sFramework/GUI/layouts/debug/window.layout", root);
		w.SetHandler(this);
		consumeBg(w);
		consumeSize(w);
		consumePos(w);
		
		TextWidget.Cast(w.FindAnyWidget("title")).SetText(title);
		if (windows.peek() == null) {
			CheckBoxWidget.Cast(w.FindAnyWidget("disable")).SetChecked(!disabled);
		} else {
			w.FindAnyWidget("disable").Unlink();
		}
		w.FindAnyWidget("body").SetColor(bodyColor.getARGB());
		windows.push(w);
		
		return w;
	}
	
	
	/**
	*	@brief Create a checkbox
	*	@param name - name of the widget
	*	@param variable - variable to link
	*	@return CheckBoxWidget
	*/
	CheckBoxWidget check(string name, out bool variable) {
		if (isServer() || disabled) return null;
		CheckBoxWidget w = CheckBoxWidget.Cast(widget("MyMODS/sFramework/GUI/layouts/debug/checkbox.layout"));
		if (!w) return null;
		w.SetName(name);
		w.SetText(name);
		if (statesCheckbox.Contains(name)) {
			bool checked = statesCheckbox.Get(name);
			w.SetChecked(checked);
			variable = checked;
		} else {
			w.SetChecked(variable);
			statesCheckbox.Set(name, variable);
		}
		return w;
	}

	/**
	*	@brief Create a button
	*	@param text - text to show next i nthe button
	*	@return ButtonWidget
	*/
	ButtonWidget button(string text, Class instance, string function, Param params = null) {
		if (isServer() || disabled) return null;
		ButtonWidget w  = ButtonWidget.Cast(widget("MyMODS/sFramework/GUI/layouts/debug/button.layout"));
		if (!w) return null;
		w.SetText(text);
		w.SetName(text);
		buttonsCallbacks.Set(w, new SDebugButtonCallback(instance, function, params));
		
		return w;
	}
	
	/**
	*	@brief Create a text widget
	*	@param text -
	*	@return TextWidget
	*/
	TextWidget text(string text) {
		if (isServer() || disabled) return null;
		TextWidget w  = TextWidget.Cast(widget("MyMODS/sFramework/GUI/layouts/debug/text.layout"));
		if (!w) return null;
		w.SetText(text);
		return w;
	}
	
	/**
	*	@brief Create a rich text widget
	*	@param text -
	*	@return RichTextWidget
	*/
	RichTextWidget textrich(string text) {
		if (isServer() || disabled) return null;
		RichTextWidget w  = RichTextWidget.Cast(widget("MyMODS/sFramework/GUI/layouts/debug/textrich.layout"));
		if (!w) return null;
		w.SetText(text);
		return w;
	}
	
	/**
	*	@brief Create a slider
	*	@param name - name of the slider
	*	@param float - value to link to the slider
	*	@param float - step value of the slider
	*	@param float - max value of the slider
	*	@param float - min value of the slider
	*	@return SliderWidget
	*	
	*	@note options:
	*	slider.step
	*		type: float
	*		default: 0.01
	*	slider.min
	*		type: float
	*		default: 0
	*	slider.max
	*		type: float
	*		default: 1
	*/
	SliderWidget slider(string name, out float value) {
		if (isServer() || disabled) return null;
		SliderWidget w = SliderWidget.Cast(widget("MyMODS/sFramework/GUI/layouts/debug/slider.layout"));
		if (!w) return null;
		
		float step = consumeOrDefault("slider.step", 0.01);
		float min = consumeOrDefault("slider.min", 0);
		float max = consumeOrDefault("slider.max", 1);
		
		w.SetName(name);
		w.SetStep(step);
		w.SetMinMax(min, max);
		if (statesSlider.Contains(name)) {
			value = statesSlider.Get(name);
		}
		w.SetCurrent(value);
		TextWidget.Cast(w.FindAnyWidget("name")).SetText(name);
		TextWidget.Cast(w.FindAnyWidget("value")).SetText(""+value);
		return w;
	}
	
	/**
	*	@brief Create a table which contains text.
	*	@param sizePx - X and Y size defined in pixel
	*	@param data - matrix of rows and columns of string data
	*	@return WrapSpacerWidget
	*
	*	@note options:
	*	table.cell.flags.set
	*		type: WidgetFlags
	*		default:
	*	table.cell.flags.unset
	*		type: WidgetFlags
	*		default:
	*/
	WrapSpacerWidget table(array<ref array<string>> data) {
		if (isServer() || disabled) return null;
		WrapSpacerWidget w = WrapSpacerWidget.Cast(widget("MyMODS/sFramework/GUI/layouts/debug/table.layout"));
		if (!w) return null;

		if (data == null || data.Count() == 0) return w;
		
		string cellFlags = consume("table.cell.flags.set");
		string cellFlagsUnset = consume("table.cell.flags.unset");
		
		float height = 1 / data.Count();
		foreach (auto row : data) {
			if (row == null || row.Count() == 0) continue;
			float width = 1 / row.Count();
			foreach (auto entry : row) {
				TextWidget t = TextWidget.Cast(GetGame().GetWorkspace().CreateWidget(TextWidgetTypeID, 0, 0, 1, 1, WidgetFlags.VISIBLE, 0xffffffff, 0, w));
				t.SetFlags(WidgetFlags.CENTER | WidgetFlags.VCENTER | WidgetFlags.IGNOREPOINTER);
				if (cellFlagsUnset != string.Empty) {
					t.ClearFlags(cellFlagsUnset.ToInt());
				}
				if (cellFlags != string.Empty) {
					t.SetFlags(cellFlags.ToInt());
				}
				t.SetSize(width, height);
				t.SetText(entry);
				
				//Print(SFlagOperator.from(t.GetFlags()).collectBinaryString());
			}
		}
		return w;
	}
	
	
	
	/**
	*	@brief Create a plot to draw lines
	*	@param sizePx - X and Y size defined in pixel
	*	@param title - title of the plot
	*	@return CanvasWidget
	*
	*	@note options:
	*	canvas.title
	*		type: string
	*		default: ""
	*/
	CanvasWidget canvas() {
		if (isServer() || disabled) return null;
		Widget w = widget("MyMODS/sFramework/GUI/layouts/debug/canvas.layout", false);
		if (!w) return null;
		
		string title = consume("canvas.title");
		CanvasWidget c = CanvasWidget.Cast(w.FindAnyWidget("canvas"));		
		TextWidget t = TextWidget.Cast(w.FindAnyWidget("title"));
		t.SetText(title);
		consumeBg(c);
		consumeSize(c);
		consumePos(c);
		return c;
	}
	
	/**
	*	@brief Plot live data
	*	@param title - title of the plot
	*	@param y - y axis data
	*	@return CanvasWidget
	*	@note options:
	*	@see canvas()
	*	plot.min
	*		type: float
	*		default: 0.0
	*	plot.max
	*		type: float
	*		default: 1.0
	*	plot.offset.x
	*		type: px
	*		default: 0
	*	plot.offset.y
	*		type: px
	*		default: 0
	*	plot.scale.x
	*		type: px
	*		default: 1
	*	plot.scale.y
	*		type: px
	*		default: 1
	*	plot.history.size
	*		type: int
	*		default: 50
	*	plot.pen.width
	*		type: px
	*		default: 3
	*	plot.pen.color
	*		type: "#RRGGBBaa"
	*		default: "#F0544Cff"
	*
	*/
	CanvasWidget plotlive(string title, float y) {
		if (isServer() || disabled) return null;
		CanvasWidget c = canvas();
		if (!c) return null;
		
		float min = consumeOrDefault("plot.min", 0.0);
		float max = consumeOrDefault("plot.max", 1.0);
		float offsetX = consumeOrDefault("plot.offset.x", 0.0);
		float offsetY = consumeOrDefault("plot.offset.y", 0.0);
		float scaleX = consumeOrDefault("plot.scale.x", 1.0);
		float scaleY = consumeOrDefault("plot.offset.y", 1.0);
		int penWidthPx = consumeOrDefault("plot.pen.width", 3);
		SColor penColor = SColor.of(consumeOrDefault("plot.pen.color", "F0544C"));
		int historySize = consumeOrDefault("plot.history.size", 50);
		
		TextWidget.Cast(c.FindAnyWidget("min")).SetText(""+min);
		TextWidget.Cast(c.FindAnyWidget("max")).SetText(""+max);
		TextWidget.Cast(c.FindAnyWidget("current")).SetText(""+y);
		
		auto line = plotsHistory.Get(title);
		if (!line) {
			line = {{0.5,0.5}, {0,y}};
			plotsHistory.Set(title, line);
		} else {
			line.InsertAt({0,y}, 0);
		}
		line.Remove(line.Count() - 1);
		line.Resize(historySize);

		float width, height;
		c.GetSize(width, height);
		
		float precision = 1 / historySize;
		float x;
		for (int i = line.Count() - 1; i > 0; i--) {
			if (line[i] == null) continue;
			c.DrawLine(
				SMath.mapTo(offsetX + scaleX * x,              0,   1,   0,      width),
				SMath.mapTo(offsetY + scaleY * line[i][1],     min, max, height, 0),
				SMath.mapTo(offsetX + scaleX * x + precision,  0,   1,   0,      width),
				SMath.mapTo(offsetY + scaleY * line[i - 1][1], min, max, height, 0),
				penWidthPx,
				penColor.getARGB());
			x += precision;
		}
		return c;
	}
	
	/**
	*	@brief Create a plot to draw lines
	*	@param lines - list of lines (which is a list of point (which is a couple of X and Y coordinates))
	*	@return CanvasWidget
	*
	*	@note options:
	*	@see canvas()
	*	plot.min
	*		type: float
	*		default: 0.0
	*	plot.max
	*		type: float
	*		default: 1.0
	*	plot.offset.x
	*		type: px
	*		default: 0
	*	plot.offset.y
	*		type: px
	*		default: 0
	*	plot.scale.x
	*		type: px
	*		default: 1
	*	plot.scale.y
	*		type: px
	*		default: 1
	*	plot.pen.width
	*		type: px
	*		default: 3
	*	plot.pen.color
	*		type: "#RRGGBBaa"
	*		default: "#F0544Cff"
	*
	*/
	CanvasWidget plot(array<ref TLine> lines = null) {
		if (isServer() || disabled) return null;
		CanvasWidget c = canvas();
		if (!c) return null;
		
		float min = consumeOrDefault("plot.min", 0.0);
		float max = consumeOrDefault("plot.max", 1.0);
		float offsetX = consumeOrDefault("plot.offset.x", 0.0);
		float offsetY = consumeOrDefault("plot.offset.y", 0.0);
		float scaleX = consumeOrDefault("plot.scale.x", 1.0);
		float scaleY = consumeOrDefault("plot.scale.y", 1.0);
		int penWidthPx = consumeOrDefault("plot.pen.width", 3);
		SColor penColor = SColor.of(consumeOrDefault("plot.pen.color", "F0544C"));
		
		float width, height;
		c.GetSize(width, height);
		
		foreach (auto line : lines) {	
			for (int i = 0; i < line.Count() - 1; i++) {
				c.DrawLine(
					SMath.mapTo(offsetX + scaleX * line[i][0],   0, 1, 0,      width),
					SMath.mapTo(offsetY + scaleY * line[i][1],   0, 1, height, 0),
					SMath.mapTo(offsetX + scaleX * line[i+1][0], 0, 1, 0,      width),
					SMath.mapTo(offsetY + scaleY * line[i+1][1], 0, 1, height, 0),
					penWidthPx,
					penColor.getARGB());
			}
		}
		return c;
	}
	
	/**
	*	@brief Create a spacer
	*	@param size - X and Y size defined in screen space (0.0 - 1.0)
	*	@return Widget
	*/
	Widget spacer() {
		if (isServer() || disabled) return null;
		return widget("MyMODS/sFramework/GUI/layouts/debug/spacer.layout");
	}
	
	
	/**
	*	@brief Create a new line
	*	@param size - height of the empty line
	*	@return Widget
	*/
	Widget newline(string height = "1px") {
		if (isServer() || disabled) return null;
		return withOpt("size", "1 " + height).spacer();
	}
	
	/**
	*	@brief Create a widget
	*	@param layout - .layout file to load
	*	@param name - name of the root widget
	*	@return Widget
	*
	*	@note options:
	*	name
	*		type: string
	*		default:
	*	bg
	*		type: color
	*		default: #F0544CFF
	*	size
	*		type: "dimension dimension"
	*		default:
	*	pos
	*		type: "dimension dimension"
	*		default:
	*/
	Widget widget(string layout, bool consumeCommonOptions = true) {
		if (isServer() || disabled) return null;
		Widget window = windows.peek();
		if (!window) {
			SLog.e("No window to place widget on!",""+this);
			return null;
		}
		Widget w = GetGame().GetWorkspace().CreateWidgets(layout, window.FindAnyWidget("body"));
		w.SetHandler(this);

		if (consumeCommonOptions) {
			w.SetName(consumeOrDefault("name", ""));
			consumeBg(w);
			consumeSize(w);
			consumePos(w);
		}
		
		return w;
	}
	
	/**
	*	@brief Mark the beginning of the DUI scope
	*/
	void begin() {
		if (isServer()) return;
		foreach (auto button, auto callback : buttonsCallbacks) {
			if (button.GetState()) {
				GetGame().GameScript.CallFunctionParams(callback.instance, callback.function, null, callback.params);
			}
		}
		clear();
	}
	
	/**
	*	@brief Mark the end of the DUI scope
	*/
	void end() {
	}
	
	void enable() {
		disabled = false;
		//show();
	}
	
	void disable() {
		disabled = true;
		//hide();
	}
	
	void show() {
		root.Show(true);
	}
	
	void hide() {
		root.Show(false);
	}
	
	/**
	*	@brief Clear the widgets
	*/
	void clear() {
		buttonsCallbacks.Clear();
		Widget w = windows.pop();
		while (w != null) {
			w.Unlink();
			w = windows.pop();
		}
	}
	
	protected string consumeOrDefault(string option, string defaultValue) {
		if (options.Contains(option)) {
			string value = options.Get(option);
			options.Remove(option);
			return value;
		}
		return defaultValue;
	}
	
	protected int consumeOrDefault(string option, int defaultValue) {
		if (options.Contains(option)) {
			string value = options.Get(option);
			options.Remove(option);
			return value.ToInt();
		}
		return defaultValue;
	}
	
	protected float consumeOrDefault(string option, float defaultValue) {
		if (options.Contains(option)) {
			string value = options.Get(option);
			options.Remove(option);
			return value.ToFloat();
		}
		return defaultValue;
	}
	
	protected string consume(string option) {
		string value = options.Get(option);
		options.Remove(option);
		return value;
	}
	
	/**
	*	@brief Recolor the widget and consume the color
	*	@param widget to color
	*/
	protected void consumeBg(Widget w) {
		if (options.Contains("bg")) {
			string value = consume("bg");
			value.Replace("#", "");
			w.SetColor(SColor.of(value).getARGB());
		}
	}
	
	/**
	*	@brief Resize the widget and consume the size
	*	@param widget to resize
	*/
	protected void consumeSize(Widget w) {
		if (!options.Contains("size")) return;
		
		string value = consume("size");
		
		array<ref TScreenUnit> units = parseScreenUnits(value);
		if (units.Count() == 0) return;
		
		if (units.Count() == 1) {
			if (units[0].param2) {
				w.SetFlags(WidgetFlags.EXACTSIZE);
			} else {
				w.ClearFlags(WidgetFlags.EXACTSIZE);
			}
			w.SetSize(units[0].param1, units[0].param1);
			
		} else if (units.Count() >= 2) {
			
			if (units[0].param2) {
				w.SetFlags(WidgetFlags.HEXACTSIZE);
			} else {
				w.ClearFlags(WidgetFlags.HEXACTSIZE);
			}
			
			if (units[1].param2) {
				w.SetFlags(WidgetFlags.VEXACTSIZE);
			} else {
				w.ClearFlags(WidgetFlags.VEXACTSIZE);
			}
			
			w.SetSize(units[0].param1, units[1].param1);
		}
		
	}
	
	/**
	*	@brief Reposition the widget and consume the position
	*	@param widget to reposition
	*/
	protected void consumePos(Widget w) {
		if (!options.Contains("pos")) return;
		
		string value = consume("pos");
		array<ref TScreenUnit> units = parseScreenUnits(value);
		
		if (units.Count() == 0) return;
		
		if (units.Count() == 1) {
			if (units[0].param2) {
				w.SetFlags(WidgetFlags.EXACTPOS);
			} else {
				w.ClearFlags(WidgetFlags.EXACTPOS);
			}
			w.SetSize(units[0].param1, units[0].param1);
			
		} else if (units.Count() >= 2) {
			
			if (units[0].param2) {
				w.SetFlags(WidgetFlags.HEXACTPOS);
			} else {
				w.ClearFlags(WidgetFlags.HEXACTPOS);
			}
			
			if (units[1].param2) {
				w.SetFlags(WidgetFlags.VEXACTPOS);
			} else {
				w.ClearFlags(WidgetFlags.VEXACTPOS);
			}
			
			w.SetPos(units[0].param1, units[1].param1);
		}
	}
	
	/**
	*	@brief Parse a string into an array of dimensions
	*	@param words to parse
	*	@return array<(float_value, isInPixel)>
	*
	*	@code
	*	parseScreenUnits("69px");           //69px
	*	parseScreenUnits("0.69");           //69%
	*	parseScreenUnits("69px 420px");     //69px, 420px
	*	parseScreenUnits("0.5 0.42");       //50%, 42%
	*	parseScreenUnits("69px 0.42 1.0");  //69px, 42%, 100%
	*/
	protected static array<ref TScreenUnit> parseScreenUnits(string words) {
		array<ref TScreenUnit> units = {};
		array<string> temp = {};
		words.Split(" ", temp);
		if (!temp || temp.Count() < 1) return units;
		foreach (string word : temp) {
			bool isExact = word.Contains("px");
			word.Replace("px", "");
			units.Insert(new TScreenUnit(word.ToFloat(), isExact));
		}
		return units;
	}
	
	
	
	
	override bool OnClick(Widget w, int x, int y, int button) {
		//                    reeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee
		Widget rootWindow = w.GetParent().GetParent().GetParent().GetParent();
		switch (w.Type()) {
			case ButtonWidget:
			ButtonWidget btn = ButtonWidget.Cast(w);
			switch (w.GetName()) {
				case "collapse":
				Widget body = rootWindow.FindAnyWidget("body");
				if (body.IsVisible()) {
					btn.SetText("+");
				} else {
					btn.SetText("-");
				}
				body.Show(!body.IsVisible());
				break;
				
				case "maximize":
				break;
				
				case "close":
				break;
			}
			break;
			
			case CheckBoxWidget:
			CheckBoxWidget chk = CheckBoxWidget.Cast(w);
			string name = chk.GetName();
			switch (name) {
				case "disable":
				rootWindow.FindAnyWidget("body").Show(disabled);
				disabled = !disabled;
				break;
				
				default:
				if (statesCheckbox.Contains(name)) {
					statesCheckbox.Set(name, !statesCheckbox.Get(name));
				}
			}
			break;
			
		}
				
		return true;
	}
	
	override bool OnMouseEnter(Widget w, int x, int y);
	override bool OnModalResult(Widget w, int x, int y, int code, int result);
	override bool OnDoubleClick(Widget w, int x, int y, int button);
	override bool OnSelect(Widget w, int x, int y);
	override bool OnItemSelected(Widget w, int x, int y, int row, int column, int oldRow, int oldColumn);
	override bool OnFocus(Widget w, int x, int y);
	override bool OnFocusLost(Widget w, int x, int y);
	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y);
	override bool OnMouseWheel(Widget w, int x, int y, int wheel) {
		switch (w.Type()) {
			//@todo make atomic method
			case SliderWidget:
			auto slider = SliderWidget.Cast(w);
			if (KeyState(KeyCode.KC_LSHIFT)) wheel *= 10;
			slider.SetCurrent(slider.GetCurrent() + (wheel * slider.GetStep()));
			statesSlider.Set(slider.GetName(), slider.GetCurrent());
			break;
		}
		return true;
	}
	override bool OnMouseButtonDown(Widget w, int x, int y, int button);
	override bool OnMouseButtonUp(Widget w, int x, int y, int button);
	override bool OnController(Widget w, int control, int value);
	override bool OnKeyDown(Widget w, int x, int y, int key);
	override bool OnKeyUp(Widget w, int x, int y, int key);
	override bool OnKeyPress(Widget w, int x, int y, int key);
	override bool OnChange(Widget w, int x, int y, bool finished) {
		switch (w.Type()) {
			case SliderWidget:
			SliderWidget slider = SliderWidget.Cast(w);
			statesSlider.Set(slider.GetName(), slider.GetCurrent());
			break;
		}
		return true;
	}
	
	
	private vector dragOffset;
	
	override bool OnDrag(Widget w, int x, int y) {
		float wx,wy;
		w.GetParent().GetPos(wx,wy);
		dragOffset[0] = x - wx;
		dragOffset[1] = y - wy;
		return true;
	}
	
	override bool OnDragging(Widget w, int x, int y, Widget reciever) {
		w.GetParent().SetFlags(WidgetFlags.EXACTPOS);
		w.GetParent().SetPos(x - dragOffset[0], y - dragOffset[1]);
		return true;
	}
	override bool OnDraggingOver(Widget w, int x, int y, Widget reciever);
	override bool OnDrop(Widget w, int x, int y, Widget reciever);	
	override bool OnDropReceived(Widget w, int x, int y, Widget reciever);
	override bool OnResize(Widget w, int x, int y);
	override bool OnChildAdd(Widget w, Widget child);
	override bool OnChildRemove(Widget w, Widget child);
	override bool OnUpdate(Widget w);
	override bool OnEvent(EventType eventType, Widget target, int parameter0, int parameter1);
	
	
	protected bool isServer() {
		return GetGame().IsDedicatedServer();
	}
	
}


class SDebugButtonCallback : Managed {
	Class instance;
	string function;
	ref Param params;
	
	void SDebugButtonCallback(Class i, string f, Param p) {
		instance = i;
		function = f;
		params = p;
	}
}

typedef array<float> TPoint;
typedef array<ref TPoint> TLine;
typedef Param2<float, bool> TScreenUnit;
typedef array<string> TTableRow;
typedef array<ref TTableRow> TTable;

#endif