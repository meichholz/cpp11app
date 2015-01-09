ENV["LANG"]="C"
load "devsupport/tasks/setup.rb"

# some hints:
# - run check suite in Vim by <F11>
# - binary is `./build_dir/src/cppeleven`
#
ds_tasks_for :cmake

ds_configure do |c|
  c.debug_rake = true # or run with: DEBUG_RAKE=1 rake ...
  c.run_arguments = "showcase"
  c.editfiles = FileList.new(["README.md", "**/test_*.cc"])
  c.gcc_versions = [ "4.9.3", "4.8.4", "4.8.3", "4.8.2", "4.8", "4.7.3", "4.7.2", "4.6.3" ]
  c.cmake_options = "-DDEBUG=ON"
end

ds_post_configure

CLEAN.include "doxygen"

desc 'pull in google test framework source'
task 'pull:gtest' do
  chdir 'googletest' do
    Dir['g*-1*'].each do |treename|
      puts "removing #{treename}"
      FileUtils.rm_rf treename
    end
    [ 'test', 'mock' ].each do |atype|
      zipfile = "g#{atype}-1.7.0.zip"
      sh "wget https://google#{atype}.googlecode.com/files/#{zipfile}"
      puts "unpacking #{zipfile}"
      sh "unzip #{zipfile}"
      FileUtils.rm zipfile
    end
  end
end


