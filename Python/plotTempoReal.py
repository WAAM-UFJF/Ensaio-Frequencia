import matplotlib.pyplot as plt
from math import ceil
import pandas as pd
from plotly import io
from plotly import graph_objects as go
from plotly.subplots import make_subplots
from datetime import datetime
from identificaParametros import identificaParametros


    

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
        self._tempoAmostrado = 0.001
        print((1/self._tempoAmostrado)*self._tamJanela + 1)
        self._tempo = [self._tempoAmostrado*i for i in range(0,int((1/self._tempoAmostrado)*self._tamJanela + 1))]        
        self._axs = kwargs.get('eixos')
        self._fig = kwargs.get('figura')
        self._now = datetime.now()
        self._arquivoNome = ''


    def _analiseDados(self, nomeArquivo):
        """
        Método para plotar o momento de mudança no degrau em tempo real.
        """
        dados = pd.read_csv(f"Python\\Dados\\{nomeArquivo}.csv", sep=";")
        grafico = make_subplots(rows = 3, cols = 1)
        grafico.update_layout(height=720, width=960, title_text="Análise de Dados")
        grafico.add_trace(go.Scatter(x = dados["Tempo"], y = dados["Degrau"], name = 'Degrau'), row = 1, col = 1)
        grafico.add_trace(go.Scatter(x = dados["Tempo"], y = dados["Corrente"], name = 'Corrente'), row = 2, col = 1)
        grafico.add_trace(go.Scatter(x = dados["Tempo"], y = dados["Velocidade"], name = 'Velocidade'), row = 3, col = 1)
        grafico.update_xaxes(range = [4,6])
        grafico.show()
        io.write_image(grafico, 'Python\\Dados\\Figuras\\analiseDegrau.pdf', format = 'pdf')
        identificaParametros(nomeArquivo)


    
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
            # self._axs[0].set_ylim(0, 50)
            # self._axs[1].set_ylim(60, 120) # Comentar aqui
            self._axs[0].set_xticks([i for i in range(0, self._tamJanela+1)]) 
            self._axs[1].set_xticks([i for i in range(0, self._tamJanela+1)])            
            self._axs[0].set_xlabel("Tempo [s]", fontsize = 16) 
            self._axs[1].set_xlabel("Tempos [s]", fontsize = 16)
            self._axs[0].set_ylabel("Corrente [mA]", fontsize = 16)
            self._axs[1].set_ylabel("Velocidade [rad/s]", fontsize = 16)
            self._axs[0].legend(loc='upper left', fontsize = 16)
            self._axs[1].legend(loc='upper left', fontsize = 16)
            self._axs[0].tick_params(axis='both', which='major', labelsize=14)
            self._axs[1].tick_params(axis='both', which='major', labelsize=14)
            self._axs[0].set_xlabel("Tempo [s]") 
            self._axs[1].set_xlabel("Tempos [s]")
            self._axs[0].set_ylabel("Corrente [mA]")
            self._axs[1].set_ylabel("Velocidade [rad/s]")
            self._axs[0].legend(loc='upper left')
            self._axs[1].legend(loc='upper left')
            self._axs[0].tick_params(axis='both', which='major')
            self._axs[1].tick_params(axis='both', which='major')
            self._axs[0].grid()
            self._axs[1].grid()            
            self._fig.tight_layout()
        except:
            print("Erro ao tentar traçar o grafico!")

        
    def trataDados(self, ser, velocidade, corrente, degrau):
        """
        Método que reliza o recebimento e tratamento dos dados
        :param ser: variavel de conexão, do tipo serial.serial
        :param velocidade: vetor com os valores de velocidade
        :param corrente: veotr com os valores de corrente
        """
        self._corrente = corrente
        self._velocidade = velocidade
        self._degrau = degrau
        try:
            dados = ser.read(ser.inWaiting()).decode('utf-8')
            if dados != "":
                try:
                    dados = dados.split('\r\n')
                    dados.pop()
                    dados.pop(0)
                    try:
                        for data in dados:
                            self._velocidadeAux, self._correnteAux, self._degrauAux = data.split(';')
                            self._corrente.append(float(self._correnteAux))
                            self._velocidade.append(float(self._velocidadeAux))
                            self._degrau.append(int(self._degrauAux))
                            if len(self._velocidade) > len(self._tempo):
                                self._corrente.pop(0)
                                self._velocidade.pop(0)
                                self._degrau.pop(0)
                            if self._degrau[int(ceil(len(self._degrau)/2))] == 0 and self._degrau[int(ceil(len(self._degrau)/2) + 1)] == 1:
                                arquivos = {'Tempo' : self._tempo, 'Corrente' : self._corrente, 'Velocidade' : self._velocidade, 'Degrau' : self._degrau}
                                df = pd.DataFrame(data = arquivos)
                                self._arquivoNome = self._now.strftime('%b-%d-%Y--%H-%M-%S')
                                df.to_csv(f"Python\\Dados\\{self._arquivoNome}.csv", sep=";", index = False)    
                                self._analiseDados(self._arquivoNome)                            
                    except:
                        print("Falha ao separar dados!")
                        print(f'Dados recebidos: {dados}')                    
                except:
                    print("Erro ao tentar receber dados!")
        except:
            print("Erro ao decodificar os dados!")
        return self._velocidade, self._corrente, self._degrau    


if __name__ == '__main__':
    dados = plotTempoReal()
    dados._analiseDados('dados')