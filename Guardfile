# A sample Guardfile
# More info at https://github.com/guard/guard#readme

# all_on_start is problematic
guard :shell do
  watch(%r{/src/.*\.(cc|h)$}) do |m|
    n m[0], "changed"
    p m
  end
end

#guard :rake, :task => 'doc:all' do
#  watch(%r{/src/.*\.(cc|h)$})
#end
# livereload could be fine

