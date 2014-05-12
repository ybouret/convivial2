
import os
from model import *
from pairs import *

class step:

	#Obtenir $CONV_ROOT, adaptation du modele
	def initialization(self):

		self.link = os.popen('echo $CONV_ROOT').read()
		self.link = self.link[:-1]
		print(self.link)

		P = model()
		P.initialization()
		P.modelAdaptation()



	#Initialisation pour le Pairing, pas d'adaptation de modele
	def pairingInitialization(self):

		self.link = os.popen('echo $CONV_ROOT').read()
		self.link = self.link[:-1]



	#Obtenir le numero du step du modele.inp concerne
	def getStep(self):
		
		modele = open(self.link + '/src/ui/python/work/modele.inp','r')
		for i in range(0,4):
			modele.readline()

		num = int(modele.readline())

		modele.close()

		return num
		


	#Execution d'un step a partir d'un modele.inp deja existant, pour contractions a 1 seul mode
	def convivExecution(self):

		os.system('cd work ; $CONV_ROOT/src/ui/python/work/conviv.e < $CONV_ROOT/src/ui/python/work/modele.inp > $CONV_ROOT/src/ui/python/work/out')

		if self.getStep()==0:
			os.system('grep " 1)" $CONV_ROOT/src/ui/python/work/out >> $CONV_ROOT/src/ui/python/work/zpe.out')
			os.system('sort -k2 $CONV_ROOT/src/ui/python/work/zpe.out > $CONV_ROOT/src/ui/python/work/zpe_trie.out')

		else:
			os.system('grep "' + str(self.getStep()) + ' ZPE" $CONV_ROOT/src/ui/python/work/out >> $CONV_ROOT/src/ui/python/work/zpe.out')
			os.system('sort -k6 $CONV_ROOT/src/ui/python/work/zpe.out > $CONV_ROOT/src/ui/python/work/zpe_trie.out')

		os.system('cp $CONV_ROOT/src/ui/python/work/out $CONV_ROOT/src/ui/python/work/out' + str(self.getStep()))
		os.system('cp $CONV_ROOT/src/ui/python/work/zpe_trie.out $CONV_ROOT/src/ui/python/work/zpe_trie.out' + str(self.getStep()))
		os.system('rm $CONV_ROOT/src/ui/python/work/zpe.out')



	#Execution d'un step a partir d'un modele.inp deja existant, pour couples (comme on fait tourner les paires par tour de danse il ne faut pas copier les zpe
	#a chaque fois comme au dessus)
	def pairingConvivExecution(self):

		os.system('cd work ; $CONV_ROOT/src/ui/python/work/conviv.e < $CONV_ROOT/src/ui/python/work/inp > $CONV_ROOT/src/ui/python/work/out')

		os.system('grep "' + str(self.getStep()) + ' ZPE" $CONV_ROOT/src/ui/python/work/out >> $CONV_ROOT/src/ui/python/work/zpe.out')



	#Verification convergence des energies
	def convergence(self,scftol):

		zpe = open(self.link + '/src/ui/python/work/zpe_trie.out','r')
		#Chaque ligne du fichier va etre listee
		texte = zpe.readlines()

		#au step 0 l'energie est la colonne 2
		#la fonction split permet de separer chaque colonne dans une liste
		if self.getStep()==0:
			deb = (texte[0].split())[1]
			fin = (texte[len(texte)-1].split())[1]

		else:
			deb = (texte[0].split())[len(texte[0].split())-1]
			fin = (texte[len(texte)-1].split())[len(texte[len(texte)-1].split())-1]

		if abs(float(deb)-float(fin))>=scftol:
			conv = False

		else:
			conv = True

		zpe.close()

		return conv



	#Step pour contractions a 1 mode (SCF) : ecriture modele, execution conviv
	def scfStep(self,step,nom,N):

		#Liste qui va contenir les modes
		B=[]

		for i in range(1,N+1):
			B.append([i,0])

		########################ECRIRE MODELE.INP
		M = model()
		M.initialization()
		M.completeWriting(B)

		########################CONVIV
		self.convivExecution()
		
		os.system('cp ' + self.link + '/src/ui/python/work/zpe_trie.out ' + self.link + '/src/ui/python/work/zpe_trie.out' + str(step))



	#Step pour les couples
	def pairingStep(self,N,basis_trunc):

		########################GENERER COUPLES
		X = pairs()
		X.initialization()

		#A liste qui contient tous les couples de chaque tour a la suite
		A = X.generatePairs(N)

		#Calcul nombre de tours
		if N%2==0:
			tour = N-1

		else:
			tour = N

		print 'tours : ', tour

		#Execution de chaque tour de danse
		for i in range(1,tour+1):
			print 'tour', i

			#On recupere les couples du tour concerne
			B = X.getPairs(N,A,i)

			########################ECRITURE MODELE.INP
			Y = model()
			Y.initialization()
		 	Y.modesWriting(basis_trunc, B)

			########################CONVIV
			self.pairingConvivExecution()

		########################ZPE.OUT TRIE
		os.system('sort -k6 ' + self.link + '/src/ui/python/work/zpe.out > ' + self.link + '/src/ui/python/work/zpe_trie.out')

		#Suppression zpe.out sinon les energies des contractions optimales vont etre ecrites a la suite des paires precedentes
#		os.system('rm ' + self.link + '/src/ui/python/work/zpe.out')

		########################CONTRACTIONS OPTIMALES

		#C liste qui contient les contractions optimales
		C = X.getOptimalContractions(N)

		#Ecriture modele.inp
		Y.contractionsModelWriting()
		Y.modesWriting(basis_trunc, C)

		self.pairingConvivExecution()

		os.system('sort -k6 ' + self.link + '/src/ui/python/work/zpe.out > ' + self.link + '/src/ui/python/work/zpe_trie.out' + str(self.getStep()))
		os.system('rm ' + self.link + '/src/ui/python/work/zpe.out')
		os.system('rm ' + self.link + '/src/ui/python/work/zpe_trie.out')
		os.system('cp ' + self.link + '/src/ui/python/work/out ' + self.link + '/src/ui/python/work/out' + str(self.getStep()))

