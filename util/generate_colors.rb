#!/usr/bin/env ruby
# file name:  generate_colors.rb
# author:     Michael Brailsford
# created:    Mon Oct 15 22:21:58 -0500 2007
# copyright:  (c) 2007 Michael Brailsford
# version:    $Revision$
# contents:   This reads /usr/share/X11/rgb.txt into C++ code.

class Color
    attr_accessor :name, :red, :green, :blue

    def initialize(color_def)
        color_def.strip!
        line = color_def.split(/\s+/)

        @red   = line[0]
        @green = line[1]
        @blue  = line[2]

        @name = line[3..-1].join(" ").downcase
        @name.gsub!(/\s+/, "_")
    end
end

class Colors
    attr_accessor :colors

    def initialize(color_def_fname)
        @colors = Array.new
        File.open(color_def_fname).each { |line|
            next if line =~ /^!.*/
            @colors << Color.new(line.strip)
        }
    end
end

if ARGV[0].downcase == "xml"
    colors = Colors.new(ARGV[1])

    puts "<!--"
    puts "  This is generated from X11's rgb.txt file.  For each color listed in that file a "
    puts "  color and a solid material will be generated."
    puts "-->"
    puts "<scene>"
    puts "    <colors>"
    colors.colors.each { |color|
        puts "        <color name=\"#{color.name}\" red=\"#{color.red}\" green=\"#{color.green}\" blue=\"#{color.blue}\"/>"
    }
    puts "    </colors>"
    puts "    <materials>"
    colors.colors.each{ |color|
        puts "        <material name=\"#{color.name}\" type=\"solid\" color=\"#{color.name}\" />"
    }
    puts "    </materials>"
    puts "</scene>"
else
    colors = Colors.new(ARGV[0])

    puts '#include "color.h"'
    puts '#include "material.h"'
    puts '#include <map>'
    puts '#include <string>'
    puts ''
    puts 'static std::map<std::string, Color *> color_map;'
    puts 'static std::map<std::string, Material *> material_map;'
    puts 'void init_color_map() {'
    colors.colors.each { |color|
        puts "    color_map[\"#{color.name}\"] = new Color(#{color.red}, #{color.green}, #{color.blue});"
    }
    puts '}'
    puts ''
    puts 'void init_material_map() {'
    #colors.colors.each { |color|
        puts "//this is unfinished"
    #}
    puts '}'
end

=begin
	change log

	$Log$
=end

