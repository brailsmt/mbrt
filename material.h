
#ifndef MATERIAL_H
#define MATERIAL_H

/// This class defines various properties of materials from which primitives are made.
class Material {
    protected:
        /// This is the percentage of light that is reflected off this material.  This value is
        /// between 0 and 1, 0 means the material reflects 0% of the light, a 1 means the material
        /// reflects 100% of the light.
        double m_reflection_coefficient;

        /// This describes how much this material scatters the light off the surface.  This value is
        /// between 0 and 1, 0 means the material scatters 0% of the light, a 1 means the material
        /// scatters 100% of the light.
        double m_diffusion_factor;

        /// This number describes how reflective this material is.  Higher numbers mean shinier
        /// objects.
        double m_reflectivity;

        /// This is the color of the material.
        Color * m_color;

        /// Is this material a light source?
        bool m_is_light_source;

        /// This is how opaque the object is, or more correctly how transparent the object.  A value
        /// of 0 is fully opaque, a value of 1.0 is fully transparent.
        double m_opacity;

        /// This describes the amount of refraction occuring within the object
        double m_refraction_index; 

        /// Ensure m_opacity remains within the range of 0.0f to 1.0f.
        void normalize_opacity() {
            m_opacity = m_opacity > TRANSPARENT ? TRANSPARENT
                        : m_opacity < OPAQUE ? OPAQUE
                        : m_opacity;
        }

    public:
        Material()
                : m_reflection_coefficient(1.0f),
                m_diffusion_factor(0.25),
                m_color(new Color()),
                m_is_light_source(false),
                m_reflectivity(35.0f),
                m_refraction_index(1.35),
        m_opacity(OPAQUE) {}

        Material(bool is_light)
                : m_reflection_coefficient(1.0f),
                m_diffusion_factor(0.25f),
                m_color(new Color()),
                m_is_light_source(is_light),
                m_reflectivity(35.0f),
                m_refraction_index(1.35),
        m_opacity(OPAQUE) {}

        Material(Color * m_color, bool is_light)
                : m_reflection_coefficient(1.0f),
                m_diffusion_factor(0.25f),
                m_color(m_color),
                m_is_light_source(is_light),
                m_reflectivity(35.0f),
                m_refraction_index(1.35),
        m_opacity(OPAQUE) {}

        Material(Color * m_color, double reflect = 1.0f, double diffuse = 0.25f, bool is_light = false, double opacity = OPAQUE)
                : m_reflection_coefficient(reflect),
                m_diffusion_factor(diffuse),
                m_color(m_color),
                m_is_light_source(is_light),
                m_reflectivity(35.0f),
                m_refraction_index(1.35),
        m_opacity(opacity) {}

        Material(const Material &other)
                : m_reflection_coefficient(other.m_reflection_coefficient),
                m_diffusion_factor(other.m_diffusion_factor),
                m_color(other.m_color),
                m_is_light_source(other.m_is_light_source),
                m_reflectivity(other.m_reflectivity),
        m_opacity(other.m_opacity) {}

        ~Material() {
            delete m_color;
        }

        Color * get_color() const { return m_color; }

        bool is_light() const { return m_is_light_source; }
        void set_is_light(bool v) { m_is_light_source = v; }

        double get_diffuse() const { return m_diffusion_factor; }
        void set_diffuse(double diffuse) { m_diffusion_factor = diffuse; }

        double get_reflection() const { return m_reflection_coefficient; }
        double get_reflectivity() const { return m_reflectivity; }
        double get_refraction_index() const { return m_refraction_index; }

        double get_opacity() const { return m_opacity; }
        void set_opacity(double opacity) { m_opacity = opacity; normalize_opacity(); }
};
#endif
