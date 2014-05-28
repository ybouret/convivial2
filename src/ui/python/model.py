
import os

class model:


	#Obtenir $CONV_ROOT
	def initialization(self):

		self.link = os.popen('echo $CONV_ROOT').read()
		self.link = self.link[:-1]



	#Adaptation du modele (suppression de step ecrits en trop)
	def modelAdaptation(self):

		N_modes = self.getModes(False)

		fichier = open(self.link + '/src/ui/python/work/modele.inp','r')
		texte = fichier.readlines()
		fichier.close()

		fichier = open(self.link + '/src/ui/python/work/modele.inp','w')

		#lignes_adaptation : nombre de lignes que doit contenir le fichier (en fonction du nombre de steps et de modes, voir methode modes())
		for i in range(0, self.lignes_adaptation):
			fichier.write(str(texte[i]))

		fichier.close()


		
	#Ecriture modele.inp en out ou no (sans les couples a la fin, ie changement de step, rajout prise en compte fichier de restart pour le nouveau step)
	#Ecriture en out : pour SCF
	#Ecriture en no pour tours de danse lors du pairing
        #pcc suppose no window of eigenvectors  anyway the script will fail if debug mode
	def modelWriting(self,no_out):
	
	    modele = open(self.link + '/src/ui/python/work/modele.inp','r+')
		
	    #On stocke toutes les lignes du modele
	    texte = []
	    for line in modele:
		texte.append(line)

	    modele.close()

	    modele = open(self.link + '/src/ui/python/work/modele.inp','w')

	    #Recup num ancien step et calcul nouveau
	    old_step = int(texte[4])
	    new_step = old_step + 1

	    #Reecriture from start up to debug flag
	    for i in range(0,4):
		modele.write(texte[i])

	    modele.write(str(new_step) + '\n')
	    modele.write(texte[5])

            if old_step==0:
	        ligne = texte[6].split()

                if ligne[0]=='NO':
	            modele.write(texte[6])
	            plus_ligne = 0
                #if IN or OUT found, consider that restart files exist
                # and restart from these files
	        elif ligne[0]=='IN':
	            modele.write(texte[6])
	            modele.write(texte[7])
	            plus_ligne = 1
	        elif ligne[0]=='OUT': 
		    modele.write('IN' + '\n')
	            modele.write(texte[7])
	            plus_ligne = 1
                else:
                    print "Could not find proper restart keyword for step 0"

		if no_out=='OUT':
			modele.write('OUT' + '\n')
			modele.write('step' + str(new_step) + '\n')
		else:
			modele.write('NO' + '\n')

	        #Reecriture de la fin 
		for i in range(7+plus_ligne,len(texte)):
		     modele.write(texte[i])

            else:
                # at least 2 steps
		#analyse restart keyword for all steps before last 
		#afin de calculer le nombre de lignes a reecrire 
                #if IN or OUT they will be transformed into NO and a line has to be deduced
                i=0
                j=0
                while ( i < old_step ):
	            ligne = texte[6 + i + j].split()
                    if ligne[0]!='NO' and ligne[0]!='IN' and ligne[0]!='OUT':
                        print "Could not find proper restart keyword for step",i
	            if ligne[0]!='NO':
                        j=j+1
                    i=i+1
		#nb ligne de l'ancien dernier step
                line_nb=6 + i + j

		#analyse restart keyword for last step
	        ligne = texte[line_nb].split()

                if ligne[0]=='NO':
	            step_in = False
	            plus_ligne = 0
                #if IN or OUT found, consider that restart files exist
                # and restart from these files
	        elif ligne[0]=='IN' or ligne[0]=='OUT':
		    #Recup du nom de l'ancien dernier step
		    name_old_step = texte[line_nb+1]
	            step_in= True
	            plus_ligne = 1
                else:
                    print "Could not find proper restart keyword for last old step",i


		for i in range(0,old_step):
			modele.write('NO' + '\n')

		if step_in==True:
			modele.write('IN' + '\n')
			modele.write(name_old_step)

		else:
			modele.write('NO' + '\n')

		if no_out=='OUT':
			modele.write('OUT' + '\n')
			modele.write('step' + str(new_step) + '\n')
		else:
			modele.write('NO' + '\n')

	        #Reecriture de la fin 
		for i in range(line_nb+plus_ligne+1,len(texte)):
		     modele.write(texte[i])
		
		modele.close()		



	#Ecriture couples dans inp a partir de modele.inp
	def modesWriting(self,basis_trunc,couples):

		#Creation fichier inp
		os.system('cp ' + self.link + '/src/ui/python/work/modele.inp ' + self.link + '/src/ui/python/work/inp')

		fichier = open(self.link + '/src/ui/python/work/inp','r+')

		#Calcul nombre de lignes dans modele.inp
		nb_lignes = len(fichier.readlines())

		#On se place a la fin de modele.inp
		for i in range(0,nb_lignes):
			fichier.readline()

		#Calcul nombre de contractions
		nb_contractions = len(couples)

		#Ecriture nombre de contractions
		fichier.write(str(nb_contractions)+'\n')

		#Ecriture couples
		for i in range(0,len(couples)):
			v = couples[i]
			v.sort()
			x = v[0]
			y = v[1]
			if x==0:
				fichier.write(str(1) + '\n')
				fichier.write(str(y) + '\n')
				fichier.write('0.0d0 0.0d0' + '\n')

			else: 
				fichier.write(str(2) + '\n')
				fichier.write(str(x) + ' ' + str(y) + '\n')
				fichier.write('0.0d0 0.0d0 0.0d0' + '\n')

		#Ecriture basis trunc a la fin
		for k in range(0, nb_contractions-1):			
			fichier.write(str(basis_trunc) + ' ')

		fichier.write(str(basis_trunc) + '\n')

		#Fermeture fichier
		fichier.close()



	#Ecriture debut modele.inp pour contractions optimales (sans contractions a la fin, avec un OUT)
	def contractionsModelWriting(self):

		modele = open(self.link + '/src/ui/python/work/modele.inp','r+')
		
		N_modes = self.getModes(True)

		#On stocke toutes les lignes du modele
		texte = []
		for line in modele:
			texte.append(line)

		modele.close()

		step = int(texte[4])

		modele = open(self.link + '/src/ui/python/work/modele.inp','w')

		ligne = texte[5 + step].split()

		if ligne[0]=='NO':
			for i in range(0, 6 + step):
				modele.write(texte[i])

			step_in = False

		else:
			for i in range(0, 7 + step):
				modele.write(texte[i])

			step_in = True

		#On verifie si N=1 car ce serait le dernier step et pas d'interet de OUT
		if N_modes!=2:
			modele.write('OUT' + '\n')
			modele.write('step' + str(step) + '\n')

		else:
			modele.write('NO' + '\n')

		if step_in==False:
			for i in range(7 + step, len(texte)):
				modele.write(texte[i])

		else:
			for i in range(8 + step, len(texte)):
				modele.write(texte[i])

		modele.close()



	#Reecriture modele.inp dont le dernier step contient les contractions optimales, le OUT devient NO (pour Hierarchy)
	def modelSCFHierarchy(self):

		modele = open(self.link + '/src/ui/python/work/modele.inp','r+')
		
		#On stocke toutes les lignes du modele
		texte = []
		for line in modele:
			texte.append(line)

		modele.close()

		step = int(texte[4])

		modele = open(self.link + '/src/ui/python/work/modele.inp','w')

		for i in range(0, 6 + step + 1):
			modele.write(texte[i])

		modele.write('NO' + '\n')

		for i in range(6 + step + 3, len(texte)):
			modele.write(texte[i])

		modele.close()


	#Ecriture modele.inp complet (OUT + modes) pour SCF
	def completeWriting(self,couples):

		basis_trunc = 0

		self.modelWriting('OUT')
		self.modesWriting(basis_trunc, couples)

		#ecritures_modes cree un fichier inp, celui ci devient le nouveau modele.inp (couples ont besoin du inp)
		os.system('cp ' + self.link + '/src/ui/python/work/inp ' + self.link + '/src/ui/python/work/modele.inp')
		os.system('rm ' + self.link + '/src/ui/python/work/inp')



	#Obtenir nombre de modes du dernier step (pour SCFRun)
	def getModes(self,pairing):

		fichier = open(self.link + '/src/ui/python/work/modele.inp','r')

		total_lignes = len(fichier.readlines())

		fichier.close()

		cpt_lignes = 0

		fichier = open(self.link + '/src/ui/python/work/modele.inp','r')

		for i in range(0,4):
			fichier.readline()
			cpt_lignes = cpt_lignes + 1

		num_step = int(fichier.readline())
		cpt_lignes = cpt_lignes + 1

		for i in range(0,num_step):
			fichier.readline()
			cpt_lignes = cpt_lignes + 1

		ligne = fichier.readline().split()
		cpt_lignes = cpt_lignes + 1


		if ligne[0]=='IN':
			fichier.readline()
			cpt_lignes = cpt_lignes + 1

		ligne = fichier.readline().split()
		cpt_lignes = cpt_lignes + 1

		if ligne[0]=='OUT':
			fichier.readline()
			cpt_lignes = cpt_lignes + 1

		modes_deg = fichier.readline().split()
		cpt_lignes = cpt_lignes + 1

		modes_init = int(modes_deg[0]) + int(modes_deg[1]) + int(modes_deg[2])

		if pairing==True:
			num_step = num_step - 1

		if num_step==0:
			modes = modes_init

		else:

			#on se place a la fin du bloc step0
			for i in range(0,2+3*modes_init):
				fichier.readline()
				cpt_lignes = cpt_lignes + 1

			modes = int(fichier.readline())
			cpt_lignes = cpt_lignes + 1
			cpt_step = 1

			while (cpt_step < num_step):

				for i in range(0,modes*3+1):
					fichier.readline()
					cpt_lignes = cpt_lignes + 1

				modes = int(fichier.readline())	
				cpt_lignes = cpt_lignes + 1
				cpt_step = cpt_step + 1

		if num_step==0:
			self.lignes_adaptation = cpt_lignes + modes*3 + 2

		else:
			self.lignes_adaptation = cpt_lignes + modes*3 + 1

		return modes

