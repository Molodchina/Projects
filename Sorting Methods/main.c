#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>

// /home/cmc/Spring/main/main.c

clock_t start, end;

struct{
    unsigned short time: 1;
    unsigned short amount: 1;
    unsigned short inc: 1;
    unsigned short dec: 1;      Aeyrwbz
    unsigned short visible: 1;
} FLAGS;

void simple_choice(int n, int* a){
    int low; // Рабочая память, меньший ключ в данном просмотре;
    int source; // Рабочая память, его адрес;
    int COMP = 0; // Количество сравнений в течение сортировки
    int SWAP = 0; // Количество обменов в течение сортировки
    start = clock();

    int j = 1;
    while(j<n){
        low = a[j-1];
        source = j-1;
        for(int i=j; i<n; i++){
            ++COMP;
            if(a[i]<low){
                ++SWAP;
                low = a[i];
                source = i;
            } // Если значение ключа рассматриваемого элемента
                // меньше ключа в рабочей памяти, то обновляем рабочую память
        }
        // Обмениваем значениями наименьший просмотренный элемент и элемент с наименьшей позицией из рассмотренных
        a[source] = a[j-1];
        a[j-1] = low;
        source = j-1;
        ++SWAP;
        ++j;
    }

    end = clock();
    if(FLAGS.time)
        printf("ВРЕМЯ, затраченное на сортировку ПРОСТАЯ ВЫБОРКА: %lf sec.\n", ((double)(end - start)) / CLOCKS_PER_SEC);
    if(FLAGS.amount)
        printf("СРАВНЕНИЯ/ОБМЕНЫ сортировки ПРОСТАЯ ВЫБОРКА: %d/%d\n", COMP, SWAP);
}

int Hibbard_fun(int n){
    int temp = 1;
    while(1){
        if(temp * 2 >= n)
            return(temp-1);

        temp = temp * 2;
    } // Вычисление первого шага функции Хиббарда
    return 0;
}

void shella(int n, int* a){
    int step; // Шаг сортировки
    int flag; // Флаг, нужный для вторичного сравнивания элементов одной части
    int COMP = 0; // Количество сравнений в течение сортировки
    int SWAP = 0; // Количество обменов в течение сортировки
    step = Hibbard_fun(n); // Задаем шаг сортировки через функцию Хиббарда
    int temp; // Переменная для хранения промежуточных значений во время сортировки
    start = clock();

    while(step){
        for(int i=0; i<n-step; ++i){
            ++COMP;
            if(a[i] > a[i+step]){
                ++SWAP;
                temp = a[i];
                a[i] = a[i+step];
                a[i+step] = temp;
                if(i>=step)
                    flag = 1;
            } // Первичная сортировка части методом Шелла
            if(flag){
                for(int j=i; j>=step; j-=step){
                    ++COMP;
                    if(a[j] >= a[j-step])
                        break;
                    // printf("SWITCH_@2: %d - - %d\n-----\n", a[j], a[j-step]);
                    ++SWAP;
                    temp = a[j];
                    a[j] = a[j-step];
                    a[j-step] = temp;
                }
                flag = 0;
            } // Вторичная сортировка части
        }
        step = step / 2; // Изменяем шаг сортировки
    }

    end = clock();
    if(FLAGS.time)
        printf("ВРЕМЯ, затраченное на сортировку ШЕЛЛА: %lf sec.\n", ((double)(end - start)) / CLOCKS_PER_SEC);
    if(FLAGS.amount)
        printf("СРАВНЕНИЯ/ОБМЕНЫ сортировки ШЕЛЛА: %d/%d\n", COMP, SWAP);
}

int main(int argc, char *argv[]){
    srand(time(NULL));
    int *a,*b; // Задаем указатели на массивы целых чисел
    int flag = 0;

    int n = 10; // Стандартное значение длины массива

    int set = 0;
    while ((set = getopt(argc, argv, "htsaidpfn:"))!=-1){
        switch (set) {
            case 'h': printf("\n============================== Help ==============================\n"
                             "Эта программа может отсортировать массив методом Шелла и/или простой выборкой.\n"
                             "Если опции на вход программе не поданы, то она отсортирует массив одной из длин n=10, 100 ... 10000 псевдослучайных чисел.\n"
                             "Возможные опции:\n"
                             "-t \t\t показывает время работы каждого из алгортимов сортировки\n"
                             "-s \t\t показывает количество сравнений и перемещений для каждого из алгоритмов сортировки\n"
                             "-a \t\t позволяет увидеть исходный и отсортированный массив\n"
                             "-i \t\t позволяет заполнить массив числами по неубыванию\n"
                             "-d \t\t позволяет заполнить массив числами по невозрастанию\n"
                             "-p \t\t позволяет выполнить только сортировку простой выборкой\n"
                             "-f \t\t позволяет выполнить только сортировку Шелла\n"
                             "-n <num> \t выполнение программы с массивом из <num> элементов\n"); break;
            case 't': FLAGS.time = 1; break;
            case 's': FLAGS.amount = 1; break;
            case 'a': FLAGS.visible = 1; break;
            case 'i': FLAGS.inc = 1; break;
            case 'd': FLAGS.dec = 1; break;
            case 'p': flag = -1; break;
            case 'f': flag = 1; break;
            case 'n': n = atoi(optarg); break;
        }
    } // Анализ параметров, заданных на вход программе

    a = (int *) malloc(sizeof(int) * n);
    b = (int *) malloc(sizeof(int) * n); // Выделяем память под массивы

    if(FLAGS.inc)
        for(int i=0; i<n; i++){
            b[i] = i;
            a[i] = i;
        } // Заполняем массивы числами от 0 до n, где n- размер массива
    else if(FLAGS.dec)
        for(int i=0; i<n; i++){
            b[i] = n - i - 1;
            a[i] = n - i - 1;
        } // Заполняем массивы числами от n до 0, где n- размер массива
    else
        for(int i=0; i<n; i++){
            set = rand();
            b[i] = set;
            a[i] = set;
        } // Заполняем массивы псевдослучайными числами от 0 до 32767


    if(FLAGS.visible){
        for(int i=0; i<n; i++)
            printf("%d\t", a[i]);
        printf("\n");
    }

    if((flag==0)||(flag==-1))
        simple_choice(n, a);

    if((flag==0)||(flag==1))
        shella(n, b);


    if(FLAGS.visible){
        for(int i=0; i<n; i++)
            printf("%d\t", a[i]);
        printf("\n");
    }

    free(a);
    free(b);

    return 0;
}
