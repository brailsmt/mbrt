#!/usr/bin/env ruby
# file name:  nff2mbrt.rb
# author:     Michael Brailsford
# created:    Mon Oct 22 22:02:13 -0500 2007
# copyright:  (c) 2007 Michael Brailsford
# version:    $Revision$
# contents:   This script reads the contents of a file in NFF format and
#             translates it into mbrt's XML format.

#{{{
class Point
    attr_accessor :x, :y, :z
    def initialize(x, y, z)
        @x = x
        @y = y
        @z = z
    end
    def to_s
        "(#{@x}, #{@y}, #{@z})"
    end
end
#}}}
#{{{
class Color
    attr_accessor :name, :red, :green, :blue
    def initialize(name, r, g, b)
        @name  = name
        @red   = r
        @green = g
        @blue  = b
    end
    def to_s
        "<color name='#{@name}' red='#{@red}' green='#{@red}' blue='#{@red}' />"
    end
end
#}}}
#{{{
class View
    attr_accessor :camera
    def initialize(camera_pt)
        @camera = camera_pt
    end
end
#}}}
#{{{
class Sphere
    attr_accessor :origin, :r, :clr
    def initialize(x, y, z, r, clr="def")
        @origin = Point.new(x,y,z)
        @r = r
        @color = clr
    end
    def to_s
        "<sphere center='#{@origin.to_s}' radius='#{@r}' color='#{@color}' />"
    end
end
#}}}
#{{{
class Light < Sphere
    attr_accessor :color
    def initialize(x, y, z, clr="def_light")
        super(x,y,z,0.001,clr)
        @color = clr
    end
end
#}}}
#{{{
class Material
    def initialize(clr, diffuse, specular, phong, transmittance, refract_index)
        @color         = clr
        @diffuse       = diffuse
        @specular      = specular
        @phong         = phong
        @transmittance = transmittance
        @refract_index = refract_index
    end
    def to_s
        "<material name='#{@color.name}' type='solid' color='#{@color.name}' />"
    end
end
#}}}
#{{{
class ConeCylinder
    def initialize(base_pt, base_r, apex_pt, apex_r)
        @basept = base_pt
        @baser = base_r
        @apexpt = apex_pt
        @apexr = apex_r
    end
    def to_s
        "<conic base='#{@basept.to_s}' base_radius='#{@baser}' apex='#{@apexpt.to_s}' apex_radius='#{@apexr}' />"
    end
end
#}}}
#{{{
class Polygon
    attr_accessor :vertices
    def initialize
        @vertices = Array.new
        puts "here"
    end
    def <<(pt)
        puts "there"
        @vertices << pt
    end
    def to_s
        str = "<polygon material=\"solidBlue\">\n"
        @vertices.each { |p|
            str += "    <vertex at=\"" + p.to_s + "\"/>\n"
        }
        str += "</polygon>\n"
    end
end
#}}}
#{{{
class Mesh
    attr_accessor :vertices_normals
    def initialize
        @vertices_normals = Array.new
    end
    def <<(pt_normal)
        @vertices_normals << pt_normal
    end
    def to_s
        str  = "<mesh>\n"
        @vertices_normals.each { |p|
            str += "    <vertex at=\"#{p[0].to_s}\" normal=\"#{p[1].to_s}\"/>\n"
        }
        str += "</mesh>"
    end
end
#}}}

#{{{
class Meta
    attr_accessor :width, :height
    def initialize
    end
    def to_s
        str = <<-EOL
        <meta>
            <width     pixels="#{@width}"  /> 
            <height    pixels="#{@height}" /> 
            <subpixels square_of="5"       /> 
            <recurse   max_depth="15"      /> 
        </meta>
        EOL
    end
end
#}}}
#{{{
class Mbrt
    attr_accessor :meta, :camera, :colors, :materials, :light, :objects
    def initialize
        @colors    = Array.new
        @materials = Array.new
        @light     = Array.new
        @objects   = Array.new
    end
    def to_s
        str = ""
        @colors.each { |c| str << c.to_s + "\n" }
        @materials.each { |m| str << m.to_s + "\n" }
        @objects.each { |obj| str << obj.to_s + "\n" }
        str
    end
end
#}}}

def main
    mbrt = Mbrt.new
    cname = "color"
    cnum = 0
    nff = File.open(ARGV[0])
    while true
        line = nff.readline.split('#')[0]
        if line =~ /^#/
            next
        end

        case line
        when /^s/
            (type, x, y, z, r) = line.split
            mbrt.objects << Sphere.new(x,y,z,r,cname+cnum.to_s)
        when /^c/
            (type, x1, y1, z1, r1, x2, y2, z2, r2) = line.split
            p1 = Point.new(x1,y1,z1)
            p2 = Point.new(x2,y2,z2)
            mbrt.objects << ConeCylinder.new(p1, r1, p2, r2)
        when /^f/
            (type, r, g, b, diff, spec, ph, trans, refract) = line.split
            cnum += 1
            clr = Color.new(cname+cnum.to_s, r, g, b)
            mbrt.colors << clr
            mbrt.materials << Material.new(clr, diff, spec, ph, trans, refract)
        when /^p /
            (type, num_vertices) = line.split
            p = Polygon.new()
            1.upto(num_vertices.to_i) {
                line = nff.readline.split('#')[0]
                if line =~ /^#/
                    next
                end
                (x, y, z) = line.split
                p << Point.new(x, y, z)
            }
            mbrt.objects << p
        when /^pp/
            (type, num_vertices) = line.split
            p = Mesh.new()
            1.upto(num_vertices.to_i) {
                line = nff.readline.split('#')[0]
                if line =~ /^#/
                    next
                end
                (x, y, z, nx, ny, nz) = line.split
                p << [ Point.new(x, y, z), Point.new(nx, ny, nz) ]
            }
            mbrt.objects << p
        end
    end
rescue EOFError
    puts mbrt.to_s
end

main()

=begin
	change log

	$Log$
=end

