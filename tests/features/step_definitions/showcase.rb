When /^I start with (\-\S+)$/ do |opt|
  @args=opt
end

Then /^I should see "(.*?)"$/ do |arg1|
  start_app @args
  @stdout.should =~ /#{arg1}/
end

