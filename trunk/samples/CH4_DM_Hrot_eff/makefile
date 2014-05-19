
all:

gnu:
	gfortran$(VERSION) hameff2-omp.f90 -cpp -ffixed-line-length-none -ffree-line-length-none -Wall -o ghameff2 -O3 -static
	gfortran$(VERSION) hameff2-omp.f90 -cpp -ffixed-line-length-none -ffree-line-length-none -Wall -o ghameff2-omp -O3 -fopenmp

intel:
	ifort -warn all hameff2-omp.f90 -cpp -o hameff2 -O2
	ifort -warn all hameff2-omp.f90 -cpp -o hameff2-omp -O2 -openmp


clean:
	find . -name '*~' | xargs rm -f
	rm -f hameff2 hameff2.exe hameff2-omp hameff2-omp.exe
	rm -f ghameff2 ghameff2.exe ghameff2-omp ghameff2-omp.exe
	rm -f *.mod pert1__genmod.f90 pert2__genmod.f90 pert3__genmod.f90 pert4__genmod.f90

