
import os

class output:

	#Obtenir $CONV_ROOT
	def initialization(self):

		self.link = os.popen('echo $CONV_ROOT').read()
		self.link = self.link[:-1]



	#Obtenir valeur tolerance
	def getTol(self):

		fichier = open(self.link + '/src/ui/python/work/tol.txt','r')

		tol = float(fichier.readline())

		fichier.close()

		return tol



	#Obtenir valeur basis_trunc
	def getTrunc(self):

		fichier = open(self.link + '/src/ui/python/work/trunc.txt','r')

		trunc = int(fichier.readline())

		fichier.close()

		return trunc
