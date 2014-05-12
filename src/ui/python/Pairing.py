
from output import *
from model import *
from step import *


#Recuperation valeur basis trunc
O = output()
O.initialization()

basis_trunc = O.getTrunc()


#Recuperation nombre de modes
M = model()
M.initialization()

N_modes = M.getModes(True)

print '-> Pairing :'
print 'N = ', N_modes


#Step
S = step()
S.pairingInitialization()

S.pairingStep(N_modes, basis_trunc)

print '-> End pairing'
