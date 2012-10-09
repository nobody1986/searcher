#  Copyright (c) 2004-2009 Sergey Lyubka
#
#  Permission is hereby granted, free of charge, to any person obtaining a copy
#  of this software and associated documentation files (the "Software"), to deal
#  in the Software without restriction, including without limitation the rights
#  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
#  copies of the Software, and to permit persons to whom the Software is
#  furnished to do so, subject to the following conditions:
#
#  The above copyright notice and this permission notice shall be included in
#  all copies or substantial portions of the Software.
#
#  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
#  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
#  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
#  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
#  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
#  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
#  THE SOFTWARE.
#
#  $Id: mongoose.py 471 2009-08-30 14:30:21Z valenok $

"""
This module provides python binding for the Mongoose web server.

There are two classes defined:

  Connection: - wraps all functions that accept struct mg_connection pointer
    as first argument.

  Mongoose: wraps all functions that accept struct mg_context pointer as
    first argument.

  Creating Mongoose object automatically starts server, deleting object
  automatically stops it. There is no need to call mg_start() or mg_stop().
"""


import ctypes
import os


DATA_ELEMENT_LEN = 64

class node(ctypes.Structure):
    """A wrapper for struct node."""
    _fields_ = [
        ('c', ctypes.c_ubyte),
        ('friend', ctypes.c_uint),
        ('next', ctypes.c_uint),
        ('data', ctypes.c_int),
    ]

class data(ctypes.Structure):
    """A wrapper for struct data."""
    _fields_ = [
        ('d', ctypes.c_char),
    ]

class content(ctypes.Structure):
    """A wrapper for struct content."""
    _fields_ = [
        ('head', ctypes.c_char_p),
        ('body', ctypes.POINTER(node)),
        ('data', ctypes.c_void_p),
    ]
    


HAS_NO_ORDER = 1
HAS_ORDER_BUT_NEEDNT_CAT = 2
HAS_ORDER = 3



class Searcher(object):
    """A wrapper class for Searcher shared library."""

    def __init__(self,**kwargs):
        dll_extension = os.name == 'nt' and 'dll' or 'so'
        #self.dll = ctypes.CDLL('_searcher.%s' % dll_extension)
        self.dll = ctypes.cdll.LoadLibrary('./_searcher.%s' % dll_extension)
        self.nn = ctypes.c_int(0)

        self.dll.parseRuleFile.restype = ctypes.c_void_p
        self.dll.releaseFromFile.restype = ctypes.c_long
        self.dll.saveToFile.restype = ctypes.c_int
        self.dll.search.restype = ctypes.c_int

        if(kwargs.has_key('rulefile')):
            self.c = self.dll.parseRuleFile(ctypes.c_char_p(kwargs['rulefile']),ctypes.pointer(self.nn))
        else:
            if(kwargs.has_key('imgfile')):
                self.ret = self.dll.releaseFromFile(ctypes.c_char_p(kwargs['imgfile']),ctypes.pointer(self.c))
            else:
                pass
            pass
        self.c = ctypes.c_void_p(self.c)


    def __del__(self):
        """Destructor, stop Searcher instance."""
        #self.dll.mg_stop(self.ctx)
    
    def saveToFile(self,filename):
        return self.dll.saveToFile(self.c,self.nn,ctypes.c_char_p(filename))
    
    def search(self,str,level):
        return self.dll.search(self.c,ctypes.c_char_p(str),len(str),ctypes.c_int(level))

