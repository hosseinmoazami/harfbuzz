/*
 * Copyright © 2011  Google, Inc.
 *
 *  This is part of HarfBuzz, a text shaping library.
 *
 * Permission is hereby granted, without written agreement and without
 * license or royalty fees, to use, copy, modify, and distribute this
 * software and its documentation for any purpose, provided that the
 * above copyright notice and the following two paragraphs appear in
 * all copies of this software.
 *
 * IN NO EVENT SHALL THE COPYRIGHT HOLDER BE LIABLE TO ANY PARTY FOR
 * DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES
 * ARISING OUT OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN
 * IF THE COPYRIGHT HOLDER HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH
 * DAMAGE.
 *
 * THE COPYRIGHT HOLDER SPECIFICALLY DISCLAIMS ANY WARRANTIES, INCLUDING,
 * BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
 * FITNESS FOR A PARTICULAR PURPOSE.  THE SOFTWARE PROVIDED HEREUNDER IS
 * ON AN "AS IS" BASIS, AND THE COPYRIGHT HOLDER HAS NO OBLIGATION TO
 * PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS.
 *
 * Google Author(s): Behdad Esfahbod
 */

#ifndef TEST_OT_FACE_NO_MAIN
#include "hb-test.h"
#endif
#include <hb-ot.h>

/* Unit tests for hb-ot-*.h */


static void
test_face (hb_face_t *face,
	   hb_codepoint_t cp)
{
  hb_font_t *font = hb_font_create (face);
  hb_set_t *set;
  hb_codepoint_t g;
  hb_position_t x, y;
  char buf[5] = {0};
  unsigned int len;
  hb_glyph_extents_t extents;
  hb_ot_font_set_funcs (font);

  set = hb_set_create ();
  hb_face_collect_unicodes (face, set);
  hb_face_collect_variation_selectors (face, set);
  hb_face_collect_variation_unicodes (face, cp, set);

  hb_font_get_nominal_glyph (font, cp, &g);
  hb_font_get_variation_glyph (font, cp, cp, &g);
  hb_font_get_glyph_h_advance (font, cp);
  hb_font_get_glyph_v_advance (font, cp);
  hb_font_get_glyph_h_origin (font, cp, &x, &y);
  hb_font_get_glyph_v_origin (font, cp, &x, &y);
  hb_font_get_glyph_extents (font, cp, &extents);
  hb_font_get_glyph_contour_point (font, cp, 0, &x, &y);
  hb_font_get_glyph_name (font, cp, buf, sizeof (buf));
  hb_font_get_glyph_from_name (font, buf, strlen (buf), &g);

  hb_ot_color_has_palettes (face);
  hb_ot_color_palette_get_count (face);
  hb_ot_color_palette_get_name_id (face, cp);
  hb_ot_color_palette_color_get_name_id (face, cp);
  hb_ot_color_palette_get_flags (face, cp);
  hb_ot_color_palette_get_colors (face, cp, 0, NULL, NULL);
  hb_ot_color_has_layers (face);
  hb_ot_color_glyph_get_layers (face, cp, 0, NULL, NULL);
  hb_ot_color_has_svg (face);
  hb_blob_destroy (hb_ot_color_glyph_reference_svg (face, cp));
  hb_ot_color_has_png (face);
  hb_blob_destroy (hb_ot_color_glyph_reference_png (font, cp));

  hb_set_t *lookup_indexes = hb_set_create ();
  hb_set_add (lookup_indexes, 0);
  hb_ot_layout_closure_lookups (face, HB_OT_TAG_GSUB, set, lookup_indexes);

  hb_map_t *lookup_mapping = hb_map_create ();
  hb_map_set (lookup_mapping, 0, 0);
  hb_set_t *feature_indices = hb_set_create ();
  hb_ot_layout_closure_features (face, HB_OT_TAG_GSUB, lookup_mapping, feature_indices);
  hb_set_destroy (lookup_indexes);
  hb_set_destroy (feature_indices);
  hb_map_destroy (lookup_mapping);

  hb_ot_layout_get_baseline (font, HB_OT_LAYOUT_BASELINE_TAG_HANGING, HB_DIRECTION_RTL, HB_SCRIPT_HANGUL, HB_TAG_NONE, NULL);

  hb_ot_layout_has_glyph_classes (face);
  hb_ot_layout_has_substitution (face);
  hb_ot_layout_has_positioning (face);

  hb_ot_math_has_data (face);
  hb_ot_math_get_constant (font, HB_OT_MATH_CONSTANT_MATH_LEADING);
  hb_ot_math_get_glyph_italics_correction (font, cp);
  hb_ot_math_get_glyph_top_accent_attachment (font, cp);
  hb_ot_math_is_glyph_extended_shape (face, cp);
  hb_ot_math_get_glyph_kerning (font, cp, HB_OT_MATH_KERN_BOTTOM_RIGHT, 0);
  hb_ot_math_get_glyph_variants (font, cp, HB_DIRECTION_TTB, 0, NULL, NULL);
  hb_ot_math_get_min_connector_overlap (font, HB_DIRECTION_RTL);
  hb_ot_math_get_glyph_assembly (font, cp, HB_DIRECTION_BTT, 0, NULL, NULL, NULL);

  hb_ot_meta_get_entry_tags (face, 0, NULL, NULL);
  hb_blob_destroy (hb_ot_meta_reference_entry (face, HB_OT_META_TAG_DESIGN_LANGUAGES));

  hb_ot_metrics_get_position (font, HB_OT_METRICS_TAG_HORIZONTAL_ASCENDER, NULL);
  hb_ot_metrics_get_variation (font, HB_OT_METRICS_TAG_UNDERLINE_OFFSET);
  hb_ot_metrics_get_x_variation (font, HB_OT_METRICS_TAG_STRIKEOUT_OFFSET);
  hb_ot_metrics_get_y_variation (font, HB_OT_METRICS_TAG_SUPERSCRIPT_EM_X_OFFSET);

  len = sizeof (buf);
  hb_ot_name_list_names (face, NULL);
  hb_ot_name_get_utf8 (face, cp, NULL, &len, buf);
  hb_ot_name_get_utf16 (face, cp, NULL, NULL, NULL);
  hb_ot_name_get_utf32 (face, cp, NULL, NULL, NULL);

  hb_ot_var_get_axis_count (face);
  hb_ot_var_get_axis_infos (face, 0, NULL, NULL);
  hb_ot_var_normalize_variations (face, NULL, 0, NULL, 0);
  hb_ot_var_normalize_coords (face, 0, NULL, NULL);

  hb_ot_glyph_decompose_funcs_t *funcs = hb_ot_glyph_decompose_funcs_create ();
  for (unsigned gid = 0; gid < 10; ++gid)
    hb_ot_glyph_decompose (font, gid, funcs, NULL);
  hb_ot_glyph_decompose_funcs_destroy (funcs);

  hb_set_destroy (set);
  hb_font_destroy (font);
}

#ifndef TEST_OT_FACE_NO_MAIN
static void
test_ot_face_empty (void)
{
  test_face (hb_face_get_empty (), 0);
}

static void
test_ot_var_axis_on_zero_named_instance (void)
{
  hb_face_t *face = hb_test_open_font_file ("fonts/Zycon.ttf");
  g_assert (hb_ot_var_get_axis_count (face));
  hb_face_destroy (face);
}

int
main (int argc, char **argv)
{
  hb_test_init (&argc, &argv);

  hb_test_add (test_ot_face_empty);
  hb_test_add (test_ot_var_axis_on_zero_named_instance);

  return hb_test_run();
}
#endif
