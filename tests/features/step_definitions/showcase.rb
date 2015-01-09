When /^I start with "(.*?)"$/ do |opt|
  @sut ||= Sut::Daemon.new
  @sut.push_arg(opt)
  @sut.run
end

Then /^I should see "(.*?)"$/ do |arg1|
  @sut.output.should =~ /#{arg1}/
end

