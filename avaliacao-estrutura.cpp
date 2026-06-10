using namespace std;
#include <iostream>
#include <vector>
#include <ctime>

void sleep_seconds(double seconds) {
    clock_t start = clock();
    while ((clock() - start) / CLOCKS_PER_SEC < seconds);
}  

struct NoFilmes{
    
    NoFilmes* prox;
};

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
        case 4:
            
            break;
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
