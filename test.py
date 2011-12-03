
import rpg

print rpg.version()

N = 10
poly = rpg.rpg(N)

print "generated polygon with",N,"vertices"

print "The vertices are:"
n=0
for p in poly:
    print n," ",p
    n=n+1
