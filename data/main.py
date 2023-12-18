import matplotlib.pyplot as plt
import numpy as np

# Lista de valores para os eixos x e y
tamanho_buffer      = ["0.064", "0.128", "0.256", "0.512", "1", "2", "4", "8"]
tempo_execucao_udp  = [0.28, 0.24, 0.19, 0.17, 0.15, 0.14, 0.12, 0.09]
tempo_execucao_tcp  = [0.40,0.23,0.20,0.18,0.17,0.16,0.16,0.13]
tempo_execucao_pipe = [0.01,0.01,0.01,0.01,0.01,0.01,0.012,0.01]
tempo_execucao_unix = [0.22,0.19,0.19,0.12,0.15,0.10,0.08,0.05]

tempo_execucao_udp.reverse()
tempo_execucao_tcp.reverse()
tempo_execucao_unix.reverse()

# Cria o gráfico de linha
plt.plot(tamanho_buffer, tempo_execucao_tcp,  marker='o', linestyle='-', label='Socket TCP')
plt.plot(tamanho_buffer, tempo_execucao_pipe, marker='o', linestyle='-', label='Pipe Connection')
plt.plot(tamanho_buffer, tempo_execucao_unix, marker='o', linestyle='-', label='Socket UNIXDOMAIN')
plt.plot(tamanho_buffer, tempo_execucao_udp,  marker='o', linestyle='-', label='Socket UDP')

plt.yscale('log')
plt.legend()
ticks_y = [0.01, 0.1,1,10]
plt.yticks(ticks_y, [f'$10^{{{int(np.log10(val))}}}$' for val in ticks_y])

plt.xlabel('Tamanho do buffer em (KB)')
plt.ylabel('Tempo de execução em (ms)')
plt.title('Desempenho de conexões UDP, TCP e UNIXDOMAIN')

plt.grid(axis='y', linestyle='--', alpha=0.7)

plt.show()
