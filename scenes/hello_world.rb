#!/usr/bin/env ruby
# file name:  scene.rb
# author:     Michael Brailsford
# created:    Thu Nov 15 21:46:02 -0600 2007
# copyright:  (c) 2007 Michael Brailsford
# version:    $Revision$
# contents:   

$: << Dir.pwd

require 'ifaces/mbrt.rb'
include Mbrt

scene = Scene::get_instance

# Set up the scene meta information
meta = SceneMeta.new
meta.pixel_dimension = Dimension.new(4096, 4096)
meta.output_fname = "renders/single_red_sphere_rb.png"
meta.subpixels = 1
meta.max_depth = 1
# Add the meta information to the scene
scene << meta

# Add the camera to the scene
scene << Camera.new(Point3D.new(0,0,-50), Point3D.new(0,0,-2))

# Add Materials to the scene
scene << Material

# Add the light sources
light = Sphere.new(Point3D.new(20,-25,-50), 0.1)
#light.set_is_light(true)
scene << light

# Add the objects to the scene
scene << Sphere.new(Point3D.new(0,0,0), 1)

puts scene
puts scene.get_geometry
puts "Hello World!"

=begin
	change log

	$Log$
=end

