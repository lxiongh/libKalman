#include "Matrix.h"

Matrix::Matrix(){
    m_rows = 0;
    m_columns = 0;
    m_data = NULL;
}
Matrix::Matrix(int r, int c){
    m_rows = r;
    m_columns = c;

    m_data = new double[r*c];
    ClearMtr();
}
Matrix::Matrix(int r, int c, double* data){
    m_rows = r;
    m_columns = c;

    m_data = new double[r*c];
    Setdata(data,r*c);
}

Matrix::~Matrix(){
    //dtor
    this->Empty();
}

Matrix::Matrix(const Matrix& other){
    //copy ctor
    int r = other.m_rows;
    int c = other.m_columns;
    ResizeMtr(r,c);
    for(int i=0; i<r*c; i++){
        m_data[i] = other.m_data[i];
    }
}

Matrix& Matrix::operator=(const Matrix& rhs){
    if (this == &rhs) return *this; // handle self assignment
    //assignment operator
    int r = rhs.m_rows;
    int c = rhs.m_columns;
    this->ResizeMtr(r, c);
    for(int i=0; i<r*c; i++)
        this->m_data[i] = rhs.m_data[i];

    return *this;
}

Matrix Matrix::operator+(const Matrix& val){
    Matrix m = Matrix(m_rows,m_columns);
    try{
        if(m_rows!=val.Getrows() || m_columns!=val.Getcolumns())
            throw string("作用于+的操作数维数不同\n");
        for(int i=0; i< m_rows*m_columns; i++)
            m.m_data[i] = m_data[i] + val.m_data[i];
    }
    catch(string& e){
        printf("%s\n", e.c_str());
    }
    return m;
}

Matrix Matrix::operator+(const double val){
    Matrix m = Matrix(m_rows,m_columns);
    for(int i=0; i< m_rows*m_columns; i++)
        m.m_data[i] = m_data[i] + val;
    return m;
}

Matrix Matrix::operator-(const Matrix& val){
    Matrix m = Matrix(m_rows,m_columns);
    try{
        if(m_rows!=val.m_rows || m_columns!=val.m_columns)
            throw string("作用于-的操作数维数不同\n");
        for(int i=0; i< m_rows*m_columns; i++)
            m.m_data[i] = m_data[i] - val.m_data[i];
    }
    catch(string& e){
        printf("%s\n", e.c_str());
    }
    return m;
}

Matrix Matrix::operator*(const Matrix& val){
    double sum;
    Matrix m = Matrix(m_rows,val.m_columns);
    try{
        if(m_columns!=val.m_rows)
            throw string("两个操作数无法相乘\n");
        for(int row=1; row<=m_rows; row++){
            for(int col=1; col<=val.m_columns; col++){
                sum = 0;
                for(int i=1; i<=val.m_columns; i++)
                    sum += (this->Get(row,i) * val.Get(i, col));
                m.Set(row, col, sum);
            }
        }
    }
    catch(string& e){
        printf("%s\n", e.c_str());
    }
    return m;
}

void Matrix::ResizeMtr(int r, int c){
    if(m_rows!=r||m_columns!=c){
        this->Empty();
        m_rows = r;
        m_columns = c;
        m_data = new double[r*c];
    }
    ClearMtr();
}

void Matrix::ClearMtr(){
    for(int i=0; i<m_rows*m_columns; i++)
        m_data[i] = 0;
}

double Matrix::Get(int r, int c) const{
    r--;
    c--;
    return m_data[c*m_rows+r];
}

void Matrix::Set(int r, int c, double val){
    r--;
    c--;
    m_data[c*m_rows+r] = val;
}

void Matrix::Setdata(double* val, int len){
    for(int i=0; i<len; i++)
        m_data[i] = val[i];
}

void Matrix::DisplayMtr(){
    for(int r=1; r<=Getrows(); r++){
        for(int c=1; c<=Getcolumns(); c++)
            cout << Get(r,c) << "\t";
        cout << "\n";
    }
}

Matrix Matrix::InverseMtr(){
    Matrix m = Matrix(this->Getrows(),this->Getcolumns(),this->Getdata());

    try{
        if(m_rows!=m_columns)
            throw string("不是方阵，没有逆矩阵\n");
        inv(m.m_data, m_rows);
    }
    catch(string& e){
        printf("%s\n", e.c_str());
    }
    return m;
}

Matrix Matrix::TransposeMtr(){
    Matrix m = Matrix(m_columns, m_rows);
    for(int row=1; row<=m.m_rows; row++){
        for(int col=1; col<=m.m_columns; col++)
            m.Set(row,col,Get(col,row));
    }
    return m;
}

void Matrix::inv(double *a,int n){
    try{
        int *is,*js,i,j,k,l,u,v;
        double d,p;
        is=(int*)malloc(n*sizeof(int));
        js=(int*)malloc(n*sizeof(int));
        for (k=0; k<=n-1; k++){
            d=0.0;
            for (i=k; i<=n-1; i++)
                for (j=k; j<=n-1; j++){
                    l=i*n+j; p=fabs(a[l]);
                    if (p>d){
                        d=p; is[k]=i; js[k]=j;
                    }
                }

            if (d+1.0==1.0){
                free(is); free(js); throw string("err**not inv\n");
            }
            if (is[k]!=k)
                for (j=0; j<=n-1; j++){
                    u=k*n+j; v=is[k]*n+j;
                    p=a[u]; a[u]=a[v]; a[v]=p;
                }
            if (js[k]!=k)
                for (i=0; i<=n-1; i++){
                    u=i*n+k; v=i*n+js[k];
                    p=a[u]; a[u]=a[v]; a[v]=p;
                }
            l=k*n+k;
            a[l]=1.0/a[l];
            for (j=0; j<=n-1; j++)
                if (j!=k){
                    u=k*n+j; a[u]=a[u]*a[l];
                }
            for (i=0; i<=n-1; i++)
                if (i!=k)
                    for (j=0; j<=n-1; j++)
                    if (j!=k){
                        u=i*n+j;
                        a[u]=a[u]-a[i*n+k]*a[k*n+j];
                    }
            for (i=0; i<=n-1; i++)
                if (i!=k){
                    u=i*n+k; a[u]=-a[u]*a[l];
                }
        }
        for (k=n-1; k>=0; k--){
            if (js[k]!=k)
                for (j=0; j<=n-1; j++){
                    u=k*n+j; v=js[k]*n+j;
                    p=a[u]; a[u]=a[v]; a[v]=p;
                }
            if (is[k]!=k)
                for (i=0; i<=n-1; i++){
                    u=i*n+k; v=i*n+is[k];
                    p=a[u]; a[u]=a[v]; a[v]=p;
                }
        }
        free(is); free(js);
    }
    catch(string& e){
            printf("%s\n", e.c_str());
    }
}

void Matrix::Empty(){
    if(m_data!=NULL){
        delete[] m_data;
        m_data = NULL;
    }
}
