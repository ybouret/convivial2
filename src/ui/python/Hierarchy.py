
import os
from model import *


N = model()
N.initialization()

#Recuperation nombre de modes
N_modes = N.getModes(False)

print '--> Hierarchy :'

while N_modes!=1:

	#Duplication pot et obs si existence (car suppression automatique a la fin du script ExeSCFRun.py)
	if os.path.exists(N.link + '/src/ui/python/work/potential')==True:
		pot = True
		os.system('cp ' + N.link + '/src/ui/python/work/potential ' + N.link + '/src/ui/python/work/potential2')

	else:
		pot = False

	if os.path.exists(N.link + '/src/ui/python/work/obs')==True:
		obs = True
		os.system('cp ' + N.link + '/src/ui/python/work/obs ' + N.link + '/src/ui/python/work/obs2')

	else:
		obs = False


	#SCF
	os.system('python ExeSCFRun.py')

	#On recopie les duplicata de pot et obs en les renommant normalement
	if pot==True:
		os.system('cp ' + N.link + '/src/ui/python/work/potential2 ' + N.link + '/src/ui/python/work/potential')
	
	if obs==True:
		os.system('cp ' + N.link + '/src/ui/python/work/obs2 ' + N.link + '/src/ui/python/work/obs')


	#Creation modele pour pairing
	N.modelWriting('NO')


	#Pairing
	os.system('python Pairing.py')

	#On renomme inp (dernier modele utilise, qui contient les contractions optimales) en modele.inp
	os.system('mv ' + N.link + '/src/ui/python/work/inp ' + N.link + '/src/ui/python/work/modele.inp')	

	#On recopie les duplicata de pot et obs en les renommant normalement (car suppression automatique des autres a la fin du script Pairing.py)
	if pot==True:
		os.system('cp ' + N.link + '/src/ui/python/work/potential2 ' + N.link + '/src/ui/python/work/potential')
	if obs==True:
		os.system('cp ' + N.link + '/src/ui/python/work/obs2 ' + N.link + '/src/ui/python/work/obs')

	#Recuperation nombre de modes pour le step suivant (ie nombre de couples du dernier Pairing)
	N_modes = N.getModes(False)

	#Le modele.inp a pour dernier step les contractions optimales mais un OUT, on va remplacer ce OUT par un NO, car comme le script SCF execute automatiquement le 	#step indique, il va reexecuter le step des contractions optimales sauf que les fichiers de restart viennent d'etre crees donc il va trouver une erreur
	if N_modes!=1:
		N.modelSCFHierarchy()

print '--> End hierarchy'
