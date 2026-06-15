using namespace std;
#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <fstream>
#include <sstream>

struct NoFilmes{
    int id;
    string nome, dataLancamento, genero, nomeDiretor;
    bool emprestado = false;
    vector<string> listaEspera; // a minha ideia é q isso seja uma fila q guarde as pessoas q querem alugar o disco
    NoFilmes* prox;
};

NoFilmes* inicio = nullptr;

class FormaPagamentos{
    private:
        vector<string> pagamentos = {"Débito", "Crédito", "Dinheiro", "PIX"};

    public:
        void exibirOpcoes(){
            for(int i = 0; i < (int)pagamentos.size(); i++){
                cout << i + 1 << ". " << pagamentos[i] << "\n";
            }
        }

        string getForma(int escolhida){
            if(escolhida == -1){
                return "Nenhuma forma definida";
            }
            return pagamentos[escolhida];
        }
};

class Clientes{
    private:
        int id;
        string nome, telefone, cpf;
        FormaPagamentos pagamento;
    public:
        Clientes(){
            nome = "Não definido";
            telefone = "Não definido";
            cpf = "Não definido";
        }

        Clientes(string nome, string telefone, string cpf){
            this->nome = nome;
            this->cpf = cpf;
            this->telefone = telefone;
        }
}; 

void carregarClientesTXT(const string& nomeArquivo){
    ifstream arquivo(nomeArquivo);
    if(!arquivo.is_open()){
        cout << "Erro ao abrir o arquivo!" << endl;
        return;
    }

    string linha;

    while(getline(arquivo, linha)){
        if (linha.empty()) continue;


    }
}

void adicionarFilme(string nome, string dataLancamento, string genero, string Diretor, bool foiEmprestado){
    NoFilmes* novo = new NoFilmes;
    int id = 0;
    novo->dataLancamento = dataLancamento;
    novo->nome = nome;
    novo->genero = genero;
    novo->nomeDiretor = Diretor;
    novo->emprestado = foiEmprestado;
    novo->prox = nullptr;

    if (inicio == nullptr){
        inicio = novo;
        return;
    }

    NoFilmes* aux = inicio;
    while(aux->prox != nullptr){
        aux = aux->prox;
    }

    novo->id = aux->id + 1;
    aux->prox = novo;

    // (...) -> codigo para salvar o novo filme no txt, melhor criar uma função fora e chamar aqui
}

string getCampo(NoFilmes* novo, int campo){
    switch(campo){
        case 1: 
            return novo->nome;
        case 2: 
            return novo->nomeDiretor;
        case 3: 
            return novo->dataLancamento;
        case 4: 
            return novo->genero;
        case 5:
            return to_string(novo->id); // tinha q transformar isso na sua propria funcao
        default: 
            return "ERROR 404";
    }
}

void ordenarFilmes(int campo){ // eu me perdi pra caralho aqui, depois eu mudo pra algo entendivel mas ta funcionando
    try{
        if(inicio == nullptr || inicio->prox == nullptr){
            throw runtime_error("Lista vazia ou com apenas um elemento, nada a ordenar.");
        }

        if(campo < 1 || campo > 5){
            throw invalid_argument("Campo de ordenação inválido.");
        }

        NoFilmes* ordenada = nullptr;
        NoFilmes* atual = inicio;

        while(atual != nullptr){
            NoFilmes* proximo = atual->prox; // guarda o próximo ANTES de mexer nos ponteiros

            if(ordenada == nullptr || getCampo(atual, campo) < getCampo(ordenada, campo)){
                atual->prox = ordenada;
                ordenada = atual;
            } else {
                NoFilmes* busca = ordenada;
                // avança SÓ pra encontrar a posição, sem religar nada ainda
                while (busca->prox != nullptr && getCampo(busca->prox, campo) <= getCampo(atual, campo)){
                    busca = busca->prox;
                }
                // agora sim, insere na posição encontrada
                atual->prox = busca->prox;
                busca->prox = atual;
            }

            atual = proximo; // anda pro próximo da lista original, fora do else
        }

        inicio = ordenada;

    }catch(const exception& e){
        cout << "Erro: " << e.what() << endl;
        return;
    }
}

void exibir(){
    NoFilmes* atual = inicio;
    while (atual != nullptr) {
            cout << atual->id << ". " << atual->nome << " - " << atual->dataLancamento << " - " << atual->genero << " - " << atual->nomeDiretor << endl;
            atual = atual->prox;
        }
}

void carregarFilmesTXT(const string& nomeArquivo){
    ifstream arquivo(nomeArquivo);
    if(!arquivo.is_open()){
        cout << "Erro ao abrir o arquivo!" << endl;
        return;
    }

    string linha;

    while(getline(arquivo, linha)){
        if (linha.empty()) continue;

        NoFilmes* novo = new NoFilmes();
        novo->prox = nullptr;

        int pontoPos = linha.find(". ");
        novo->id = stoi(linha.substr(0, pontoPos));

        string resto = linha.substr(pontoPos + 2);
        int pos1 = resto.find(" - ");
        int pos2 = resto.find(" - ", pos1 + 1);
        int pos3 = resto.find(" - ", pos2 + 1);

        novo->nome = resto.substr(0, pos1);
        novo->nomeDiretor = resto.substr(pos1 + 3, pos2 - pos1 - 3);
        novo->dataLancamento = resto.substr(pos2 + 3, pos3 - pos2 - 3);
        novo->genero = resto.substr(pos3 + 3);

        if (inicio == nullptr){
            inicio = novo;
            continue;
        }
        NoFilmes* aux = inicio;

        while(aux->prox != nullptr){
            aux = aux->prox;
        }
        aux->prox = novo;
    }
}

class FormaPagamento{
    private:
        vector<string> pagamentos = {"Débito", "Crédito", "Dinheiro", "PIX"};
    public:
        string checkForma(int index){
            return pagamentos[index];
        }
};

class Cliente : public FormaPagamento{
    private:
        string nome, telefone, cpf;
        int formaPagamento;
        
    public:
        void getInfo(){
            cout << "Nome: " << nome << " Telefone: " << telefone << " CPF: " << cpf << " Forma de Pagamento: " << checkForma(formaPagamento) << endl;
        }
};

// é busca linear por enquanto, mas eu vou mudar pra binaria assim q essa musica do kurt cobain acabar
NoFilmes* buscarFilme(string tituloFilme){ // to pensando em fazer com q vc possa procurar apenas pelo nome
    NoFilmes* aux = inicio;

    try{
        if(inicio == nullptr){
            throw runtime_error("Lista vazia!");
        }

        while (aux->prox != nullptr || aux->nome != tituloFilme){
            aux = aux->prox;
        }

        if(aux->prox == nullptr && aux->nome != tituloFilme){
            throw invalid_argument("Titulo inserido não foi encontrado");
        }

        return aux;
    }catch(const exception& e){
        cout << "Erro: " << e.what() << endl;
        return;
    }
}

void emprestarFilme(){

}

void devolverFilme(){

}

int main(){ 
    system("chcp 65001");
    carregarFilmesTXT("filmes.txt");
    int escolha;
    do{
        system("cls");
        cout << "----------------------- Menu de Emprestimo -----------------------\n";
        cout << "1. Emprestar filme\n2. Devolver filme\n3. Cadastrar cliente\n4. Mostrar filmes\n5. Cadastrar filme\n6. Selecionar filtro\n0. Sair\nEscolha: ";
        cin >> escolha;
        switch (escolha)
        {
        case 1:

            break;
        case 2:
            
            break;
        case 3:
            
            break;
        case 4:{
            while(escolha != 2){
                cout << "\033[0m";
                system("cls");
                cout << "----------------------- FILMES -----------------------\n";
                exibir();
                cout << "\033[32m"; 
                cout << "Deseja prosseguir 2 Sim, 1 Não:";
                cin >> escolha;
            }
            cout << "\033[0m"; 
            break;
        }
        case 5:{
            string titulo,diretor, data, genero;
            bool emprestado;
            system("cls");
            cout << "----------------------- CADASTRAR FILME -----------------------\n";
            cout << "Nome do Filme\n-> ";
            cin >> titulo;
            system("cls");

            cout << "Nome do Diretor\n-> ";
            cin >> diretor;
            system("cls");
            
            cout << "Data de lançamento\n-> ";
            cin >> data;
            system("cls");
            
            cout << "Genero do Filme\n-> ";
            cin >> genero;
            system("cls");
            
            cout << "Filme já está emprestado\n-> ";
            cin >> emprestado;
            
            system("cls");
            adicionarFilme(titulo, data, genero, diretor, emprestado);
            
            break;}
        case 6:
            int campo;
            system("cls");
            cout << "----------------------- SORTS FILTROS -----------------------\n";
            cout << "1. Por Titulo\n2. Por Diretor\n3. Por Data de lançamento\n4. Por Genero\n5. ID\n-> ";
            cin >> campo;
            ordenarFilmes(campo);
            this_thread::sleep_for(chrono::milliseconds(500));
            cout << "CONCLUIDO!";
            this_thread::sleep_for(chrono::milliseconds(500));
            system("cls");
            break;
        case 0:
            system("cls");
            cout << "\033[31m"; 
            cout << "Saindo";

            this_thread::sleep_for(chrono::milliseconds(500));
            system("cls");
            cout << "Saindo.";

            this_thread::sleep_for(chrono::milliseconds(500));
            system("cls");
            cout << "Saindo..";

            this_thread::sleep_for(chrono::milliseconds(500));
            system("cls");
            cout << "Saindo...";
            break;
        
        default:
            break;
        }
    } while (escolha != 0);  
}
