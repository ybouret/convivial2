
import os

class pairs:

	#Obtenir $CONV_ROOT
	def initialization(self):

		self.link = os.popen('echo $CONV_ROOT').read()
		self.link = self.link[:-1]



	#Genere tous les couples tous les tours a la suite
	def generatePairs(self,N):

		C = [] 		#tableau de couples

		#cas N pair
		if N%2==0:
			for i in range(0,N-1):
				x = [] 		#vecteur couple
				x.append(1)
				x.append(i+2)
				C.append(x)
				for j in range(1,N/2):
					x = []
					x.append((i+j)%(N-1)+2)
					x.append((i-j)%(N-1)+2)
					C.append(x)

		#cas N impair
		else: 
			for i in range(0,N):
				k=N+1
				if i+2<k:
					x = []
					x.append(1)
					x.append(i+2)
					C.append(x)

				for j in range(1,k/2):
					if (i+j)%(k-1)+2!=k and (i-j)%(k-1)+2!=k:
						x = []
						x.append((i+j)%(k-1)+2)
						x.append((i-j)%(k-1)+2)
						C.append(x)

		return C



	#Permet d'obtenir les couples d'un tour precis a partir du tableau de couples C, rajoute le tout seul si N impair
	def getPairs(self,N,C,tour):

		couples_tour = []

		#Calcul du nombre de couples par tour
		if N%2==0:
			nb_couples = N/2

		else:
			nb_couples = (N-1)/2

		#Se place au premier couple du tour et les ajoute a la liste jusqu'au dernier du tour
		for i in range(nb_couples*(tour-1),nb_couples*tour):
			couples_tour.append(C[i])

		#On cherche le tout seul
		A=[]
		for i in range(0,len(couples_tour)):
			u=couples_tour[i]
			A.append(u[0])
			A.append(u[1])

		#Si un numero n'est pas dans la liste qui contient tous les numeros des couples a la suite on l'ajoute en couple avec un 0
		for i in range(1,N+1):
			if (i in A)==False:
				couples_tour.append([i,0])

		#Tri ordre croissant
		couples_tour.sort()

		return couples_tour
		


	#Genere les contractions optimales a partir du fichier zpe_trie.out
	def getOptimalContractions(self, N):

		fichier = open(self.link + '/src/ui/python/work/zpe_trie.out','r')

		M = [] 		#tableau des couples optimaux
		
		for line in fichier:

			liste = line.split()	#decoupe les champs en liste

			couples = liste[3]	#champ des couples

			num = list(couples)	#isole chaque caractere

			#Premier mode du couple
			x = num[1]

			i=1
			coord_y=True

			#Verification si x est composee de plusieurs chiffres
			while num[i+1]!="-":
				if num[i+1]==")":	#verification si x est tout seul
					coord_y=False	#si x est tout seul, on a pas de coord y
					break

				else:
					x=int(str(x)+str(num[i+1]))
					i=i+1


			if coord_y==True:
				k=i+2
				y = num[k]

				while num[k+1]!=")":
					y=int(str(y)+str(num[k+1]))
					k=k+1

			else:
				y=0


			v = []		#vecteur couple
			v.append(int(x))
			v.append(int(y))

			M.append(v)

		#suppression couples deja utilises
		for i in range(0,len(M)):
			u=M[i]
			x=u[0]
			y=u[1]

			for j in range(i+1,len(M)):
				if (x in M[j]) or (y in M[j]):
					M[j]=[0,0]

		while [0,0] in M:
			M.remove([0,0])

		#On cherche le tout seul
		A=[]
		for i in range(0,len(M)):
			u=M[i]
			A.append(u[0])
			A.append(u[1])

		for i in range(1,N+1):
			if (i in A)==False:
				M.append([i,0])

		M.sort()

		return M
