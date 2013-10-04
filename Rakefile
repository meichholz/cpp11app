ENV["LANG"]="C"

# auto-detect maximum c compiler
[
#  "4.8.1",
  "4.7.3", "4.7.2",
  "4.6.3",
].each do |ver|
  if system("g++-#{ver} --version")
    # puts "setting gcc-#{ver} as preferred compiler"
    ENV["CXX"]="g++-#{ver}"
    ENV["CC"]="gcc-#{ver}"
    break
  end
end
@run_arguments = "--testrun=showcase"
@editfiles = FileList.new(["README.ROADMAP", "**/test_*.cc"])

load "devsupport/tasks/cmake.rake"

desc "send test suite through valgrind"
task :grindcheck => :build do
  sh "valgrind --leak-check=full --show-reachable=yes build_dir/tests/unit/test_main"
end

desc "run cucumber"
task :cuke => :build do
  sh "cucumber tests/features"
end

