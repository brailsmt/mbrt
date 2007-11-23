#!/usr/bin/env ruby
# file name:  mbrt_ruby.rb
# author:     Michael Brailsford
# created:    Sat Nov 17 14:14:27 -0600 2007
# copyright:  (c) 2007 Michael Brailsford
# version:    $Revision$
# contents:   This file contains ruby code to help round out the mbrt ruby
#             interface.  


module Mbrt

    require 'mbrt_wrap.so'
    include Mbrt_wrap

    Dimension = Struct.new(:x, :y)
    SceneMeta = Struct.new(:pixel_dimension, :output_fname, :subpixels, :max_depth, :win_dimension, :rpt_freq)

    class Mbrt_wrap::Scene
        def to_s
            puts "foo"
        end

        def <<(obj)
            if obj.is_a?(Renderable)
                add_primitive(obj)
            elsif obj.is_a?(Camera)
                set_camera(obj)
            elsif obj.is_a?(Material)
                add_material(obj)
            elsif obj.is_a?(BumpMap)
                add_bumpmap(obj)
            elsif obj.is_a?(SceneMeta)
                set_pixel_width(obj.pixel_dimension.x)
                set_pixel_height(obj.pixel_dimension.y)
                set_subpixel_sqrt(obj.subpixels)
                set_max_recurse_depth(obj.max_depth)
                set_window_size(obj.win_dimension.x, obj.win_dimension.y)
            end
        end
    end
end

=begin
	change log

	$Log$
=end

