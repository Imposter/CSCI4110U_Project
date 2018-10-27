--
-- Utility Functions
--

-- Relative to Absolute
function _RA(p)
	return path.getabsolute(p)
end

--
-- Main Functions
--

function createProjects(dir)
	local projectFiles = os.matchfiles(dir .. "/**.proj.lua")
	for i, v in ipairs(projectFiles) do
		print("Generating project " .. v)
		include(v)
	end
end

function main()	
	-- Generate projects
	workspace("CSCI4110U Project")
		configurations({ "Debug", "Release" })
		location("build")
	createProjects("dependencies")
	createProjects("project")
end

main()