#include <list>
#include <BuggFX.h>

struct Game{
    int largura;
    int altura;
    int acertos;
    int erros;
    int max_acertos;
    int max_erros;
    int y_chao;

    Game(int largura, int altura){
        this->largura = largura;
        this->altura = altura;
        acertos = 0;
        erros = 0;
        max_acertos = 25;
        max_erros = 10;
        y_chao = altura - 50;
    }

    bool over(){
        if(erros >= max_erros)
            return true;
        return false;
    }

    void show(){
        Escritor e;
        e.setColor(Color::Red);
        e.write(0,0,Util::str("Pontos faltantes %d :",
                              max_acertos - acertos));
        e.write(0,25,Util::str("Acertos %d :", acertos));
        e.write(0,50,Util::str("Erros %d :", erros));
    }

    void msgFinal(){
        Escritor e;
        if (over())
            e.write(0,10,"Treine mais meu filho!\n");
        else
            e.write(0,10,"Voce eh quase um X-MEN, parabens Wolverine!\n");
    }
};

struct Tecla{
    float x, y, vy;
    int limX;
    char value;
    int tam;

    void init(){
        value = Util::rand()%26+'a';
        x = (Util::rand() % limX);
        y = 0;
        tam = Util::rand()%10 + 10;
    }

    Tecla(int limX){
        vy = 10;
        this->limX = limX;
        init();
    }


    void update(){
        y = y + vy;
        x += Util::rand()%10 - 5;
    }

    void show(Escritor &e){
        e.write(x, y, Util::str("%c",value));
    }
};

struct Chao{
    int y;
    int largura;
    Chao(int y, int largura){
        this->y = y;
        this->largura = largura;
    }

    bool hit(Tecla t){
        if(t.y + 20 >= y)
            return true;
        return false;
    }

    void subir(){
        y -= 10;
    }

    void descer(){
        y += 1;
    }

    void show(Pintor &p){
        p.setThickness(3);
        p.setColor(Color::Green);
        p.drawLine(0, y, largura, y);
    }
};

int main(){
    Game game(1000, 750);
    Janela janela(game.largura, game.altura,"Digite ou morra!");
    Chao chao(700, game.largura);
    Escritor escritor;
    Pintor pintor;

    list<Tecla> teclas;

    Tecla t(game.largura - 20);
    teclas.push_back(t);

    while(janela.isOpen() && !game.over()) {
        //INPUT
        char c = janela.getKeyChar();
        for( Tecla &tecla : teclas ){
            if(c == tecla.value) {
                game.acertos++;
                tecla.init();
                chao.descer();
            }
            if(chao.hit(tecla)){
                game.erros++;
                tecla.init();
                chao.subir();
                Tecla t(game.largura - 20);
                teclas.push_back(t);
            }
        }

        //UPDATE
        for( auto &tecla : teclas)
            tecla.update();

        //SHOW
        janela.clear();
        game.show();
        for(auto &tecla : teclas)
            tecla.show(escritor);
        chao.show(pintor);
        janela.update();

        //SLEEP
        //Util::sleep(30 - game.acertos/1.5);
    }

    janela.clear();
    game.msgFinal();

    escritor.write(0,30,"Aperter x para sair!");
    janela.update();

    while(janela.getKeyChar() != 'x');

    return 0;
}