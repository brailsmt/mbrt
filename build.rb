
class CompilerConfig
    attr_accessor :cc, :cxx
    #{{{
    def initialize(var)
        @cflags  = Array.new
        @libs    = Array.new
        @libdirs = Array.new
        @incdirs = Array.new
        @ldflags = Array.new
        @cc      = "/usr/bin/gcc"
        @cxx     = "/usr/bin/g++"
        @var     = var
    end
    #}}}
    #{{{
    def normalize(arg, option = nil)
        rv = ''
        if arg.kind_of? Array
            rv = arg.flatten
        elsif arg.kind_of? Rant::FileList
            rv = arg.to_ary.flatten
        elsif arg.kind_of? String
            rv = arg.strip.split
        else
            raise "Unsupported type #{arg.class} in CompilerConfig::normalize"
        end

        if option
            rv.map! { |item|
                if not item.match(/^#{option}/)
                    item = "#{option}#{item}"
                end
                item
            }
        end
        rv
    end
    #}}}
    #{{{
    def add_cflags(cflags)
        @cflags << normalize(cflags)
        @cflags.flatten!.uniq!
    end
    #}}}
	#TODO:  make this so that you can add objects that indicate evaling the
	#string or just adding the string when it is used.
    #{{{
    def add_incdirs(incdirs)
        @incdirs << normalize(incdirs, "-I")
        @incdirs.flatten!.uniq!
    end
    #}}}
	#TODO:  make this so that you can add objects that indicate evaling the
	#string or just adding the string when it is used.
    #{{{
    def add_libs(libs)
        @libs << normalize(libs, "-l")
        @libs.flatten!.uniq!
    end
    #}}}
	#TODO:  make this so that you can add objects that indicate evaling the
	#string or just adding the string when it is used.
    #{{{
    def add_libdirs(libdirs)
        @libdirs << normalize(libdirs, "-L")
        @libdirs.flatten!.uniq!
    end
    #}}}
    #{{{
    def add_ldflags(ldflags)
        @ldflags << normalize(ldflags)
        @ldflags.flatten!.uniq!
    end
    #}}}
    #{{{
    def pkg_exists(pkg)
        if(`pkg-config #{pkg} --exists; echo $?` == "0\n")
            return true;
        else 
            return false;
        end
    end
    #}}}
    #{{{
    def add_pkg_config(pkg, define = nil)
        if(pkg_exists(pkg))
            self.add_cflags(  `pkg-config #{pkg} --silence-errors --cflags`      )
            self.add_libs(    `pkg-config #{pkg} --silence-errors --libs-only-l` )
            self.add_libdirs( `pkg-config #{pkg} --silence-errors --libs-only-L` )

            if(define != nil)
                self.add_cflags( "-D#{define}" )
            end
        end
    end
    #}}}
    #{{{
    def libs
        @libs.join(" ")
    end
    #}}}
    #{{{
    def cflags
        @cflags.join(" ")
    end
    #}}}
    #{{{
    def libdirs
        @libdirs.join(" ")
    end
    #}}}
    #{{{
    def incdirs
        @incdirs.join(" ")
    end
    #}}}
    #{{{
    def optional_config(option, default_enabled = true, args = nil)
        if(default_enabled == true)
            #If the option is by defualt enabled, then call the block if the
            #option is anything other than 'no'
            if(@var[option] != "no")
                yield args
            end
        else
            #If the option is by defualt disabled, then call the block only if the
            #option is 'yes'
            if(@var[option] == "yes")
                yield args
            end
        end
    end
    #}}}
    #{{{
    def ldflags
        @ldflags.join(" ")
    end
    #}}}
end
