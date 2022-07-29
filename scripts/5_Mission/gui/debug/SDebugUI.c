class SDebugUI : Managed {
	
	static Widget root;
	
	/**
	*	Stack (LIFO) of the instantiated windows.
	*	The first element is the last created window
	*/
	static ref array<Widget> windows = {};
	
	
	/**
	*	@brief Build a new window
	*	@param title - title of the window
	*	@param sizePx - X and Y size defined in pixels
	*	@param pos - X and Y position defined in screen space (0.0 - 1.0)
	*	@return Widget - new window created
	*/
	static Widget window(string title = "", array<float> sizePx = null, array<float> pos = null) {
		if (!root) {
			root = GetGame().GetWorkspace().CreateWidgets("MyMODS/sFramework/GUI/layouts/debug/root.layout");
		}
		Widget w = GetGame().GetWorkspace().CreateWidgets("MyMODS/sFramework/GUI/layouts/debug/window.layout", root);
		if (sizePx == null || sizePx.Count() != 2) {
			w.SetSize(512, 512);
		} else {
			w.SetSize(sizePx[0], sizePx[1]);
		}
		if (pos == null || pos.Count() != 2) {
			w.SetPos(0.25, 0.25);
		} else {
			w.SetPos(pos[0], pos[1]);
		}
		TextWidget.Cast(w.FindAnyWidget("title")).SetText(title);
		windows.InsertAt(w, 0);
		return w;
	}
		
	
	/**
	*	@brief Create a checkbox
	*	@param text - text to show next to the checkbox
	*	@return CheckBoxWidget
	*/
	static CheckBoxWidget check(string text) {
		CheckBoxWidget w = CheckBoxWidget.Cast(widget("MyMODS/sFramework/GUI/layouts/debug/checkbox.layout"));
		w.SetName(text);
		w.SetText(text);
		return w;
	}
	
	/**
	*	@brief Create a button
	*	@param text - text to show next i nthe button
	*	@return ButtonWidget
	*/
	static ButtonWidget button(string text) {
		ButtonWidget w  = ButtonWidget.Cast(widget("MyMODS/sFramework/GUI/layouts/debug/button.layout"));
		w.SetText(text);
		return w;
	}
	
	/**
	*	@brief Create a text widget
	*	@param text -
	*	@return TextWidget
	*/
	static TextWidget text(string text) {
		TextWidget w  = TextWidget.Cast(widget("MyMODS/sFramework/GUI/layouts/debug/text.layout"));
		w.SetText(text);
		return w;
	}
	
	/**
	*	@brief Create a slider
	*	@param float - current value of the slider
	*	@param float - step value of the slider
	*	@param float - max value of the slider
	*	@param float - min value of the slider
	*	@return SliderWidget
	*/
	static SliderWidget slider(float value, float step = 0.1, float min = 0, float max = 1) {
		SliderWidget w = SliderWidget.Cast(widget("MyMODS/sFramework/GUI/layouts/debug/slider.layout"));
		w.SetStep(step);
		w.SetMinMax(min, max);
		w.SetCurrent(value);
		return w;
	}
	
	/**
	*	@brief Create a table which contains text.
	*		The dimensions of the table are automatically calculated based on the given data
	*	@param sizePx - X and Y size defined in pixel
	*	@param data - matrix of rows and columns of string data
	*	@return WrapSpacerWidget
	*/
	static WrapSpacerWidget table(array<int> sizePx, array<ref array<string>> data) {
		WrapSpacerWidget w = WrapSpacerWidget.Cast(widget("MyMODS/sFramework/GUI/layouts/debug/table.layout"));
		w.SetFlags(WidgetFlags.EXACTSIZE);
		w.SetSize(sizePx[0], sizePx[1]);
		if (data == null || data.Count() == 0) return w;
		float height = 1 / data.Count();
		foreach (auto row : data) {
			if (row == null || row.Count() == 0) continue;
			float width = 1 / row.Count();
			foreach (auto entry : row) {
				TextWidget t = TextWidget.Cast(GetGame().GetWorkspace().CreateWidget(TextWidgetTypeID, 0, 0, 1, 1, WidgetFlags.VISIBLE, 0xffffffff, 0, w));
				t.ClearFlags(WidgetFlags.EXACTSIZE);
				t.SetFlags(WidgetFlags.CENTER);
				t.SetSize(width, height);
				t.SetText(entry);
			}
		}
		return w;
	}
	
	/**
	*	@brief Create a plot to draw lines
	*	@param sizePx - X and Y size defined in pixel
	*	@param title - title of the plot
	*	@param lines - list of lines (which is a list of point (which is a couple of X and Y coordinates))
	*	@param scale - X and Y scale of the plot data
	*	@param offset - X and Y offset of the plot data
	*	@param widthPx - width of the lines defined in pixels
	*	@param color - color of the line
	*	@return CanvasWidget
	*/
	static CanvasWidget plot(array<int> sizePx, string title = "", array<ref array<ref array<float>>> lines = null, array<float> scale = null, array<float> offset = null, int widthPx = 5, SColor color = null) {
		CanvasWidget c = canvas(sizePx, title);
		
		if (!color) color = SColor.rgba(0xF0544Cff);
		
		auto s = scale;
		if (!s || s.Count() < 2) s = {1, 1};
		
		auto o = offset;
		if (!o || o.Count() < 2) o = {0, 0};
		
		foreach (auto line : lines) {	
			for (int i = 0; i < line.Count() - 1; i++) {
				c.DrawLine(
					SMath.mapTo(o[0] + s[0] * line[i][0],   0, 1, 0,         sizePx[0]),
					SMath.mapTo(o[1] + s[1] * line[i][1],   0, 1, sizePx[1], 0),
					SMath.mapTo(o[0] + s[0] * line[i+1][0], 0, 1, 0,         sizePx[0]),
					SMath.mapTo(o[1] + s[1] * line[i+1][1], 0, 1, sizePx[1], 0),
					widthPx,
					color.getARGB());
			}
		}
		
		return c;
	}
	
	/**
	*	@brief Create a plot to draw lines
	*	@param sizePx - X and Y size defined in pixel
	*	@param title - title of the plot
	*	@return CanvasWidget
	*/
	static CanvasWidget canvas(array<int> sizePx, string title = "") {
		Widget r = widget("MyMODS/sFramework/GUI/layouts/debug/canvas.layout");
		CanvasWidget c = CanvasWidget.Cast(r.FindAnyWidget("canvas"));
		c.SetSize(sizePx[0], sizePx[1]);
		TextWidget t = TextWidget.Cast(r.FindAnyWidget("title"));
		t.SetText(title);
		return c;
	}
	
	/**
	*	@brief Create a spacer
	*	@param size - X and Y size defined in screen space (0.0 - 1.0)
	*	@return Widget
	*/
	static Widget spacer(array<float> size = null) {
		auto s = size;
		if (!s || s.Count() < 2) s = {0.3, 0.3};
		Widget w = GetGame().GetWorkspace().CreateWidget(FrameWidgetTypeID, 0, 0, 1, 1, WidgetFlags.VISIBLE, 0xffffffff, 0, windows[0].FindAnyWidget("body"));
		w.SetSize(s[0], s[1]);
		return w;
	}
	
	
	/**
	*	@brief Create a new line
	*	@param sizePx - height in pixels of the empty line
	*	@return Widget
	*/
	static Widget newLine(int sizePx = 1) {
		Widget w = spacer();
		w.SetFlags(WidgetFlags.VEXACTSIZE);
		w.SetSize(1.0, sizePx);
		return w;
	}
	
	/**
	*	@brief Create a widget
	*	@param layout - .layout file to load
	*	@param name - name of the root widget
	*	@return Widget
	*/
	static Widget widget(string layout, string name = string.Empty) {
		Widget w = GetGame().GetWorkspace().CreateWidgets(layout, windows[0].FindAnyWidget("body"));
		if (name != string.Empty) {
			w.SetName(name);
		}
		return w;
	}
	
	/**
	*	@brief Clear the widgets
	*/
	static void clear() {
		foreach (Widget w : windows) {
			w.Unlink();
		}
		windows.Clear();
	}
	
}