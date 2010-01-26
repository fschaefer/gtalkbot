#!/bin/env python2
# -*- coding: utf-8 -*-
"""
google搜索
"""

import sys
import urllib, urllib2
from lxml import etree

# 显示帮助
def show_help():
    """使用帮助
    """
    print("Usage:google keyword")

def google_search(query='', start=0):
    """使用ajax进行google搜索
    """
    url = 'http://ajax.googleapis.com/ajax/services/search/web?v=1.0&q={0}&start={1}&rsz=large'
    url = url.format(urllib.quote_plus(query), start)
    try: 
        content = urllib2.urlopen(url).read()
    except urllib2.URLError: 
        print "------ Error opening " + url + "..... Timed out?" 
        return None 
    # 在python中使用None表示空值
    content = content.replace(": null,", ": None,")
    exec("data = " + content)
    results = data["responseData"]["results"]
    return results

if __name__ == '__main__':
    if len(sys.argv) != 2:
        sys.exit(show_help())
    res = google_search(sys.argv[1])
    for x in res:
        print x['titleNoFormatting']
        print x['url']
