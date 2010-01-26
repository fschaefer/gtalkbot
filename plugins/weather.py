#!/bin/env python2
# -*- coding: utf-8 -*-
"""
sina天气预报
"""
import sys
import urllib, urllib2
from lxml import etree

# 显示帮助
def show_help():
    """使用帮助
    """
    print('Usage: weather argument')
    print('argument can be city name or phone code')

def sina_weather(query=''):
    url = 'http://php.weather.sina.com.cn/search.php?city={0}'
    url = url.format(urllib.quote_plus(query))
    request = urllib2.urlopen(url)
    content = request.read().decode('gb2312', 'ignore')
    # 使用xpath获取网页中的特定元素
    tree = etree.fromstring(content,etree.HTMLParser())
    links = tree.xpath('/html/body/div[@id="wrap"]/div[@class="nav2"]/div[@class="nav22"]/a')
    if not links:
        return
    return links[-1].values()[0]

if __name__ == '__main__':
    if len(sys.argv) != 2:
        sys.exit(show_help())
    res = sina_weather(sys.argv[1])
    if res:
        res = res.split('\'')[3]
        print(res.encode('utf-8', 'ignore'))
    else:
        print('cann\'t found')
