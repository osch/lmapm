package = "lmapm"
version="scm-0"
local versionNumber = version:gsub("^(.*)-.-$", "%1")
source = {
  url = "https://github.com/starwing/lmapm/archive/master.zip",
  dir = "lmapm-master",
}
description = {
   summary = "A complete arbitrary precision library",
   detailed = [[
      A complete arbitrary precision library for Lua, based on MAPM.
   ]],
   homepage = "http://www.tecgraf.puc-rio.br/~lhf/ftp/lua/#lmapm",
   license = "Public domain"
}
dependencies = {
  "lua >= 5.1, <= 5.4",
}

build = {
  type = "builtin",
  modules = {
    mapm = {
      defines = { 
      	"VERSION=\""..versionNumber.."\"" 
      },
      sources = { 
          "lmapm.c",
          "mapm_one.c"
      },
      incdirs = {
        "mapm"
      },
    },
  }
}
