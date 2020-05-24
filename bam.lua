conf = ScriptArgs["conf"] or "debug"
build_dir = PathJoin("build", conf)
src_dir = "src"
obj_dir = PathJoin(build_dir, "obj")

AddTool(function(s)
	s.cc.flags:Add("-Wall")
	s.cc.flags:Add("-Wextra")
	s.cc.flags_cxx:Add("--std=c++17")
	s.cc.includes:Add(src_dir)
	s.link.libs:Add("vulkan")

	s.cc.Output = function(s, input)
		input = input:gsub("^"..src_dir.."/", "")
		return PathJoin(obj_dir, PathBase(input))
	end

	s.link.Output = function(s, input)
		return PathJoin(build_dir, PathBase(input))
	end
end)

s = NewSettings()

src = CollectRecursive(PathJoin(src_dir, "*.cpp"))
obj = Compile(s, src)
bin = Link(s, "bin", obj)
PseudoTarget("c", bin)

AddJob("r", "running '"..bin.."'...", "./"..bin)
AddDependency("r", bin)

DefaultTarget("c")
