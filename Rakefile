ENV["LANG"]="C"

# auto-detect maximum c compiler
# TODO: should go to cmake.rake as well
[
#  "4.8.1",
  "4.7.3", "4.7.2",
  "4.6.3",
].each do |ver|
  if system("g++-#{ver} --version")
    # puts "setting gcc-#{ver} as preferred compiler"
    ENV["CXX"]="g++-#{ver}"
    ENV["CC"]="gcc-#{ver}"
    ENV["GCOV"]="gcov-#{ver}"
    break
  end
end
@run_arguments = "--testrun=showcase"
@editfiles = FileList.new(["README.ROADMAP", "**/test_*.cc"])

load "devsupport/tasks/cmake.rake"

@cmake_options += " -DDEBUG=ON"

