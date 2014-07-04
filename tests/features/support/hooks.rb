Before('@verbose') do
  $verbose_app = true
end
Before('~@verbose') do
  $verbose_app = false 
end

Before('@debug') do
  $debug_app = true
end
Before('~@debug') do
  $debug_app = false 
end
