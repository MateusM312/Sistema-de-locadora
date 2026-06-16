using namespace std;
#include <iostream>
#include <vector>
#include <thread>
#include <string>
#include <chrono>
#include <fstream>
#include <sstream>

class Clientes;

struct NoFilmes{
    int id;
    string nome, dataLancamento, genero, nomeDiretor;
    bool emprestado = false;
    vector<Clientes> listaEspera; // a minha ideia é q isso seja uma fila q guarde as pessoas q querem alugar o disco
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
        int formaPagamento;
    public:
        Clientes(){
            nome = "Não definido";
            telefone = "Não definido";
            cpf = "Não definido";
        }

        Clientes(int id, string nome, string telefone, string cpf){
            this->nome = nome;
            this->cpf = cpf;
            this->id = id;
            this->telefone = telefone;
        }

        string getNome(){
            return nome;
        }

        int getId(){
            return id;
        }

        string getTelefone(){
            return telefone;
        }

        string getCPF(){
            return cpf;
        }
}; 

void salvarClientesTXT(const string& nomeArquivo, vector<Clientes>& clientesCadastrados){
    ofstream arquivo(nomeArquivo);
    if(!arquivo.is_open()){
        cout << "Erro ao abrir o arquivo para salvar clientes!" << endl;
        return;
    }
 
    for(int i = 0; i < (int)clientesCadastrados.size(); i++){
        arquivo << clientesCadastrados[i].getId() << ". "
                << clientesCadastrados[i].getNome() << " - "
                << clientesCadastrados[i].getTelefone() << " - "
                << clientesCadastrados[i].getCPF() << "\n";
    }
}

void salvarFilmesTXT(const string& nomeArquivo){
    ofstream arquivo(nomeArquivo);
    if(!arquivo.is_open()){
        cout << "Erro ao abrir o arquivo para salvar filmes!" << endl;
        return;
    }
 
    NoFilmes* atual = inicio;
    while(atual != nullptr){
        arquivo << atual->id << ". "
                << atual->nome << " - "
                << atual->nomeDiretor << " - "
                << atual->dataLancamento << " - "
                << atual->genero << "\n";
        atual = atual->prox;
    }
}


int quantosIDS(vector<Clientes> clientesCadastrados){
    int id = 0;
    for(int i = 0; i < clientesCadastrados.size(); i++){
        id++;
    }
    return id;
}

void adicionarCliente(string nome, string telefone, string cpf, vector<Clientes>& clientesCadastrados){
    Clientes cliente(quantosIDS(clientesCadastrados) + 1, nome, telefone, cpf);
    clientesCadastrados.push_back(cliente);
    salvarClientesTXT("clientes.txt", clientesCadastrados);
}
 
void carregarClientesTXT(const string& nomeArquivo, vector<Clientes>& clientesCadastrados){
    ifstream arquivo(nomeArquivo);
    if(!arquivo.is_open()){
        cout << "Erro ao abrir o arquivo!" << endl;
        return;
    }
 
    string linha;
    while(getline(arquivo, linha)){
        if (linha.empty()) continue;
 
        size_t pontoPos = linha.find(". ");
        int id = stoi(linha.substr(0, pontoPos));
 
        string resto = linha.substr(pontoPos + 2);
        size_t pos1 = resto.find(" - ");
        size_t pos2 = resto.find(" - ", pos1 + 1);
 
        string nome     = resto.substr(0, pos1);
        string telefone = resto.substr(pos1 + 3, pos2 - pos1 - 3);
        string cpf      = resto.substr(pos2 + 3);
 
        clientesCadastrados.push_back(Clientes(id, nome, telefone, cpf));
    }
}

void adicionarFilme(string nome, string dataLancamento, string genero, string Diretor, bool foiEmprestado){
    NoFilmes* novo = new NoFilmes;
    novo->dataLancamento = dataLancamento;
    novo->nome = nome;
    novo->genero = genero;
    novo->nomeDiretor = Diretor;
    novo->emprestado = foiEmprestado;
    novo->prox = nullptr;
 
    if (inicio == nullptr){
        novo->id = 1;
        inicio = novo;
        salvarFilmesTXT("filmes.txt");
        return;
    }
 
    NoFilmes* aux = inicio;
    while(aux->prox != nullptr){
        aux = aux->prox;
    }
 
    novo->id = aux->id + 1;
    aux->prox = novo;
 
    salvarFilmesTXT("filmes.txt");
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

        while (aux->prox != nullptr && aux->nome != tituloFilme){
            aux = aux->prox;
        }

        if(aux->prox == nullptr && aux->nome != tituloFilme){
            throw invalid_argument("Titulo inserido não foi encontrado");
        }

        return aux;
    }catch(const exception& e){
        cout << "Erro: " << e.what() << endl;
        return nullptr;
    }
}

Clientes buscarCliente(vector<Clientes>& clienteCadastrado, string nomeCliente){
    try{
        for(int i = 0; i < (int)clienteCadastrado.size(); i++){
            if(clienteCadastrado[i].getNome() == nomeCliente){
                return clienteCadastrado[i];
            }
        }
        throw invalid_argument("Cliente não encontrado");
    }catch(const exception& e){
        cout << "Erro: " << e.what() << endl;
        return Clientes(); // usa o construtor default
    }
}

void emprestarFilme(string tituloFilme, Clientes cliente){
    NoFilmes* filmeEscolhido = buscarFilme(tituloFilme);
    if (filmeEscolhido == nullptr){
        system("cls");
        cout << "Filme Inexistente!\n";
        return;
    }
    if (filmeEscolhido->emprestado == false){
        filmeEscolhido->emprestado = true;
        filmeEscolhido->listaEspera.push_back(cliente);
    }else{
        system("cls");
        cout << "----------------------- LISTA DE ESPERA -----------------------\n";
        cout << "Filme já emprestado, você foi colocado na lista de espera\n";
        filmeEscolhido->listaEspera.push_back(cliente);
        for(int i = 0; i < filmeEscolhido->listaEspera.size(); i++){
            cout << "Nome: " << filmeEscolhido->listaEspera[i].getNome() << " - Telefone: " << filmeEscolhido->listaEspera[i].getTelefone() << " - CPF: " << filmeEscolhido->listaEspera[i].getCPF() << endl;
        }
    }
}

void devolverFilme(string tituloFilme){
    NoFilmes* filmeEscolhido = buscarFilme(tituloFilme);
    if (filmeEscolhido == nullptr){
        system("cls");
        cout << "Filme Inexistente!\n";
        return;
    }

    if (filmeEscolhido->emprestado == false){
        cout << "\nEsse filme não está emprestado.\n";
        return;
    }

    // Remove o cliente do index 0 da fila de espera
    if(!filmeEscolhido->listaEspera.empty()){
        filmeEscolhido->listaEspera.erase(filmeEscolhido->listaEspera.begin());
    }

    if(filmeEscolhido->listaEspera.empty()){
        filmeEscolhido->emprestado = false;
        cout << "\nFilme devolvido com sucesso! Nenhum cliente na lista de espera.\n";
    } else {
        for(int i = 0; i < filmeEscolhido->listaEspera.size(); i++){
            cout << "Nome: " << filmeEscolhido->listaEspera[i].getNome() << " - Telefone: " << filmeEscolhido->listaEspera[i].getTelefone() << " - CPF: " << filmeEscolhido->listaEspera[i].getCPF() << endl;
        }
    }
}

int main(){ 
    system("chcp 65001");
    carregarFilmesTXT("filmes.txt");
    vector<Clientes> clientesCadastrados;
    carregarClientesTXT("clientes.txt", clientesCadastrados);
    int escolha;
    do{
        system("cls");
        cout << "----------------------- Menu de Emprestimo -----------------------\n";
        cout << "1. Emprestar filme\n2. Devolver filme\n3. Cadastrar cliente\n4. Mostrar filmes\n5. Cadastrar filme\n6. Selecionar filtro\n7. Mostrar Clientes cadastrados\n0. Sair\nEscolha: ";
        cin >> escolha;
        switch (escolha)
        {
        case 1:{
            string filme, nomecliente;   
            system("cls");
            cin.ignore();
            cout << "----------------------- EMPRESTAR -----------------------\n";

            cout << "Nome Filme: ";
            getline(cin, filme);

            cout << "Cadastro do cliente: ";
            getline(cin, nomecliente);

            if(filme.empty() || nomecliente.empty()){
                cout << "\nTodos os campos são obrigatórios!\n";
                this_thread::sleep_for(chrono::seconds(2));
                break;
            }

            Clientes cliente = buscarCliente(clientesCadastrados, nomecliente);

            if(cliente.getNome() == "Não definido"){
                cout << "\nCliente não encontrado, operação cancelada.\n";
                this_thread::sleep_for(chrono::seconds(2));
                break;
            }

            emprestarFilme(filme, cliente);

            cout << "\nPressione Enter para continuar...";
            cin.ignore();
            break;
        }
        case 2:{
            string filme;   
            system("cls");
            cin.ignore();
            cout << "----------------------- DEVOLVER -----------------------\n";

            cout << "Nome Filme: ";
            getline(cin, filme);

            if(filme.empty()){
                cout << "\nTodos os campos são obrigatórios!\n";
                this_thread::sleep_for(chrono::seconds(2));
                break;
            }

            devolverFilme(filme);

            cout << "\nPressione Enter para continuar...";
            cin.ignore();
            break;
        }
        case 3:{
            string nomeC, cpfC, telefoneC;
            system("cls");
            cin.ignore(); 
            cout << "----------------------- CADASTRAR CLIENTE -----------------------\n";
            cout << "Nome cliente: ";
            getline(cin, nomeC);
            cout << "\nTelefone: ";
            getline(cin, telefoneC);
            cout << "\nCPF: ";
            getline(cin, cpfC);
            adicionarCliente(nomeC, telefoneC, cpfC, clientesCadastrados);
            cout << "CONCLUIDO";
            this_thread::sleep_for(chrono::milliseconds(500));
            break;
        }
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
            cin.ignore();
            cout << "----------------------- CADASTRAR FILME -----------------------\n";
            cout << "Nome do Filme\n-> ";
            getline(cin, titulo);
            system("cls");

            cout << "Nome do Diretor\n-> ";
            getline(cin, diretor);
            system("cls");
            
            cout << "Data de lançamento\n-> ";
            getline(cin, data);
            system("cls");
            
            cout << "Genero do Filme\n-> ";
            getline(cin, genero);
            system("cls");
            
            emprestado = false;
            
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
        case 7:
            int choice;
            do{
                system("cls");
                cout << "----------------------- NOSSOS CLIENTES -----------------------\n";
                for(int i = 0; i < clientesCadastrados.size(); i++){
                    cout << "Nome:" << clientesCadastrados[i].getNome() << " - ID: " << clientesCadastrados[i].getId() << " - Telefone: " << clientesCadastrados[i].getTelefone() << " - CPF: " << clientesCadastrados[i].getCPF() << endl; 
                }
                cout << "1. Sair\n-> ";
                cin >> choice;
            }while(choice != 1);
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
