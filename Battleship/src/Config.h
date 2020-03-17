#ifndef _CONFIG_H
#define _CONFIG_H

namespace cfg
{
    static const int mode_width = 800;
    static const int mode_height = 400;
    //
    static const unsigned int field_rows = 10;
    static const unsigned int field_cols = 10;
    static const float field_cell_size = 30.f;
    //
    static const int max_ship_decks_count = 20; // макс число палуб каждого игрока
};

#endif // !_CONFIG_H