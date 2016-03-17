/**
 * Copyright Jakub Staroń 2015
 * numer indeksu 361025
 */

#ifndef PROJECT_CONSTANTS_H
#define PROJECT_CONSTANTS_H

// Losowo wybrany identyfikator kolejki
#define MESSAGE_QUEUE_KEY 23452346

// Adresy odbiorców w obrębie kolejki.
// Adres, postbox to od teraz jest dla nas inna
// nazwa na typ komunikatu.
#define MILION (1000*1000)
#define BANK_POSTBOX 1
#define MUZEUM_POSTBOX 2
#define MUZEUM_ALTERNATIVE_POSTBOX 3
#define FIRM_POSTBOX(Fid) (Fid + 3)
#define DELEGATE_POSTBOX(Fid) (Fid + 2 * MILION)
#define FIELD_POSTBOX(index) (index + 4 * MILION)

// ---Specyfikacja komunikacji---

/**
 * Bank zgłasza do muzeum, że zakończył swoją inicjalizację i przesyła
 * do muzeum potrzebne informacje.
 * BankInitialized(liczba_firm)
 */
#define BANK_INITIALIZED 1

/**
 * Muzeum zgłasza bankowi, że zakończyło swoją inicjalizację i przesyła
 * bankowi stosowne informacje.
 * MuzeumInitialized(Dlugosc, Glebokosc)
 *
 * Razem z komunkatem BANK_INITIALIZED tworzą wstępny handshake
 * między bankiem a muzeum.
 */
#define MUZEUM_INITIALIZED 2

/*
 * GetSaldo(who, postbox)
 * who > 0, oznacza firmę
 * postbox id na które wysłać odpowiedź
*/
#define BANK_GET_SALDO 3

/**
 * SendMoney(from, to, how_much, postbox)
 * from, to, postbox jak w GetMySaldo, z dodatkowym warunkiem że jeśli
 * 0 to muzeum a nie firma
*/
#define BANK_SEND_MONEY 4

/**
 * Wysyłane przez muzeum do banku by wysłał do firm wiadomość o zamknięciu,
 * poczekał na zamknięcie firm, a następnie przygotował się na śmierć
 * wysłał potwierdzenie do muzeum (BANK_READY_FOR_DEATH)
*/
#define BANK_SHUTDOWN 5

/**
 * Wysyłane przez bank firmie by ta zakończyła swoją działalność
 */
#define FIRM_SHUTDOWN 6

/**
 * RaportPart(Fid)
 * Firma wysyła kawałek raportu.
*/
#define MUZEUM_RAPORT_PART 7

/**
 * RaportEnd(Fid)
 * Firma zakończyła wysyłanie wszystkich części raportu.
 */
#define MUZEUM_RAPORT_END 8

/**
 * Typ wiadomości używany wewnętrznie przez MessageBuffer do informowania
 * deamona, że jego trud już skoczon.
 */
#define MESSAGE_BUFFER_DEAMON_KILL_YOURSELF 42

/**
 * Wysyłany przez bank do muzeum by zasygnalizować swoją śmierć.
 */
#define BANK_READY_TO_DEATH 9

/**
 * Wysyłany przez firmę do banku by zasygnalizować swoją śmierć.
 * FirmReadyToDeath(Fid)
 *
 */
#define FIRM_READY_TO_DEATH 10

/**
 * Firma prosi o pozwolenie na wydobycie.
 * RequestForPermission(Fid, Fk, Z)
 */
#define FIRM_REQUEST_FOR_PERMISSION 11

/**
 * Muzeum odrzuca prośbę.
 */
#define MUZEUM_REQUEST_REJESTED 12

/**
 * Muzeum akceptuje prośbę.
 * RequestAccepted(L, G)
 */
#define MUZEUM_REQUEST_ACCEPTED 13

/*
 * GetEsteem(Fid, L, P, G)
 **/
#define MUZEUM_GET_ESTEEM 14

/**
 * Pracownik firmy przesyła żądanie pracy na polu index.
 * Pola numerujemy od 1
 * IWantToDig(index)
 */
#define DELEGATE_I_WANT_TO_DIG 15

/**
 * Pracownik firmy wysyła do delegata informację, że kończy pracę
 * Odbywa się to w momencie, gdy firma otrzyma sygnał sigint
 */
#define DELEGATE_I_WANT_TO_END 16

/**
 * Firma wyeksploatowała wszystkie zasoby na polu index, wysyłane na
 * port FIELD_POSTBOX(index)
 * FieldWorn()
 */
#define WORKER_FIELD_WORN 17

/**
 * Delegat wysyła workerowi symbol zbioru
 * FieldSymbol(symbol)
 */
#define WORKER_FIELD_SYMBOL 18

/**
 * Worker wysyła delegatowi raport wykopalisk.
 * Pierwszy int to index.
 * Drugi int wiadomości n określa liczbę różnych
 * artefaktów, następnie n par {artefakt, liczność}
 */
#define DELEGATE_WORK_REPORT 19

/**
 * Spoko, spoko, raport przyjęty.
 */
#define WORKER_RAPORT_OK 20

/**
 * SellArtifacts(Fid, artifact, number)
 */
#define MUZEUM_SELL_ARTIFACTS 21

/**
 * Muzeum potwierdza, że kupiło artefakty i przelało hajs.
 */
#define FIRM_SELL_OK 22

/**
 * Gdy sprzedaż nie doszła do skutku
 */
#define FIRM_SELL_NOT_OK 23

#endif //PROJECT_CONSTANTS_H
