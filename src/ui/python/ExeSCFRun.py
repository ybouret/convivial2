
from step import *
from output import *


#Recuperation valeur de tolerance
O = output()
O.initialization()

scftol = O.getTol()


#Recuperation nombre de modes
M = model()
M.initialization()

N_modes = M.getModes(False)

print '-> SCF :'
print 'N = ', N_modes


#Execution step
S = step()
S.initialization()

S.convivExecution()

#Recuperation nombre de steps
num_step = S.getStep()

#Verification convergence, tant qu'elle est False on continue le SCF
while S.convergence(scftol)==False:
	num_step = num_step + 1
	print 'step ' + str(num_step) + ' scf', 'N = ', N_modes
	S.scfStep(num_step,'step'+str(num_step),N_modes)

print '-> End SCF'
