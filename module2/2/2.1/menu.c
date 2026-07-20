#include <stdlib.h>
#include <stdio.h>
#include "menu.h"
#include "contacts.h"

void readString(char *buffer, int maxLength) {
    fgets(buffer, maxLength, stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
}


void printMenu(){

    printf("Меню\n");
    printf("1. Инициализировать список\n");
    printf("2. Добавить контакт\n");
    printf("3. Удалить контакт\n");
    printf("4. Редактировать контакт\n");
    printf("6. Отобразить спсиок контактов\n");
    printf("7. Выход\n");

}

void initContacts(struct Entry* entry, int initSize){

    int code = initPhoneBook(entry, initSize);
    if(code == -1){
        printf("Объект entry не создан");
    }
    else if(code == -2){
        printf("Ошибка выделения памяти для contacts");
    }
    else{
        printf("Успешно выделена память для %d контактов", initSize);
    }
}

void addContact(struct Entry* entry, int* ptrId){

    char buffer[100] = "";

    char firstName[NAME_LEN];
    char lastName[NAME_LEN];
    char job[JOB_LEN];
    char phoneNumber[PHONE_LEN];
    char country[COUNTRY_LEN];
    char city[CITY_LEN];
    char street[STREET_LEN];
    
    printf("\n Ввод нового контакта (ID: %d) \n", *ptrId);
    
    printf("Имя: ");
    readString(firstName, NAME_LEN);
                
    printf("Фамилия: ");
   readString(lastName, NAME_LEN);
                
                printf("Работа/Должность: ");
                readString(job, JOB_LEN);
                
                printf("Номер телефона: ");
                readString(phoneNumber, PHONE_LEN);
                
                printf("Страна: ");
                readString(country, COUNTRY_LEN);
                
                printf("Город: ");
                readString(city, CITY_LEN);
                
                printf("Улица: ");
                readString(street, STREET_LEN);

                // Вызываем ВАШУ функцию из модуля contacts
                int result = pushContact(entry, *ptrId, firstName, lastName, 
                                         job, phoneNumber, country, city, street);
    (*ptrId)++;

}