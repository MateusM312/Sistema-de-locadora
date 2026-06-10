using namespace std;
#include <iostream>
#include <vector>
#include <ctime>
#include <fstream>
#include <sstream>

void sleep_seconds(double seconds) {
    clock_t start = clock();
    while ((clock() - start) / CLOCKS_PER_SEC < seconds);
}  

struct NoFilmes{
    int id;
    string nome, dataLancamento, genero, nomeDiretor;
    NoFilmes* prox;
};

void exibir(NoFilmes* l){
    NoFilmes* atual = l;
    while (atual != nullptr) {
            cout << atual->id << " | " << atual->nome << " | " << atual->dataLancamento << " | " << atual->genero << " | " << atual->nomeDiretor << endl;
            atual = atual->prox;
        }
}

NoFilmes* inicio = nullptr;

NoFilmes* carregarFilmesTXT(const string& nomeArquivo){
    ifstream arquivo(nomeArquivo);
    if(!arquivo.is_open()){
        cout << "Erro ao abrir o arquivo!" << endl;
        return nullptr;
    }

    NoFilmes* cabeca = nullptr;
    NoFilmes* cauda  = nullptr;
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

        if (!cabeca) { 
            cabeca = cauda = novo; 
        }else{ 
            cauda->prox = novo; 
            cauda = novo;
        }
    }
    return cabeca;

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

int main(){ 
    system("chcp 65001");
    NoFilmes* lista = carregarFilmesTXT("filmes.txt");
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
                system("cls");
                cout << "----------------------- FILMES -----------------------\n";
                exibir(lista);
                cout << "Deseja prosseguir 2 Sim, 1 Não:";
                cin >> escolha;
            }
            break;
        }
        case 5:
            
            break;
        case 6:
            
            break;
        case 0:
            system("cls");
            cout << "Saindo";
            sleep_seconds(0.1);
            system("cls");
            cout << "Saindo.";
            sleep_seconds(0.1);
            system("cls");
            cout << "Saindo..";
            sleep_seconds(0.1);
            system("cls");
            cout << "Saindo...";
            sleep_seconds(0.1);

            break;
        
        default:
            break;
        }
    } while (escolha != 0);  
}
