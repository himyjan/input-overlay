/**
 * This file is part of input-overlay
 * which is licensed under the MPL 2.0 license
 * See LICENSE or mozilla.org/en-US/MPL/2.0/
 * github.com/univrsal/input-overlay
 */

#include "../../sources/input_source.hpp"
#include "../../../ccl/ccl.hpp"
#include "element_dpad.hpp"
#include "../util.hpp"

void element_dpad::load(ccl_config* cfg, const std::string& id)
{
    element_texture::load(cfg, id);
    auto i = 1;
    for (auto& map : m_mappings)
    {
        map = m_mapping;
        map.x += i * (m_mapping.cx + CFG_INNER_BORDER);
        i++;
    }
    m_keycode = VC_DPAD_DATA;
}

void element_dpad::draw(gs_effect_t* effect,
    gs_image_file_t* image, element_data* data, sources::shared_settings* settings)
{
    const auto d = dynamic_cast<element_data_dpad*>(data);

    if (d && d->get_direction() != DPAD_CENTER)
    {
        /* Enum starts at one (Center doesn't count)*/
        const auto map = &m_mappings[d->get_direction() - 1];
        element_texture::draw(effect, image, map);
    }
    else
    {
        element_texture::draw(effect, image, nullptr);        
    }
}

void element_data_dpad::merge(element_data* other)
{

    const auto d = dynamic_cast<element_data_dpad*>(other);
#ifdef WINDOWS
    if (d)
    {
        m_direction = d->m_direction;
    }
#else
    if (d)
    {
        m_direction = merge_directions(m_directions, d->m_direction);
    }
#endif /* !WINDOWS*/
}

dpad_direction element_data_dpad::merge_directions(const dpad_direction a, const dpad_direction b)
{
    switch (a)
    {
    case DPAD_UP:
        switch (b)
        {
        case DPAD_LEFT:
            return DPAD_TOP_LEFT;
        case DPAD_RIGHT:
            return DPAD_TOP_RIGHT;
        default:
            return DPAD_UP;
        }
    case DPAD_DOWN:
        switch (b)
        {
        case DPAD_LEFT:
            return DPAD_BOTTOM_LEFT;
        case DPAD_RIGHT:
            return DPAD_BOTTOM_RIGHT;
        default:
            return DPAD_DOWN;
        }
    case DPAD_LEFT:
        switch (b)
        {
        case DPAD_UP:
            return DPAD_TOP_LEFT;
            
        case DPAD_DOWN:
            return DPAD_BOTTOM_LEFT;
            
        default:
            return DPAD_LEFT;
        }
    case DPAD_RIGHT:
        switch (b)
        {
        case DPAD_UP:
            return DPAD_TOP_RIGHT;
        case DPAD_DOWN:
            return DPAD_BOTTOM_RIGHT;
        default:
            return DPAD_RIGHT;
        }
    default: ;
        return a;
    }
}
