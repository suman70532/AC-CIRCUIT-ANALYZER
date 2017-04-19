#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


char* remove$(char *str)
{
    int i,j = 1;
    char *result = malloc(sizeof(char)*(strlen(str) + 1));
    for(i = 0; i<strlen(str); i++)
    {
        if(str[i]>=48 && str[i]<=57 || str[i]>=65 && str[i]<=90 || str[i]>=97 && str[i]<=122 || str[i] == 32 || str[i] == 95)
            result[i] = str[i];
        else
        {
        j = 0;
            result[i] = '_';
        }
    }
    result[strlen(str)] = '\0';
    //if(j == 0)
    //printf("\"%s\"->This name contains special character, removing and continuing execution!!",str);
    return result;
}

int count = 0;
int counts = 0;
int dc = 0;
int starti,startj,flag =0;
struct net
{
    int name;
    double x,y;
};

struct element
{
    int present ;
    char* elem;int ele;
    struct net left ;
    struct net right ;
    double value;
    char unit;
    int b ,meg,check;
    double offset,amplitude,frequency,delay,damping;
    struct element* next;
};

struct frequency
{
    double value;
    char unit;
    struct frequency* next;
};

struct location_array
{
    double x,y;
    struct location_array* next;
};
struct frequency*  listfreq = NULL;
struct element* top = NULL;
struct element  g[1000][1000];
int             visited[1000][1000];
char*           name[1000];
struct location_array* location;
char* filename ;
char* resfile ;
double net_location [1000][2];
int is_present [1000];
double i_length = 43;
double r_length = 100;
double c_length = 34;
double v_length = 90;
double a_length = 80;

struct complex
{
    double real,img;
};

struct complex add(struct complex a, struct complex b)
{
    struct complex c;
    c.real = a.real + b.real;
    c.img = a.img + b.img;
    return c;
}

struct complex multiply(struct complex a, struct complex b)
{
    struct complex c;
    c.real = a.real*b.real - a.img*b.img;
    c.img = a.real*b.img + a.img*b.real;
    return c;
}

struct complex mulinv(struct complex a)
{
    struct complex c;
    double x = a.real*a.real + a.img*a.img;
    c.real = a.real/x;
    c.img = -a.img/x;
    return c;
}

struct complex impedence(struct element* a , double w)
{
    struct complex c;
    c.real = 0.0;
    c.img = 0.0;
    double x;
    while(a != NULL)
    {
        //printf("%c\n", a->unit);
        if(a->unit == '.')
            x = a->value;
        else if(a->unit == 'K')
            x = a->value*1000.0;
        else if(a->unit == 'F')
            x = a->value/1000000000000000.0;
        else if(a->unit == 'P')
            x = a->value/1000000000000.0;
        else if(a->unit == 'N')
            x = a->value/1000000000.0;
        else if(a->unit == 'U')
            x = a->value/1000000.0;
        else if(a->unit == 'M')
            x = a->value/1000.0;
        if(a->ele == 0)
            c.real += 1/x;
        else if(a->ele == 1)
            c.img -= 1/(w*x);
        else if(a->ele == 2)
            c.img += w*x;
        a = a->next;
    }
    return c;
}


struct complex subtraction( struct complex a, struct complex b){
    struct complex ans;
    ans.real = a.real - b.real;
    ans.img = a.img - b.img;
    return ans;
}

struct complex division(struct complex a, struct complex b){
    double temp = (b.real*b.real) + (b.img*b.img);
    b.img = b.img * -1.0;
    struct complex ans = multiply(a,b);
    ans.real = ans.real/temp;
    ans.img = ans.img/temp;
    return ans;
}

/*void printComplex(struct complex a)
{	
	
    printf(f,"%lf , %lf\t",a.real,a.img);
}*/

int vname[1000];

void calcv()
{
	   
	FILE *f = fopen(resfile, "w");
	struct frequency* sort = listfreq;
    while(sort != NULL)
    {
        struct frequency* jl = sort;
        while(jl != NULL)
        {   

            double x1,x2;
            if(sort->unit == '.')
                x1 = sort->value;
            else if(sort->unit == 'K')
                x1 = sort->value*1000.0;
            else if(sort->unit == 'F')
                x1 = sort->value/1000000000000000.0;
            else if(sort->unit == 'P')
                x1 = sort->value/1000000000000.0;
            else if(sort->unit == 'N')
                x1 = sort->value/1000000000.0;
            else if(sort->unit == 'U')
                x1 = sort->value/1000000.0;
            else if(sort->unit == 'M')
                x1 = sort->value/1000.0;

            if(jl->unit == '.')
                x2 = jl->value;
            else if(jl->unit == 'K')
                x2 = jl->value*1000.0;
            else if(jl->unit == 'F')
                x2 = jl->value/1000000000000000.0;
            else if(jl->unit == 'P')
                x2 = jl->value/1000000000000.0;
            else if(jl->unit == 'N')
                x2 = jl->value/1000000000.0;
            else if(jl->unit == 'U')
                x2 = jl->value/1000000.0;
            else if(jl->unit == 'M')
                x2 = jl->value/1000.0;

            if(x1 > x2)
            {
                struct frequency ttt ;

                ttt.value = sort->value;
                sort->value = jl->value;
                jl->value = ttt.value;

                ttt.unit = sort->unit;
                sort->unit = jl->unit;
                jl->unit = ttt.unit;
            }

            jl = jl->next;
        }

        sort = sort->next;
    }
    struct frequency* freq = listfreq;

    struct frequency* fd = listfreq;

    /*while(fd != NULL)
    {
        printf("%.10lf %c\n",fd->value,fd->unit);
        fd = fd->next;
    }  */

    while(freq != NULL)
    {

    struct complex A[count-1 + counts][count-1 + counts];
    struct complex B[count-1][counts];
    struct complex G[count-1][count-1];
    int V[count-1 + counts];
    struct complex C[count-1 + counts];

    int arr[1000];
    int cntr = 0;

    struct complex mone;
    mone.real = -1;
    mone.img  = 0;

    struct complex one;
    one.real = 1;
    one.img  = 0;

    struct complex zero;
    zero.real = 0;
    zero.img  = 0;

    int i=0,j=0;

    //printf("%d is count and %d is counts\n\n",count,counts);

    for(i=0;i<(count-1+counts);i++)    
    {     
        V[i] = 0;

        C[i].real = 0;
        C[i].img  = 0;


        for(j=0;j<(count-1+counts);j++)
        {
            if(i<(count-1) && j<(counts))
            {
               
                B[i][j].real = 0;
                B[i][j].img  = 0;
             
                A[i][j].real = 0;
                A[i][j].img  = 0;
         
                G[i][j].real = 0;
                G[i][j].img  = 0;

            }
            else if(i<(count-1) && j<(count-1))
            {
               
                A[i][j].real = 0;
                A[i][j].img  = 0;
              
                G[i][j].real = 0;
                G[i][j].img  = 0;
            }
            else
            {
               
                A[i][j].real = 0;
                A[i][j].img  = 0;


            }
        }
    }

    for(i=0;i<1000;i++)
     {for(j=0;j<1000;j++)
        {   
            if(g[i][j].present == 1 )
                if(strcmp(name[i],"0"))
                {     
                    arr[i] = cntr; /*printf("%s mapped to %d\n",name[i],arr[i]);*/V[cntr] = i; cntr++;break;
                }
        }
     }

    for(i=0;i<1000;i++)
    {
        int flag = 0;
        for(j=0; j<1000;j++)
        {   

            if(g[i][j].present == 1 && strcmp(name[i],"0"))
            {   
                g[i][j].check = 0;
                if(g[i][j].b == 1)
                {
                    if(g[i][j].ele == 3 && g[i][j].frequency == freq->value)
                    {
                        if(g[i][j].left.name == i)
                        {   
                            struct complex ne;
                            ne.real = g[i][j].amplitude;
                            ne.img  = 0;
                            C[arr[i]] = add(C[arr[i]],ne);
                        }
                        else if(g[i][j].right.name == i)
                        {   

                            struct complex ne;
                            ne.real = -1*g[i][j].amplitude;
                            ne.img  = 0;
                            C[arr[i]] = add(C[arr[i]],ne);
                        }
                    }

                    if(g[i][j].next != NULL)
                    {
                        struct element* temp = g[i][j].next;
                        while(temp!=NULL)
                        {
                            temp->check = 0;
                            if(temp->ele == 3 && temp->frequency == freq->value)
                            {
                                if(temp->left.name == i)
                                {   

                                    struct complex ne;
                                    ne.real = temp->amplitude;
                                    ne.img  = 0;
                                    C[arr[i]] = add(C[arr[i]],ne);
                                }
                                else if(temp->right.name == i)
                                {   

                                    struct complex ne;
                                    ne.real = -1*temp->amplitude;
                                    ne.img  = 0;
                                    C[arr[i]] = add(C[arr[i]],ne);
                                }
                            }

                            temp = temp->next;
                        }
                    }
                }
                else
                {   
                    double x;
                    if(freq->unit == '.')
            x = freq->value;
        else if(freq->unit == 'K')
            x = freq->value*1000.0;
        else if(freq->unit == 'F')
            x = freq->value/1000000000000000.0;
        else if(freq->unit == 'P')
            x = freq->value/1000000000000.0;
        else if(freq->unit == 'N')
            x = freq->value/1000000000.0;
        else if(freq->unit == 'U')
            x = freq->value/1000000.0;
        else if(freq->unit == 'M')
            x = freq->value/1000.0;
                    struct complex hi = impedence(&g[i][j],2*3.14159*x);

                    //printf("%d  %d \n\n\n",arr[i],arr[j]);
                    if(strcmp(name[j],"0"))
                    {   
                        G[arr[i]][arr[j]] = multiply(mone , hi);
                    }
                   
                    G[arr[i]][arr[i]] = add(G[arr[i]][arr[i]],hi);

                    if(g[i][j].next != NULL)
                    {
                        struct element* temp = g[i][j].next;
                        while(temp!=NULL)
                        {   
                            temp->check = 0;
                            if(temp->ele == 3 && temp->frequency == freq->value)
                            {
                                if(temp->left.name == i)
                                {   

                                    struct complex ne;
                                    ne.real = temp->amplitude;
                                    ne.img  = 0;
                                    C[arr[i]] = add(C[arr[i]],ne);
                                }
                                else if(temp->right.name == i)
                                {   

                                    struct complex ne;
                                    ne.real = -1*temp->amplitude;
                                    ne.img  = 0;
                                    C[arr[i]] = add(C[arr[i]],ne);
                                }
                            }

                            temp = temp->next;
                        }
                    }
                }
            }
        }
    }

   
    int bt = 0;

    for(i=0;i<1000;i++)
        for(j=0;j<1000;j++)
        {
            if(g[i][j].present == 1)
            {
                
                if(g[i][j].b == 1 && g[i][j].ele == 4 && g[i][j].check != 1)
                {   
                    //printf("%s   -----------\n",g[i][j].elem);
                    int x = g[i][j].left.name;
                    int y = g[i][j].right.name;
                    if(strcmp(name[x],"0"))
                    {
                        B[arr[x]][bt].real = -1;
                        B[arr[x]][bt].img  = 0;
                    }
                    if(strcmp(name[y],"0"))
                    {
                        B[arr[y]][bt].real = 1;
                        B[arr[y]][bt].img  =  0;
                    }

                    if(g[i][j].frequency == freq->value)
                    {
                    C[count-1+bt].real = g[i][j].amplitude;;
                    C[count-1+bt].img  =  0;

                    
                    }

                    vname[bt] = getHashIndex(g[i][j].elem);
                    g[i][j].check = 1;
                    g[j][i].check = 1;
                
                    bt++;
                }

                if(g[i][j].next != NULL)
                    {
                        struct element* temp = g[i][j].next;
                        struct element* temp1= g[j][i].next;
                        while(temp!=NULL)
                        {

                            if(temp->b == 1 && temp->ele == 4 && temp->check != 1)
                            {
                                //printf("%s   -----------\n",temp->elem);
                                int x = temp->left.name;
                                int y = temp->right.name;

                                if(strcmp(name[x],"0"))
                                {
                                    B[arr[x]][bt].real = -1;
                                    B[arr[x]][bt].img  = 0;
                                }
                                if(strcmp(name[y],"0"))
                                {
                                    B[arr[y]][bt].real = 1;
                                    B[arr[y]][bt].img  =  0;
                                }
                                if(temp->frequency == freq->value)
                                {
                                C[count-1+bt].real = temp->amplitude;;
                                C[count-1+bt].img  =  0;
                                
                                }
                                temp->check = 1;
                                temp1->check= 1;

                                vname[bt] = getHashIndex(temp->elem);
                                bt++;
                            }

                            temp = temp->next;temp1 = temp1->next;
                        }
                    }
            }
        }

    for(i=0;i<(count-1);i++)
        for(j=0;j<(count-1);j++)
        {
            A[i][j] = G[i][j];
        }

    for(i=0;i<(count-1);i++)
        for(j=(count - 1);j<(count-1 + counts);j++)
        {
            A[i][j] = B[i][j - (count - 1)];
        }

    for(i=(count-1);i<(count-1 + counts);i++)
        for(j=0;j<(count-1);j++)
        {
            A[i][j] = B[j][i - (count - 1)];
        }

    fprintf(f,"FREQ = %f%chz\n",freq->value,freq->unit);

    /*for(i=0;i<(count-1+counts);i++)
        {for(j=0;j<(count-1+counts);j++)
        {
            printf("\t\t%.10lf,%.10lf",A[i][j].real,A[i][j].img);
        }
        printf("\n");}*/


    /*for(i=0;i<(count-1);i++)
        {
            printf("\t\tVoltage_%s\n",name[V[i]]);
        }*/

 
    /*for(i=0;i<(count-1+counts);i++)
        {
        
            printf("\t\t%.10lf,%.10lf\n",C[i].real,C[i].img);
        
        }*/

        //printf("======================================================\n");

    int size = count+ counts- 1;

    //creating augmented Matrix
    struct complex Y[count - 1 + counts][count + counts];

    struct complex solution[size];
    for(i = 0 ; i < size;i ++){
        for(j = 0 ; j <= size; j++){
            if(j == size){
                Y[i][j] = C[i];
            }
            else{
                Y[i][j] = A[i][j];
            }
        }
    }

    
    //creating upper Triangular matrix

    int col_start = 0;
    int row_start = 0;
    while(row_start < size-1){
        if(Y[row_start][col_start].real == 0.0 && Y[row_start][col_start].img == 0.0)
        {
            int flag = 0;
            for(i = row_start + 1;i < size;i++)
            {
                if(Y[i][col_start].real != 0.0 || Y[i][col_start].img != 0.0)
                {
                    struct complex pole;
                    flag = 1;
                    for(j = 0;j < size+1;j++)
                    {
                        pole = Y[i][j];
                        Y[i][j] = Y[row_start][j];
                        Y[row_start][j] = pole;
                    }
                }
                if(flag == 1)
                    break;
            }
        }

        for(i = row_start + 1;i < size; i ++)
        {   
            if(Y[i][col_start].real == 0.0 && Y[i][col_start].img == 0.0)
                continue;

            struct complex index = division(Y[i][col_start], Y[row_start][col_start]);
            for(j = 0;j<= size; j++){
                
                    struct complex temp = multiply(index, Y[row_start][j]);
                    Y[i][j] = subtraction(Y[i][j],temp);
           
            }
        }
        row_start++;
        col_start++;
    }

    //compute values of variables
    struct complex sum;
    sum.real = 0;
    sum.img = 0;
    solution[size-1] = division(Y[size-1][size],Y[size-1][size-1]);
    for(i = size-2; i >= 0; i--){
        sum.real = 0;
        sum.img = 0;
        for(j = i + 1;j < size; j++){
            sum = add(sum, multiply(Y[i][j], solution[j])); 
        }
        sum = subtraction(Y[i][size],sum);
        solution[i] = division(sum, Y[i][i]);
    }

    /*for(i = 0 ;i < size;i ++){
        //printComplex(solution[i]);
        printf("%lf , %lf\n",solution[i].real,solution[i].img);
        //printf("\n");
    }*/
    //printf("\n\n");
    fprintf(f,"VOLTAGES\n");
    struct element* here = top;
    while(here != NULL)
    {
        fprintf(f,"%s ",here->elem);
        struct complex l;
        struct complex r;
        if(strcmp(name[here->left.name],"0"))
            l = solution[arr[here->left.name]];
        else
            {l.real = 0; l.img = 0;}

        if(strcmp(name[here->right.name],"0"))
            r = solution[arr[here->right.name]];
        else
            {r.real = 0; r.img = 0;}

        l = subtraction(l,r);
        
        if(((l.real) < 0.00000000001 && (l.real) > 0.0)||((l.real) > -0.00000000001 && (l.real) <0.0))
            l.real = 0;
        if(((l.img) < 0.00000000001 && (l.img) > 0.0)||((l.img) > -0.00000000001 && (l.img) <0.0))
            l.img = 0;

        fprintf(f,"%lf ",sqrt(l.real*l.real + l.img*l.img));


        if(sqrt(l.real*l.real + l.img*l.img) == 0.0)
            fprintf(f,"%lf\n", 0.0);
        else{
        if(l.real == 0.0)
            if(l.img > 0.0)
                fprintf(f,"%lf\n", 90.0);
            else    
                fprintf(f,"%lf\n", -90.0);
        else
        {
            double angle = 180.0*atan(l.img/l.real)/3.14159265359;

            if(abs(angle) < 0.0000000001)
                angle = 0;
            fprintf(f,"%lf\n",angle);}
        }

        here = here->next;
    }
	fprintf(f,"\n\n\n");
    fprintf(f,"CURRENTS\n");
    struct element* a = top;
    struct complex I;
    struct complex z;
    double x,y;
        if(freq->unit == '.')
            y = 2*3.14159265359*freq->value;
        else if(freq->unit == 'K')
            y = 2*3.14159265359*freq->value*1000.0;
        else if(freq->unit == 'F')
            y = 2*3.14159265359*freq->value/1000000000000000.0;
        else if(freq->unit == 'P')
            y = 2*3.14159265359*freq->value/1000000000000.0;
        else if(freq->unit == 'N')
            y = 2*3.14159265359*freq->value/1000000000.0;
        else if(freq->unit == 'U')
            y = 2*3.14159265359*freq->value/1000000.0;
        else if(freq->unit == 'M')
            y = 2*3.14159265359*freq->value/1000.0;    
    while(a != NULL)
    {
	if(a->ele != 3)
        fprintf(f,"%s ",a->elem);	
        struct complex l;
        struct complex r;
        if(strcmp(name[a->left.name],"0"))
            l = solution[arr[a->left.name]];
        else
            {l.real = 0; l.img = 0;}

        if(strcmp(name[a->right.name],"0"))
            r = solution[arr[a->right.name]];
        else
            {r.real = 0; r.img = 0;}
        l = subtraction(r,l);
        if(((l.real) < 0.00000000001 && (l.real) > 0.0)||((l.real) > -0.00000000001 && (l.real) <0.0))
            l.real = 0;
        if(((l.img) < 0.00000000001 && (l.img) > 0.0)||((l.img) > -0.00000000001 && (l.img) <0.0))
            l.img = 0;
    if(a->b == 0)
    {
        if(a->unit == '.')
            x = a->value;
        else if(a->unit == 'K')
            x = a->value*1000.0;
        else if(a->unit == 'F')
            x = a->value/1000000000000000.0;
        else if(a->unit == 'P')
            x = a->value/1000000000000.0;
        else if(a->unit == 'N')
            x = a->value/1000000000.0;
        else if(a->unit == 'U')
            x = a->value/1000000.0;
        else if(a->unit == 'M')
            x = a->value/1000.0;
        if(a->ele == 0)
        {  
        z.real = x;z.img = 0.0;          
        I = division(l, z);
            fprintf(f,"%lf ",sqrt(I.real*I.real + I.img*I.img));
            if(sqrt(I.real*I.real + I.img*I.img) == 0.0)
            fprintf(f,"%lf\n", 0.0);
        else{
        if(I.real == 0.0)
            if(I.img > 0.0)
                fprintf(f,"%lf\n", 90.0);
            else    
                fprintf(f,"%lf\n", -90.0);
        else
            {
            double angle = 180.0*atan(l.img/l.real)/3.14159265359;

            if(abs(angle) < 0.0000000001)
                angle = 0;
            fprintf(f,"%lf\n",angle);}

            }

        }
        else if(a->ele == 1)
        {
            z.real = 0.0;z.img = x*y;
            I = division(l, z);
            fprintf(f,"%lf ",sqrt(I.real*I.real + I.img*I.img));
            if(sqrt(I.real*I.real + I.img*I.img) == 0.0)
            fprintf(f,"%lf\n", 0.0);
        else{
        if(I.real == 0.0)
            if(I.img > 0.0)
                fprintf(f,"%lf\n", 90.0);
            else    
                fprintf(f,"%lf\n", -90.0);
        else
            {
            double angle = 180.0*atan(I.img/I.real)/3.14159265359;

            if(abs(angle) < 0.0000000001)
                angle = 0;
            fprintf(f,"%lf\n",angle);}


            }
        }
        else if(a->ele == 2)
        {
            z.real = 0.0;z.img = -1/(x*y);
            I = division(l, z);
            fprintf(f,"%lf ",sqrt(I.real*I.real + I.img*I.img));
            if(sqrt(I.real*I.real + I.img*I.img) == 0.0)
            fprintf(f,"%lf\n", 0.0);
        else{
        if(I.real == 0.0)
            if(I.img > 0.0)
                fprintf(f,"%lf\n", 90.0);
            else    
                fprintf(f,"%lf\n", -90.0);
        else
            {
            double angle = 180.0*atan(I.img/I.real)/3.14159265359;
            if(abs(angle) < 0.0000000001)
                angle = 0;
            fprintf(f,"%lf\n",angle);}

            }
        }
    }
    else
    {   
        //printf("c --%d--\n",a->ele);
        if(a->ele == 4)
        {   
            int i;
            //printf("%s ",a->elem);
            for(i = 0; i < counts;i++)
            {
                if(vname[i] == getHashIndex(a->elem))
                {
                    //printf("c");
                    I = solution[i + count - 1];

                    fprintf(f,"%lf ",sqrt(I.real*I.real + I.img*I.img));
                    if(sqrt(I.real*I.real + I.img*I.img) == 0.0)
                        fprintf(f,"%lf\n", 0.0);
                    else
                    {
                        if(I.real == 0)
                        if(I.img >= 0)
                            fprintf(f,"%lf\n", 90.0);
                        else    
                            fprintf(f,"%lf\n", -90.0);
                        else
                        {
                            double angle = 180.0*atan(I.img/I.real)/3.14159265359;
                            //printf("idd  %lf",angle);
                            if(abs(angle) < 0.0000000001)
                                angle = 0;
                            fprintf(f,"%lf\n",angle);
                        }

                    }

                }
            }

        }
    }
        a = a->next;
    }
	fprintf(f,"\n\n\n");

  //  printf("======================================================\n");

   
    freq = freq->next;

    }

    struct frequency* bv = (struct frequency*) malloc(sizeof(struct frequency));
    bv->value = 0.00000000000000000001;
    bv->unit = 'K';
    bv->next = NULL;
    freq = bv;

    while(freq != NULL)
    {
    struct complex A[count-1 + counts][count-1 + counts];
    struct complex B[count-1][counts];
    struct complex G[count-1][count-1];
    int V[count-1 + counts];
    struct complex C[count-1 + counts];

    int arr[1000];
    int cntr = 0;

    struct complex mone;
    mone.real = -1;
    mone.img  = 0;

    struct complex one;
    one.real = 1;
    one.img  = 0;

    struct complex zero;
    zero.real = 0;
    zero.img  = 0;

    int i=0,j=0;

    //printf("%d is count and %d is counts\n\n",count,counts);

    for(i=0;i<(count-1+counts);i++)    
    {     
        V[i] = 0;

        C[i].real = 0;
        C[i].img  = 0;


        for(j=0;j<(count-1+counts);j++)
        {
            if(i<(count-1) && j<(counts))
            {
               
                B[i][j].real = 0;
                B[i][j].img  = 0;
             
                A[i][j].real = 0;
                A[i][j].img  = 0;
         
                G[i][j].real = 0;
                G[i][j].img  = 0;

            }
            else if(i<(count-1) && j<(count-1))
            {
               
                A[i][j].real = 0;
                A[i][j].img  = 0;
              
                G[i][j].real = 0;
                G[i][j].img  = 0;
            }
            else
            {
               
                A[i][j].real = 0;
                A[i][j].img  = 0;


            }
        }
    }

    for(i=0;i<1000;i++)
    {
        int flag = 0;
        for(j=0; j<1000;j++)
        {   

            if(g[i][j].present == 1 && strcmp(name[i],"0"))
            {   
                g[i][j].check = 0;
                if(g[i][j].b == 1)
                {
                    if(g[i][j].ele == 3 )
                    {
                        if(g[i][j].left.name == i)
                        {   
                            struct complex ne;
                            ne.real = g[i][j].offset;
                            ne.img  = 0;
                            C[arr[i]] = add(C[arr[i]],ne);
                        }
                        else if(g[i][j].right.name == i)
                        {   

                            struct complex ne;
                            ne.real = -1*g[i][j].offset;
                            ne.img  = 0;
                            C[arr[i]] = add(C[arr[i]],ne);
                        }
                    }

                    if(g[i][j].next != NULL)
                    {
                        struct element* temp = g[i][j].next;
                        while(temp!=NULL)
                        {
                            temp->check = 0;
                            if(temp->ele == 3 && temp->frequency == freq->value)
                            {
                                if(temp->left.name == i)
                                {   

                                    struct complex ne;
                                    ne.real = temp->offset;
                                    ne.img  = 0;
                                    C[arr[i]] = add(C[arr[i]],ne);
                                }
                                else if(temp->right.name == i)
                                {   

                                    struct complex ne;
                                    ne.real = -1*temp->offset;
                                    ne.img  = 0;
                                    C[arr[i]] = add(C[arr[i]],ne);
                                }
                            }

                            temp = temp->next;
                        }
                    }
                }
                else
                {   
                    double x;
                    if(freq->unit == '.')
                        x = freq->value;
                    else if(freq->unit == 'K')
                        x = freq->value*1000.0;
                    else if(freq->unit == 'F')
                        x = freq->value/1000000000000000.0;
                    else if(freq->unit == 'P')
                        x = freq->value/1000000000000.0;
                    else if(freq->unit == 'N')
                        x = freq->value/1000000000.0;
                    else if(freq->unit == 'U')
                        x = freq->value/1000000.0;
                    else if(freq->unit == 'M')
                        x = freq->value/1000.0;
                    struct complex hi = impedence(&g[i][j],freq->value);

                    //printf("%d  %d \n\n\n",arr[i],arr[j]);
                    if(strcmp(name[j],"0"))
                    {   
                        G[arr[i]][arr[j]] = multiply(mone , hi);
                    }
                   
                    G[arr[i]][arr[i]] = add(G[arr[i]][arr[i]],hi);

                    if(g[i][j].next != NULL)
                    {
                        struct element* temp = g[i][j].next;
                        while(temp!=NULL)
                        {   
                            temp->check = 0;
                            if(temp->ele == 3)
                            {
                                if(temp->left.name == i)
                                {   

                                    struct complex ne;
                                    ne.real = temp->amplitude;
                                    ne.img  = 0;
                                    C[arr[i]] = add(C[arr[i]],ne);
                                }
                                else if(temp->right.name == i)
                                {   

                                    struct complex ne;
                                    ne.real = -1*temp->amplitude;
                                    ne.img  = 0;
                                    C[arr[i]] = add(C[arr[i]],ne);
                                }
                            }

                            temp = temp->next;
                        }
                    }
                }
            }
        }
    }

   
    int bt = 0;

    for(i=0;i<1000;i++)
        for(j=0;j<1000;j++)
        {
            if(g[i][j].present == 1)
            {
                
                if(g[i][j].b == 1 && g[i][j].ele == 4 && g[i][j].check != 1)
                {   
                    //printf("%s   -----------\n",g[i][j].elem);
                    int x = g[i][j].left.name;
                    int y = g[i][j].right.name;
                    if(strcmp(name[x],"0"))
                    {
                        B[arr[x]][bt].real = -1;
                        B[arr[x]][bt].img  = 0;
                    }
                    if(strcmp(name[y],"0"))
                    {
                        B[arr[y]][bt].real = 1;
                        B[arr[y]][bt].img  =  0;
                    }

                   
                    C[count-1+bt].real = g[i][j].offset;;
                    C[count-1+bt].img  =  0;
                    
                    vname[bt] = getHashIndex(g[i][j].elem);

                    g[i][j].check = 1;
                    g[j][i].check = 1;
                
                    bt++;
                }

                if(g[i][j].next != NULL)
                    {
                        struct element* temp = g[i][j].next;
                        struct element* temp1= g[j][i].next;
                        while(temp!=NULL)
                        {

                            if(temp->b == 1 && temp->ele == 4 && temp->check != 1)
                            {
                               // printf("%s   -----------\n",temp->elem);
                                int x = temp->left.name;
                                int y = temp->right.name;

                                if(strcmp(name[x],"0"))
                                {
                                    B[arr[x]][bt].real = -1;
                                    B[arr[x]][bt].img  = 0;
                                }
                                if(strcmp(name[y],"0"))
                                {
                                    B[arr[y]][bt].real = 1;
                                    B[arr[y]][bt].img  =  0;
                                }
                                
                                vname[bt] = getHashIndex(temp->elem);
                                C[count-1+bt].real = temp->offset;;
                                C[count-1+bt].img  =  0;
                                
                                temp->check = 1;
                                temp1->check= 1;

                            
                                bt++;
                            }

                            temp = temp->next;temp1 = temp1->next;
                        }
                    }
            }
        }

    for(i=0;i<(count-1);i++)
        for(j=0;j<(count-1);j++)
        {
            A[i][j] = G[i][j];
        }

    for(i=0;i<(count-1);i++)
        for(j=(count - 1);j<(count-1 + counts);j++)
        {
            A[i][j] = B[i][j - (count - 1)];
        }

    for(i=(count-1);i<(count-1 + counts);i++)
        for(j=0;j<(count-1);j++)
        {
            A[i][j] = B[j][i - (count - 1)];
        }
	if(dc == 1)
{
    fprintf(f,"FREQ = 0Khz\n");

    /*for(i=0;i<(count-1+counts);i++)
        {for(j=0;j<(count-1+counts);j++)
        {
            printf("\t\t%.10lf,%.10lf",A[i][j].real,A[i][j].img);
        }
        printf("\n");}*/

    //printf("======================================================\n");

    int size = count+ counts- 1;

    //creating augmented Matrix
    struct complex Y[count - 1 + counts][count + counts];

    struct complex solution[size];
    for(i = 0 ; i < size;i ++){
        for(j = 0 ; j <= size; j++){
            if(j == size){
                Y[i][j] = C[i];
            }
            else{
                Y[i][j] = A[i][j];
            }
        }
    }

    
    //creating upper Triangular matrix

    int col_start = 0;
    int row_start = 0;
    while(row_start < size-1){
        if(Y[row_start][col_start].real == 0.0 && Y[row_start][col_start].img == 0.0)
        {
            int flag = 0;
            for(i = row_start + 1;i < size;i++)
            {
                if(Y[i][col_start].real != 0.0 || Y[i][col_start].img != 0.0)
                {
                    struct complex pole;
                    flag = 1;
                    for(j = 0;j < size+1;j++)
                    {
                        pole = Y[i][j];
                        Y[i][j] = Y[row_start][j];
                        Y[row_start][j] = pole;
                    }
                }
                if(flag == 1)
                    break;
            }
        }

        for(i = row_start + 1;i < size; i ++)
        {   
            if(Y[i][col_start].real == 0.0 && Y[i][col_start].img == 0.0)
                continue;

            struct complex index = division(Y[i][col_start], Y[row_start][col_start]);
            for(j = 0;j<= size; j++){
                
                    struct complex temp = multiply(index, Y[row_start][j]);
                    Y[i][j] = subtraction(Y[i][j],temp);
           
            }
        }
        row_start++;
        col_start++;
    }


    //compute values of variables
    struct complex sum;
    sum.real = 0;
    sum.img = 0;
    solution[size-1] = division(Y[size-1][size],Y[size-1][size-1]);
    for(i = size-2; i >= 0; i--){
        sum.real = 0;
        sum.img = 0;
        for(j = i + 1;j < size; j++){
            sum = add(sum, multiply(Y[i][j], solution[j])); 
        }
        sum = subtraction(Y[i][size],sum);
        solution[i] = division(sum, Y[i][i]);
    }

    /*for(i = 0 ;i < size;i ++){
        //printComplex(solution[i]);
        printf("%lf , %lf\n",solution[i].real,solution[i].img);
        //printf("\n");
    }*/
    //printf("\n\n");
    fprintf(f,"VOLTAGES\n");
    struct element* here = top;
    while(here != NULL)
    {
        fprintf(f,"%s ",here->elem);
        struct complex l;
        struct complex r;
        if(strcmp(name[here->left.name],"0"))
            l = solution[arr[here->left.name]];
        else
            {l.real = 0; l.img = 0;}

        if(strcmp(name[here->right.name],"0"))
            r = solution[arr[here->right.name]];
        else
            {r.real = 0; r.img = 0;}

        l = subtraction(l,r);
        
        if(((l.real) < 0.00000000001 && (l.real) > 0.0)||((l.real) > -0.00000000001 && (l.real) <0.0))
            l.real = 0;
        if(((l.img) < 0.00000000001 && (l.img) > 0.0)||((l.img) > -0.00000000001 && (l.img) <0.0))
            l.img = 0;

        fprintf(f,"%lf\n",sqrt(l.real*l.real + l.img*l.img));
        here = here->next;
    }
	fprintf(f,"\n\n\n");
    fprintf(f,"CURRENTS\n");
    struct element* a = top;
    struct complex I;
    struct complex z;
    double x,y;
        if(freq->unit == '.')
            y = 2*3.14159265359*freq->value;
        else if(freq->unit == 'K')
            y = 2*3.14159265359*freq->value*1000.0;
        else if(freq->unit == 'F')
            y = 2*3.14159265359*freq->value/1000000000000000.0;
        else if(freq->unit == 'P')
            y = 2*3.14159265359*freq->value/1000000000000.0;
        else if(freq->unit == 'N')
            y = 2*3.14159265359*freq->value/1000000000.0;
        else if(freq->unit == 'U')
            y = 2*3.14159265359*freq->value/1000000.0;
        else if(freq->unit == 'M')
            y = 2*3.14159265359*freq->value/1000.0;    
    while(a != NULL)
    {
	if(a->ele != 3)
        fprintf(f,"%s ",a->elem);
        struct complex l;
        struct complex r;
        if(strcmp(name[a->left.name],"0"))
            l = solution[arr[a->left.name]];
        else
            {l.real = 0; l.img = 0;}

        if(strcmp(name[a->right.name],"0"))
            r = solution[arr[a->right.name]];
        else
            {r.real = 0; r.img = 0;}
    l = subtraction(r,l);
        if(((l.real) < 0.00000000001 && (l.real) > 0.0)||((l.real) > -0.00000000001 && (l.real) <0.0))
            l.real = 0;
        if(((l.img) < 0.00000000001 && (l.img) > 0.0)||((l.img) > -0.00000000001 && (l.img) <0.0))
            l.img = 0;
    if(a->b == 0)
    {
        if(a->unit == '.')
            x = a->value;
        else if(a->unit == 'K')
            x = a->value*1000.0;
        else if(a->unit == 'F')
            x = a->value/1000000000000000.0;
        else if(a->unit == 'P')
            x = a->value/1000000000000.0;
        else if(a->unit == 'N')
            x = a->value/1000000000.0;
        else if(a->unit == 'U')
            x = a->value/1000000.0;
        else if(a->unit == 'M')
            x = a->value/1000.0;
        if(a->ele == 0)
        {  
            z.real = x;z.img = 0.0;          
            I = division(l, z);
            fprintf(f,"%lf\n",sqrt(I.real*I.real + I.img*I.img));
        }
        else if(a->ele == 1)
        {
            z.real = 0.0;z.img = x*y;
            I = division(l, z);
            fprintf(f,"%lf\n",sqrt(I.real*I.real + I.img*I.img));
        }
        else if(a->ele == 2)
        {
            z.real = 0.0;z.img = -1/(x*y);
            I = division(l, z);
            fprintf(f,"%lf\n",sqrt(I.real*I.real + I.img*I.img));
        }
    }
//a.b != 0 ka code----------------------------------------------------------
    else
    {   
        if(a->ele == 4)
        {

        int i;
        //printf("%s ",a->elem);

        for(i = 0; i < counts;i++)
        {
            if(vname[i] == getHashIndex(a->elem))
            {

                I = solution[i + count - 1];
                fprintf(f,"%lf\n",sqrt(I.real*I.real + I.img*I.img));
            }
        }
        }
    }

        a = a->next;
    }

}
fprintf(f,"\n\n\n");
    //printf("======================================================\n");

    freq = freq->next;
    }
}

int getHashIndex(unsigned char *str)
{
    unsigned long hash = 5381;
    int c;

    while (c = *str++)
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    int dd = (int)(hash%1000);
    return (dd);
}

void set_points(int n)
{
    FILE *f = fopen(filename, "a");
    struct location_array* top = (struct location_array *) malloc(sizeof(struct location_array));
    int i;
    double r = 100.0,cx = 400.0,cy = 400.0, thita = 0.0;
    top->x = cx;
    top->y = cy + r;
    fprintf(f, "<circle fill = \"black\" stroke = \"black\" stroke-width = \"1\"\n");
    fprintf(f, "cx = \"%lf\" cy = \"%lf\" r= \"1\"/>\n", top->x, top->y);
    location = top;
    for(i = 1; i < n; i++)
    {
        struct location_array* temp = (struct location_array *) malloc(sizeof(struct location_array));
        thita += 2*3.14/n;
        temp->x = cx + r*sin(thita);
        temp->y = cy + r*cos(thita);
        fprintf(f, "<circle fill = \"black\" stroke = \"black\" stroke-width = \"1px\"\n");
        fprintf(f, "cx = \"%lf\" cy = \"%lf\" r= \"1\"/>\n", temp->x, temp->y);
        top->next = temp;
        top = temp;
    }
    fclose(f);
}

void print(struct element* a)
{
    FILE *f = fopen(filename, "a");
    double length,scale,tx,ty,rotate,inx,iny,fiy,fix,slopep;
    int i = 0;
    char c;

    while(a != NULL)
    { 
    c = a->unit;
        if(is_present[a->left.name] != 1)
        {
            net_location[a->left.name][0] = location->x;
            net_location[a->left.name][1] = location->y;
            location = location->next;
            is_present[a->left.name] = 1;
            fprintf(f, "<text x=\"%lf\" y=\"%lf\" font-size = \"10\">%s</text>", net_location[a->left.name][0], net_location[a->left.name][1], remove$(name[a->left.name]));
        }
        if(is_present[a->right.name] != 1)
        {
            net_location[a->right.name][0] = location->x;
            net_location[a->right.name][1] = location->y;
            location = location->next;
            is_present[a->right.name] = 1;
            fprintf(f, "<text x=\"%lf\" y=\"%lf\" font-size = \"10\">%s</text>", net_location[a->right.name][0], net_location[a->right.name][1], remove$(name[a->right.name]));
        }
        a->left.x = net_location[a->left.name][0];
        a->left.y = net_location[a->left.name][1];
        a->right.x = net_location[a->right.name][0];
        a->right.y = net_location[a->right.name][1];
        slopep = -atan((a->right.x - a->left.x)/(a->right.y - a->left.y));


        if(slopep >= 0.001 )
        {inx = a->left.x + 70.0*i*cos(slopep)*abs(slopep)/slopep;
        fix = a->right.x + 70.0*i*cos(slopep)*abs(slopep)/slopep;
        iny = a->left.y + 70.0*i*sin(slopep);
        fiy = a->right.y + 70.0*i*sin(slopep);}
        else
        {inx = a->left.x + 70.0*i*cos(slopep);
        fix = a->right.x + 70.0*i*cos(slopep);
        iny = a->left.y + 70.0*i*sin(slopep);
        fiy = a->right.y + 70.0*i*sin(slopep);}




        fprintf(f, "<path fill = \"black\" stroke = \"black\" stroke-width = \"1\"\n");
        fprintf(f, "d = \"M %lf %lf L %lf %lf\"/>\n",a->left.x,a->left.y, inx, iny);
        //  fprintf(f, "L %lf %lf\"/>\n", inx, iny);
        fprintf(f, "<path fill = \"black\" stroke = \"black\" stroke-width = \"1\"\n");
        fprintf(f, "d = \"M %lf %lf L %lf %lf\"/>\n",a->right.x,a->right.y,fix,fiy);
        //  fprintf(f, "L %lf %lf\"/>\n",fix,fiy);        
        fprintf(f, "<path fill = \"black\" stroke = \"black\" stroke-width = \"1\"\n");
        fprintf(f, "d = \"M %lf %lf L %lf %lf\"/>\n", inx, iny, (3*inx/4 + fix/4), (3*iny/4 + fiy/4));
        fprintf(f, "<path fill = \"black\" stroke = \"black\" stroke-width = \"1\"\n");
        fprintf(f, "d = \"M %lf %lf L %lf %lf\"/>\n", fix, fiy, (inx/4 + 3*fix/4), (iny/4 + 3*fiy/4));
        inx = (3*inx/4 + fix/4);
        iny = (3*iny/4 + fiy/4); 
        fix = (inx/4 + 3*fix/4);
        fiy = (iny/4 + 3*fiy/4); 
        if(a->ele == 0)//resistor
        {
            length = sqrt(pow((fix - inx),2) + pow((fiy - iny),2));
            scale = length/r_length;
            if(inx < fix)
            {
                tx = inx/scale - 0.0;
                ty = iny/scale - 40.0;
            }
            else
            {
                tx = fix/scale - 0.0;
                ty = fiy/scale - 40.0;  
            }
            rotate = 180*atan((fiy - iny)/(fix - inx))/3.14159;
            fprintf(f, "<polyline fill = \"none\" stroke = \"black\" stroke-width = \"1\"\n");
            fprintf(f, "points=\"0,40 10,40 15,50 25,30 35,50 45,30 55,50 65,30 75,50 85,30 90,40 100,40\"");
            fprintf(f, "\n transform = \"scale(%lf)", scale);
            fprintf(f, "translate(%lf, %lf)", tx, ty);
            fprintf(f, "rotate(%lf %lf %lf)\"\n", rotate, 0.0, 40.0);
            fprintf(f, "/>\n");
        fprintf(f, "<text x=\"%lf\" y=\"%lf\" font-size = \"10\" stroke = \"black\">%s   %g%c</text>", a->elem, (inx + fix)/2, iny + 10.0, a->value, c);    
        }
        else if(a->ele == 1)//inductor
        {
            length = sqrt(pow((fix - inx),2) + pow((fiy - iny),2));
            scale = length/i_length;
            if(inx < fix)
            {
                tx = inx/scale - 1;
                ty = iny/scale - 8.5;
            }
            else
            {
                tx = fix/scale - 1;
                ty = fiy/scale - 8.5;   
            }
            rotate = 180*atan((fiy - iny)/(fix - inx))/3.14159;
            fprintf(f, "<path fill = \"none\" stroke = \"black\" stroke-width = \"0.5\"\n");
            fprintf(f , "d=\"M 1,8.5 L 6.5,8.5 C 6.5,8.5 6.5,4.5 10.5,4.5 C 14.5,4.5 14.5,8.5 14.5,8.5 C 14.5,8.5 14.5,4.5 18.5,4.5 C 22.5,4.5 ");
            fprintf(f , "22.5,8.5 22.5,8.5 C 22.5,8.5 22.5,4.5 26.5,4.5 C 30.5,4.5 30.5,8.5 30.5,8.5 C 30.5,8.5 30.5,4.5 34.5,4.5 C 38.5,4.5 38.5,8.5 ");
            fprintf(f, "38.5,8.5 L 44,8.5\"");
            fprintf(f, "\n transform = \"scale(%lf)", scale);
            fprintf(f, "translate(%lf, %lf)", tx, ty);
            fprintf(f, "rotate(%lf %lf %lf)\"\n", rotate, 1.0, 8.5);
            fprintf(f, "/>\n");
        fprintf(f, "<text x=\"%lf\" y=\"%lf\" font-size = \"10\" stroke = \"black\">%s   %g%cH</text>", a->elem, inx + 10.0, iny + 10.0, a->value, c);
        }
        else if(a->ele == 2)//capacitor
        {
            length = sqrt(pow((fix - inx),2) + pow((fiy - iny),2));
            scale = length/c_length;
            if(inx < fix)
            {
                tx = inx/scale - 1.0;
                ty = iny/scale - 16.0;
            }
            else
            {
                tx = fix/scale - 1.0;
                ty = fiy/scale - 16.0;  
            }
            rotate = 180*atan((fiy - iny)/(fix - inx))/3.14159;
            fprintf(f, "<g transform = \"scale(%lf) translate(%lf, %lf) rotate(%lf %lf %lf)\">\n", scale, tx, ty, rotate, 1.0, 16.0);
            fprintf(f, "<path fill = \"none\" stroke = \"black\" stroke-width = \"0.5\"\n");
            fprintf(f, "d=\"M 16,10.0010577 C 16,21.999992 16,21.999992 16,21.999992\"/>");
            fprintf(f, "<path fill = \"none\" stroke = \"black\" stroke-width = \"0.5\"\n");
            fprintf(f, "d=\"M 19.000016,21.999994 L 19.000016,10.0010588\"/>");
            fprintf(f, "<path fill = \"none\" stroke = \"black\" stroke-width = \"0.5\"\n");
            fprintf(f, "d=\"M 16.11348,16 1,16\"/>");
            fprintf(f, "<path fill = \"none\" stroke = \"black\" stroke-width = \"0.5\"\n");
            fprintf(f, "d=\"M 18.980533,16 L 35,16\"");
            fprintf(f, "/>\n");
            fprintf(f, "</g>");
        fprintf(f, "<text x=\"%lf\" y=\"%lf\" font-size = \"10\" stroke = \"black\">%s   %g%cF</text>", a->elem, (inx + fix)/2, iny + 10.0, a->value, c);
        }
        else if(a->ele == 3)//current
        {
            length = sqrt(pow((fix - inx),2) + pow((fiy - iny),2));
            scale = length/a_length;
            if(inx < fix)
            {
                tx = inx/scale - 25.0;
                ty = iny/scale - 0.0;
            }
            else
            {
                tx = fix/scale - 25.0;
                ty = fiy/scale - 0.0;   
            }
            rotate = -90 + (180*atan((fiy - iny)/(fix - inx))/3.14159);
            fprintf(f, "<g transform = \"scale(%lf) translate(%lf, %lf) rotate(%lf, %lf, %lf)\">\n", scale, tx, ty, rotate, 25.0, 0.0);
            fprintf(f, "<path fill = \"none\" stroke = \"black\" stroke-width = \"1\"\n");
            fprintf(f, "d=\"M 25,35.04878 L 25,17\"/>\n");
            fprintf(f, "<path fill = \"none\" stroke = \"black\" stroke-width = \"1\"\n");
            fprintf(f, "d=\"M 25,15.5 L 27,18.5 L 23,18.5 L 25,15.5 z \"/>\n");
            fprintf(f, "<path fill = \"none\" stroke = \"black\" stroke-width = \"1\"\n");
            fprintf(f, "d=\"M 40,25 C 40,33.284271 33.284271,40 25,40 C 16.715729,40 10,33.284271 10,25 C 10,16.715729 16.715729,10 25,10 C 33.284271,10 40,16.715729 40,25 L 40,25 z \"/>\n");
            fprintf(f, "<path fill = \"none\" stroke = \"black\" stroke-width = \"1\"\n");
            fprintf(f, "d=\"M 25,10 L 25,0\"/>\n");
            fprintf(f, "<path fill = \"none\" stroke = \"black\" stroke-width = \"1\"\n");
            fprintf(f, "d=\"M 25,40 L 25,80\"/>\n");
            fprintf(f, "</g>");
        fprintf(f, "<text x=\"%lf\" y=\"%lf\" font-size = \"10\" stroke = \"red\">%s  SINE ( %g %g %g%chz %gS %g )</text>", a->elem, (inx + fiy)/2, iny + 10.0, a->offset, a->amplitude, a->frequency, c, a->delay, a->damping);
        }
        else    //battery
        {
            length = sqrt(pow((fix - inx),2) + pow((fiy - iny),2));
            scale = length/v_length;
            if(inx < fix)
            {
                tx = inx/scale - 0.0;
                ty = iny/scale - 25.0;
            }
            else
            {
                tx = fix/scale - 0.0;
                ty = fiy/scale - 25.0;  
            }
            rotate = 180*atan((fiy - iny)/(fix - inx))/3.14159;
            fprintf(f, "<g transform = \"scale(%lf) translate(%lf, %lf) rotate(%lf, %lf, %lf)\">\n", scale, tx, ty, rotate, 0.0, 25.0);
            fprintf(f, "<path fill = \"none\" stroke = \"black\" stroke-width = \"1\"\n");
            fprintf(f, "d=\"M 40,25 C 40,33.284271 33.284271,40 25,40 C 16.715729,40 10,33.284271 10,25 C 10,16.715729 16.715729,10 25,10 C 33.284271,10 40,16.715729 40,25 L 40,25 z \"/>");
            fprintf(f, "<path fill = \"none\" stroke = \"black\" stroke-width = \"1\"\n");
            fprintf(f, "d=\"M 0,25 L 10,25\"/>\n");
            fprintf(f, "<path fill = \"none\" stroke = \"black\" stroke-width = \"1\"\n");
            fprintf(f, "d=\"M 40,25 L 90,25\"/>\n");
            fprintf(f, "<path fill = \"none\" stroke = \"black\" stroke-width = \"1\"\n");
            fprintf(f, "d=\"M 16,25.000005 C 16,25.000005 17.5,20.000004 20.5,20.000004 C 23.5,20.000004 26.5,30.000006 29.5,30.000006 C 32.5,30.000006 34,25.000005 34,25.000005\"/>\n");
            fprintf(f, "</g>");
        fprintf(f, "<text x=\"%lf\" y=\"%lf\" font-size = \"10\" stroke = \"red\">%s   SINE ( %g %g %g%chz %gS %g )</text>", a->elem, (inx + fix)/2, iny + 10.0, a->offset, a->amplitude, a->frequency, c, a->delay, a->damping);
        }
        a = a->next;
        i++;
    }
    fclose(f);
}

void dfs(int i,int j)
{   
    if(visited[i][j]==1)
        return;
    else
    {   
        visited[i][j] = 1;
        visited[j][i] = 1;
        print(&g[i][j]);
        int k,flag = 0;
        for(k=0;k<1000;k++)
        {   
            
            if(g[i][k].present == 1)
            {   
                if(k != j)
                    flag = 1;
                dfs(i,k);
            }
        }
        
        if(flag == 0 && strcmp(name[i], "0"))
            printf("Circuit has open component named : %s !!\n",remove$(name[i]));

        flag = 0;

        for(k=0;k<1000;k++)
        {
            if(g[k][j].present == 1)
            {   
                if( k != i)
                    flag = 1;
                dfs(k,j);
            }
        }
         
        if(flag == 0 && strcmp(name[j] , "0"))
            printf("Circuit has open component named : %s !!\n",remove$(name[j]));

        flag = 0;
    }
}
void printcircuit()
{   
    int i,j;
    int w = 1000,h =1000;
    FILE *f = fopen(filename, "w");
    //printf("Count : %d \n",count);
    for(i=0;i<1000;i++)
        for(j=0;j<1000;j++)
            visited[i][j] = 0;

    if(name[getHashIndex("0")] == NULL)
        printf("Ground Net Not Found!! Ignoring and drawing circuit..........\n");

    fprintf(f, "<html>\n<body>\n<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\"\n");
    fprintf(f, " width=\"%dpx\" height=\"%dpx\">\n", w, h);
    fclose(f);
    set_points(count);

    
    dfs(starti,startj);

    f = fopen(filename, "a");
    fprintf(f, "</svg>\n</body>\n</html>");
    fclose(f);
    calcv();
    exit(0);
}
/*void printg()
{   
    printf("\n");
    
    int i,j,k;
    for(i=0;i<1000;i++)
        for(j=0;j<1000;j++)
        {
            if(g[i][j].present == 1)
            {
                print(g[i][j]);
                struct element* temp = g[i][j].next;
                
            }
        }

    printf("\n");
}*/
struct element set_elementR(char* yytext)
{
  struct element* temp1 = (struct element *) malloc(sizeof(struct element));
  struct element a = *temp1;
  char * pch;
  char temp[20];
  int i=-1;
  
  pch = strtok (yytext," ,-");
  while (pch != NULL)
  {
    int n = getHashIndex(pch);
    if(i== -1)
    {
        a.elem = malloc(strlen(pch)+1); strcpy(a.elem,pch);
    }
    else if(i==0)
    {
        a.left.name = n; 
        
        if(name[n] == NULL)
        {
            name[n] = malloc(strlen(pch)+1); strcpy(name[n],pch);count++;
        }
        

    }
    else if(i==1)
    {
        a.right.name = n; 
        if(name[n] == NULL)
        {
            name[n] = malloc(strlen(pch)+1); strcpy(name[n],pch);count++;
        }
    }
    else if (i==2)
    {   
        
        //printf("%d  ^^^  %s\n",strlen(pch),pch);
        int j;
        for(j=0;j<(strlen(pch) - 1);j++)
        {
            temp[j] = pch[j];
        }
        a.value = atof(temp);
        a.unit = pch[j];
    }
    i++;
    pch = strtok (NULL, " ,-");
  }
  
    a.b = 0;
    a.present = 1;
    a.next = NULL;

    if(flag == 0)
        {starti = a.left.name;startj = a.right.name; flag = 1;}

    struct element* hj = (struct element *) malloc(sizeof(struct element));
    hj->left = a.left;
    hj->right = a.right;
    hj->elem = a.elem;
    a.ele = 0;
    hj->ele = a.ele;
    hj->value = a.value;
    hj->unit = a.unit;
    hj->b = a.b;
    hj->present = a.present;	

    struct element* lk = top;
    if(lk == NULL)
    {
        hj->next = NULL;
        top = hj;
    }
    else
    {
        while(lk->next != NULL)
        {   
            lk = lk->next;
        }
        lk->next = hj;hj->next = NULL;
    }
    return a;
}
struct element set_elementRR(char* yytext)
{
    struct element* temp1 = (struct element *) malloc(sizeof(struct element));
  struct element a = *temp1;
  char * pch;
  char temp[20];
  int i=-1;
  
  pch = strtok (yytext," ,-");
  while (pch != NULL)
  {
    int n = getHashIndex(pch);
    if(i== -1)
    {
        a.elem = malloc(strlen(pch)+1); strcpy(a.elem,pch);
    }
    else if(i==0)
    {
        a.left.name = n; 
        
        if(name[n] == NULL)
        {
            name[n] = malloc(strlen(pch)+1); strcpy(name[n],pch);count++;
        }
        

    }
    else if(i==1)
    {
        a.right.name = n; 
        if(name[n] == NULL)
        {
            name[n] = malloc(strlen(pch)+1); strcpy(name[n],pch);count++;
        }
    }
    else if (i==2)
    {   
        
        //printf("%d  ^^^  %s\n",strlen(pch),pch);
        int j;
        for(j=0;j<(strlen(pch));j++)
        {
            temp[j] = pch[j];
        }
        a.value = atof(temp);
        a.unit = '.';
    }
    i++;
    pch = strtok (NULL, " ,-");
  }
  
    a.b = 0;
    a.present = 1;
    a.next = NULL;

    if(flag == 0)
        {starti = a.left.name;startj = a.right.name; flag = 1;}


    struct element* hj = (struct element *) malloc(sizeof(struct element));
    hj->left = a.left;
    hj->right = a.right;
    hj->elem = a.elem;
    a.ele = 0;
    hj->ele = a.ele;
    hj->value = a.value;
    hj->unit = a.unit;
    hj->b = a.b;
    hj->present = a.present;
    struct element* lk = top;
    if(lk == NULL)
    {
        hj->next = NULL;
        top = hj;
    }
    else
    {
        while(lk->next != NULL)
        {   
            lk = lk->next;
        }
        lk->next = hj;hj->next = NULL;
    }
    return a;
}
struct element set_elementRRR(char* yytext)
{
  struct element* temp1 = (struct element *) malloc(sizeof(struct element));
  struct element a = *temp1;
  char * pch;
  char temp[20];
  int i=-1;
  
  pch = strtok (yytext," ,-");
  while (pch != NULL)
  {
    int n = getHashIndex(pch);
    if(i== -1)
    {
        a.elem = malloc(strlen(pch)+1); strcpy(a.elem,pch);
    }
    else if(i==0)
    {
        a.left.name = n; 
        
        if(name[n] == NULL)
        {
            name[n] = malloc(strlen(pch)+1); strcpy(name[n],pch);count++;
        }
        

    }
    else if(i==1)
    {
        a.right.name = n; 
        if(name[n] == NULL)
        {
            name[n] = malloc(strlen(pch)+1); strcpy(name[n],pch);count++;
        }
    }
    else if (i==2)
    {   
        
        //printf("%d  ^^^  %s\n",strlen(pch),pch);
        int j;
        for(j=0;j<(strlen(pch) - 3);j++)
        {
            temp[j] = pch[j];
        }
        a.value = atof(temp);
        a.meg = 1;
    }
    i++;
    pch = strtok (NULL, " ,-");
  }
  
    a.b = 0;
    a.present = 1;
    a.next = NULL;

    if(flag == 0)
        {starti = a.left.name;startj = a.right.name; flag = 1;}

    struct element* hj = (struct element *) malloc(sizeof(struct element));
    hj->left = a.left;
    hj->right = a.right;
    hj->elem = a.elem;
    a.ele = 0;
    hj->ele = a.ele;
    hj->value = a.value;
    hj->unit = a.unit;
    hj->b = a.b;
    hj->present = a.present;
    struct element* lk = top;
    if(lk == NULL)
    {
        hj->next = NULL;
        top = hj;
    }
    else
    {
        while(lk->next != NULL)
        {   
            lk = lk->next;
        }
        lk->next = hj;hj->next = NULL;
    }

    return a;
}
struct element set_elementLC(char* yytext)
{

  struct element* temp1 = (struct element *) malloc(sizeof(struct element));
  struct element a = *temp1;
  char * pch;
  char temp[20];
  int i=-1;
  
  pch = strtok (yytext," ,-");
  while (pch != NULL)
  {
    int n = getHashIndex(pch);
    if(i== -1)
    {
        a.elem = malloc(strlen(pch)+1); strcpy(a.elem,pch);
    }
    else if(i==0)
    {
        a.left.name = n; 
        if(name[n] == NULL)
        {
            name[n] = malloc(strlen(pch)+1); strcpy(name[n],pch);count++;
        }
    }
    else if(i==1)
    {
        a.right.name = n; 
        if(name[n] == NULL)
        {
            name[n] = malloc(strlen(pch)+1); strcpy(name[n],pch);count++;
        }
    }
    else if (i==2)
    {   
        
        //printf("%d  ^^^  %s\n",strlen(pch),pch);
        int j;
        for(j=0;j<(strlen(pch) - 2);j++)
        {
            temp[j] = pch[j];
        }
        a.value = atof(temp);
        a.unit = pch[j];
	if(pch[j+1] == 'F')
		a.ele = 2;
	else
		a.ele = 1;
    }
    i++;
    pch = strtok (NULL, " ,-");
  }
  
    a.b = 0;
    a.present = 1;
    a.next = NULL;
    if(flag == 0)
        {starti = a.left.name;startj = a.right.name; flag = 1;}

    struct element* hj = (struct element *) malloc(sizeof(struct element));
    hj->left = a.left;
    hj->right = a.right;
    hj->elem = a.elem;
    hj->ele = a.ele;
    hj->value = a.value;
    hj->unit = a.unit;
    hj->b = a.b;
    hj->present = a.present;
    struct element* lk = top;
    if(lk == NULL)
    {
        hj->next = NULL;
        top = hj;
    }
    else
    {
        while(lk->next != NULL)
        {   
            lk = lk->next;
        }
        lk->next = hj;hj->next = NULL;
    }
    return a;
}
struct element set_elementIV(char* yytext)
{

 
  struct element* temp1 = (struct element *) malloc(sizeof(struct element));
  struct element a = *temp1;
  char * pch;
  
  int i=-1;
  
  pch = strtok (yytext," ,-");
  while (pch != NULL)
  {
    int n = getHashIndex(pch);
    if(i== -1)
    {
        a.elem = malloc(strlen(pch)+1); strcpy(a.elem,pch);
        if(a.elem[0] == 'V')
            {counts++;a.ele = 4;}
        else
            a.ele = 3;

    }
    else if(i==0)
    {
        a.left.name = n; 
        if(name[n] == NULL)
        {
            name[n] = malloc(strlen(pch)+1); strcpy(name[n],pch);count++;
        }
    }
    else if(i==1)
    {
        a.right.name = n; 
        if(name[n] == NULL)
        {
            name[n] = malloc(strlen(pch)+1); strcpy(name[n],pch);count++;
        }
    }

    else if (i==4)
    {   
        
        //printf("%d  ^^^  %s\n",strlen(pch),pch);
        int j;char temp[20];
        for(j=0;j<(strlen(pch) );j++)
        {
            temp[j] = pch[j];
        }
        a.offset = atof(temp);
	if(a.offset != 0)
		dc = 1;
        for(j=0;j<20;j++)
        temp[j] = '\0';
        
    }
    else if (i==5)
    {   
        
        //printf("%d  ^^^  %s\n",strlen(pch),pch);
        int j;char temp[20];
        for(j=0;j<(strlen(pch) );j++)
        {
            temp[j] = pch[j];
        }
        a.amplitude = atof(temp);
        for(j=0;j<20;j++)
        temp[j] = '\0';
        
    }
    else if (i==6)
    {   
        
        //printf("%d  ^^^  %s\n",strlen(pch),pch);
        int j;char temp[20];
        for(j=0;j<(strlen(pch) -3 );j++)
        {
            temp[j] = pch[j];
        }
        a.frequency = atof(temp);
        a.unit = pch[j];


        for(j=0;j<20;j++)
        temp[j] = '\0';



        
    }
    else if (i==7)
    {   
        
        //printf("%d  ^^^  %s\n",strlen(pch),pch);
        int j;char temp[20];
        
        for(j=0;j<(strlen(pch) -1);j++)
        {
            temp[j] = pch[j];
        }
        
        a.delay = atof(temp);

        for(j=0;j<20;j++)
        temp[j] = '\0';
        
    }
    else if (i==8)
    {   
        
        //printf("%d  ^^^  %s\n",strlen(pch),pch);
        int j;char temp[20];
        if(pch != NULL) 
        for(j=0;j<(strlen(pch) );j++)
        {
            temp[j] = pch[j];
        }

        if(temp != NULL)
        a.damping = atof(temp);

        for(j=0;j<20;j++)
        temp[j] = '\0';
        
    }
    i++;
    pch = strtok (NULL, " ,-");
  }
  
    a.b = 1;
    a.present = 1;
  

    a.next = NULL;

    if(flag == 0)
        {starti = a.left.name;startj = a.right.name; flag = 1;}

        struct frequency* fg = (struct frequency*) malloc(sizeof(struct frequency));
        fg->value = a.frequency;
        fg->unit = a.unit;

        struct frequency* temp11 = listfreq;
        int flag = 0;
        while(temp11 != NULL)
            {   
                if(temp11->value == fg->value && temp11->unit == fg->unit)
                    flag = 1;
                temp11 = temp11->next;
            }

            if(flag == 0)
            {
                fg->next = listfreq;
                listfreq  = fg;

               // printf("%lf ============\n",listfreq->value);
            }

    struct element* hj = (struct element *) malloc(sizeof(struct element));
    hj->left = a.left;
    hj->right = a.right;
    hj->elem = a.elem;
    hj->ele = a.ele;
    hj->unit = a.unit;
    hj->frequency = a.frequency;
    hj->b = a.b;
    hj->present = a.present;
    struct element* lk = top;
    if(lk == NULL)
    {
        hj->next = NULL;
        top = hj;
    }
    else
    {
        while(lk->next != NULL)
        {   
            lk = lk->next;
        }
        lk->next = hj;hj->next = NULL;
    }

    return a;
}

