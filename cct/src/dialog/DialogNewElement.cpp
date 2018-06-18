#include "DialogNewElement.hpp"

void DialogNewElement::init()
{
	set_flags(DIALOG_TOP_MOST | DIALOG_FLUID);
	Dialog::init();
	m_element_y = get_top() + 30;

	add_element_id();

	switch (m_type)
	{
	case BUTTON_KEYBOARD:
	case BUTTON_GAMEPAD:
	case BUTTON_MOUSE:
		add_keycode_elements();
	case TEXTURE:
		add(m_selector = new AtlasSelector(m_id++, get_left() + 270,
			get_top() + 30, m_dimensions.w - 278, m_dimensions.h - 38, m_tool->get_atlas(), this));
		m_selector->set_selection(&m_selection_1);
		add_selection_elements();
		break;
	}

	add(m_ok = new Button(ACTION_OK, 8, m_dimensions.h - 32, "OK", this));
	add(m_cancel = new Button(ACTION_CANCEL, 124, m_dimensions.h - 32, "Cancel", this));
}

void DialogNewElement::action_performed(int8_t action_id)
{
	switch (action_id)
	{
	case ACTION_OK:
		m_tool->action_performed(TOOL_ACTION_NEW_ELEMENT_ADD);
		break;
	case ACTION_CANCEL:
		m_tool->action_performed(TOOL_ACTION_NEW_ELEMENT_EXIT);
		break;
	}
}

void DialogNewElement::draw_background(void)
{
	Dialog::draw_background();
	switch (m_type)
	{
	case TEXTURE:
	case BUTTON_KEYBOARD:
	case BUTTON_GAMEPAD:
	case MOUSE_MOVEMENT:

		break;
	}
}

void DialogNewElement::draw_foreground(void)
{
	Dialog::draw_foreground();
	switch (m_type)
	{
	case TEXTURE:
	case BUTTON_KEYBOARD:
	case BUTTON_GAMEPAD:
	case MOUSE_MOVEMENT:
	case ANALOG_STICK:
		break;
	}
}

bool DialogNewElement::handle_events(SDL_Event * event)
{
	bool was_handled = false;
	if (Dialog::handle_events(event))
		was_handled = true;

	switch (m_type)
	{
	case TEXTURE:
	case BUTTON_KEYBOARD:
	case BUTTON_GAMEPAD:
	case MOUSE_MOVEMENT:
	case ANALOG_STICK:
		if (event->type == SDL_WINDOWEVENT)
		{
			if (event->window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
			{
				SDL_Rect t = { get_left() + 270,
					get_top() + 30, m_dimensions.w - 278, m_dimensions.h - 38 };
				m_selector->set_dim(t);
				m_ok->set_pos(8, m_dimensions.h - 32);
				m_cancel->set_pos(124, m_dimensions.h - 32);
			}
		}
		m_h->set_text(std::to_string(m_selection_1.h));
		m_w->set_text(std::to_string(m_selection_1.w));
		m_u->set_text(std::to_string(m_selection_1.x));
		m_v->set_text(std::to_string(m_selection_1.y));
		break;
	}

	return was_handled;
}

ElementType DialogNewElement::get_type(void)
{
	return m_type;
}

SDL_Rect DialogNewElement::get_selection_1(void)
{
	return m_selection_1;
}

uint16_t DialogNewElement::get_key_code(void)
{
	if (m_keycode)
		return std::atoi(m_keycode->get_text()->c_str());
	else
		return 0x0;
}

const std::string * DialogNewElement::get_id(void)
{
	return m_element_id->get_text();
}

void DialogNewElement::add_selection_elements(void)
{
	uint16_t panel_w = 254;

	m_selection_1 = { 0, 0, 0, 0 };

	if (m_element_y == 0)
		m_element_y = 30;

	add(new Label(m_id++, 8, m_element_y, "Texture selection", this));

	m_element_y += 25;
	add(new Label(m_id++, 8, m_element_y, "Width:", this));
	add(new Label(m_id++, 16 + panel_w / 2, m_element_y, "Height:", this));
	add(m_w = new Textbox(m_id++, 8 + panel_w / 4 + 4, m_element_y, panel_w / 4, 20, "0", this));
	add(m_h = new Textbox(m_id++, 8 + panel_w / 4 * 3 + 4, m_element_y, panel_w / 4, 20, "0", this));

	m_element_y += 25;
	add(new Label(m_id++, 8, m_element_y, "U:", this));
	add(new Label(m_id++, 16 + panel_w / 2, m_element_y, "V:", this));
	add(m_u = new Textbox(m_id++, 8 + panel_w / 4 + 4, m_element_y, panel_w / 4, 20, "0", this));
	add(m_v = new Textbox(m_id++, 8 + panel_w / 4 * 3 + 4, m_element_y, panel_w / 4, 20, "0", this));

	m_w->set_flags(TEXTBOX_NUMERIC); m_w->set_cutoff(20);
	m_h->set_flags(TEXTBOX_NUMERIC); m_h->set_cutoff(20);
	m_u->set_flags(TEXTBOX_NUMERIC); m_u->set_cutoff(20);
	m_v->set_flags(TEXTBOX_NUMERIC); m_v->set_cutoff(20);

	m_element_y += 40;
}

void DialogNewElement::add_keycode_elements(void)
{
	uint16_t panel_w = 254;

	if (m_element_y == 0)
		m_element_y = 30;
	add(new Label(m_id++, 8, m_element_y, "Key code:", this));
	m_element_y += 25;
	add(m_keycode = new Textbox(m_id++, 8, m_element_y, panel_w, 20, "0", this));
	m_keycode->set_cutoff(10);
	m_element_y += 40;
}

void DialogNewElement::add_element_id(void)
{
	uint16_t panel_w = 254;

	if (m_element_y == 0)
		m_element_y = 30;
	add(new Label(m_id++, 9, m_element_y, "Element id:", this));
	m_element_y += 25;
	add(m_element_id = new Textbox(m_id++, 8, m_element_y, panel_w, 20, "unnamed", this));
	m_element_y += 40;
}
