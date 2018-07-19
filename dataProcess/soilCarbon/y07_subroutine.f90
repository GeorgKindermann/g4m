MODULE yasso
IMPLICIT NONE
CONTAINS
  SUBROUTINE mod5c(a,t,cl,init,inf,s,z) !components separately
  IMPLICIT NONE
    !*********************************************
    !GENERAL DESCRIPTION FOR ALL THE MEASUREMENTS
    !*********************************************
    !returns the model prediction for given parameters
    ! 1-16 matrix A entries: 4*k, 12*p
    !17-25 T-dependence parameters: b1, b2, -, -, -, -, -, -, -
    !26-34 P-dependence parameters: g1, -, -, -, -, -, -, -, -
    !35-38 Humus parametens: kH, pH, -, -
    !39-42 Woody parameters: -, -, -, -
    !43-44 Other system parameters: S, -
    REAL,DIMENSION(45),INTENT(IN) :: a !parameters
    REAL,INTENT(IN) :: t,s !time,size
    REAL,DIMENSION(3),INTENT(IN) :: cl !climatic conditions
    REAL,DIMENSION(5),INTENT(IN) :: init
    REAL,DIMENSION(5),INTENT(IN) :: inf !infall
    REAL,DIMENSION(5),INTENT(OUT) :: z
    REAL,DIMENSION(5,5) :: m,mt,m2,mi
    INTEGER :: i
    REAL,PARAMETER :: pi=3.1415926535
    REAL :: tem
    REAL,DIMENSION(5) :: te
    REAL,DIMENSION(5) :: z1,z2
    !temperature annual cycle approximation
    te(1)=cl(1)+4*cl(3)*(1/SQRT(2.0)-1)/pi
    te(2)=cl(1)-4*cl(3)/SQRT(2.0)/pi
    te(3)=cl(1)+4*cl(3)*(1-1/SQRT(2.0))/pi
    te(4)=cl(1)+4*cl(3)/SQRT(2.0)/pi
    tem=0.0
    DO i=1,4 !Annual cycle, different models
      tem=tem+EXP(a(17)*te(i)+a(18)*te(i)**2.0)/4.0 !Gaussian
    END DO
    !Precipitation dependence
    tem=tem*(1.0-EXP(a(26)*cl(2)/1000))
    !Size class dependence -- no effect if sc = 0.0
    m(1,1)=a(1)*tem*(s*a(39)+s**2.0*a(40)+1.0)**a(41)
    m(2,2)=a(2)*tem*(s*a(39)+s**2.0*a(40)+1.0)**a(41)
    m(3,3)=a(3)*tem*(s*a(39)+s**2.0*a(40)+1.0)**a(41)
    m(4,4)=a(4)*tem*(s*a(39)+s**2.0*a(40)+1.0)**a(41)
    !Calculating matrix M, normal
    m(2,1)=a(5)*ABS(m(2,2))
    m(3,1)=a(6)*ABS(m(3,3))
    m(4,1)=a(7)*ABS(m(4,4))
    m(5,1)=0.0
    m(1,2)=a(8)*ABS(m(1,1))
    m(3,2)=a(9)*ABS(m(3,3))
    m(4,2)=a(10)*ABS(m(4,4))
    m(5,2)=0.0
    m(1,3)=a(11)*ABS(m(1,1))
    m(2,3)=a(12)*ABS(m(2,2))
    m(4,3)=a(13)*ABS(m(4,4))
    m(5,3)=0.0
    m(1,4)=a(14)*ABS(m(1,1))
    m(2,4)=a(15)*ABS(m(2,2))
    m(3,4)=a(16)*ABS(m(3,3))
    m(5,4)=0.0
    m(5,5)=a(35)*tem !no size effect in humus
    DO i=1,4
      m(i,5)=a(36)*ABS(m(i,i)) !mass flows EWAN -> H
    END DO
    DO i=1,5
      z1(i)=DOT_PRODUCT(m(:,i),init)+inf(i)
    END DO
    mt=m*t
    CALL matrixexp(mt,m2)
    DO i=1,5
      z2(i)=DOT_PRODUCT(m2(:,i),z1)-inf(i)
    END DO
    CALL inverse(m,mi)
    DO i=1,5
      z1(i)=DOT_PRODUCT(mi(:,i),z2)
    END DO
    z=z1

  CONTAINS

      SUBROUTINE matrixexp(a,b)
      IMPLICIT NONE
        !returns approximated matrix exponential
        !Taylor (Bade to be written) approximation..another algorithm perhaps?
        REAL,DIMENSION(5,5),INTENT(IN) :: a
        REAL,DIMENSION(5,5),INTENT(OUT) :: b
        REAL,DIMENSION(5,5) :: c,d
        REAL :: p,normiter
        INTEGER :: i,q,j
        q=10
        b=0.0
        DO i=1,5
          b(i,i)=1.0
        END DO
        normiter=2.0
        j=1
        CALL matrix2norm(a, p)
        DO
          IF(p<normiter)THEN
            EXIT
          END IF
          normiter=normiter*2.0
          j=j+1
        END DO
        c=a/normiter
        b=b+c
        d=c
        DO i=2,q
          d=MATMUL(c,d)/REAL(i)
          b=b+d
        END DO
        DO i=1,j
          b=MATMUL(b,b)
        END DO
      END SUBROUTINE matrixexp

      SUBROUTINE matrix2norm(a,b)
      IMPLICIT NONE
        !returns matrix 2-norm with cartesian vector x, ||x|| = 1
        !square matrix input (generalize if necessary)
        REAL,DIMENSION(5,5),INTENT(IN) :: a
        REAL,INTENT(OUT) :: b
        INTEGER :: n,i
        n=SIZE(a(1,:))
        b=0.0
        DO i=1,n
          b=b+SUM(a(:,i))**2.0
        END DO
        b=SQRT(b)
      END SUBROUTINE matrix2norm

      SUBROUTINE inverse(a,b)
      IMPLICIT NONE
        !returns an inverse of matrix a (column elimination strategy)
        !input has to be a square matrix, otherwise erroneous
        REAL,DIMENSION(5,5),INTENT(IN)  :: a
        REAL,DIMENSION(5,5),INTENT(OUT)  :: b
        REAL,DIMENSION(5,5) :: c
        INTEGER :: n,m,i,j
        n=SIZE(a(1,:))
        m=SIZE(a(:,1))
        IF(m/=n) THEN
          WRITE(*,*) "Does not compute."
          WRITE(*,*) "No square matrix input."
          WRITE(*,*) "Error in function: inverse"
        ELSE
    !      ALLOCATE(b(n,n),c(n,n))
          c=a
          b=0.0
          DO i=1,n !setting b a unit matrix
            b(i,i)=1.0
          END DO
          DO i=1,n
          !what if diagonal values are zeros?
            IF(c(i,i)==0.0)THEN!case of singuar matrix, is it?
              b(i,:)=0.0
              c(i,:)=0.0
              b(:,i)=0.0
              c(:,i)=0.0
              !        b(i,i)=1.0
              !        c(i,i)=1.0
            ELSE
              b(i,:)=b(i,:)/c(i,i)
              c(i,:)=c(i,:)/c(i,i)
            END IF
            DO j=1,i-1
              b(j,:)=b(j,:)-b(i,:)*c(j,i)
              c(j,:)=c(j,:)-c(i,:)*c(j,i)
            END DO
            DO j=i+1,n
              b(j,:)=b(j,:)-b(i,:)*c(j,i)
              c(j,:)=c(j,:)-c(i,:)*c(j,i)
            END DO
          END DO
          IF(c(n,n)==0.0)THEN
            b(n,:)=0.0
            b(:,n)=0.0
            !      b(n,n)=1.0
          ELSE
            b(n,:)=b(n,:)/c(n,n)
          END IF
        !now, b is supposed to be the requested inverse
        END IF
      END SUBROUTINE inverse
  END SUBROUTINE mod5c
END MODULE yasso
