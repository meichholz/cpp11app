def temp_cleanup
  `rm tmp/* 2>/dev/null`
end

def read_logfile(logname)
  log = Array.new
  begin
    File.open logname do |f| # FIXME: shorter, faster?
      f.each_line do |l|
        log << l.chomp
      end
    end
  rescue Errno::ENOENT => err
    fail "no logfile `#{logname}'"
  end
  return log
end

