#include <iostream>
#include <queue>
#include <stack>
#include <thread>
#include <chrono>
#include <random>
#include <algorithm>
#include <mutex>

using namespace std;

const int NUM_CAJAS = 4;
const int NUM_NUMEROS = 200;
const int MIN_NUM = 1;
const int MAX_NUM = 90;

queue < int > colas[NUM_CAJAS];
stack < int > pilas[NUM_CAJAS];
mutex mtx;
int contadorClientes = 0;


void agregarNumeros() {
  random_device rd;
  mt19937 gen(rd());
  uniform_int_distribution < int > dis(MIN_NUM, MAX_NUM);

  while (contadorClientes < NUM_NUMEROS) {
    this_thread::sleep_for(chrono::seconds(1));

    int num = dis(gen);
    int caja = contadorClientes % NUM_CAJAS;

    mtx.lock();
    colas[caja].push(num);
    cout << "Numero " << num << " asignado a la cola de la caja " << caja + 1 << endl;
    contadorClientes++;
    mtx.unlock();
  }
}


void atenderNumeros(int caja) {
  int numerosAtendidos = 0;
  while (numerosAtendidos < NUM_NUMEROS) {
    this_thread::sleep_for(chrono::seconds(2));

    mtx.lock();
    if (!colas[caja].empty()) {
      int num = colas[caja].front();
      colas[caja].pop();
      pilas[caja].push(num);
      cout << "Caja " << caja + 1 << ": Atendido numero " << num << " y almacenado en la pila" << endl;
      numerosAtendidos++;
    }
    mtx.unlock();
  }
}

int main() {

  thread hiloInsercion(agregarNumeros);

 
  thread cajas[NUM_CAJAS];
  for (int i = 0; i < NUM_CAJAS; i++) {
    cajas[i] = thread(atenderNumeros, i);
  }

  
  hiloInsercion.join();

  
  for (int i = 0; i < NUM_CAJAS; i++) {
    cajas[i].join();
  }

 
  int sumaTotal = 0;
  for (int i = 0; i < NUM_CAJAS; i++) {
    int sumaPila = 0;
    while (!pilas[i].empty()) {
      int num = pilas[i].top();
      pilas[i].pop();
      sumaPila += num;
    }
    cout << "Suma de los numeros atendidos en la Pila " << i + 1 << ": " << sumaPila << endl;
    sumaTotal += sumaPila;
  }

  
  if (contadorClientes >= NUM_NUMEROS) {
    cout << "¡Todos los numeros han sido atendidos!" << endl;
  }

  cout << "Suma total de todos los numeros atendidos: " << sumaTotal << endl;
  cout << "Total de clientes atendidos: " << contadorClientes << endl;

  return 0;
}
