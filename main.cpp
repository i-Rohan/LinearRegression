#include <iostream>
#include <math.h>
#include <fstream>
#include <iomanip>
#include <stdlib.h>
#include <windows.h>

using namespace std;

COORD coord={0,0};  //global variable (used for gotoxy)

void gotoxy(int x,int y)    //function for gotoxy
{
    coord.X=x;
    coord.Y=y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),coord);
}

float pro1_inverse[200][200];   //inverse of X'X

void inverse(int n) //function to calculate inverse of X'X
{
    int i,j,k;
    float d;
    for(i=0;i<n;i++)
        for(j=0;j<2*n;j++)
            if(j==(i+n))
                pro1_inverse[i][j]=1;
    /************** partial pivoting **************/
    for(i=n-1;i>0;i--)
    {
        if(pro1_inverse[i-1][1]<pro1_inverse[i][1])
        for(j=0;j<n*2;j++)
        {
            d=pro1_inverse[i][j];
            pro1_inverse[i][j]=pro1_inverse[i-1][j];
            pro1_inverse[i-1][j]=d;
        }
    }
    /********** reducing to diagonal  matrix ***********/
    for(i=0;i<n;i++)
    {
        for(j=0;j<n*2;j++)
        if(j!=i)
        {
            d=pro1_inverse[j][i]/pro1_inverse[i][i];
            for(k=0;k<n*2;k++)
                pro1_inverse[j][k]-=pro1_inverse[i][k]*d;
        }
    }
    /************** reducing to unit matrix *************/
    for(i=0;i<n;i++)
    {
    d=pro1_inverse[i][i];
        for(j=0;j<n*2;j++)
            pro1_inverse[i][j]=pro1_inverse[i][j]/d;
    }
    for(i=0;i<n;i++)
        for(j=0,k=n;j<n,k<n*2;j++,k++)
            pro1_inverse[i][j]=pro1_inverse[i][k];
}

int main()
{
    int i=0,j,k,row=0,col=14,temp=0;
    float x_transpose[200][200],pro2[200][200],pro3[200][200],y[200],x[200][200],pro1[200][200];
    cout<<"\t\t\tLinear Regression";
    cout<<"\n\n\nReading Data from files...\n\n";
    ifstream fin;   //input stream to read data
    //Reading data
    fin.open("y.txt",ios::in);
    if(!fin)
    {
        cout<<"\nMissing dependent variable file 'y.txt'\n\n";
        system("pause");
        exit(1);
    }
    fin.seekg(0);
    while(!fin.eof())
        fin>>y[temp++];
    fin.close();
    temp=1;
    fin.open("x.txt",ios::in);
    if(!fin)
    {
        cout<<"\nMissing independent variable(s) file 'x.txt'";
        system("pause");
        exit(1);
    }
    fin.seekg(0);
    while(!fin.eof())
    {
        fin>>x[row][temp++];
        if(temp==col)
        {
            row++;
            temp=1;
        }
    }
    fin.close();
    //done reading data

    for(i=0;i<row;i++)
    {
        for(j=1;j<col;j++)
            cout<<x[i][j]<<" ";
        cout<<"\n";
    }

    for(i=0;i<row;i++)
    {
        gotoxy(27,5+i);
        cout<<y[i]<<"\n";
    }

    for(i=0;i<row;i++)
        x[i][0]=1;

    for(i=0;i<row;i++)  //finding X'
        for(j=0;j<col;j++)
            x_transpose[j][i]=x[i][j];

    for(i=0;i<col;i++)    //calculating X'X
        for(j=0;j<col;j++)
        {
            pro1[i][j]=0;
            for(k=0;k<row;k++)
                pro1[i][j]+=x_transpose[i][k]*x[k][j];
        }
    for(i=0;i<col;i++)
        for(j=0;j<col;j++)
            pro1_inverse[i][j]=pro1[i][j];
    inverse(col); //calling inverse function

    for(i=0;i<col;i++)    //calculating X'Y
        for(j=0;j<1;j++)
        {
            pro2[i][j]=0;
            for(k=0;k<row;k++)
                pro2[i][j]+=x_transpose[i][k]*y[k];
        }

    for(i=0;i<col;i++)    //calculating final beta values
        for(j=0;j<1;j++)
        {
            pro1[i][j]=0;
            for(k=0;k<col;k++)
                pro3[i][j]+=pro1_inverse[i][k]*pro2[k][j];
        }

    ofstream fout;  //output stream to write data
    //writing data
    fout.open("b.txt",ios::out|ios::trunc);
    cout<<"\nBeta values:\n\n";
    for(i=0;i<col;i++)
    {
        fout<<pro3[i][0]<<"\n";
        cout<<pro3[i][0]<<"\n";
    }
    cout<<"\nBeta values stored in `b.txt`\n\n\n";

    system("pause");
    return 0;
}
