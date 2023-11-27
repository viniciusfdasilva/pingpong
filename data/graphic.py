import os
from decimal import Decimal, getcontext
import matplotlib.pyplot as plt

valores = []

getcontext().prec = 50

def rm_files():
    for i in range(1,9):
        os.system(f"rm -r time_file{i} >/dev/null 2>&1")

def get_time(filename):

    f = open(f"{filename}", "r")
    
    try:
        return Decimal(str(f.read()))
    
    except Exception:
        print("[ERRO] A porta está em uso!")
        exit(1)

rm_files()

os.system('make run_server buffer_size=64 >/dev/null 2>&1')
os.system('make run_client buffer_size=64 > time_file1')
os.system('sudo kill -9 $(sudo lsof -t -i:8099) >/dev/null 2>&1')
valores.append(get_time("time_file1"))

os.system('make run_server buffer_size=128 >/dev/null 2>&1')
os.system('make run_client buffer_size=128 > time_file2')
os.system('sudo kill -9 $(sudo lsof -t -i:8099) >/dev/null 2>&1')
valores.append(get_time("time_file2"))

os.system('make run_server buffer_size=256 >/dev/null 2>&1')
os.system('make run_client buffer_size=256 > time_file3')
os.system('sudo kill -9 $(sudo lsof -t -i:8099) >/dev/null 2>&1')
valores.append(get_time("time_file3"))

os.system('make run_server buffer_size=512 >/dev/null 2>&1')
os.system('make run_client buffer_size=512 > time_file4')
os.system('sudo kill -9 $(sudo lsof -t -i:8099) >/dev/null 2>&1')
valores.append(get_time("time_file4"))

os.system('make run_server buffer_size=1024 >/dev/null 2>&1')
os.system('make run_client buffer_size=1024 > time_file5')
os.system('sudo kill -9 $(sudo lsof -t -i:8099) >/dev/null 2>&1')
valores.append(get_time("time_file5"))

os.system('make run_server buffer_size=2048 >/dev/null 2>&1')
os.system('make run_client buffer_size=2048 > time_file6')
os.system('sudo kill -9 $(sudo lsof -t -i:8099) >/dev/null 2>&1')
valores.append(get_time("time_file6"))

os.system('make run_server buffer_size=4096 >/dev/null 2>&1')
os.system('make run_client buffer_size=4096 > time_file7')
os.system('sudo kill -9 $(sudo lsof -t -i:8099) >/dev/null 2>&1')
valores.append(get_time("time_file7"))

os.system('make run_server buffer_size=8192 >/dev/null 2>&1')
os.system('make run_client buffer_size=8192 > time_file8')
os.system('sudo kill -9 $(sudo lsof -t -i:8099) >/dev/null 2>&1')
valores.append(get_time("time_file8"))

rm_files()

categorias = ["0,064KB","0,128KB","0,256KB","0,512KB","1KB","2KB","4KB","8KB"]

plt.bar(categorias, valores, color='purple', width=0.8)

plt.yscale('log')

plt.xlabel('Tamanho do buffer em (KB)')
plt.ylabel('Tempo de execução em (ms)')
plt.title('Transmissão de Buffers entre Cliente e Servidor via Conexão por Socket')

plt.show()
