ENV["LANG"]="C"

load 'devsupport/tasks/setup.rb'
ds_tasks_for :cmake

ds_configure do |c|
  c.run_arguments = "--testrun=showcase"
  c.editfiles = FileList.new(["README.ROADMAP", "**/test_*.cc"])
  c.gcc_versions = [ "4.8.2", "4.7.3", "4.7.2", "4.6.3" ]
  c.cmake_options = " -DDEBUG=ON"
end

ds_post_configure

CLEAN.include "doxygen"

