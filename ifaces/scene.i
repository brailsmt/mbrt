
%{
#include "scene.h"
%}

%nodefaultctor Scene;
class Scene {
    public:
        static Scene * get_instance();

        Camera get_camera () const;
        void set_camera (Camera _camera);

        std::string get_output_filename() const;
        void set_output_filename(std::string fname);

        int get_subpixel_sqrt() const;
        void set_subpixel_sqrt(int subpx_sqrt);

        int get_report_frequency();
        void set_report_frequency(int freq);

        void set_pixel_width(int pxwidth);
        int get_viewport_pixel_width() const;

        void set_pixel_height(int pxheight);
        int get_viewport_pixel_height() const;

        int get_max_recurse_depth();
        void set_max_recurse_depth(int recurse_depth);

        void set_window_size(double w, double h);
        double get_window_width();
        double get_window_height();

        void add_primitive(Renderable * prim);
};
