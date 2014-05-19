  !     PROGRAMME HAMEFF
  !     Routine which computes an effective molecular rotational Hamiltonian
  
  !   Do not forget a factor -i in front of terms of the type obs(i<4) obs(j>3) -  obs(j>3) obs(i<4)
  !   Do not forget a factor -1 in front of terms of the type obs(i<4) obs(j<4) +  obs(j<4) obs(i<4)
  
  
  
  IMPLICIT NONE
  CHARACTER FLN*20,FLNN*40 
  integer, parameter :: mpert=4, real_code=8, int_code=8
  INTEGER(KIND=int_code) :: num,ndeg,npert,nobs,nvec
  INTEGER(KIND=int_code) :: i, ii,iii
  real(KIND=real_code) :: eps,eps4
  real(KIND=real_code), dimension(:), allocatable :: ener
  real(KIND=real_code), dimension(:,:,:), allocatable :: obs

  PARAMETER (eps=1.0D-13,eps4=1.0D-16)
  !
  !  *********************************************************************
  !
  !     mpert :  maximum perturbation order implemented
  !     npert :  perturbation order
  !     nobs  :  number of observable to be averaged
  !     nvec  :  number of vectors on which observables are
  !              averaged
  !     ener  :  array of energies
  !     obs(i,j,k) : matrix element of observable i between vectors j and k
  !
  !                  READ, TEST, WRITE INPUT DATA
  !
  !                  CALCULATE
  !
  !  *********************************************************************
  !
  write(6,*)'-- Read parameters'
  open(unit=3,file='hameff.inp',status='old')
  read(3,fmt='(20a)')FLN

  FLNN='roteff'//FLN
  OPEN(UNIT=4,FILE=FLNN)

  read(3,fmt='(40a)')FLNN
  read(3,*)npert
  IF (npert.lt.1.or.npert.gt.mpert) THEN
     WRITE(4,*)'ERROR: perturbation order',npert,'not implemented'
     stop
  ELSE
     WRITE(4,*)'perturbation order: ',npert
  ENDIF
  read(3,*)nobs
  read(3,*)num,ndeg
  close(3)

  write(4,*) 'first vector, state (quasi) degeneracy',num,ndeg
  WRITE(4,*)
  WRITE(4,*)'number of observables=  ',nobs

  write(6,*)'-- Load data...'
  flush(6)
  open(unit=2,file=FLNN,status='old',form='unformatted')
  READ(2)nvec 
  write(4,*)
  WRITE(4,*)'number of vectors=  ',nvec
  allocate(ener(nvec))
  allocate(obs(nobs,nvec,nvec))
  READ(2)(ener(i),i=1,nvec) 
  write(4,*)
  write(4,*)'vector energies (cm-1)'
  write(4,*)
  do i=1,nvec
     write(4,'(i5,2x,f15.9)') i,ener(i)
  enddo
  write(4,*)
  do i=1,nobs
     READ(2)((obs(i,ii,iii),ii=1,nvec),iii=1,nvec)
  enddo
  obs=obs*219474.631371d0
  !pcc because the eigenvalues are in cm-1 units
  close(2)
  write(6,*)'-- ...done. All data are loaded !'
  flush(6)

  IF (npert.eq.1) THEN
     write(6,*)'-- Computing Order 1...'
     call pert1(nvec,num,ndeg,obs,eps,FLN)
  endif
  IF (npert.eq.2) THEN
     write(6,*)'-- Computing Order 2...'
     call pert2(nvec,num,ndeg,obs,eps,FLN,ener)
  endif
  IF (npert.eq.3) THEN
     write(6,*)'-- Computing Order 3...'
     call pert3(nvec,num,ndeg,obs,eps,FLN,ener)
  endif
  IF (npert.eq.4) THEN
     write(6,*)'-- Computing Order 4...'
     call pert4(nvec,num,ndeg,obs,eps,eps4,FLN,ener)
  endif

  close(4)
  deallocate(ener)
  deallocate(obs)
  stop
end program

subroutine pert1(nvec,num,ndeg,obs,eps,FLN)

  IMPLICIT NONE
  CHARACTER FLN*20,FLNN*40 
  integer, parameter :: real_code=8, int_code=8
  INTEGER(KIND=int_code) :: num,ndeg,nobs,nvec
  INTEGER(KIND=int_code) :: i
  INTEGER(KIND=int_code) :: ideg,kdeg,ifile
  real(KIND=real_code) :: eps
  real(KIND=real_code), dimension(1:9,1:nvec,1:nvec) :: obs
  real(KIND=real_code) :: xobs
  INTEGER(KIND=int_code) :: lll,kk,kkk
  CHARACTER FMT1*100

  fmt1='(e19.12,a1,a5,i2,a3)'

  nobs=size(obs,1)

  kk=num-1

  ! first order 


  ifile=5

  do ideg=1,ndeg
     kkk=kk+ideg 
     do kdeg=1,ndeg
        lll=kk+kdeg

        FLNN='H'//char(ichar('0')+mod(ideg,10))//char(ichar('0')+ mod(kdeg,10))//FLN
        OPEN(UNIT=ifile,FILE=FLNN)
        !deg        if(ideg.eq.kdeg)then
        !deg          write(ifile,*) (ener(kkk)-ener(1)),'*','ident','+'
        !deg        endif

        do i=1,nobs
           xobs=obs(i,kkk,lll)
           if(dabs(xobs).gt.eps) then
              write(ifile,fmt1) xobs,'*','obs[[',i,']]+'
           endif
        enddo
        write(ifile,*) 0
        close(ifile)
     enddo
  enddo

  return
end subroutine pert1

subroutine pert2(nvec,num,ndeg,obs,eps,FLN,ener)

  IMPLICIT NONE
  CHARACTER FLN*20,FLNN*40 
  integer, parameter :: real_code=8, int_code=8
  INTEGER(KIND=int_code) :: num,ndeg,nobs,nvec
  INTEGER(KIND=int_code) :: i,j,k
  INTEGER(KIND=int_code) :: ideg,kdeg,ifile
  real(KIND=real_code) :: eps
  real(KIND=real_code), dimension(1:nvec) :: ener
  real(KIND=real_code), dimension(1:9,1:nvec,1:nvec) :: obs
  real(KIND=real_code) :: xobs,obs2,en
  INTEGER(KIND=int_code) :: ll,lll,kk,kkk
  CHARACTER FMT1*100,FMT2*100

  fmt1='(e19.12,a1,a5,i2,a3)'
  fmt2='(e19.12,a1,a5,i2,a7,i2,a3)'

  nvec=size(ener)
  nobs=size(obs,1)

  !         nvec=25

  kk=num-1
  ll=num+ndeg

  ifile=5

  do ideg=1,ndeg
     kkk=kk+ideg
     do kdeg=1,ndeg
        lll=kk+kdeg

        en=ener(kkk)+ener(lll) 
        en=en/2.0d0

        ! first order 

        FLNN='H'//char(ichar('0')+mod(ideg,10))//char(ichar('0')+ mod(kdeg,10))//FLN
        OPEN(UNIT=ifile,FILE=FLNN)
        !deg        if(ideg.eq.kdeg)then
        !deg          write(ifile,*) (ener(kkk)-ener(1)),'*','ident','+'
        !deg        endif

        do i=1,nobs
           xobs=obs(i,kkk,lll)
           if(dabs(xobs).gt.eps) then
              write(ifile,fmt1) xobs,'*','obs[[',i,']]+'
           endif
        enddo

        ! Second order 

        do i=1,nobs
           do j=1,nobs
              obs2=0.0d0
              do k=1,kk
                 obs2=obs2+obs(i,kkk,k)*obs(j,k,lll)/(ener(kkk)-ener(k))
              enddo
              do k=ll,nvec
                 obs2=obs2+obs(i,kkk,k)*obs(j,k,lll)/(ener(kkk)-ener(k))
              enddo
              if(dabs(obs2).gt.eps) then
                 write(ifile,fmt2) obs2,'*','obs[[',i,']].obs[[',j,']]+'
              endif
           enddo
        enddo
        write(ifile,*) 0
     enddo
  enddo

  return
end subroutine pert2

subroutine pert3(nvec,num,ndeg,obs,eps,FLN,ener)

  IMPLICIT NONE
  CHARACTER FLN*20,FLNN*40 
  integer, parameter :: real_code=8, int_code=8
  INTEGER(KIND=int_code) :: num,ndeg,nobs,nvec
  INTEGER(KIND=int_code) :: i,j,k
  INTEGER(KIND=int_code) :: ideg,kdeg,ifile
  real(KIND=real_code) :: eps
  real(KIND=real_code), dimension(1:nvec) :: ener
  real(KIND=real_code), dimension(1:9,1:nvec,1:nvec) :: obs
  real(KIND=real_code) :: xobs,obs2,obs3,en
  INTEGER(KIND=int_code) :: i1,i2,i3,ll,lll,kk,kkk,jj,k1,k2
  CHARACTER FMT1*100,FMT2*100,FMT3*100

  fmt1='(e19.12,a1,a5,i2,a3)'
  fmt2='(e19.12,a1,a5,i2,a7,i2,a3)'
  fmt3='(e19.12,a1,a5,i2,a7,i2,a7,i2,a3)'

  nvec=size(ener)
  nobs=size(obs,1)

  !         nvec=25

  kk=num-1
  ll=num+ndeg
  jj=ll-1

  ifile=5


  do ideg=1,ndeg
     kkk=kk+ideg
     do kdeg=1,ndeg
        lll=kk+kdeg

        en=ener(kkk)+ener(lll) 
        en=en/2.0d0

        ! first order 

        FLNN='H'//char(ichar('0')+mod(ideg,10))//char(ichar('0')+ mod(kdeg,10))//FLN
        OPEN(UNIT=ifile,FILE=FLNN)
        !deg        if(ideg.eq.kdeg)then
        !deg          write(ifile,*) (ener(kkk)-ener(1)),'*','ident','+'
        !deg        endif


        do i=1,nobs
           xobs=obs(i,kkk,lll)
           if(dabs(xobs).gt.eps) then
              write(ifile,fmt1) xobs,'*','obs[[',i,']]+'
           endif
        enddo

        ! second order 

        do i=1,nobs
           do j=1,nobs
              obs2=0.0d0
              do k=1,kk
                 obs2=obs2+obs(i,kkk,k)*obs(j,k,lll)/(en-ener(k))
              enddo
              do k=ll,nvec
                 obs2=obs2+obs(i,kkk,k)*obs(j,k,lll)/(en-ener(k))
              enddo
              if(dabs(obs2).gt.eps) then
                 write(ifile,fmt2) obs2,'*','obs[[',i,']].obs[[',j,']]+'
              endif
           enddo
        enddo

        ! third order 


        do i1=1,nobs
           do i2=1,nobs
              do i3=1,nobs
                 obs3=0.0d0
                 do k1=1,kk
                    do k2=1,kk
                       obs3=obs3+obs(i1,kkk,k1)*obs(i2,k1,k2)*obs(i3,k2,lll)/(en-ener(k1))/(en-ener(k2))
                    enddo
                    do k2=ll,nvec
                       obs3=obs3+obs(i1,kkk,k1)*obs(i2,k1,k2)*obs(i3,k2,lll)/(en-ener(k1))/(en-ener(k2))
                    enddo
                    do k2=num,jj
                       obs3=obs3-obs(i1,kkk,k1)*obs(i2,k1,k2)*obs(i3,k2,lll)/(en-ener(k1))**2/2.0d0
                       obs3=obs3-obs(i1,kkk,k2)*obs(i2,k2,k1)*obs(i3,k1,lll)/(en-ener(k1))**2/2.0d0
                    enddo
                 enddo
                 do k1=ll,nvec
                    do k2=1,kk
                       obs3=obs3+obs(i1,kkk,k1)*obs(i2,k1,k2)*obs(i3,k2,lll)/(en-ener(k1))/(en-ener(k2))
                    enddo
                    do k2=ll,nvec
                       obs3=obs3+obs(i1,kkk,k1)*obs(i2,k1,k2)*obs(i3,k2,lll)/(en-ener(k1))/(en-ener(k2))
                    enddo
                    do k2=num,jj
                       obs3=obs3-obs(i1,kkk,k1)*obs(i2,k1,k2)*obs(i3,k2,lll)/(en-ener(k1))**2/2.0d0
                       obs3=obs3-obs(i1,kkk,k2)*obs(i2,k2,k1)*obs(i3,k1,lll)/(en-ener(k1))**2/2.0d0
                    enddo
                 enddo
                 if(dabs(obs3).gt.eps) then
                    write(ifile,fmt3) obs3,'*','obs[[',i1,']].obs[[',i2,']].obs[[',i3,']]+'
                 endif
              enddo
           enddo
        enddo
        write(ifile,*) 0
     enddo
  enddo

  return
end subroutine pert3

subroutine pert4(nvec,num,ndeg,obs,eps,eps4,FLN,ener)

#if defined(_OPENMP)
  use OMP_LIB
#       define GET_WALL_TIME(X) X = omp_get_wtime()
#else
#       define GET_WALL_TIME(X) call cpu_time(X)
#endif
  IMPLICIT NONE
  CHARACTER FLN*20,FLNN*40 
  integer, parameter :: real_code=8, int_code=8
  INTEGER(KIND=int_code) :: num,ndeg,nobs,nvec
  INTEGER(KIND=int_code) :: i,j,k
  INTEGER(KIND=int_code) :: ideg,kdeg,ifile
  !temp      INTEGER(KIND=int_code) :: mvec
  real(KIND=real_code) :: eps,eps4
  real(KIND=real_code), dimension(1:nvec) :: ener
  real(KIND=real_code), dimension(1:9,1:nvec,1:nvec) :: obs
  real(KIND=real_code) :: xobs,obs2,obs3,obs4,en,sum4
  real(KIND=real_code) :: tt1,tt2
  INTEGER(KIND=int_code) :: i1,i2,i3,i4,ll,lll,kk,kkk,jj,k1,k2,k3
  CHARACTER FMT1*100,FMT2*100,FMT3*100,FMT4*100

  fmt1='(e19.12,a1,a5,i2,a3)'
  fmt2='(e19.12,a1,a5,i2,a7,i2,a3)'
  fmt3='(e19.12,a1,a5,i2,a7,i2,a7,i2,a3)'
  fmt4='(e19.12,a1,a5,i2,a7,i2,a7,i2,a7,i2,a3)'

  write(6,*) '-- Enter: Pert4'
  nvec=size(ener)
  nobs=size(obs,1)

  kk=num-1
  ll=num+ndeg
  jj=ll-1

  ifile=5

  do ideg=1,ndeg
     kkk=kk+ideg
     do kdeg=1,ndeg
        write(6,*) '---- ideg=',ideg,' kdeg=',kdeg
        lll=kk+kdeg

        en=ener(kkk)+ener(lll) 
        en=en/2.0d0

        ! first order 

        FLNN='H'//char(ichar('0')+mod(ideg,10))//char(ichar('0')+mod(kdeg,10))//FLN
        OPEN(UNIT=ifile,FILE=FLNN)
        
        !deg        if(ideg.eq.kdeg)then
        !deg          write(ifile,*) (ener(kkk)-ener(1)),'*','ident','+'
        !deg        endif

#define SKIP123 0
#if SKIP123 != 1
        write(6,*)'---- Order 1'
        do i=1,nobs
           xobs=obs(i,kkk,lll)
           if(dabs(xobs).gt.eps) then
              write(ifile,fmt1) xobs,'*','obs[[',i,']]+'
           endif
        enddo

        ! second order 
        write(6,*)'---- Order 2'
        do i=1,nobs
           do j=1,nobs
              obs2=0.0d0
              do k=1,kk
                 obs2=obs2+obs(i,kkk,k)*obs(j,k,lll)/(en-ener(k))
              enddo
              do k=ll,nvec
                 obs2=obs2+obs(i,kkk,k)*obs(j,k,lll)/(en-ener(k))
              enddo
              if(dabs(obs2).gt.eps) then
                 write(ifile,fmt2) obs2,'*','obs[[',i,']].obs[[',j,']]+'
              endif
           enddo
        enddo

        ! third order 
        write(6,*)'---- Order 3'
        do i1=1,nobs
           do i2=1,nobs
              do i3=1,nobs
                 obs3=0.0d0
                 do k1=1,kk
                    do k2=1,kk
                       obs3=obs3+obs(i1,kkk,k1)*obs(i2,k1,k2)*obs(i3,k2,lll)/(en-ener(k1))/(en-ener(k2))
                    enddo
                    do k2=ll,nvec
                       obs3=obs3+obs(i1,kkk,k1)*obs(i2,k1,k2)*obs(i3,k2,lll)/(en-ener(k1))/(en-ener(k2))
                    enddo
                    do k2=num,jj
                       obs3=obs3-obs(i1,kkk,k1)*obs(i2,k1,k2)*obs(i3,k2,lll)/(en-ener(k1))**2/2.0d0
                       obs3=obs3-obs(i1,kkk,k2)*obs(i2,k2,k1)*obs(i3,k1,lll)/(en-ener(k1))**2/2.0d0
                    enddo
                 enddo
                 do k1=ll,nvec
                    do k2=1,kk
                       obs3=obs3+obs(i1,kkk,k1)*obs(i2,k1,k2)*obs(i3,k2,lll)/(en-ener(k1))/(en-ener(k2))
                    enddo
                    do k2=ll,nvec
                       obs3=obs3+obs(i1,kkk,k1)*obs(i2,k1,k2)*obs(i3,k2,lll)/(en-ener(k1))/(en-ener(k2))
                    enddo
                    do k2=num,jj
                       obs3=obs3-obs(i1,kkk,k1)*obs(i2,k1,k2)*obs(i3,k2,lll)/(en-ener(k1))**2/2.0d0
                       obs3=obs3-obs(i1,kkk,k2)*obs(i2,k2,k1)*obs(i3,k1,lll)/(en-ener(k1))**2/2.0d0
                    enddo
                 enddo
                 if(dabs(obs3).gt.eps) then
                    write(ifile,fmt3) obs3,'*','obs[[',i1,']].obs[[',i2,']].obs[[',i3,']]+'
                 endif
              enddo
           enddo
        enddo

#endif
        ! fourth order
        write(6,*)'---- Order 4'
        nvec=4022
        !nvec=22
        write(4,*)
        write(4,*)'new nvec',nvec
        write(4,*)

        do i1=1,nobs
           do i2=1,nobs
              do i3=1,nobs
                 do i4=1,nobs
                    obs4=0.0d0
                    do k1=1,kk
                       do k2=1,kk
                          do k3=1,kk
                             obs4=obs4+obs(i1,kkk,k1)*obs(i2,k1,k2)*obs(i3,k2,k3)*obs(i4,k3,lll)/(en-ener(k1))/(en-ener(k2))/(en-ener(k3))
                          enddo
                          do k3=ll,nvec
                             obs4=obs4+obs(i1,kkk,k1)*obs(i2,k1,k2)*obs(i3,k2,k3)*obs(i4,k3,lll)/(en-ener(k1))/(en-ener(k2))/(en-ener(k3))
                          enddo
                          do k3=num,jj
                             obs4=obs4-obs(i1,kkk,k1)*obs(i2,k1,k2)*obs(i3,k2,k3)*obs(i4,k3,lll)/(en-ener(k1))/(en-ener(k2))*(0.5d0/(en-ener(k1))+0.5d0/(en-ener(k2)))
                             obs4=obs4-obs(i1,kkk,k3)*obs(i2,k3,k1)*obs(i3,k1,k2)*obs(i4,k2,lll)/(en-ener(k1))/(en-ener(k2))*(0.5d0/(en-ener(k1))+0.5d0/(en-ener(k2)))
                          enddo
                          obs4=obs4-obs(i1,kkk,k1)*obs(i2,k1,kkk)*obs(i3,lll,k2)*obs(i4,k2,lll)/(en-ener(k1))/(en-ener(k2))/(en-ener(k1))/2.0d0
                          obs4=obs4-obs(i1,kkk,k1)*obs(i2,k1,kkk)*obs(i3,lll,k2)*obs(i4,k2,lll)/(en-ener(k1))/(en-ener(k2))/(en-ener(k2))/2.0d0
                       enddo
                       do k2=ll,nvec
                          do k3=1,kk
                             obs4=obs4+obs(i1,kkk,k1)*obs(i2,k1,k2)*obs(i3,k2,k3)*obs(i4,k3,lll)/(en-ener(k1))/(en-ener(k2))/(en-ener(k3))
                          enddo
                          do k3=ll,nvec
                             obs4=obs4+obs(i1,kkk,k1)*obs(i2,k1,k2)*obs(i3,k2,k3)*obs(i4,k3,lll)/(en-ener(k1))/(en-ener(k2))/(en-ener(k3))
                          enddo
                          do k3=num,jj
                             obs4=obs4-obs(i1,kkk,k1)*obs(i2,k1,k2)*obs(i3,k2,k3)*obs(i4,k3,lll)/(en-ener(k1))/(en-ener(k2))*(0.5d0/(en-ener(k1))+0.5d0/(en-ener(k2)))
                             obs4=obs4-obs(i1,kkk,k3)*obs(i2,k3,k1)*obs(i3,k1,k2)*obs(i4,k2,lll)/(en-ener(k1))/(en-ener(k2))*(0.5d0/(en-ener(k1))+0.5d0/(en-ener(k2)))
                          enddo
                          obs4=obs4-obs(i1,kkk,k1)*obs(i2,k1,kkk)*obs(i3,lll,k2)*obs(i4,k2,lll)/(en-ener(k1))/(en-ener(k2))/(en-ener(k1))/2.0d0
                          obs4=obs4-obs(i1,kkk,k1)*obs(i2,k1,kkk)*obs(i3,lll,k2)*obs(i4,k2,lll)/(en-ener(k1))/(en-ener(k2))/(en-ener(k2))/2.0d0
                       enddo
                       obs4=obs4+obs(i1,kkk,k1)*obs(i2,k1,kkk)*obs(i3,lll,kkk)*obs(i4,kkk,lll)/(en-ener(k1))/(en-ener(k1))/(en-ener(k1))/2.0d0
                       obs4=obs4+obs(i1,kkk,lll)*obs(i2,lll,kkk)*obs(i3,lll,k1)*obs(i4,k1,lll)/(en-ener(k1))/(en-ener(k1))/(en-ener(k1))/2.0d0
                    enddo

                    GET_WALL_TIME(tt1)

                    !$OMP PARALLEL DO DEFAULT(SHARED) PRIVATE(k1,k2,k3,sum4) SCHEDULE(STATIC)
                    do k1=ll,nvec
                       sum4=0.0d0
                       do k2=1,kk
                          do k3=1,kk
                             sum4=sum4+obs(i1,kkk,k1)*obs(i2,k1,k2)*obs(i3,k2,k3)*obs(i4,k3,lll)/(en-ener(k1))/(en-ener(k2))/(en-ener(k3))
                          enddo
                          do k3=ll,nvec
                             sum4=sum4+obs(i1,kkk,k1)*obs(i2,k1,k2)*obs(i3,k2,k3)*obs(i4,k3,lll)/(en-ener(k1))/(en-ener(k2))/(en-ener(k3))
                          enddo
                          do k3=num,jj
                             sum4=sum4-obs(i1,kkk,k1)*obs(i2,k1,k2)*obs(i3,k2,k3)*obs(i4,k3,lll)/(en-ener(k1))/(en-ener(k2))*(0.5d0/(en-ener(k1))+0.5d0/(en-ener(k2)))
                             sum4=sum4-obs(i1,kkk,k3)*obs(i2,k3,k1)*obs(i3,k1,k2)*obs(i4,k2,lll)/(en-ener(k1))/(en-ener(k2))*(0.5d0/(en-ener(k1))+0.5d0/(en-ener(k2)))
                          enddo
                          sum4=sum4-obs(i1,kkk,k1)*obs(i2,k1,kkk)*obs(i3,lll,k2)*obs(i4,k2,lll)/(en-ener(k1))/(en-ener(k2))/(en-ener(k1))/2.0d0
                          sum4=sum4-obs(i1,kkk,k1)*obs(i2,k1,kkk)*obs(i3,lll,k2)*obs(i4,k2,lll)/(en-ener(k1))/(en-ener(k2))/(en-ener(k2))/2.0d0
                       enddo
                       do k2=ll,nvec
                          do k3=1,kk
                             sum4=sum4+obs(i1,kkk,k1)*obs(i2,k1,k2)*obs(i3,k2,k3)*obs(i4,k3,lll)/(en-ener(k1))/(en-ener(k2))/(en-ener(k3))
                          enddo
                          do k3=ll,nvec
                             sum4=sum4+obs(i1,kkk,k1)*obs(i2,k1,k2)*obs(i3,k2,k3)*obs(i4,k3,lll)/(en-ener(k1))/(en-ener(k2))/(en-ener(k3))
                          enddo
                          do k3=num,jj
                             sum4=sum4-obs(i1,kkk,k1)*obs(i2,k1,k2)*obs(i3,k2,k3)*obs(i4,k3,lll)/(en-ener(k1))/(en-ener(k2))*(0.5d0/(en-ener(k1))+0.5d0/(en-ener(k2)))
                             sum4=sum4-obs(i1,kkk,k3)*obs(i2,k3,k1)*obs(i3,k1,k2)*obs(i4,k2,lll)/(en-ener(k1))/(en-ener(k2))*(0.5d0/(en-ener(k1))+0.5d0/(en-ener(k2)))
                          enddo
                          sum4=sum4-obs(i1,kkk,k1)*obs(i2,k1,kkk)*obs(i3,lll,k2)*obs(i4,k2,lll)/(en-ener(k1))/(en-ener(k2))/(en-ener(k1))/2.0d0
                          sum4=sum4-obs(i1,kkk,k1)*obs(i2,k1,kkk)*obs(i3,lll,k2)*obs(i4,k2,lll)/(en-ener(k1))/(en-ener(k2))/(en-ener(k2))/2.0d0
                       enddo
                       sum4=sum4+obs(i1,kkk,k1)*obs(i2,k1,kkk)*obs(i3,lll,kkk)*obs(i4,kkk,lll)/(en-ener(k1))/(en-ener(k1))/(en-ener(k1))/2.0d0
                       sum4=sum4+obs(i1,kkk,lll)*obs(i2,lll,kkk)*obs(i3,lll,k1)*obs(i4,k1,lll)/(en-ener(k1))/(en-ener(k1))/(en-ener(k1))/2.0d0
                       !$OMP CRITICAL
                       obs4=obs4+sum4
                       !$OMP END CRITICAL
                    enddo
                    !$OMP END PARALLEL DO
                    GET_WALL_TIME(tt2)
                    write(6,*) "Pert4 Loop Time: ", tt2-tt1
                    if(dabs(obs4).gt.eps4) then
!                       write(ifile,*) obs4,'*','obs[[',i1,']].obs[[',i2,']].obs[[',i3, ']].obs[[',i4,']]+'
                       write(ifile,fmt4) obs4,'*','obs[[',i1,']].obs[[',i2,']].obs[[',i3, ']].obs[[',i4,']]+'

                    endif
                 enddo
              enddo
           enddo
        enddo
        write(ifile,*) 0
     enddo
  enddo


  return
end subroutine pert4
