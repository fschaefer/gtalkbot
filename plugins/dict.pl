#!/usr/bin/perl -w
# 为了除去对WWW::Mechanize的依赖， 使用了curl命令
$_ = `curl -s "http://dict.cn/ws.php?q=$ARGV[0]"`;
use Encode qw/from_to/;
from_to($_, "gbk", "utf8");
/<def>(.*)<\/def>/s;
print "$1\n";
