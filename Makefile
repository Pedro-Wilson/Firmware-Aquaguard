PROJECT = Aquaguardian
ENV = esp32dev
BAUD = 115200

.PHONY: all build upload monitor 

all: compilar gravar monitor

compilar:
	@echo "=============================================="
	@echo "        AQUAGUARDIAN - COMPILANDO"
	@echo "=============================================="
	pio run -e $(ENV)

gravar:
	@echo "=============================================="
	@echo "        AQUAGUARDIAN - GRAVANDO ESP32"
	@echo "=============================================="
	pio run -e $(ENV) --target upload

monitor:
	@echo "=============================================="
	@echo "        AQUAGUARDIAN - SERIAL MONITOR"
	@echo "=============================================="
	pio device monitor -b $(BAUD)

limpar:
	@echo "Limpando arquivos de compilacao..."
	pio run -e $(ENV) --target clean

apagar:
	@echo "Apagando a memoria flash do ESP32..."
	pio run -e $(ENV) --target erase

info:
	
	@echo "\n\n" 
	@echo "=============================================="
	@echo "              $(PROJECT)"
	@echo "=============================================="
	@echo "Ambiente : $(ENV)"
	@echo "Baud     : $(BAUD)"
	@echo "Link para o repositório: \nhttps://github.com/Pedro-Wilson/Firmware-Aquaguard.git"
	@echo "=============================================="
	@echo "\n"
	
	pio project config