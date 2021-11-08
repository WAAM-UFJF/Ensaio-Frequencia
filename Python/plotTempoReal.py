import matplotlib.pyplot as plt


class plotTempoReal():
    """
    Classe para realizar o plot em tempo real
    """
    def __init__(self, **kwargs):
        """
        Construtor da classe plotTempoReal
        """
        print("Plot em tempo real inicializado")
        self._tamJanela = 10
        self._tempo = [0.01*i for i in range(0,100*self._tamJanela + 1)]
        self._axs = kwargs.get('eixos')
        self._fig = kwargs.get('figura')

    
    def plot(self, velocidade, corrente):
        """
        Método que realiza o plot dos dados.
        :param tempo: lista com os valores de tempo
        :param corrente: lista com os valores de corrente
        """        
        self._axs[0].cla()
        self._axs[1].cla()
        if len(corrente) < len(self._tempo):
            try:
                self._axs[0].plot(self._tempo[:len(corrente)], corrente, linewidth = 3, color = 'tab:blue', label = "Corrente")
                self._axs[1].plot(self._tempo[:len(velocidade)], velocidade, linewidth = 3, color = 'tab:orange', label = "Velocidade")
            except:
                print("Erro ao tentar realizar o plot com dados incompletos!")
        else:
            try:
                self._axs[0].plot(self._tempo, corrente, linewidth = 3, color = 'tab:blue', label = "Corrente")
                self._axs[1].plot(self._tempo, velocidade, linewidth = 3, color = 'tab:orange', label = "Velocidade")
            except:
                print("Erro ao tentar realizar o plot com dados completos!")

        try:
            self._axs[0].set_xlim(0, self._tamJanela)
            self._axs[1].set_xlim(0, self._tamJanela)
            self._axs[0].set_xticks([i for i in range(0, self._tamJanela+1)]) 
            self._axs[1].set_xticks([i for i in range(0, self._tamJanela+1)])            
            self._axs[0].set_xlabel("Tempo [s]", fontsize = 16) 
            self._axs[1].set_xlabel("Tempos [s]", fontsize = 16)
            self._axs[0].set_ylabel("Corrente [mA]", fontsize = 16)
            self._axs[1].set_ylabel("Velocidade [rad/s]", fontsize = 16)
            self._axs[0].legend(loc='upper left', fontsize = 16)
            self._axs[1].legend(loc='upper left', fontsize = 16)
            self._axs[0].grid()
            self._axs[1].grid()
            self._axs[0].tick_params(axis='both', which='major', labelsize=14)
            self._axs[1].tick_params(axis='both', which='major', labelsize=14)
            self._fig.tight_layout()
        except:
            print("Erro ao tentar traçar o grafico!")

        
    def trataDados(self, ser, velocidade, corrente):
        """
        Método que reliza o recebimento e tratamento dos dados
        :param ser: variavel de conexão, do tipo serial.serial
        :param velocidade: vetor com os valores de velocidade
        :param corrente: veotr com os valores de corrente
        """
        self._corrente = corrente
        self._velocidade = velocidade
        try:
            dados = ser.read(ser.inWaiting()).decode('utf-8')
            if dados != "":
                try:
                    dados = dados.split('\r\n')
                    dados.pop()
                    dados.pop(0)
                    try:
                        for data in dados:
                            self._velocidadeAux, self._correnteAux = data.split(';')
                            self._corrente.append(float(self._correnteAux))
                            self._velocidade.append(float(self._velocidadeAux))
                            if len(self._velocidade) > len(self._tempo):
                                self._corrente.pop(0)
                                self._velocidade.pop(0)
                    except:
                        print("Falha ao separar dados!")
                        print(f'Dados recebidos: {dados}')                    
                except:
                    print("Erro ao tentar receber dados!")
        except:
            print("Erro ao decodificar os dados!")
        

        return self._velocidade, self._corrente

    

    