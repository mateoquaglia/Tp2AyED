#include <iostream>
#include <string>
#include <sstream>

using namespace std;

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

class Usuario {
public:
    string nombre;
    int interacciones;
    Usuario(string nombre) : nombre(nombre), interacciones(0) {}

    void mostrarInteraccionesUsuario(Usuario* usuario) {
        cout << "Interacciones de " << usuario->nombre << ": " << usuario->interacciones << endl;
    }
};

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

class Fecha {
public:
    int dia, mes, anio;
    Fecha(int dia, int mes, int anio) : dia(dia), mes(mes), anio(anio) {}

};

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

class Publicacion {
public:
    Usuario* usuario;
    Fecha* fecha;
    string tipo;
    string contenido;
    string titulo;
    int votos;

    Publicacion(Usuario* usuario, Fecha* fecha, string tipo, string contenido, string titulo) : usuario(usuario), fecha(fecha), tipo(tipo), contenido(contenido), titulo(titulo), votos(0) {}
    Publicacion(Usuario* usuario, Fecha* fecha, string tipo, string contenido) : usuario(usuario), fecha(fecha), tipo(tipo), contenido(contenido), votos(0) {}

    string getTipo() { return tipo; }
    bool esIgual(Publicacion* a) { if (this->getTipo() == a->getTipo()) { return true; } else { return false; } }
    string getTitulo() { return this->contenido; }
    void setTipo(string tipo) { this->tipo = tipo; }

    void Votar() {
        votos++;
    }
    void desVotar() {
        votos--;
    }
    int ObtenerVotos() {
        return votos;
    }
};

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

class Comentario : public Publicacion {
public:
    Comentario* siguiente;
    Comentario(Usuario* usuario, Fecha* fecha, string contenido)
            : Publicacion(usuario, fecha, "Comentario", contenido), siguiente(NULL) {}
};

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

class Respuesta : public Publicacion {
public:
    Respuesta* siguiente;
    Respuesta(Usuario* usuario, Fecha* fecha, string contenido)
            : Publicacion(usuario, fecha, "Respuesta", contenido), siguiente(NULL) {}

};

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

class nodo {
public:
    Publicacion* info;
    nodo* der, * izq;
};

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

class ArbolPublicaciones {
private:
    nodo* raiz = new nodo();
    nodo* dirComentar = new nodo();
    nodo* dirBorrar = new nodo();

    void ArbolBusq(Publicacion* publicacion, nodo*& nuevo);
    void show(nodo* aux, int n);
    void borrar(nodo*& p, Publicacion* publicacion);
    void bor(nodo* d);

    Publicacion* menor(nodo* aux);

    bool esta(nodo* aux, Publicacion* publicacion);

public:
    ArbolPublicaciones() { raiz = NULL; };
    ~ArbolPublicaciones() {};
    //Metodos de insercion y navegacion en el Arbol de Publicaciones:
    nodo* getRaiz() {return raiz;}
    nodo* buscarIRD(nodo* aux, Publicacion* p);
    void AgregarPublicacion(Publicacion* publicacion);
    void Comentar(Publicacion* comentario, Publicacion* p);
    void Responder(Publicacion* respuesta, Publicacion* p);
    //Metodos de interacción con elementos del Arbol de Publicaciones:
    void Votar(Publicacion* publicacion);
    void desVotar(Publicacion* publicacion);
    void MostrarVotos();
    void MostrarVotosAux(nodo* aux);
    void MostrarArbol() { show(raiz, 0); }
    Usuario* encontrarUsuarioMasParticipativo(nodo* nodoActual, Usuario* usuarioMasParticipativo);
    nodo* EncontrarMasVotado(nodo* nodoActual, nodo* maxVotado);
    nodo* EncontrarPublicacionMasVotada();
    void borrarPost(Publicacion* publicacion);
    void getUsuarioMasParticipativo();
    void getPublicacionMasVotada();


    Publicacion* Menor() { return menor(raiz); }
    bool Esta(Publicacion* publicacion) { return esta(raiz, publicacion); }
};


//---Metodos de insercion y navegacion en el Arbol de Publicaciones--------------------------------------------------------------------------------------------------------------------------------


void ArbolPublicaciones::ArbolBusq(Publicacion* publicacion, nodo*& nuevo)
{
    if (nuevo == NULL) {
        nuevo = new nodo;
        nuevo->info = publicacion;
        nuevo->der = nuevo->izq = NULL;
    }
    else if (publicacion->esIgual(nuevo->info))
        ArbolBusq(publicacion, nuevo->der);
    else { ArbolBusq(publicacion, nuevo->izq); }

}

nodo* ArbolPublicaciones::buscarIRD(nodo* aux, Publicacion* p) //busca el nodo de la publicacion que se pasa como parametro y lo retorna
{
    if (aux != NULL) {
        if (aux->info != p) {
            nodo* resultadoIzq = buscarIRD(aux->izq, p);
            nodo* resultadoDer = buscarIRD(aux->der, p);

            if (resultadoIzq != NULL) {
                return resultadoIzq;
            }
            else if (resultadoDer != NULL) {
                return resultadoDer;
            }
        }
        else {
            return aux;
        }
    }

    return NULL;  // Si no se encuentra la publicación, se devuelve NULL
}

void ArbolPublicaciones::AgregarPublicacion(Publicacion* publicacion)
{
    ArbolBusq(publicacion, raiz);
    publicacion->usuario->interacciones++;
}

void ArbolPublicaciones::Comentar(Publicacion* comentario, Publicacion* publicacion)
{
    this->dirComentar = buscarIRD(raiz, publicacion);
    ArbolBusq(comentario, dirComentar);
    comentario->usuario->interacciones++;
}

void ArbolPublicaciones::Responder(Publicacion* respuesta, Publicacion* comentario)
{
    this->dirComentar = buscarIRD(raiz, comentario);
    ArbolBusq(comentario, dirComentar);
    respuesta->usuario->interacciones++;
}


//---Metodos de interacción con elementos del Arbol de Publicaciones--------------------------------------------------------------------------------------------------------------------------------

void ArbolPublicaciones::Votar(Publicacion* publicacion) {
    nodo* direccionPublicacion = buscarIRD(raiz, publicacion);
    if (direccionPublicacion != NULL) {
        publicacion->Votar();
    }
}

void ArbolPublicaciones::desVotar(Publicacion* publicacion) {
    nodo* direccionPublicacion = buscarIRD(raiz, publicacion);
    if (direccionPublicacion != NULL) {
        publicacion->desVotar();
    }
}

void ArbolPublicaciones::MostrarVotos() {
    MostrarVotosAux(raiz);
}

void ArbolPublicaciones::MostrarVotosAux(nodo* aux) {
    if (aux != NULL) {
        MostrarVotosAux(aux->izq);
        cout << "Contenido: " << aux->info->contenido << " - Votos: " << aux->info->ObtenerVotos() << endl;
        MostrarVotosAux(aux->der);
    }
}

Usuario* ArbolPublicaciones::encontrarUsuarioMasParticipativo(nodo* nodoActual, Usuario* usuarioMasParticipativo) {
    if (nodoActual == NULL) {
        return usuarioMasParticipativo;
    }

    if (usuarioMasParticipativo == NULL || nodoActual->info->usuario->interacciones > usuarioMasParticipativo->interacciones) {
        usuarioMasParticipativo = nodoActual->info->usuario;
    }

    Usuario* participativoIzq = encontrarUsuarioMasParticipativo(nodoActual->izq, usuarioMasParticipativo);
    Usuario* participativoDer = encontrarUsuarioMasParticipativo(nodoActual->der, usuarioMasParticipativo);

    if (participativoIzq != NULL && participativoIzq->interacciones > usuarioMasParticipativo->interacciones) {
        usuarioMasParticipativo = participativoIzq;
    }

    if (participativoDer != NULL && participativoDer->interacciones > usuarioMasParticipativo->interacciones) {
        usuarioMasParticipativo = participativoDer;
    }

    return usuarioMasParticipativo;
}

nodo* ArbolPublicaciones::EncontrarMasVotado(nodo* nodoActual, nodo* maxVotado) {
    if (nodoActual == NULL) {
        return maxVotado;
    }

    if (maxVotado == NULL || nodoActual->info->ObtenerVotos() > maxVotado->info->ObtenerVotos()) {
        maxVotado = nodoActual;
    }

    nodo* masVotadoIzq = EncontrarMasVotado(nodoActual->izq, maxVotado);
    nodo* masVotadoDer = EncontrarMasVotado(nodoActual->der, maxVotado);

    if (masVotadoIzq != NULL && masVotadoIzq->info->ObtenerVotos() > maxVotado->info->ObtenerVotos()) {
        maxVotado = masVotadoIzq;
    }

    if (masVotadoDer != NULL && masVotadoDer->info->ObtenerVotos() > maxVotado->info->ObtenerVotos()) {
        maxVotado = masVotadoDer;
    }

    return maxVotado;
}

nodo* ArbolPublicaciones::EncontrarPublicacionMasVotada() {
    return EncontrarMasVotado(raiz, NULL);
}

void ArbolPublicaciones::borrar(nodo*& p, Publicacion* publicacion)
{
    if (p == NULL) cout << "\n El dato NO esta\n\n";
    else if (publicacion != p->info) borrar(p->der, publicacion);
    else if (publicacion != p->info) borrar(p->izq, publicacion);
    else {// lo encontre en el nodo p
        dirBorrar = p;
        if (dirBorrar->der == NULL) p = dirBorrar->izq;// raiz == raiz del subarbol izq
        else if (dirBorrar->izq == NULL) p = dirBorrar->der;//raiz == raiz del subarbol der
        else bor(dirBorrar->izq);//busca en el sub arbol izq
        delete dirBorrar;
    }
}

void ArbolPublicaciones::bor(nodo* d)
{
    if (d->izq != NULL) bor(d->der);//busca el elemento mas a la derecha
    else {
        dirBorrar->info = d->info;
        dirBorrar = d;
        dirBorrar =d->izq;
    }
}

void ArbolPublicaciones::borrarPost(Publicacion *p) {
    borrar(raiz,p);
}

void ArbolPublicaciones::getUsuarioMasParticipativo() {
    cout << "El usuario mas participativo es: " << encontrarUsuarioMasParticipativo(this->getRaiz(), NULL)->nombre << "\n";
}

void ArbolPublicaciones::getPublicacionMasVotada() {
    cout << "La publicacion mas votada es: " << this->EncontrarPublicacionMasVotada()->info->getTitulo() << "\n";
}


//---Método de impresion del Arbol de Publicaciones-------------------------------------------------------------------------------------------------------------------------------------

void ArbolPublicaciones::show(nodo* aux, int n)
{
    int i;
    if (aux != NULL) {
        show(aux->der, n + 1);
        for (i = 1; i <= n; i++) cout << "     ";
        cout << aux->info->contenido << "\n";
        show(aux->izq, n + 1);
    }
}

int main() {

    Usuario* usuario1 = new Usuario("Tomas");
    Usuario* usuario2 = new Usuario("Mateo");
    Usuario* usuario3 = new Usuario("Facundo");

    Fecha* fecha1 = new Fecha(31, 5, 2023);
    Fecha* fecha2 = new Fecha(1, 6, 2023);
    Fecha* fecha3 = new Fecha(2, 6, 2023);

    Publicacion* post1 = new Publicacion(usuario1, fecha1, "Post", "Post1", "AyED");
    Publicacion* post2 = new Publicacion(usuario1, fecha2, "Post", "Post2", "Futbol");
    Publicacion* post3 = new Publicacion(usuario1, fecha3, "Post", "Post3", "Fitness");

    Comentario* comentario1 = new Comentario(usuario2, fecha2, "Comentario 1");
    Comentario* comentario2 = new Comentario(usuario1, fecha3, "Comentario 2");
    Comentario* comentario3 = new Comentario(usuario2, fecha1, "Comentario 3");
    Comentario* comentario4 = new Comentario(usuario1, fecha2, "Comentario 4");
    Comentario* comentario5 = new Comentario(usuario1, fecha2, "Comentario 5");

    Respuesta* respuesta1 = new Respuesta(usuario1, fecha3, "Respuesta 1");
    Respuesta* respuesta2 = new Respuesta(usuario1, fecha3, "Respuesta 2");
    Respuesta* respuesta3 = new Respuesta(usuario1, fecha3, "Respuesta 3");

    ArbolPublicaciones* reddit = new ArbolPublicaciones();

    reddit->AgregarPublicacion(post1);
    reddit->AgregarPublicacion(post2);
    reddit->AgregarPublicacion(post3);

    reddit->Comentar(comentario1,post1);
    reddit->Comentar(comentario2,post2);
    reddit->Comentar(comentario3,post3);
    reddit->Comentar(comentario4,post1);
    reddit->Comentar(comentario5, post3);

    reddit->Comentar(respuesta1,comentario1);
    reddit->Comentar(respuesta2,comentario2);
    reddit->Comentar(respuesta3,comentario3);

    reddit->Votar(post1);
    reddit->Votar(post1);
    reddit->Votar(post1);

    reddit->Votar(post2);
    reddit->Votar(post2);
    reddit->Votar(post2);
    reddit->Votar(post2);

    reddit->Votar(post3);
    reddit->Votar(post3);

    reddit->Votar(comentario1);
    reddit->Votar(comentario1);
    reddit->Votar(comentario1);
    reddit->Votar(comentario1);
    reddit->Votar(comentario1);
    reddit->Votar(comentario1);

    reddit->Votar(comentario2);
    reddit->Votar(comentario2);
    reddit->Votar(comentario2);
    reddit->Votar(comentario2);

    reddit->Votar(comentario4);
    reddit->Votar(comentario4);
    reddit->Votar(comentario4);
    reddit->Votar(comentario4);

    reddit->desVotar(comentario4);
    reddit->desVotar(comentario2);
    reddit->desVotar(comentario1);

    cout << "\n-----------------------------------\n";
    reddit->MostrarArbol();
    cout << "\n-----------------------------------\n";
    usuario1->mostrarInteraccionesUsuario(usuario1);
    usuario2->mostrarInteraccionesUsuario(usuario2);
    usuario3->mostrarInteraccionesUsuario(usuario3);
    cout << "-----------------------------------\n";
    reddit->MostrarVotos();
    cout << "-----------------------------------\n";
    reddit->getUsuarioMasParticipativo();
    reddit->getPublicacionMasVotada();



    return 0;
}