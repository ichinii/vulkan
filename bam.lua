conf = ScriptArgs["conf"] or "debug"
build_dir = PathJoin("build", conf)
src_dir = "src"
res_dir = "res"
obj_dir = PathJoin(build_dir, "obj")

AddTool(function(s)
	s.cc.flags:Add('-g')
	s.cc.flags:Add("-Wall")
	s.cc.flags:Add("-Wextra")
	s.cc.flags_cxx:Add("--std=c++23")
	s.cc.includes:Add(src_dir)
	s.link.libs:Add("vulkan")
	s.link.libs:Add("glfw")

	s.cc.Output = function(s, input)
		input = input:gsub("^"..src_dir.."/", "")
		return PathJoin(obj_dir, PathBase(input))
	end

	s.link.Output = function(s, input)
		return PathJoin(build_dir, PathBase(input))
	end
end)

s = NewSettings()

PseudoTarget("shader")
shader_files = TableFlatten({
	Collect(PathJoin(res_dir, "*.vert")),
	Collect(PathJoin(res_dir, "*.frag")),
})
for i, file in ipairs(shader_files) do
	local out = PathJoin(build_dir, file..".spv")
	AddJob(out, "compiling shader '"..file.."' > '"..out.."'", "glslangValidator -V "..file.." -o "..out)
	AddDependency(out, file)
	AddDependency("shader", out)
end

function is_shader(res_file)
	for _, shader_file in ipairs(shader_files) do
		if res_file == shader_file then
			return true
		end
	end
	return false
end

PseudoTarget("res")
res_files = Collect(PathJoin(res_dir, "*"))
for i, file in ipairs(res_files) do
	if not is_shader(file) then
		local out = PathJoin(build_dir, file)
		AddJob(out, "copy resource '"..file.."' > '"..out.."'", "cp "..file.." "..out)
		AddDependency(out, file)
		AddDependency("res", out)
	end
end

src = CollectRecursive(PathJoin(src_dir, "*.cpp"))
obj = Compile(s, src)
bin = Link(s, "bin", obj)
PseudoTarget("compile", bin)
PseudoTarget("c", "compile")

PseudoTarget("app", "compile", "shader", "res")

AddJob("run", "running '"..bin.."'...", "cd "..build_dir.." && ./"..PathFilename(bin))
AddDependency("run", "app")
PseudoTarget("r", "run")

DefaultTarget("app")
