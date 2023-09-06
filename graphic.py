import matplotlib.pyplot as plt

categorias = ["0,064KB","0,128KB","0,256KB","0,512KB","1KB","2KB","4KB","8KB"]
valores = [0.13,0.11,0.13,0.12,0.13,0.13,0.13,0.14]

plt.bar(categorias, valores, color='purple', width=0.8)

plt.xlabel('Tamanho do buffer em (KB)')
plt.ylabel('Tempo de execução em (ms)')
plt.title('Transmissão de Buffers entre Cliente e Servidor via Conexão por Socket')

plt.show()
