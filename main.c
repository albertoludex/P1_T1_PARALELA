#include <stdlib.h>
#include <stdio.h>
#include<string.h>
#include <math.h>
#include<time.h>
#include <mpi.h>



//fUNCIÓN QUE COMPRUEBA LOS PARAMETROS

int compara_argumentos(int argc, char* argv[]){
    FILE *f= fopen(argv[1],"r");
    if(argc!=5){
        
        return 1; //4 parametros
    }
    if(f==NULL){
        return 1;//comprobar que el argv1 es el fichero de la imagen que queremos que nos convierta
    }
    fclose(f);
    return 0; //TODO OK
}

int option(char* opt){
    if(strcmp(opt,"media")==0){
        return 1;

    }
    if(strcmp(opt,"mediana")==0){
        return 2;
    }
    if(strcmp(opt,"sobel")==0)
    return 3;

}

unsigned char** matriz_creacion(int ancho, int largo){
    int i;
    unsigned char** matriz = (unsigned char**) malloc (largo * sizeof(unsigned char*)); //array para el largo
    for(i=0;i<largo;i++){
        matriz[i]= (unsigned char*) malloc (ancho * sizeof(unsigned char)); //por cada largo, se reserva un ancho
    }
    
    return matriz;
    
}

void matriz_load(char* fichero, int ancho, int largo, unsigned char** matriz){
    FILE *f = fopen (fichero, "rb");	
    unsigned char lin[ancho];
	int i,j;
	
	for(i=1;i<=largo;i++){
        
        fread(lin, sizeof(lin), 1, f);
        for(j=0;j<ancho;j++){
          
            matriz[i][j+1]= lin[j];            
        }
        
    }
    
    fclose(f);
   
    for(i=0;i<ancho;i++){
        matriz[0][i]=matriz[2][i];
        matriz[largo+1][i]=matriz[largo-1][i];
    }
    for(i=1;i<largo;i++){
        matriz[i][0]=matriz[i][2];
        matriz[i][ancho+1]=matriz[i][ancho+1];
    }
    matriz[0][0]=matriz[2][2];
    matriz[0][ancho+1]=matriz[2][ancho-1];
    matriz[largo+1][0]=matriz[largo-1][2];
    matriz[largo+1][ancho+1]=matriz[largo-1][ancho-1];   

}




unsigned char media (unsigned char ** matriz, int x, int y){
    int media=0;
    unsigned char results;
    //esquina sup iz
    media+=  matriz[x-1][y-1] ;
    //mitad superior
    media += matriz[x][y-1];
    //esquina sup derecha
    media+= matriz[x+1][y-1] ;
    //mitad izquierda
    media+= matriz[x-1][y] ;
     //elemento
    media+= matriz[x][y] ;
    //mitad derechaecha
    media+= matriz[x+1][y];
    //esquina inf izquierda
    media+= matriz[x-1][y+1] ;
    //centro inf
    media+= matriz[x][y+1] ;
    //esq inf derecha
    media+= matriz[x+1][y-1] ;
    results=media/9;
    return results;
}

void media_calculate(unsigned char** matriz, unsigned char** matriz_2, int ancho, int largo){
    int i,j;

    for(i=0;i<largo;i++){ //Recorre la matriz por filas ignorando la primera y la ultima.
        for(j=0;j<ancho;j++){ //Recorre cada fila elemento a elemento.
                matriz_2[i][j]=media(matriz,i+1,j+1);
        }
    }

}

void qs(unsigned char list[],int limite_izquierda,int limite_derecha){

    int izquierda,derecha;
    unsigned char temp,pivote;
    izquierda=limite_izquierda;
    derecha = limite_derecha;
    pivote = list[(izquierda+derecha)/2];
 
    do{
        while(list[izquierda]<pivote && izquierda<limite_derecha)izquierda++;
        while(pivote<list[derecha] && derecha > limite_izquierda)derecha--;
        if(izquierda <=derecha)
        {

            temp= list[izquierda];
            list[izquierda]=list[derecha];
            list[derecha]=temp;
            izquierda++;
            derecha--;


        }
 
    }while(izquierda<=derecha);
    if(limite_izquierda<derecha){qs(list,limite_izquierda,derecha);}
    if(limite_derecha>izquierda){qs(list,izquierda,limite_derecha);}

 
}

unsigned char mediana(unsigned char ** matriz,int x, int y){
    unsigned char elementos[9];

    //Se almacenan los 9 elementos en un array para llamar a qs
    //esquina sup iz
    elementos[0]= matriz[x-1][y-1];
    //mitad superior
    elementos[1]= matriz[x][y-1];
    //esquina sup derecha
    elementos[2]= matriz[x+1][y-1];
    //mitad izquierda
    elementos[3]= matriz[x-1][y];
     //elemento
    elementos[4]= matriz[x][y];
    //mitad derechaecha
    elementos[5]= matriz[x+1][y];
    //esquina inf izquierda
    elementos[6]= matriz[x-1][y+1];
    //centro inf
    elementos[7]= matriz[x][y+1];
    //esq inf derecha
    elementos[8]= matriz[x+1][y-1];

    qs(elementos,0,8);//qs organiza los elementos del array

    return elementos[4];//cogemos el elemento de en medio
   

}

void mediana_calculate(unsigned char** matriz,unsigned char** matriz_2, int ancho, int largo){

    int i,j;

    for(i=0;i<largo;i++){//Recorre la matriz por filas ignorando la primera y la ultima.
        for(j=0;j<ancho;j++){//Recorre cada fila elemento a elemento.
                matriz_2[i][j]=mediana(matriz,i+1,j+1);

        }
    }   

}




unsigned char sobel(unsigned char** matriz,int x, int y){
     int c=0;
     int f=0;
     unsigned char j;
     //esquina sup iz
    f= matriz[x-1][y-1]*-1;
    c= matriz[x-1][y-1]*-1;
    //mitad superior
    f= f+ matriz[x][y-1]*0;
    c= c+ matriz[x][y-1]*-2;
    //esquina sup derecha
    f= f+ matriz[x+1][y-1]*1;
    c= c+ matriz[x+1][y-1]*-1;
    //mitad izquierda
    f= f+ matriz[x-1][y]*-2;
    c= c+ matriz[x-1][y]*0;
     //elemento
    f= f+ matriz[x][y]*0;
    c= c+ matriz[x][y]*0;
    //mitad derechaecha
    f= f+ matriz[x+1][y]*2;
    c= c+ matriz[x+1][y]*0;
    //esquina inf izquierda
    f= f+ matriz[x-1][y+1]*-1;
    c= c+ matriz[x-1][y+1]*1;
    //centro inf
    f= f+ matriz[x][y+1]*0;
    c= c+ matriz[x][y+1]*2;
    //esq inf derecha
    f= f+ matriz[x+1][y-1]*1;
    c= c+ matriz[x+1][y-1]*1;

    c= pow(c,2);
    f= pow(f,2);

    j=sqrt(c+f);

    return j;

}


void sobel_calculate (unsigned char** matriz, unsigned char** matriz_2, int ancho, int largo){
    int i,j;

    for(i=0;i<largo;i++){//Recorre la matriz por filas ignorando la primera y la ultima.
        for(j=0;j<ancho;j++){//Recorre cada fila elemento a elemento.
                matriz_2[i][j]=sobel(matriz,i+1,j+1);
        }
    }
}






int comp_fichero (char *fichero){
    FILE *f= fopen(fichero,"r");
    if (f==NULL){
        return 0;
        //Si no es un fichero devuelve 0 
    }
    fclose(f);
    return 1;
        //Si es un fichero devuelve 1
}
//Funcion que guarda la matriz resultante en un nuevo fihero raw
void img_create_raw (unsigned char** matriz, char* fichero,int ancho,int largo){
	int i,j;
    char name[50];
	FILE *f = fopen (fichero,"wb");
	if (f==NULL){ //Comprueba si el fichero existe
		printf("Se ha tenido un error cuando hemos querido crear el fichero");
		exit;
	}
	
	for (j = 0; j < largo; j++){
		fwrite(matriz[j],sizeof(unsigned char),ancho,f);
	}
	
	fclose(f);
   
}
//Fucion que imprime los resultss
void print_results(int ancho, int largo, char* fichero, int filtro, double temp){
    char name[50];
    int cont=0;
    FILE *f;
    int i;



    
    sprintf(name,"Results_%dx%d_%d.txt",ancho,largo, filtro);
     while(comp_fichero(name)==1){
        cont++;
        sprintf(name,"Results_%dx%d(%d).txt",ancho, largo,cont);
    }
   

    f=fopen(name,"w");
    fprintf(f,"Parametros de ejecucion \nancho: %d\n",ancho);
    fprintf(f,"largo: %d\n\n",largo);

    fprintf(f,"entrada: %s\n",fichero);
   
    

    if(filtro==1){
        fprintf(f,"filtro: media\n");
        fprintf(f,"Salida: media.raw\n");
    
    }
    if(filtro==2){
        fprintf(f,"filtro:  mediana\n");
        fprintf(f,"Salida: mediana.raw\n");
    }
    if(filtro==3){
        fprintf(f,"filtro: sobel\n");
        fprintf(f,"Salida: sobel.raw\n");
    }
    
    fprintf(f,"Tiempo  de ejecucion: %f\n",temp);
   fclose(f);



    


}

//MAIN

int main(int argc, char* argv[]){
    
	
	//-----------------------------------------------Inicializar MPI-------------------------------------------------
	MPI_Init(&argc,&argv);
	
	
	//Declaracion de variables para todos los procesos
	int i,j, rank, nproces;
	int ancho = atoi(argv[2]);
	int largo = atoi(argv[3]);
	char* fichero = argv[1];
	


	double time_0,time_1,total_time;
	time_0=MPI_Wtime();
	
	//Obtener el rank
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	
	//num_procesos
	MPI_Comm_size(MPI_COMM_WORLD,&nproces);
	
	int size_div;
	int div;
	int extra= largo%nproces;
	int op=option(argv[4]);
	
	

	if(rank==nproces-1){
		size_div= largo/nproces + 2+ extra;
		div= largo/nproces +extra;
	}
	else{
		size_div= largo/nproces + 2;
		div= largo/nproces;
	}
		
	unsigned char** parte= matriz_creacion(ancho+2,size_div);
	unsigned char** results=matriz_creacion(ancho,div);
	
	MPI_Status status;
	
	if(rank==0){
		
		//vars
		
		unsigned char ** matriz;
		
		char * name;
		double time;
		matriz=matriz_creacion(ancho+2,largo+2);
		if(matriz==NULL){
        printf("Error creando la matriz");
        return 0;
		}
		//matriz donde se almacena la otra matriz que tiene los filtros
		matriz_load(fichero,ancho,largo,matriz);
		 
		
		for(i=0;i<size_div;i++){
			memcpy(parte[i],matriz[i],(ancho+2)* sizeof(unsigned char));
		}
		
		if(nproces>1){
			
			//fragmentos a enviar
			for(j=1; j<nproces-1; j++){
				for(i=0; i<size_div; i++){
				MPI_Send(matriz[div * j - 1 + i],(ancho+2),MPI_UNSIGNED_CHAR, j, 99,MPI_COMM_WORLD);
				}
			}
			for(i=0; i<size_div+extra; i++){
					MPI_Send(matriz[div *(nproces-1) - 1 + i],(ancho+2),MPI_UNSIGNED_CHAR, (nproces-1), 99, MPI_COMM_WORLD);
			}
			
		}
		
		free(matriz);
		
	}
	else{
		for(i=0; i<size_div; i++){
		MPI_Recv(&parte[i][0], ancho+2, MPI_UNSIGNED_CHAR, 0, 99,MPI_COMM_WORLD ,&status);
		}
	}
	
    switch(op){
        case 1: //media
            media_calculate(parte,results,ancho,div);
        break;

        case 2: //mediana
            mediana_calculate(parte,results,ancho,div);
            
        break;

        case 3: //sobel
            sobel_calculate(parte,results,ancho,div);
        break;

        default:
            printf("El comando seleccionado no pertenece a ninguno de los disponibles, seleccione media, mediana o sobel");
            return 0;
        break;
    }
	
	if(rank!=0){
		for(i=0;i<div;i++){
		MPI_Send(results[i], ancho, MPI_UNSIGNED_CHAR, 0, 99, MPI_COMM_WORLD);
		}
	}
	else{ 
		unsigned char ** matriz_2;
		matriz_2=matriz_creacion(ancho,largo);
		if(matriz_2==NULL){
			printf("Error creando matriz\n");
			return 0;
		}
		
		
		for(i=0;i<div;i++){
			memcpy(matriz_2[i],results[i],ancho * sizeof(unsigned char));
			
		}
		
		if(nproces>1){
			for(i=1; i<nproces-1; i++){
				for(j=0;j<div;j++){
				MPI_Recv(&matriz_2[div*i+j][0],ancho, MPI_UNSIGNED_CHAR, i, 99,MPI_COMM_WORLD ,&status);
				}
				
			}
			for(j=0;j<div+extra;j++){
				MPI_Recv(&matriz_2[div*(nproces-1)+j][0], ancho, MPI_UNSIGNED_CHAR, (nproces-1), 99,MPI_COMM_WORLD ,&status);
			}
		}
		
		if(op==1){
		img_create_raw(matriz_2,"media.raw",ancho,largo);
		}
		if(op==2){
		img_create_raw(matriz_2,"mediana.raw",ancho,largo);
		}
		if(op==3){
		img_create_raw(matriz_2,"sobel.raw",ancho,largo);
		}
		
		free(matriz_2);
		time_1=MPI_Wtime();
		
		total_time=time_1-time_0;
		print_results(ancho,largo,fichero,op, total_time);
	}
	free(parte);
	free(results);
	MPI_Finalize();
    
    
}