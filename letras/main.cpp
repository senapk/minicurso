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
	
	bool loose(){
		if(erros > max_erros)
            return true;
        return false;
	}
	
	bool win(){
		if(acertos > max_acertos)
			return true;
        return false;
	}

    bool over(){
        if(loose() || win())
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
        if (loose())
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
    int R, G, B;

    void init(){
        value = Util::rand()%26+'a';
        x = (Util::rand() % limX);
        y = 0;
        tam = Util::rand()%10 + 10;
        vy = 3 + Util::rand() % 8;
        R = Util::rand() % 255;
        G = Util::rand() % 255;
        B = Util::rand() % 255;
    }

    Tecla(int limX){
        this->limX = limX;
        init();
    }


    void update(){
        y = y + vy;
        x += Util::rand()%10 - 5;
    }

    void show(Escritor &e){
		e.setSize(tam);
		e.setColor(Color(R, G, B));
        e.write(x, y, Util::str("%c",value));
    }
};

struct Cenario{
    int y;
    int largura;
    Imagem imagem;
    int xm, ym;
    
    Tecla ultima;
    Cenario(int y, int altura, int largura, string fundo_path):
    ultima(largura){
		imagem.load(fundo_path, largura, altura);
        this->y = y;
        this->largura = largura;
        xm = largura / 2 - 30;
		ym = altura / 2 - 40;
		ultima.x = xm;
		ultima.y = ym;
    }
    
    
    void comerLetra( const Tecla &tecla ){
		ultima = tecla;
	}
	
	void mostrarLingua(Pintor &pintor){
		pintor.setColor(Color::Red);
		pintor.setThickness(10);
		pintor.drawLine(xm, ym, ultima.x, ultima.y);
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
		imagem.draw(0,0);
        p.setThickness(3);
        p.setColor(Color::Green);
        p.drawLine(0, y, largura, y);
    }
};

int main(){
    Game game(1000, 750);
    Janela janela(game.largura, game.altura,"Digite ou morra!");
    janela.setBackgroundColor(Color::White);
    Cenario cenario(700, game.altura, game.largura, "melisma.jpg");
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
                cenario.comerLetra(tecla);
                
                tecla.init();
                
                if((game.acertos % 5) == 0)
					teclas.push_back(Tecla(game.largura - 20));
            }
            if(cenario.hit(tecla)){
                game.erros++;
                tecla.init();
                cenario.subir();
                Tecla t(game.largura - 20);
                teclas.push_back(t);
            }
        }

        //UPDATE
        for( auto &tecla : teclas)
            tecla.update();

        //SHOW
        janela.clear();
        cenario.show(pintor);
        cenario.mostrarLingua(pintor);
        game.show();
        for(auto &tecla : teclas)
            tecla.show(escritor);
        
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
