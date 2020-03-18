#ifndef _CONFIG_H
#define _CONFIG_H

namespace cfg
{
    static const int default_mode_width = 800;
    static const int default_mode_height = 400;
    static const int mode_width = 1280;
    static const int mode_height = 680;
    //
    static const unsigned int field_rows = 10;
    static const unsigned int field_cols = 10;
    static const float default_field_cell_size = 30.f;
    static const float field_cell_size = default_field_cell_size * mode_width / default_mode_width;
    //
    static const int max_ship_decks_count = 20;
};

#endif // !_CONFIG_H