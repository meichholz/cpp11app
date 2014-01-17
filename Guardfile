# A sample Guardfile
# More info at https://github.com/guard/guard#readme

# all_on_start is problematic
guard :shell do
#  watch(%r{/src/.*\.(cc|h)$}) do |m|
  watch "src/README.h" do |m|
    n m[0], "file changed"
    # puts m[0], ": file has changed"
    `rake doc:all`
  end
end

# livereload could be fine

