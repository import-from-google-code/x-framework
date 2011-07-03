
#ifndef __ui_gfx_native_theme_h__
#define __ui_gfx_native_theme_h__

#pragma once

#include "base/basic_types.h"

#include "skia/ext/platform_canvas.h"

namespace gfx
{

    class Rect;
    class Size;

    // This class supports drawing UI controls (like buttons, text fields, lists,
    // comboboxes, etc) that look like the native UI controls of the underlying
    // platform, such as Windows or Linux.
    //
    // The supported control types are listed in the Part enum.  These parts can be
    // in any state given by the State enum, where the actual definititon of the
    // state is part-specific.
    //
    // Some parts require more information than simply the state in order to be
    // drawn correctly, and this information is given to the Paint() method via the
    // ExtraParams union.  Each part that requires more information has its own
    // field in the union.
    //
    // NativeTheme also supports getting the default size of a given part with
    // the GetPartSize() method.
    class NativeTheme
    {
    public:
        // The part to be painted / sized.
        enum Part
        {
            kCheckbox,
            kInnerSpinButton,
            kMenuList,
            kMenuCheck,
            kMenuCheckBackground,
            kMenuPopupArrow,
            kMenuPopupBackground,
            kMenuPopupGutter,
            kMenuPopupSeparator,
            kMenuItemBackground,
            kProgressBar,
            kPushButton,
            kRadio,

            // The order of the arrow enums is important, do not change without also
            // changing the code in platform implementations.
            kScrollbarDownArrow,
            kScrollbarLeftArrow,
            kScrollbarRightArrow,
            kScrollbarUpArrow,

            kScrollbarHorizontalThumb,
            kScrollbarVerticalThumb,
            kScrollbarHorizontalTrack,
            kScrollbarVerticalTrack,
            kScrollbarHorizontalGripper,
            kScrollbarVerticalGripper,
            kSliderTrack,
            kSliderThumb,
            kTabPanelBackground,
            kTextField,
            kTrackbarThumb,
            kTrackbarTrack,
            kWindowResizeGripper,
            kMaxPart,
        };

        // The state of the part.
        enum State
        {
            kDisabled,
            kHovered,
            kNormal,
            kPressed,
            kMaxState,
        };

        // Each structure below holds extra information needed when painting a given
        // part.

        struct ButtonExtraParams
        {
            bool checked;
            bool indeterminate; // Whether the button state is indeterminate.
            bool is_default; // Whether the button is default button.
            bool has_border;
            int classic_state; // Used on Windows when uxtheme is not available.
            SkColor background_color;
        };

        struct InnerSpinButtonExtraParams
        {
            bool spin_up;
            bool read_only;
            int classic_state; // Used on Windows when uxtheme is not available.
        };

        struct MenuArrowExtraParams
        {
            bool pointing_right;
            // Used for the disabled state to indicate if the item is both disabled and
            // selected.
            bool is_selected;
        };

        struct MenuCheckExtraParams
        {
            bool is_radio;
            // Used for the disabled state to indicate if the item is both disabled and
            // selected.
            bool is_selected;
        };

        struct MenuItemExtraParams
        {
            bool is_selected;
        };

        struct MenuListExtraParams
        {
            bool has_border;
            bool has_border_radius;
            int arrow_x;
            int arrow_y;
            SkColor background_color;
            int classic_state; // Used on Windows when uxtheme is not available.
        };

        struct MenuSeparatorExtraParams
        {
            bool has_gutter;
        };

        struct ProgressBarExtraParams
        {
            double animated_seconds;
            bool determinate;
            int value_rect_x;
            int value_rect_y;
            int value_rect_width;
            int value_rect_height;
        };

        struct ScrollbarArrowExtraParams
        {
            bool is_hovering;
        };

        struct ScrollbarTrackExtraParams
        {
            bool is_upper;
            int track_x;
            int track_y;
            int track_width;
            int track_height;
            int classic_state; // Used on Windows when uxtheme is not available.
        };

        struct ScrollbarThumbExtraParams
        {
            bool is_hovering;
        };

        struct SliderExtraParams
        {
            bool vertical;
            bool in_drag;
        };

        struct TextFieldExtraParams
        {
            bool is_text_area;
            bool is_listbox;
            SkColor background_color;
            bool is_read_only;
            bool is_focused;
            bool fill_content_area;
            bool draw_edges;
            int classic_state; // Used on Windows when uxtheme is not available.
        };

        struct TrackbarExtraParams
        {
            int classic_state; // Used on Windows when uxtheme is not available.
        };

        union ExtraParams
        {
            ButtonExtraParams button;
            InnerSpinButtonExtraParams inner_spin;
            MenuArrowExtraParams menu_arrow;
            MenuCheckExtraParams menu_check;
            MenuItemExtraParams menu_item;
            MenuListExtraParams menu_list;
            MenuSeparatorExtraParams menu_separator;
            ProgressBarExtraParams progress_bar;
            ScrollbarArrowExtraParams scrollbar_arrow;
            ScrollbarTrackExtraParams scrollbar_track;
            ScrollbarThumbExtraParams scrollbar_thumb;
            SliderExtraParams slider;
            TextFieldExtraParams text_field;
            TrackbarExtraParams trackbar;
        };

        // Return the size of the part.
        virtual Size GetPartSize(Part part,
            State state,
            const ExtraParams& extra) const = 0;

        // Paint the part to the canvas.
        virtual void Paint(SkCanvas* canvas,
            Part part,
            State state,
            const gfx::Rect& rect,
            const ExtraParams& extra) const = 0;

        // Supports theme specific colors.
        void SetScrollbarColors(unsigned inactive_color,
            unsigned active_color,
            unsigned track_color) const;

        // Returns a shared instance of the native theme.
        // The retuned object should not be deleted by the caller.  This function
        // is not thread safe and should only be called from the UI thread.
        static const NativeTheme* instance();

    protected:
        NativeTheme() {}
        virtual ~NativeTheme() {}

        static unsigned int thumb_inactive_color_;
        static unsigned int thumb_active_color_;
        static unsigned int track_color_;

        DISALLOW_COPY_AND_ASSIGN(NativeTheme);
    };

} //namespace gfx

#endif //__ui_gfx_native_theme_h__