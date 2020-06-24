/*************************************************************************
 * This file is part of input-overlay
 * github.con/univrsal/input-overlay
 * Copyright 2020 univrsal <uni@vrsal.cf>.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 2 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *************************************************************************/

#include "element_analog_stick.hpp"
#include <keycodes.h>

void element_analog_stick::load(const QJsonObject &obj)
{
	element_texture::load(obj);
	m_side = static_cast<element_side>(obj[CFG_SIDE].toInt());
	m_radius = static_cast<uint8_t>(obj[CFG_STICK_RADIUS].toInt());
	m_keycode = VC_STICK_DATA;
	m_pressed = m_mapping;
	m_pressed.y = m_mapping.y + m_mapping.cy + CFG_INNER_BORDER;
}

void element_analog_stick::draw(gs_effect_t *effect, gs_image_file_t *image, element_data *data,
								sources::overlay_settings *settings)
{
	if (data) {
		const auto stick = dynamic_cast<element_data_analog_stick *>(data);
		if (stick) {
			auto pos = m_pos;
			gs_rect *temp = nullptr;

			if (m_side == ES_LEFT)
				temp = stick->left_pressed() ? &m_pressed : &m_mapping;
			else
				temp = stick->right_pressed() ? &m_pressed : &m_mapping;
			calc_position(&pos, stick, settings);
			element_texture::draw(effect, image, temp, &pos);
		}
	} else {
		element_texture::draw(effect, image, nullptr);
	}
}

void element_analog_stick::calc_position(vec2 *v, element_data_analog_stick *d,
										 sources::overlay_settings *settings) const
{
	UNUSED_PARAMETER(settings);
	/* TODO: deadzones combined with bindings? */
	switch (m_side) {
	case ES_LEFT:
//#if _WIN32
//		if (!DEAD_ZONE(d->get_left_stick()->x, settings->left_dz))
//#endif
//			v->x += d->get_left_stick()->x * m_radius;
//#if _WIN32
//		if (!DEAD_ZONE(d->get_left_stick()->y, settings->left_dz))
//#endif
//			v->y += d->get_left_stick()->y * m_radius;
//		break;
//	case ES_RIGHT:
//#if _WIN32
//		if (!DEAD_ZONE(d->get_right_stick()->x, settings->right_dz))
//#endif
//			v->x += d->get_right_stick()->x * m_radius;
//#if _WIN32
//		if (!DEAD_ZONE(d->get_right_stick()->y, settings->right_dz))
//#endif
			v->y += d->get_right_stick()->y * m_radius;
		break;
	default:;
	}
}

void element_data_analog_stick::set_state(const button_state left, const button_state right)
{
	m_left_state = left;
	m_right_state = right;
}

bool element_data_analog_stick::merge(element_data *other)
{
	bool result = false;
	if (other && other->get_type() == m_type) {
		const auto other_stick = dynamic_cast<element_data_analog_stick *>(other);
		if (other_stick) {
			switch (other_stick->m_data_type) {
			case SD_BOTH:
				/* If either of the two sticks
                     * are switching from unpressed to pressed
                     * input history should update */
				if (other_stick->m_left_state == BS_PRESSED && m_left_state == BS_RELEASED)
					result = true;
				if (other_stick->m_right_state == BS_PRESSED && m_right_state == BS_RELEASED)
					result = true;
				m_left_stick = other_stick->m_left_stick;
				m_right_stick = other_stick->m_right_stick;
				m_left_state = other_stick->m_left_state;
				m_right_state = other_stick->m_right_state;
				break;
			case SD_PRESSED_STATE_LEFT:
				if (other_stick->m_left_state == BS_PRESSED && m_left_state == BS_RELEASED)
					result = true;
				m_left_state = other_stick->m_left_state;
				if (m_data_type != SD_PRESSED_STATE_LEFT)
					m_data_type = SD_BOTH;
				break;
			case SD_PRESSED_STATE_RIGHT:
				if (other_stick->m_right_state == BS_PRESSED && m_right_state == BS_RELEASED)
					result = true;
				m_right_state = other_stick->m_right_state;
				if (m_data_type != SD_PRESSED_STATE_RIGHT)
					m_data_type = SD_BOTH;
				break;
			case SD_LEFT_X:
				m_left_stick.x = other_stick->m_left_stick.x;
				if (m_data_type != SD_LEFT_X)
					m_data_type = SD_BOTH;
				break;
			case SD_LEFT_Y:
				m_left_stick.y = other_stick->m_left_stick.y;
				if (m_data_type != SD_LEFT_Y)
					m_data_type = SD_BOTH;
				break;
			case SD_RIGHT_X:
				m_right_stick.x = other_stick->m_right_stick.x;
				if (m_data_type != SD_RIGHT_X)
					m_data_type = SD_BOTH;
				break;
			case SD_RIGHT_Y:
				m_right_stick.y = other_stick->m_right_stick.y;
				if (m_data_type != SD_RIGHT_Y)
					m_data_type = SD_BOTH;
				break;
			}
		}
	}
	return result;
}

element_data_analog_stick *element_data_analog_stick::from_buffer(netlib_byte_buf *buffer)
{
	auto result = new element_data_analog_stick();

	if (!netlib_read_float(buffer, &result->m_left_stick.x) || !netlib_read_float(buffer, &result->m_left_stick.y) ||
		!netlib_read_float(buffer, &result->m_right_stick.x) || !netlib_read_float(buffer, &result->m_right_stick.y)) {
#ifdef _DEBUG
		blog(LOG_INFO, "Reading of analog stick data failed: %s", netlib_get_error());
#endif
		delete result;

		return nullptr;
	}
	return result;
}
