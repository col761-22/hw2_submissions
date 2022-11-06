import sys
if __name__=='__main__':
    g_f=sys.argv[1]
    op=open("input_graph",'w')
    op.write(g_f)
    print('index build done')