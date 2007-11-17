#!/usr/bin/env ruby
# file name:  extconf.rb
# author:     Michael Brailsford
# created:    Thu Nov 15 19:26:41 -0600 2007
# copyright:  (c) 2007 Michael Brailsford
# version:    $Revision$
# contents:   

require 'mkmf'

$CPPFLAGS = "-I../plugins -I../common -I../src -I/usr/include/libxml2 -I/usr/include/sigc++-2.0 -I/usr/lib/sigc++-2.0/include -fPIC"
$LDFLAGS = "-lxml2 -lsigc-2.0 -lMagick++ -lWand -lMagick ../plugins/plugin.so ../src/renderable.o  ../src/scene.o ../src/sceneparser.o"

dir_config("mbrt")

create_makefile("mbrt")

=begin
	change log

	$Log$
=end

