'''
Created on Oct 2, 2012

@author: Robert Phillips III
'''

import operator
import sys
import os

def is_triangle(n):
        for i in range(n):
                if (n == (i*(i+1))/2):
                        return True
        
        return False


def count_triangle_words():
        words = read_file('words.txt')
        
        count = 0
        for word in words:
                if (is_triangle(get_word_value(word))):
                        count = count+1
                        
        return count+1
        
            
def get_name_value(name, index):
        return index*sum([ord(char)-64 for char in name])


def get_word_value(word):
        return sum([ord(char)-64 for char in word])


def read_file(file_name, sort=False):
        words = [word[1:len(word)-1] for word in open(file_name, 'r').read().split(',')]
        
        if sort is True:
                words.sort()
        
        return words


def add_names():
        names = read_file('names.txt', sort=True)
        
        return sum([get_name_value(names[i], i+1) for i in range(len(read_file('names.txt', sort=True)))])


def find_product():
        number = '73167176531330624919225119674426574742355349194934\
96983520312774506326239578318016984801869478851843\
85861560789112949495459501737958331952853208805511\
12540698747158523863050715693290963295227443043557\
66896648950445244523161731856403098711121722383113\
62229893423380308135336276614282806444486645238749\
30358907296290491560440772390713810515859307960866\
70172427121883998797908792274921901699720888093776\
65727333001053367881220235421809751254540594752243\
52584907711670556013604839586446706324415722155397\
53697817977846174064955149290862569321978468622482\
83972241375657056057490261407972968652414535100474\
82166370484403199890008895243450658541227588666881\
16427171479924442928230863465674813919123162824586\
17866458359124566529476545682848912883142607690042\
24219022671055626321111109370544217506941658960408\
07198403850962455444362981230987879927244284909188\
84580156166097919133875499200524063689912560717606\
05886116467109405077541002256983155200055935729725\
71636269561882670428252483600823257530420752963450'
               
        return max([reduce(operator.mul, map(int, number[i:i+5]), 1) for i in range(len(number)-4)])


def mkpath(path):
        try:
                os.makedirs(path)
        except:
                print 'Directory "' + path + '" already exists!  Not created.'
        
        
def make_symlink(term, course):
        base = '/usr/local/classes/eecs/' + term + '/' + course + '/'
        mkpath(base + 'public_html')
        mkpath(base + 'handin')
        
        symlink_safe(base + 'public_html', term + '/' + course + '/website')
        symlink_safe(base + 'handin', term + '/' + course + '/handin')

        
def symlink_safe(path, link):
	try:
		os.symlink(path, link)
	except:
		print 'File "' + link + '" already exists!  Not created.'


def create_dirs(term, course):
        dirs = ['assignments', 'examples', 'exams', 'lecture_notes', 'submission']
        for i in dirs:
                mkpath(term + '/' + course + '/' + i)
        
        make_symlink(term, course)
        
        
if __name__ == '__main__':
        user_in = sys.stdin
    
        while True:
		print ''
                print 'What would you like to do?'
                print '1 - Create directories'
                print '2 - Find the greatest product'
                print '3 - Find the name scores'
                print '4 - Find the triangle words'
                print '5 - Exit'
        
                choice = user_in.readline()
                if cmp(choice[0], '1') == 0:
                        print 'What term is it?'
                        term = user_in.readline()
            
                        print 'What course?'
                        course = user_in.readline()
            
                        create_dirs(term[:len(term)-1], course[:len(course)-1])
                        print 'Directories Created!'
            
                elif cmp(choice[0], '2') == 0:
                        print 'Maximum Product:  ' + str(find_product())
                elif cmp(choice[0], '3') == 0: 
                        print 'Sum of Names:  ' + str(add_names())
                elif cmp(choice[0], '4') == 0:
                        print 'Number of Triangle Words:  ' + str(count_triangle_words())
                else:
                        break
    
        print ''
