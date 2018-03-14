#include<stdio.h>
#include<math.h>
#define LAY0_CNT 2             /*输入层节点数*/
#define LEARN_CNT 20           /*学习实例数*/
#define LAY1_CNT 3             /*第一隐层数，后类推*/
#define LAY2_CNT 3
#define LAY3_CNT 2
#define LAY_OUT_CNT 2          /*输出层节点数*/


void matrix_input(float *p,int n1,int n2,int I1,int I2,int ct)
{
    int i;
    if(ct==1)
        printf("--------------------------------------------\n");
    if(ct==1&&I2==0)
        printf("* Please input the exampls of the import.  *\n");
    else if(ct==1&&I1==0&&I2==1)
    {
        printf("* Please input the propertion betwen       *\n");
        printf("* the importing layer and layer %3d.       *\n",I2);
    }
    else if(ct==1&&I1!=0&&I2==I1+1)
    {
        printf("* Please input the propertion betwen the   *\n");
        printf("* layer %3d and layer %3d.                 *\n",I1,I2);
    }
    if(ct==1)
    {
        printf("* Please input the exampls of the export.  *\n");
        printf("* The data you should is a matrix like:    *\n");
        printf("* %3dX%-3d,now input:                       *\n",n1,n2);
        printf("--------------------------------------------\n\n");
    }
    for(i=0;i<n1*n2;i++)
    {
        if(ct==2) *(p+i)=0.1;
        else if(ct==1) scanf("%f",p+i);
    }

}  /*输入函数，输入权值或实例*/

copy_matrix(float *p1,float *p2,int n1,int n2)
{
    int i;
    for(i=0;i<n1*n2;i++)
        *(p2+i)=*(p1+i);
}


void bp_translate_learn_layer(float *wight,float *value_front,
                    float *value_behind,int count_front,
                    int count_behind,int func_idx)
{
    int i,j;
    float *po=wight,t;
    for(i=0;i<count_behind;i++,po=wight+i)
    {
        for(j=0,t=0;j<count_front;j++,po=po+count_behind)
        t=t+(*po)*(*(value_front+j));
        switch(func_idx)
        {
            case 1:*(value_behind+i)=1/(1+exp(-t));break;
            case 2:*(value_behind+i)=(exp(t)-exp(-t))/(exp(t)+exp(-t));break;
            case 3:*(value_behind+i)=atan(t);break;
            default:printf("--------------------------------------------\n");
                    printf("* parameter error:    tranlea.             *\n");
                    printf("--------------------------------------------\n\n");
                    break;
        }
    }    /*学习函数*/
}

void modify_wight(float *err_front,float *err_bihind,int n1,int n2,float eta,float *p,float *p1,int ct)
{
    int i,j;
    float t,*po=p,f1;
    for(i=0;i<n1;i++,po=p+i*n2)
    {
        for(j=0,t=0;j<n2;j++,po=po+1)
        {
            t=t+(*po)*(*(err_bihind+j));
            *po=*po-eta*(*(err_bihind+j))*(*(p1+i));
        }
        switch(ct)
        {
            case 1:f1=*(p1+i)*(1-*(p1+i));break;
            case 2:f1=1-(*(p1+i))*(*(p1+i));break;
            case 3:f1=1/(1+(*(p1+i))*(*(p1+i)));break;
            default:printf("--------------------------------------------\n");
                    printf("* parameter error:   tranerr.              *\n");
                    printf("--------------------------------------------\n\n");
                    break;
        }
        *(err_front+i)=f1*t;
    }
}    /*误差反传并修改权值*/


void print_propertion(float *p,int n1,int n2,int I1,int I2)
{
    int i,j;
    float *po=p;

    printf("--------------------------------------------\n");
    if(I1==0) 
    {
        printf("* The propertion betwen the importing      *\n");
        printf("* layer and layer 1 is:                    *\n");
    }
    else   
    {
        printf("* The propertion betwen the layer %3d      *\n",I1);
        printf("* and layer %3d is:                        *\n",I2);
    }
    printf("--------------------------------------------\n\n");
    for(i=0;i<n1;i++,po=p+i*n2)
    {
        for(j=0;j<n2;j++,po=po+1)
        printf("%f\t\n",*po);
    }
}                                      /*输出权值矩阵*/

void print_array(float *p,int n,int I)
{
    int i,k;
    printf("--------------------------------------------\n");
    if(I==0)
        printf("*The error of the importing layer is:      *\n");
    else if(I>0&&I<10)
        printf("*The error of the  layer %3d is:           *\n",I);
    else if(I==10)
        printf("*The import of the importing layer is:     *\n");
    else if(I>10&&I<20)
        printf("*The import  of the  layer %3d is:         *\n",I-10);
    else if(I==20)
        printf("*The export of the importing layer is:     *\n");
    else
        printf("*The export  of the  layer %3d is:         *\n",I-20);
        printf("--------------------------------------------\n\n");
    for(i=0,k=1;i<n;i++,k=(k+1)%5)
    {
        printf("%f   ",*(p+i));
        if(k==0) printf("\n");
    }
    printf("\n");

}    /*输出各层误差*/

void main()
{
    float lay0[LAY0_CNT];
    float learn_factor=0.5;
    float error_limit=0.0001;
    float example_in[LEARN_CNT][LAY0_CNT];
    float source_example_in[LEARN_CNT][LAY0_CNT];
    float example_out[LEARN_CNT][LAY_OUT_CNT];
    
    float error_out[LAY_OUT_CNT];
    float source_example_out[LEARN_CNT][LAY_OUT_CNT];
    float lay_out[LAY_OUT_CNT];
    float error=100;
    float error_lay0[LAY0_CNT];       
    int i,j,k,l,order,LAN/*层数*/,ctr=0,y[LAY_OUT_CNT],func_idx; 
    long nl=0,NL=0/*学习遍数*/;
                 /*完成层与层间权值初始化*/
    float lay1[LAY1_CNT],w01[LAY0_CNT][LAY1_CNT],sw01[LAY0_CNT][LAY1_CNT],ER1[LAY1_CNT];
    float lay2[LAY2_CNT],w12[LAY1_CNT][LAY2_CNT],sw12[LAY1_CNT][LAY2_CNT],ER2[LAY2_CNT];
    float lay3[LAY3_CNT],w23[LAY2_CNT][LAY3_CNT],sw23[LAY2_CNT][LAY3_CNT],ER3[LAY3_CNT];
    /*设定各层及层间初始值完成*/

    printf("--------------------------------------------\n");
    printf("* This program is to simulate the net      *\n");
    printf("* of human's nerve.You should give the     *\n");
    printf("* examples of how to import and export.    *\n");
    printf("* It can learn the relation between the    *\n");
    printf("* import and export.Then,you can give      *\n");
    printf("* another import,it can give the           *\n");
    printf("* conclusion you need.                     *\n"); 
    printf("--------------------------------------------\n\n");
    /*如果需要，可以修改初始值*/

    LBI:
    printf("--------------------------------------------\n");
    printf("* reset the propertion of the learning?    *\n");
    printf("* <1>YES            <2>LAY_OUT_CNT                  *\n");
    printf("--------------------------------------------\n");
    scanf("%d",&order);
    if(order==1)
    {
        printf("--------------------------------------------\n");
        printf("* input the propertion of the learning:    *\n");
        printf("--------------------------------------------\n\n");
        scanf("%f",&learn_factor); 
    }       /*输入学习率*/

        printf("--------------------------------------------\n");
        printf("* reset the propertion between layers?     *\n");
        printf("* <1>YES <2>LAY_OUT_CNT  <3>keep them               *\n");
        printf("--------------------------------------------\n");
        scanf("%d",&order);

        matrix_input(*sw01,LAY0_CNT,LAY1_CNT,0,1,order);
        matrix_input(*sw12,LAY1_CNT,LAY2_CNT,1,2,order);
        matrix_input(*sw23,LAY2_CNT,LAY3_CNT,2,3,order);
        /*修改初始值完成*/
        if(order==3||order!=2)
            copy_matrix(*sw01,*w01,LAY0_CNT,LAY1_CNT);
            copy_matrix(*sw12,*w12,LAY1_CNT,LAY2_CNT);
            copy_matrix(*sw23,*w23,LAY2_CNT,LAY3_CNT);



    printf("--------------------------------------------\n");
    printf("* reset the examples?                      *\n");
    printf("* <1>YES   <2>LAY_OUT_CNT  <3>keep them             *\n");
    printf("--------------------------------------------\n");
    scanf("%d",&order);
                                               LAY:
    matrix_input(*source_example_in,LEARN_CNT,LAY0_CNT,0,0,order);
    matrix_input(*source_example_out,LEARN_CNT,LAY_OUT_CNT,9,9,order);
    copy_matrix(*source_example_in,*example_in,LEARN_CNT,LAY0_CNT);
    copy_matrix(*source_example_out,*example_out,LEARN_CNT,LAY_OUT_CNT);/*  输入输出初始化完成*/

    printf("--------------------------------------------\n");
    printf("* reset the maximum error?                 *\n");
    printf("* <1>YES            <2>LAY_OUT_CNT                  *\n");
    printf("--------------------------------------------\n");
    scanf("%d",&order);
    if(order==1)
    {
        printf("--------------------------------------------\n");
        printf("* input the maximum error:                 *\n");
        printf("--------------------------------------------\n\n");
        scanf("%f",&error_limit);} 

        printf("--------------------------------------------\n");
        printf("* Please choose the function about the     *\n");
        printf("* transporting of signal betwwen the layers*\n");
        printf("* <1>Y=1/(1+exp(x))                        *\n");
        printf("* <2>Y=th(X)                               *\n");
        printf("* <3>Y=arctan(X)                           *\n");
        printf("--------------------------------------------\n\n");
        LFUN:  scanf("%d",&func_idx);

        printf("--------------------------------------------\n");
        printf("* The propertion of the learning is   :    *\n");
        printf("--------------------------------------------\n\n");
        printf("%f\n",learn_factor);
        printf("--------------------------------------------\n");
        printf("* The maximum error is:                    *\n");
        printf("--------------------------------------------\n\n");
        printf("%f\n",error_limit);

          
        while(error>error_limit)
        { 
            NL=NL+1; /*记录学习遍数*/
            printf("*********************The learning time is %ld\n",NL);
            for(k=0;k<LEARN_CNT;k++)
            {                                 
                for(j=0;j<LAY0_CNT;j++) 
                    lay0[j]=example_in[k][j];/*调入一组实例*/     
                nl=nl+1;               /*记录学习总例数*/
L1:  
            LAN=1;
            bp_translate_learn_layer(*w01,lay0,lay1,LAY0_CNT,LAY1_CNT,func_idx);
                               
            LAN=2;
            bp_translate_learn_layer(*w12,lay1,lay2,LAY1_CNT,LAY2_CNT,func_idx);
       LAN=3;
        bp_translate_learn_layer(*w23,lay2,lay3,LAY2_CNT,LAY3_CNT,func_idx);

                 /*完成一次学习*/
















                                                        
switch(LAN)
{
    case 3:
        for(i=0;i<LAY3_CNT;i++)
        {
            lay_out[i]=lay3[i];
            ER3[i]=lay3[i]-example_out[k][i];/*输出层误差*/
            error_out[i]=ER3[i];
        }
        break;
    case 2:
        for(i=0;i<LAY2_CNT;i++)
        {
            lay_out[i]=lay2[i];
            ER2[i]=lay2[i]-example_out[k][i];/*输出层误差*/
            error_out[i]=ER2[i];
        }
        break;
    default:break;
}


                                                        

                                        

if(ctr==1) goto L2;
for(l=1,error=error_out[0];l<LAY_OUT_CNT;l++)
{
if(error<0.000001&&error>error_out[i]) error=error_out[i];
else if(error>0.000001&&error<error_out[i]) error=error_out[i];}

if(error<0.000001) error=-error;                                             
if(error<=error_limit) goto L3; /*误差精度已满足，学习结束*/





switch(LAN)
{
    /*误差反传*/
    case 3:
        modify_wight(ER2,ER3,LAY2_CNT,LAY3_CNT,learn_factor,*w23,lay2,func_idx);/*修改第2到3层权值*/
    case 2: 
        modify_wight(ER1,ER2,LAY1_CNT,LAY2_CNT,learn_factor,*w12,lay1,func_idx);/*修改第1到2层权值*/
    case 1: 
        modify_wight(error_lay0,ER1,LAY0_CNT,LAY1_CNT,learn_factor,*w01,lay0,func_idx);/*修改输入层到第1层权值*/
    break;
    /*完成一次误差反传*/
    default:break;
}
                                                                                                                                                     
}  /*第一遍学习完成*/

}                                                                                   


/*学习过程已完成*/

L3:     
if(ctr==0);
{
    printf("--------------------------------------------\n");
    printf("* The learning procession is over.         *\n");
    printf("--------------------------------------------\n\n");
    printf("--------------------------------------------\n");
    printf("* The total times of learning is :         *\n");
    printf("--------------------------------------------\n\n");
    printf(" %d\n",nl);
    printf("--------------------------------------------\n");
    printf("* The times learning all examples is :     *\n");
    printf(" %d\n",NL);
    printf("--------------------------------------------\n\n");}
    ctr=1;

L4: 
    printf("--------------------------------------------\n");    
    printf("* Now choose the other options:            *\n");
    printf("* <1>check the systom                      *\n");
    printf("* <2>output the propertion                 *\n");
    printf("* <3>output the error of each layer        *\n");
    printf("* <4>output the final export of each layer *\n");
    printf("* <5>learn it again                        *\n");
    printf("* <6>come to the end                       *\n");
    printf("* <7>help                                  *\n");
    printf("--------------------------------------------\n\n");
    scanf("%d",&order);

switch(order)
{
case 1:
       printf("--------------------------------------------\n");
       printf("* Please input the exampls of the import:  *\n");
       printf("--------------------------------------------\n\n");
       for(i=0;i<LAY0_CNT;i++)
       scanf("%f",&lay0[i]);
       goto L1;


L2:    
printf("--------------------------------------------\n");
printf("* The export of the check is:              *\n");
printf("--------------------------------------------\n\n");
       for(i=0;i<LAY_OUT_CNT;i++)
       printf("%f\t",lay_out[i]);
      printf("\n");
printf("--------------------------------------------\n");       
printf("* Do you want to check the judgement?      *\n");
printf("* <1>YES        <2>LAY_OUT_CNT                      *\n" );
printf("--------------------------------------------\n\n");
       scanf("%d",&order);
       if(order==1)
	   {   printf("--------------------------------------------\n");
       for(i=0;i<LAY_OUT_CNT;i++)
	   {
		y[i]=(int)((lay_out[i]+0.05)*10);
       switch(y[i])
	   {
       case 10:printf("* The example imported  is                 *\n");
               printf("* sure to belong to R%-d                   *\n",i+1);break;
       case 9: printf("* The example imported  is                 *\n");
               printf("* nearly to belong to R%-d                 *\n",i+1);break;
       case 8: printf("* The example imported  is                 *\n");
               printf("* infinitely to belong to R%-d             *\n",i+1);break;
       case 7: printf("* The example imported  is                 *\n");
               printf("* very likely to belong to R%-d            *\n",i+1);break;
       case 6: printf("* The example imported  is                 *\n");
               printf("fairly to belong to R%-d                   *\n",i+1);break;
       case 5: printf("* The example imported  is                 *\n");
               printf("* almost to belong to R%-d                 *\n",i+1);break;
       case 4: printf("* The example imported  is                 *\n");
               printf("* relatively to belong to R%-d             *\n",i+1);break;
       case 3: printf("* The example imported  is                 *\n");
               printf("* somewhat to belong to R%-d               *\n",i+1);break;
       case 2: printf("* The example imported  is                 *\n ");
               printf("* a little to belong to R%-d               *\n",i+1);break;
       case 1: printf("* The example imported  is                 *\n");
               printf("* slightly to belong to R%-d               *\n",i+1);break;
       case 0: printf("* The example imported  is                 *\n");
               printf("* of course not to belong to R%-d          *\n",i+1);break;
       default:printf("* The system can't give the                *\n");
               printf("* judgement whether the example imported   *\n");
               printf("* belong to R%-d                           *\n",i+1 );
	   }
	   }
               printf("--------------------------------------------\n\n");
	   }goto L4;

case 2:
#if LAY1_CNT  
print_propertion(*w01,LAY0_CNT,LAY1_CNT,0,1);
#endif 
#if LAY2_CNT  
print_propertion(*w12,LAY1_CNT,LAY2_CNT,1,2);
#endif 
#if LAY3_CNT  
print_propertion(*w23,LAY2_CNT,LAY3_CNT,2,3);
#endif 

goto L4;


case 3: print_array(error_lay0,LAY0_CNT,0);
#if LAY1_CNT  
print_array(ER1,LAY1_CNT,1);
#endif 
#if LAY2_CNT  
print_array(ER2,LAY2_CNT,2);
#endif 
#if LAY3_CNT  
print_array(ER3,LAY3_CNT,3);
#endif 

goto L4;

case 4: print_array(lay0,LAY0_CNT,20);
#if LAY1_CNT  
print_array(lay1,LAY1_CNT,11);
#endif
#if LAY2_CNT  
print_array(lay2,LAY2_CNT,22);
#endif
#if LAY3_CNT  
print_array(lay3,LAY3_CNT,23);
#endif

goto L4;


case 5:NL=0;
       nl=0;
       ctr=0;
       error=100;
      goto LBI;


L5:
case 6:
printf("--------------------------------------------\n");
printf("* The   program is over.                   *\n\n");
printf("--------------------------------------------\n\n");break;

case 7:
printf("--------------------------------------------\n");    
printf("*   input 1: You can input an example,then *\n");
printf("* the system give you the export and judge.*\n");
printf("*                                          *\n");
printf("*   input 2: You can see the propertions   *\n");
printf("* between every twe layers.                *\n");
printf("*                                          *\n");
printf("*   input 3: You can see the final errors  *\n");
printf("* of every layer.                          *\n");
printf("*                                          *\n");
printf("*    input 4: You can see the final imports*\n");
printf("* of every layer.                          *\n");
printf("*                                          *\n");
printf("*   input 5: You can  run the program from *\n");
printf("* the very beginning.                      *\n");
printf("*                                          *\n");
printf("* ~~input 6: The program will be over.     *\n");
printf("* Now input:                               *\n");
printf("--------------------------------------------\n\n");
goto L4;


default:
printf("--------------------------------------------\n\n");
printf("* Order error.                             *\n");
printf("--------------------------------------------\n\n");
break;
 }       
}























































